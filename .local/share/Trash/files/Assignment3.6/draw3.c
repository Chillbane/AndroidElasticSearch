/* name:                Carin Li
 * ONE Card number:     1392213
 * Unix id              cbli
 * lecture section:     B1
 * instructor's name    Jacqueline Smith
 * lab section          H01 
 * TA's name            Nicolas Barriga
 *
 * REFERENCES: finddw.c, parse.c, test_linkedlist.c, slides on reading from 
 * buffer (class code), test_curses.c, timer.c, 
 * http://alienryderflex.com/intersect/ (for detecting line intersection)
 *
 * This program is an implementation of the Lunar Lander game using Sketchpad
 * in which the player attempts to land a rocket ship on a surface. To avoid a 
 * crash landing, the ship must land on flat ground, be upright, and be going
 * sufficiently slowly (vertical velocity <= 20). The player is able to rotate
 * and provide thrust to the ship.
 *
 * Command line arguments determine the landscape, the gravity, and the thrust.
 * The landscape is read as a file and indicated by the -f flag. Both the
 * gravity and the thrust are numbers indicated by the -g and -t flags,
 * respectively.
 *
 * The program has a struct to store images for drawing. The struct contains 
 * the total number of coordinates, the coordinates of the centre point of the 
 * image, and arrays that store the x and y coordinates. The three structs 
 * created store points for the landscape, the ship, and the thrust.
 */

/* Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include "draw3.h"

FILE *executable = NULL;
struct Image landscape;
double grav_val;
double thrust_val;

/* processes command line arguments, opens a pipe to sketchpad, and starts
 * the game
 */
int main(int argc, char *argv[])
{
  const char executable_name[] = "java -jar Sketchpad.jar";
  FILE *input_file = NULL;

  int i; // index for iterating through arg
  char grav_flag[] = "-g";
  char thrust_flag[] = "-t";
  char file_flag[] = "-f";
  grav_val = 0;
  thrust_val = 0;
  
  if (argc != NUM_ARGS) { 
    fprintf(stderr, "need flags -g gravity, -f landscape_file, -t thrust\n");
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < NUM_ARGS; i++) {
    if (strncmp(argv[i], grav_flag, FLAG_LEN) == 0) {
      grav_val = atof(argv[i+1]);
    }
    else if (strncmp(argv[i], thrust_flag, FLAG_LEN) == 0) {
      thrust_val = atof(argv[i+1]);
    }
    else if (strncmp(argv[i], file_flag, FLAG_LEN) == 0) {
      input_file = fopen(argv[i+1], "r");
      if (input_file == NULL) {
	exit(EXIT_FAILURE);
      }
    }
  }

  executable = popen(executable_name, "w");
  if (executable == NULL) {
    exit(EXIT_FAILURE);
  }

  store_landscape(input_file);

  if (fclose(input_file) == EOF) {
    exit(EXIT_FAILURE);
  }

  start_game();

  exit(EXIT_SUCCESS);
}

/* reads in the landscape file and stores the points that define the 
 * landscape in the landscape struct 
 */
void store_landscape(FILE *input_file)
{
  char line[LINE_LEN + 1];
  long double x_coord = 0;
  long double y_coord = 0;
  int i = 0;
  
  memset(line, 0, sizeof(line));
  initialize_image_struct(&landscape);
    
  while (fgets(line, sizeof(line), input_file) != NULL) {
    if (is_blank_line(line) == TRUE) {
      continue;
    }
    else {
      if (sscanf(line, "%Lf%Lf", &x_coord, &y_coord) != 2) {
	exit(EXIT_FAILURE);
      }
      else {
	landscape.x_coordinates[i] = x_coord;
	landscape.y_coordinates[i] = y_coord;
	landscape.num_coordinates += 1;
	i += 1;
      }
    }  
  }
}

/* draws an image in Sketchpad defined by its x and y coordinates stored
 * in the image struct 
 */
void draw_image(struct Image *image)
{
  int i = 0;

  for (i = 0; i < (image->num_coordinates - 1); i++) {
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n",
	    lround(image->x_coordinates[i]),
	    lround(image->y_coordinates[i]),
	    lround(image->x_coordinates[i + 1]),
	    lround(image->y_coordinates[i + 1]));
  }
}

/* erases an image in Sketchpad defined by its x and y coordinates stored
 * in the image struct 
 */
void erase_image(struct Image *image)
{
  int i = 0;

  for (i = 0; i < (image->num_coordinates - 1); i++) {
    fprintf(executable, "eraseSegment %ld %ld %ld %ld\n",
	    lround(image->x_coordinates[i]),
	    lround(image->y_coordinates[i]),
	    lround(image->x_coordinates[i + 1]),
	    lround(image->y_coordinates[i + 1]));
  }
}

void initialize_image_struct(struct Image *image)
{
  int i;

  image->num_coordinates = 0;
  image->centre_coord[0] = 0;
  image->centre_coord[1] = 0;
  for (i = 0; i < (NUM_COORDINATES + 1); i++) {
    image->x_coordinates[i] = 0;
    image->y_coordinates[i] = 0;
  }
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
