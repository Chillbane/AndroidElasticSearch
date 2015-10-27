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
 * File contains helper functions for draw2.c
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
