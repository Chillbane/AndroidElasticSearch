/* name:                Carin Li
 * ONE Card number:     1392213
 * Unix id              cbli
 * lecture section:     B1
 * instructor's name    Jacqueline Smith
 * lab section          H01 
 * TA's name            Nicolas Barriga
 *
 * REFERENCES:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "memwatch.h"
#include "draw2.h"

int main(int argc, char *argv[])
{
  FILE *input_file;
  FILE *executable;
  const char executable_name[] = "java -jar Sketchpad.jar -d";
  
  if (argc != NUM_ARGS + 1) { // Program will only run with a single input file
    print_error(0);
  }

  // Prints start-up message
  printf("%s started on ", argv[0]); 
  fflush(stdout);
  system("date");
  printf("Input file: %s\n", argv[1]);

  input_file = fopen(argv[1], "r");
  if (input_file == NULL) {
    print_error(0);
  }

  executable = popen(executable_name, "w");
  if (executable == NULL) {
    print_error(0);
  }

  process_file(input_file, executable);

  if (pclose(executable) == -1) {
    print_error(0);
  }

  if (fclose(input_file) == EOF) {
    print_error(0);
  }

  cleanup();
  return 0;
}

void process_file(FILE *input_file, FILE *executable)
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
    if (sscanf(line, "%s", command) != 1) {
      print_error(inputfile_line);
    }  
    else if (strncmp(command, "#", 1) == 0) {
      printf("%s", line); // print comments to stdout
    }
    else if (strncmp(command, "child", 5) == 0) {
      send_to_child(executable, line); 
    }	
    else {
      if (sscanf(line, "%s%s", command, image_name) != 2) {
	print_error(inputfile_line);
      }
      else if (strncmp(command, "Image", 5) == 0) {
	store_image(input_file, line, command, image_name, &inputfile_line);
      }
      else if (strncmp(command, "print", 5) == 0) {
	print_image(line, image_name, inputfile_line);
      }
      else if (strncmp(command, "draw", 4) == 0) {
	draw_image(executable, line, image_name, inputfile_line);
      }
      else if (strncmp(command, "translate", 9) == 0) {
	translate_image(line, command, image_name, inputfile_line);
      }
    }				 
  }
}

void send_to_child(FILE *executable, char *line) 
{
/* Index to iterate through the characters in a line array to print out lines 
 * beginning with the child command. Index starts at the white space after 
 * the child command
 */
int line_i = CHILD_COMMAND_LEN;

for (; line_i < LINE_LEN + 1; line_i++) {
  if (line[line_i] == 0) {
    line_i = CHILD_COMMAND_LEN;
    break;
  }
  else { 
    fprintf(executable, "%c", line[line_i]);
  }
 }
}

void store_image(FILE *input_file, char *line, char *command, char *image_name,
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
  node = malloc(sizeof(struct Node));
  if (node == NULL) {
    print_error(*inputfile_line);
  }
  initialize_node(node, image_name, *inputfile_line);
  node->x_coordinates[num_coords] = x_coord;
  node->y_coordinates[num_coords] = y_coord;
  num_coords += 1;
 
  while (image_done_flag == FALSE) {
    *inputfile_line += 1;
    if (fgets(line, LINE_LEN + 1, input_file) == NULL) {
      print_error(*inputfile_line);
    }
    if (sscanf(line, "%s", command) != 1) {
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
      realloc_coordinates(node, num_coords, *inputfile_line);
      if (sscanf(line, "%s%lf%lf", command, &x_coord, &y_coord) != 3) {
	print_error(*inputfile_line);
      }
      node->x_coordinates[num_coords-1] = (x_coord + 
					   node->x_coordinates[num_coords-2]);
      node->y_coordinates[num_coords-1] = (y_coord + 
					   node->y_coordinates[num_coords-2]);
    }
    else if (strncmp(command, "End", 3) == 0) {
      node->total_coordinates = num_coords;
      add_node(node);
      image_done_flag = TRUE;
    }
  }
}

void print_image(char *line, char *image_name, int inputfile_line)
{ 
  struct Node *node;
  int i; // index for iterating through coordinate arrays
 
  printf("Print Image %s =\n", image_name); // print header  
  node = find_node(image_name);
  
  if ((node != NULL) && (node->total_coordinates >= 1)) {
    for (i = 0; i < node->total_coordinates; i++) {
      printf("%g %g\n", node->x_coordinates[i], node->y_coordinates[i]);
    }
  }
  else {
    print_error(inputfile_line);
  }
	  
  printf("End Image %s\n", image_name); // print footer
}

void draw_image(FILE *executable, char *line, char *image_name, 
		int inputfile_line)
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

void print_error(int line_number)
{
  fprintf(stderr, "draw2b: %d, error\n", line_number);
  exit(EXIT_FAILURE);
}

void initialize_node(struct Node *node, char *image_name, int inputfile_line)
{
  memset(node->name, 0, IMAGE_NAME_LEN + 1);
  strcpy(node->name, image_name);
  node->x_coordinates = malloc(sizeof(double));
  if (node->x_coordinates == NULL) {
    free(node);
    print_error(inputfile_line);
  }
  
  node->y_coordinates = malloc(sizeof(double));
  if (node->y_coordinates == NULL) {
    free(node);
    print_error(inputfile_line);
  }
}

void realloc_coordinates(struct Node *node, int num_coordinates, 
			 int inputfile_line)
{
  node->x_coordinates = realloc(node->x_coordinates, 
				num_coordinates*sizeof(double));
  if (node->x_coordinates == NULL) {
    free(node->x_coordinates);
    free(node);
    print_error(inputfile_line);
  }
  
  node->y_coordinates = realloc(node->y_coordinates,
				num_coordinates*sizeof(double));
  if (node->y_coordinates == NULL) {
    free(node->y_coordinates);
    free(node);
    print_error(inputfile_line);
  }
}
  

 

