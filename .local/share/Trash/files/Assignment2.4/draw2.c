/* name:                Carin Li
 * ONE Card number:     1392213
 * Unix id              cbli
 * lecture section:     B1
 * instructor's name    Jacqueline Smith
 * lab section          H01 
 * TA's name            Nicolas Barriga
 *
 * REFERENCES: finddw.c, parse.c, test_linkedlist.c, slides on reading from 
 * buffer (class code), lab on MEMWATCH
 *
 * This program reads in an input file, identifies the command at the start of 
 * the line, and processes the line accordingly. Image data will be stored, 
 * manipulated, sent to Sketchpad for drawing, and written to stdout. 
 * 
 * Features: memory is allocated dynamically using a linked list, images 
 *           can be rotated, extensive error checking
 *
 * The linked list is implemented by using the Node struct which stores the 
 * image name in a char array, coordinates of points in two 
 * dynamically-allocated double arrays (x and y respectively), the total 
 * number of pairs of coordinates as an integer, and a pointer to the next node
 */

/* Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include "memwatch.h"
#include "draw2.h"

/* Global variables */
FILE *input_file = NULL;
FILE *executable = NULL;

/* Program runs with only one argument
 *
 * Prints start up message
 * Opens input file and the pipe to Sketchpad and closes both when done
 */
int main(int argc, char *argv[])
{
  const char executable_name[] = "java -jar Sketchpad.jar";
  
  if (argc != NUM_ARGS + 1) { // Program will only run with a single input file
    fprintf(stderr, "draw2: %d, error.\n", ERROR_CODE);
    exit(EXIT_FAILURE);
  }

  // Prints start-up message
  printf("%s started on ", argv[0]); 
  fflush(stdout);
  system("date");
  printf("Input file: %s\n", argv[1]);

  input_file = fopen(argv[1], "r");
  if (input_file == NULL) {
    fprintf(stderr, "draw2: %d, error.\n", ERROR_CODE);
    exit(EXIT_FAILURE);
  }

  executable = popen(executable_name, "w");
  if (executable == NULL) {
    fprintf(stderr, "draw2: %d, error.\n", ERROR_CODE);
    exit(EXIT_FAILURE);
  }

  process_file();

  if (pclose(executable) == -1) {
    fprintf(stderr, "draw2: %d, error.\n", ERROR_CODE);
    exit(EXIT_FAILURE);
  }

  if (fclose(input_file) == EOF) {
    fprintf(stderr, "draw2: %d, error.\n", ERROR_CODE);
    exit(EXIT_FAILURE);
  }

  cleanup();
  exit(EXIT_SUCCESS);

  return 0;
}

/* Reads from input file and processes each line according to its command
 *
 * Valid commands: Image, lineTo, End, print, draw, translate, rotate, 
 *                 child, #
 *
 * Called from main()
 */
void process_file()
{
  char command[LINE_LEN + 1];
  char line[LINE_LEN + 1];
  char image_name[IMAGE_NAME_LEN + 1];
  int inputfile_line = 0; // keep track of input line for error printing
 
  // Initialize arrays
  memset(command, 0, sizeof(command));
  memset(line, 0, sizeof(line));
  memset(image_name, 0, sizeof(image_name));
  
  while (fgets(line, sizeof(line), input_file) != NULL) {
    inputfile_line += 1;
    if (is_blank_line(line) == TRUE) {
      continue;
    }
    else if (sscanf(line, "%s", command) != 1) {
      print_error(inputfile_line);
    }  
    else if (strncmp(command, "#", 1) == 0) {
      printf("%s", line); // prints to stdout
    }
    else if (strncmp(command, "child", 5) == 0) {
      send_to_child(line); 
    }	
    else {
      if (sscanf(line, "%s%s", command, image_name) != 2) {
	print_error(inputfile_line);
      }
      else if (strncmp(command, "Image", 5) == 0) {
	store_image(line, command, image_name, &inputfile_line);
      }
      else if (strncmp(command, "print", 5) == 0) {
	print_image(line, image_name, inputfile_line);
      }
      else if (strncmp(command, "draw", 4) == 0) {
	draw_image(line, image_name, inputfile_line);
      }
      else if (strncmp(command, "translate", 9) == 0) {
	translate_image(line, command, image_name, inputfile_line); 
      }
      else if (strncmp(command, "rotate", 6) == 0) {
	rotate_image(line, command, image_name, inputfile_line);
      }
      else {
	print_error(inputfile_line);
      }
    }				 
  }
}

/* Copies everything in the line after the 'child' command (including the 
 * whitespace) to the child process running Sketchpad
 *
 * Called from process_file()
 */
void send_to_child(char *line) 
{
  int i; // index for iterating through characters in line
 
  for (i = 0; i < (LINE_LEN + 1); i++) {
    if (line[i] == 'c') { // first character of 'child' command
      break;
    }
  }
  
  i += (CHILD_COMMAND_LEN);

  for (; i < (LINE_LEN + 1); i++) {
    if (line[i] == '\n') {
      fprintf(executable, "\n");
      break;
    }
    else {
      fprintf(executable, "%c", line[i]);
    } 
  }
}

