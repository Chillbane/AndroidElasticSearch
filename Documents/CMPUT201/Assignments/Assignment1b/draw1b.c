/* name:                Carin Li
 * ONE Card number:     1392213
 * Unix id              cbli
 * lecture section:     B1
 * instructor's name    Jacqueline Smith
 * lab section          H01 
 * TA's name            Nicolas Barriga
 *
 * REFERENCES: finddw.c, parse.c, slides on reading from buffer (class code)
 *
 * This program reads in an input file, identifies the command at the start of 
 * the line, and processes the line accordingly. Image data will be stored, 
 * manipulated, sent to Sketchpad for drawing, and written to stdout. 
 * 
 * The Image struct stores the image name in a char array, coordinates of
 * points in two double arrays (x and y respectively), and the total number
 * of pairs of coordinates as an integer
 */

// Header Files
#include <stdio.h>
#include <string.h> // For strncmp()
#include <stdlib.h> // For exit()
#include "draw1b.h" 

/* Program runs with only one argument
 *
 * Prints start up message
 * Opens input file and the pipe to Sketchpad and closes both when done
 */

int main(int argc, char *argv[])
{
  FILE *input_file;
  FILE *executable;
  const char executable_name[] = "java -jar Sketchpad.jar -d";
  
  if (argc != NUM_ARGS + 1) { // Program will only run with a single input file
    print_error("Number of command line arguments incorrect", NULL);
  }

  // Prints start-up message
  printf("%s started on ", argv[0]); 
  fflush(stdout);
  system("date");
  printf("Input file: %s\n", argv[1]);

  input_file = fopen(argv[1], "r");
  if (input_file == NULL) {
    print_error("Could not open input file", argv[1]);
  }

  executable = popen(executable_name, "w");
  if (executable == NULL) {
    print_error("Could not open pipe", executable_name);
  }

  process_file(input_file, executable);

  if (pclose(executable) == -1) {
    print_error("Could not close pipe", executable_name);
  }

  if (fclose(input_file) == EOF) {
    print_error("Could not close input file", argv[1]);
  }

  return 0;
}

/* Reads from input file and processes each line according to its command
 *
 * Assumes no leading white space and that each line starts with one of the 
 * following key words: Image, lineTo, End, print, draw, translate, child, #
 *
 * Called from main()
 */
void process_file(FILE *input_file, FILE *executable)
{
  char command[LINE_LEN + 1];
  char line[LINE_LEN + 1];
  char image_name[IMAGE_NAME_LEN + 1];
  int num_images = 0; // total number of images stored in images struct
  struct Image images[NUM_IMAGES];
 
  // Initialize arrays
  memset(command, 0, sizeof(command));
  memset(line, 0, sizeof(line));
  memset(image_name, 0, sizeof(image_name));
  memset(images, 0, sizeof(images));
  
  while (fgets(line, sizeof(line), input_file) != NULL) {
    if (sscanf(line, "%s", command) != 1) {
      print_error("Failed to read command in file\n", NULL);
    }  
    else if (strncmp(command, "#", 1) == 0) {
      printf("%s", line); // print comments to stdout
    }
    else if (strncmp(command, "child", 5) == 0) {
      send_to_child(executable, line); 
    }	
    else {
      if (sscanf(line, "%s%s", command, image_name) != 2) {
	print_error("Failed to read command in file\n", NULL);
      }
      else if (strncmp(command, "Image", 5) == 0) {
	store_image(input_file, line, images, &num_images, command, image_name);
      }
      else if (strncmp(command, "print", 5) == 0) {
	print_image(line, images, image_name);
      }
      else if (strncmp(command, "draw", 4) == 0) {
	draw_image(executable, line, images, image_name);
      }
      else if (strncmp(command, "translate", 9) == 0) {
	translate_image(line, images, command, image_name);
      }
    }				 
  }
}

/* Copies everything in the line after the 'child' command (including the 
 * whitespace) to the child process running Sketchpad
 *
 * Called from process_file()
 */
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

/* Stores a new image's name and drawing coordinates for line segments in the 
 * images struct or overwrites an existing image
 *
 * Called from process_file()
 */
