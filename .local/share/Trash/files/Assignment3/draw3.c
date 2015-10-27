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
#include <curses.h>
#include "draw3.h"

int main(int argc, char *argv[])
{
  const char executable_name[] = "java -jar Sketchpad.jar";
  struct Image landscape;
  FILE *input_file = NULL;
  FILE *executable = NULL;
  
  int i; // index for iterating through arg
  char grav_flag[] = "-g";
  char thrust_flag[] = "-t";
  char file_flag[] = "-f";
  double grav_val = 0;
  double thrust_val = 0;
  
  if (argc != NUM_ARGS) { 
    fprintf(stderr, "need flags -g gravity, -f landscape.txt, -t thrust\n");
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

  store_landscape(input_file, &landscape);
  draw_landscape(executable, &landscape);

  if (fclose(input_file) == EOF) {
    exit(EXIT_FAILURE);
  }

  fprintf(executable, "pause 10\n");
  fprintf(executable, "end\n");
  if (pclose(executable) == -1) {
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
  return 0;
}

void store_landscape(FILE *input_file, struct Image *landscape)
{
  char line[LINE_LEN + 1];
  long double x_coord = 0;
  long double y_coord = 0;
  int i;
  
  memset(line, 0, sizeof(line));
  
  // Initialize arrays in struct
  landscape->num_coordinates = 0;
  for (i = 0; i < (NUM_COORDINATES + 1); i++) {
    landscape->x_coordinates[i] = 0;
    landscape->y_coordinates[i] = 0;
  }
  
  i = 0;
  while (fgets(line, sizeof(line), input_file) != NULL) {
    if (is_blank_line(line) == TRUE) {
      continue;
    }
    else {
      if (sscanf(line, "%Lf%Lf", &x_coord, &y_coord) != 2) {
	exit(EXIT_FAILURE);
      }
      else {
	landscape->x_coordinates[i] = x_coord;
	landscape->y_coordinates[i] = y_coord;
	landscape->num_coordinates += 1;
	i += 1;
      }
    }  
  }
}

void draw_landscape(FILE *executable, struct Image *landscape)
{
  int i = 0;

  for (i = 0; i < (landscape->num_coordinates - 1); i++) {
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n",
	    lround(landscape->x_coordinates[i]),
	    lround(landscape->y_coordinates[i]),
	    lround(landscape->x_coordinates[i + 1]),
	    lround(landscape->y_coordinates[i + 1]));
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