/* Stores a new image's name and drawing coordinates for line segments in the 
 * node struct or overwrites an existing image
 *
 * Called from process_file()
 */
void store_image(char *line, char *command, char *image_name,
		 int *inputfile_line)
{
  struct Node *node;
  int image_done_flag = FALSE;
  int num_coords = 0;
  double x_coord = 0; // holds x coordinate before storage in node
  double y_coord = 0; // holds y coordinate before storage in node

  /* Checks to see if a node already exists with the image name 
   *   -> Image already defined: deletes corresponding node
   */
  node = find_node(image_name);
  if (node != NULL) {
    delete_node(image_name);
  }

  if (sscanf(line, "%s%s%lf%lf", command, image_name, &x_coord, 
	     &y_coord) != 4) {
    print_error(*inputfile_line);
  }

  // Create node
  node = create_node(image_name, *inputfile_line);
  node->x_coordinates[num_coords] = x_coord;
  node->y_coordinates[num_coords] = y_coord;
  num_coords += 1;
 
  while (image_done_flag == FALSE) {
    *inputfile_line += 1;
    if (fgets(line, LINE_LEN + 1, input_file) == NULL) {
      print_error(*inputfile_line);
    }
    if (is_blank_line(line) == TRUE) {
      continue;
    }
    else if (sscanf(line, "%s", command) != 1) {
      print_error(*inputfile_line);
    }	
    else if (strncmp(command, "#", 1) == 0) {
      printf("%s", line); // prints to stdout
    }
    /* lineTo stores coordinate points in struct by adding new coordinates 
     * to the previous coordinates 
     */
    else if (strncmp(command, "lineTo", 6) == 0) {
      num_coords += 1;
      store_coordinates(line, node, num_coords, *inputfile_line);
    }
    else if (strncmp(command, "End", 3) == 0) {
      if (is_same_end_image(line, image_name, *inputfile_line) == TRUE) {
	node->total_coordinates = num_coords;
	image_done_flag = TRUE;
      }
      else {
	print_error(*inputfile_line);
      }	  
    }
    else {
      print_error(*inputfile_line);
    }
  }
}

/* Prints a list of the coordinates of the points stored in the node struct 
 * to stdout
 *
 * Will print no points if specified image has no stored coordinates
 *
 * Called from process_file()
 */
void print_image(char *line, char *image_name, int inputfile_line)
{ 
  struct Node *node;
  int i; // index for iterating through coordinate arrays
 
  node = find_node(image_name);
  
  if ((node != NULL) && (node->total_coordinates > 0)) {
    printf("Print Image %s =\n", image_name); // print header  
    if (node->total_coordinates > 1) {
      for (i = 0; i < node->total_coordinates; i++) {
	printf("%g %g\n", node->x_coordinates[i], node->y_coordinates[i]);
      }
    }
  }
  else {
    print_error(inputfile_line);
  }
	  
  printf("End Image %s\n", image_name); // print footer
}

/* Draws line segments and sends drawing instructions to Sketchpad
 *
 * Each line segment is made up of two subsequent sets of x and y coordinates 
 * which are stored in the struct
 *
 * Called from process_file()
 */
void draw_image(char *line, char *image_name, int inputfile_line)
{
  struct Node *node;
  int i; // index for iterating through coordinate arrays
   
  node = find_node(image_name);

  if ((node != NULL) && (node->total_coordinates  >= 1)) {
    for (i = 0; i < (node->total_coordinates - 1); i++) {
      fprintf(executable, "drawSegment %ld %ld %ld %ld\n", 
	      lround(node->x_coordinates[i]),
	      lround(node->y_coordinates[i]),
	      lround(node->x_coordinates[i + 1]),
	      lround(node->y_coordinates[i + 1]));
    }
  }
  else {
    print_error(inputfile_line);
  }
}

/* Translates all points of the specified image stored in the node struct
 * by x units horizontally and y units vertically
 *
 * Called from process_file()
 */
void translate_image(char *line, char *command, char *image_name,
		     int inputfile_line)
{
  struct Node *node;
  int i; // index for iterating through coordinate arrays
  double x_move = 0; // x units to be moved horizontally
  double y_move = 0; // y units to be moved vertically

  if (sscanf(line, "%s%s%lf%lf", command, image_name, &x_move, &y_move) != 4) {
    print_error(inputfile_line);
  }	
 
  node = find_node(image_name);
  
  if ((node != NULL) && (node->total_coordinates >= 1)) {
    for (i = 0; i < node->total_coordinates; i++) {
      node->x_coordinates[i] += x_move;
      node->y_coordinates[i] += y_move;
    }
  }
  else {
    print_error(inputfile_line);
  }
}

