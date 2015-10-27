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

void start_game(FILE *executable, struct Image *landscape, double grav_val, 
		double thrust_val)
{
  struct Image ship;
  struct Image thrust;
  
  store_ship(&ship, &thrust);
  setup_curses();

  move(5, 10);
  printw("Press any key to start.");
  move(6, 10);
  printw("(Then press arrow keys to rotate, space for thrust, 'q' to quit.)");
  refresh();

  int c = getch();

  draw_image(executable, &ship);
  draw_image(executable, landscape);
  fflush(executable);

  nodelay(stdscr, true);
  erase();

  move(5, 10);
  printw("Left arrow rotates counterclockwise, right arrow rotates clockwise,");
  move(6, 10);
  printw("space for thrust, 'q' to quit");

  while(1) {
    c = getch();

    if (c != ERR) {
      if (c == KEY_LEFT) {
	rotate_image(executable, &ship, -10);
      }
      else if (c == KEY_RIGHT) {
	rotate_image(executable, &ship, 10);
      }
      else if (c == 'q') {
        break;
      }
    }
  }

  unset_curses();
}

void store_ship(struct Image *ship, struct Image *thrust)
{
  // coordinates used to draw the ship in the top center of the Sketchpad window
  long double ship_coords[SHIP_COORDS*2] = {320, 2, 325, 2, 330, 22, 310, 22,
					    315, 2, 320, 2}; 
  long double thrust_coords[THRUST_COORDS*2] = {330, 22, 320, 27, 310, 22}; 

  initialize_image_struct(ship);
  initialize_image_struct(thrust);

  store_ship_helper(ship, SHIP_COORDS, ship_coords);
  store_ship_helper(thrust, THRUST_COORDS, thrust_coords);

  find_centre(ship);
  thrust->centre_coord[0] = ship->centre_coord[0];
  thrust->centre_coord[1] = ship->centre_coord[1];
}


void store_ship_helper(struct Image *image, int num_coords, 
		       long double *image_coords)
{
  int i = 0; // index for iterating through image_coords
  int n = 0; // index for iterating through x_coords and y_coords

  while (i < (num_coords*2)) {
    image->x_coordinates[n] = image_coords[i];
    image->y_coordinates[n] = image_coords[i+1];
    n += 1;
    i += 2;
  }
  image->num_coordinates = num_coords;
}

void rotate_image(FILE *executable, struct Image *image, double angle_deg)
{
  int i; // index for iterating through coordinate arrays
  double angle_rad = 0; // radians to be rotated
  long double x = 0; // stores x coordinate
  long double y = 0; // stores y coordinate
  long double x_move = 0; // shift x-coord to 0
  long double y_move = 0; // shift y_coord to 0

  angle_rad = (angle_deg * PI) / 180.0;
  x_move = image->centre_coord[0];
  y_move = image->centre_coord[1];

  /* erase image */
  erase_image(executable, image);

  /* translate image to (0, 0) */
  translate_image(image, -x_move, -y_move); 

  /* rotate image at (0, 0) */
  for (i = 0; i < image->num_coordinates; i++) {
    x = image->x_coordinates[i];
    y = image->y_coordinates[i];
    image->x_coordinates[i] = (x * cos(angle_rad)) - (y * sin(angle_rad)); 
    image->y_coordinates[i] = (x * sin(angle_rad)) + (y * cos(angle_rad));
  }

  /* translate image back to original position */
  translate_image(image, x_move, y_move);

  /* draw image */
  draw_image(executable, image);
  fflush(executable);
}

void translate_image(struct Image *image, long double x_move, 
		     long double y_move)
{
  int i = 0;

  for (i = 0; i < image->num_coordinates; i++) {
    image->x_coordinates[i] += x_move;
    image->y_coordinates[i] += y_move;
  }
}

void find_centre(struct Image *image) 
{
  long double max_x = 0;
  long double max_y = 0;
  long double min_x = 0;
  long double min_y = 0;
  long double centre_x = 0;
  long double centre_y = 0;

  max_x = max_value(image->x_coordinates, image->num_coordinates);
  min_x = min_value(image->x_coordinates, image->num_coordinates);
  max_y = max_value(image->y_coordinates, image->num_coordinates);
  min_y = min_value(image->y_coordinates, image->num_coordinates);
  
  centre_x = (min_x + max_x) / 2;
  centre_y = (min_y + max_y) / 2;

  image->centre_coord[0] = centre_x;
  image->centre_coord[1] = centre_y;
}

long double max_value(long double *array, int array_size) 
{
  long double max_val = 0;
  int i;

  max_val = array[0];

  for (i = 1; i < array_size; i++) {
    if (max_val < array[i]) {
      max_val = array[i];
    }
  }

  return max_val;
}

long double min_value(long double *array, int array_size) 
{
  long double min_val = 0;
  int i;

  min_val = array[0];
  
  for (i = 1; i < array_size; i++) {
    if (min_val > array[i]) {
      min_val = array[i];
    }
  }

  return min_val;
}