void store_image(FILE *input_file, char *line, struct Image *images, 
		 int *num_images, char *command, char *image_name)
{
  int i = 0; // index for current image being stored/edited
  int array_i = 0; // index for initializing arrays
  int n = 0; // index for coordinates
  int image_present_flag = FALSE;
  int image_done_flag = FALSE;
  double x = 0; // holds x coordinate before storage in struct
  double y = 0; // holds y coordinate before storage in struct

  /* Checks to see if image_name defined by Image command has already been
   * stored in the images struct
   *   -> Image already defined: find_image_index() updates i to match
   *      the index of the image's location in the struct and the image is
   *      overwritten
   *   -> Image not defined: i is updated to match num_images
   */
  find_image_index(images, image_name, &i, &image_present_flag);
  if (image_present_flag == FALSE) {
    i = *num_images;
  }

  // Initialize arrays in struct
  images[i].num_coordinates = 0;
  memset(images[i].name, 0, IMAGE_NAME_LEN + 1);
  for (; array_i < (NUM_COORDINATES + 1); array_i++) {
    images[i].x_coordinates[array_i] = 0;
    images[i].y_coordinates[array_i] = 0;
  }  

  if (sscanf(line, "%s%s%lf%lf", command, images[i].name, 
	     &images[i].x_coordinates[n], &images[i].y_coordinates[n]) != 4) {
    print_error("Number of command line arguments incorrect", NULL);
  }

  while (image_done_flag == FALSE) {
    if (fgets(line, LINE_LEN + 1, input_file) == NULL) {
      print_error("Failed to read line in file\n", NULL);
    }
    if (sscanf(line, "%s", command) != 1) {
      print_error("Failed to read command in file\n", NULL);
    }	
    else if (strncmp(command, "#", 1) == 0) {
      printf("%s", line); // prints to stdout
    }
    /* lineTo stores coordinate points in struct by adding new coordinates 
     * to the previous coordinates 
     */
    else if (strncmp(command, "lineTo", 6) == 0) {
      n += 1;
      images[i].num_coordinates += 1;
      if (sscanf(line, "%s%lf%lf", command, &x, &y) != 3) {
	print_error("Failed to read command in file\n", NULL);
      }
      images[i].x_coordinates[n] = x + images[i].x_coordinates[n - 1];
      images[i].y_coordinates[n] = y + images[i].y_coordinates[n - 1];
    }
    else if (strncmp(command, "End", 3) == 0) {
      *num_images += 1; // update total number of images stored in struct
      image_done_flag = TRUE;
    }
  }
}

/* Prints a list of the coordinates of the points stored in the images
 * struct to stdout
 *
 * Will print no points if specified image has no stored coordinates
 *
 * Called from process_file()
 */
void print_image(char *line, struct Image *images, char *image_name)
{ 
  int image_i = 0; // index of struct corresponding to image name
  int i; // index for iterating through struct array
  int flag = FALSE;
 
  printf("Print Image %s =\n", image_name); // print header  
  find_image_index(images, image_name, &image_i, &flag);
    
  if ((flag == TRUE) && (images[image_i].num_coordinates >= 1)) {
    for (i = 0; i < (images[image_i].num_coordinates + 1); i++) {
      printf("%g %g\n", images[image_i].x_coordinates[i], 
	     images[image_i].y_coordinates[i]);
    }
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
void draw_image(FILE *executable, char *line, struct Image *images, 
		char *image_name)
{
  int image_i = 0; // index of struct corresponding to image name
  int i; // index for iterating through struct array
  int flag = FALSE;
   
  find_image_index(images, image_name, &image_i, &flag);

  if ((flag == TRUE) && (images[image_i].num_coordinates >= 1)) {
    for (i = 0; i < (images[image_i].num_coordinates); i++) {
      fprintf(executable, "drawSegment %ld %ld %ld %ld\n", 
	      lround(images[image_i].x_coordinates[i]),
	      lround(images[image_i].y_coordinates[i]),
	      lround(images[image_i].x_coordinates[i + 1]),
	      lround(images[image_i].y_coordinates[i + 1]));
    }
  }
}

/* Translates all points of the specified image stored in the images struct
 * by x units horizontally and y units vertically
 *
 * Called from process_file()
 */
void translate_image(char *line, struct Image *images, char *command, 
		     char *image_name)
{
  int image_i = 0; // index of struct corresponding to image name
  int i; // index for iterating through struct array
  int flag = FALSE;
  double x_move = 0; // x units to be moved horizontally
  double y_move = 0; // y units to be moved vertically

  if (sscanf(line, "%s%s%lf%lf", command, image_name, &x_move, &y_move) != 4) {
    print_error("Failed to read command in file\n", NULL);
  }	
  find_image_index(images, image_name, &image_i, &flag);
  
  if ((flag == TRUE) && (images[image_i].num_coordinates >= 1)) {
    for (i = 0; i < (images[image_i].num_coordinates + 1); i++) {
      images[image_i].x_coordinates[i] += x_move;
      images[image_i].y_coordinates[i] += y_move;
    }
  }            
}

/* Finds the index of the image corresponding to the image_name if it exists
 * Updates the value of the image_i pointer and the flag as necessary
 */
void find_image_index(struct Image *images, char *image_name, int *image_i, 
		      int *flag)
{
  int struct_i = 0; // index to iterate through images struct

  for (; struct_i < NUM_IMAGES; struct_i++) {
    if (strncmp(images[struct_i].name, image_name, IMAGE_NAME_LEN + 1) == 0) {
      *image_i = struct_i;
      *flag = TRUE;
      break;
    }
  }
}

/* Prints error message and filename if not NULL 
 *
 * Called when there's an error
 */
void print_error(const char *message, const char *filename)
{
  fprintf(stderr, "%s", message);
  if (filename != NULL) {
    fprintf(stderr, ": %s", filename);
  }
  fprintf(stderr, "\n");
  exit(EXIT_FAILURE);
}