/* Rotates all points of the specified image stored in the node struct
 * by the angle specified (angle is given in degrees -> function converts it
 * into radians)
 *
 * Called from process_file()
 */
void rotate_image(char *line, char *command, char *image_name,
		  int inputfile_line)
{
  struct Node *node;
  int i; // index for iterating through coordinate arrays
  double angle_deg = 0; // degrees to be rotated
  double angle_rad = 0; // radians to be rotated
  double x = 0; // stores x coordinate
  double y = 0; // stores y coordinate

  if (sscanf(line, "%s%s%lf", command, image_name, &angle_deg) != 3) {
    print_error(inputfile_line);
  }	

  node = find_node(image_name);
  angle_rad = (angle_deg * PI) / 180.0;

  if ((node != NULL) && (node->total_coordinates >= 1)) {
    for (i = 0; i < node->total_coordinates; i++) {
      x = node->x_coordinates[i];
      y = node->y_coordinates[i];
      node->x_coordinates[i] = (x * cos(angle_rad)) - (y * sin(angle_rad)); 
      node->y_coordinates[i] = (x * sin(angle_rad)) + (y * cos(angle_rad));
    }
  }
  else {
    print_error(inputfile_line);
  }
}

/* Prints error message and the input file's line where the error occurred
 * to stderr. Frees memory and closes the pipe and file.
 */
void print_error(int line_number)
{
  // Free memory
  cleanup();

  // Close pipe
  fprintf(executable, "end\n");
  if (pclose(executable) == -1) {
    fprintf(stderr, "draw2: %d, error.\n", ERROR_CODE);
    exit(EXIT_FAILURE);
  }

  // Close file
  if (fclose(input_file) == EOF) {
    fprintf(stderr, "draw2: %d, error.\n", ERROR_CODE);
    exit(EXIT_FAILURE);
  }

  fprintf(stderr, "draw2: %d, error.\n", line_number);
  exit(EXIT_FAILURE);
}

/* Checks to see if a line in the input file is blank */
int is_blank_line(char *line)
{
  int i; // index for iterating through characters in line

  for (i = 0; i < (LINE_LEN + 1); i++) {
    if (isgraph(line[i])) {
      return FALSE;
    }
    if (line[i] == '\n') {
      break;
    }
  }
  return TRUE;
}

/* Checks to see if the image specified by the End command is the same 
 * image as specified by the Image command 
 */
int is_same_end_image(char *line, char *image_name, int inputfile_line)
{
  char command[LINE_LEN + 1];
  char image_string[LINE_LEN + 1];
  char end_image_name[IMAGE_NAME_LEN + 1];
  
  if (sscanf(line, "%s%s%s", command, image_string, end_image_name) != 3) {
    print_error(inputfile_line);
  }
  else if (strncmp(image_name, end_image_name, IMAGE_NAME_LEN + 1) == 0) {
    return TRUE;
  }
  return FALSE;
}

/* Creates and initializes a node and adds it to the linked list */
struct Node *create_node(char *image_name, int inputfile_line)
{
  struct Node *node;

  node = malloc(sizeof(struct Node));
  if (node == NULL) {
    free(node);
    print_error(inputfile_line);
  }
  add_node(node);

  memset(node->name, 0, IMAGE_NAME_LEN + 1);
  strncpy(node->name, image_name, IMAGE_NAME_LEN + 1);
  node->x_coordinates = malloc(sizeof(double));
  if (node->x_coordinates == NULL) {
    print_error(inputfile_line);
  }
  
  node->y_coordinates = malloc(sizeof(double));
  if (node->y_coordinates == NULL) {
    print_error(inputfile_line);
  }
  
  return node;
}

/* Allocates memory to store each coordinate in the node */
void realloc_coordinates(struct Node *node, int num_coords, int inputfile_line)
{
  node->x_coordinates = realloc(node->x_coordinates, num_coords*sizeof(double));
  if (node->x_coordinates == NULL) {
     print_error(inputfile_line);
  }
  
  node->y_coordinates = realloc(node->y_coordinates, num_coords*sizeof(double));
  if (node->y_coordinates == NULL) {
    print_error(inputfile_line);
  }
}

/* Stores image coordinates in the node */
void store_coordinates(char *line, struct Node *node, int num_coords, 
		       int inputfile_line)
{
  char command[LINE_LEN + 1];
  double x_coord = 0; // holds x coordinate before storage in node
  double y_coord = 0; // holds y coordinate before storage in node

  realloc_coordinates(node, num_coords, inputfile_line);
  if (sscanf(line, "%s%lf%lf", command, &x_coord, &y_coord) != 3) {
    print_error(inputfile_line);
  }
  node->x_coordinates[num_coords-1] = (x_coord + 
				       node->x_coordinates[num_coords-2]);
  node->y_coordinates[num_coords-1] = (y_coord + 
				       node->y_coordinates[num_coords-2]);
}

 

