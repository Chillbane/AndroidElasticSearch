/* name:                Carin Li
 * ONE Card number:     1392213
 * Unix id              cbli
 * lecture section:     B1
 * instructor's name    Jacqueline Smith
 * lab section          H01 
 * TA's name            Nicolas Barriga
 *
 * REFERENCES: finddw.c, parse.c, test_linkedlist.c, slides on reading from 
 * buffer (class code), test_curses.c, timer.c
 *
 * This .c file contains functions used to manipulate the ship. 
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

struct Image ship;
struct Image thrust;
int game_done;
long double x_velocity;
long double y_velocity;
long double x_angle;
long double y_angle;
long double thrust_angle;

/* prints instructions for player, initializes the game, and starts the timer */
void start_game(struct Image *landscape)
{
  store_ship(&ship, &thrust);
  setup_curses();

  move(5, 10);
  printw("Press any key to start.");
  move(6, 10);
  printw("(Then press arrow keys to rotate, space for thrust, 'q' to quit.)");
  refresh();
  getch();

  draw_image(&ship);
  draw_image(landscape);
  fflush(executable);

  nodelay(stdscr, true);
  erase();

  move(5, 10);
  printw("Left arrow rotates counterclockwise, right arrow rotates clockwise,");
  move(6, 10);
  printw("space for thrust, 'q' to quit");
  game_done = FALSE;

  /* set initial values */
  x_velocity = 0;
  y_velocity = 0;
  thrust_angle = PI/2;
  
  /* starts timer */
  timer_start();
}

/* controls ship's descent due to gravity by erasing the old position of the ship,
 * performing calculations to determine the ship's new position, and redrawing it 
 */   
void fall_due_to_grav() 
{
  long double x_move = 0;
  long double y_move = 0;

  y_move = (y_velocity * TIME_INT) + (0.5 * grav_val * TIME_INT * TIME_INT);
  y_velocity += (grav_val * TIME_INT);

  x_move = (x_velocity * TIME_INT) + (0.5 * grav_val * TIME_INT * TIME_INT);
  x_velocity += (grav_val * TIME_INT);

  erase_image(&ship);

  translate_image(&ship, x_move, y_move);
  translate_image(&thrust, x_move, y_move);

  draw_image(&ship);
  fflush(executable);
}

/* provides ship's thrust */
void thrust_ship()
{
  sigset_t old_mask;
  long double y_move = 0;
  long double x_move = 0;

  draw_image(&thrust);

  // block SIGALRM
  /* ***************************************************************** */
  if (sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) < 0)
    exit(EXIT_FAILURE);

  x_angle = thrust_val * cos(thrust_angle);
  y_angle = grav_val + thrust_val * sin(thrust_angle); 

  y_move = (y_velocity * TIME_INT) + (0.5 * y_angle * TIME_INT * TIME_INT);
  y_velocity += (y_angle * TIME_INT);

  x_move = (x_velocity * TIME_INT) + (0.5 * x_angle * TIME_INT * TIME_INT);
  x_velocity += (x_angle * TIME_INT);

  erase_image(&ship);

  translate_image(&ship, x_move, y_move);
  translate_image(&thrust, x_move, y_move);

  draw_image(&ship);
  fflush(executable);

  // unblock SIGALRM
  /* ***************************************************************** */
  if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0)
    exit(EXIT_FAILURE);
}

/* stores coordinates that draws the ship and the thrust in image structs */
void store_ship()
{
  // coordinates used to draw the ship in the top center of the Sketchpad window
  long double ship_coords[SHIP_COORDS*2] = {320, 2, 325, 2, 330, 22, 310, 22,
					    315, 2, 320, 2}; 
  long double thrust_coords[THRUST_COORDS*2] = {330, 22, 320, 27, 310, 22}; 

  initialize_image_struct(&ship);
  initialize_image_struct(&thrust);

  store_ship_helper(&ship, SHIP_COORDS, ship_coords);
  store_ship_helper(&thrust, THRUST_COORDS, thrust_coords);

  find_centre();
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

/* rotates the ship using the coordinates of its center */
void rotate_image(long double angle_deg)
{
  sigset_t old_mask;

  // block SIGALRM
  /* ***************************************************************** */
  if (sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) < 0)
    exit(EXIT_FAILURE);

  long double angle_rad = 0; // radians to be rotated
  long double x_move = 0; // shift x-coord to 0
  long double y_move = 0; // shift y_coord to 0

  angle_rad = (angle_deg * PI) / 180.0;
  find_centre();
  x_move = ship.centre_coord[0];
  y_move = ship.centre_coord[1];

  /* erase image */
  thrust_angle += angle_rad; // update thrust angle
  erase_image(&ship);

  /* translate image to (0, 0) */
  translate_image(&ship, -x_move, -y_move);
  translate_image(&thrust, -x_move, -y_move);

  /* rotate image at (0, 0) */
  rotate_image_helper(&ship, angle_rad);
  rotate_image_helper(&thrust, angle_rad);

  /* translate image back to original position */
  translate_image(&ship, x_move, y_move);
  translate_image(&thrust, x_move, y_move);

  /* draw image */
  draw_image(&ship);
  fflush(executable);

  // unblock SIGALRM
  /* ***************************************************************** */
  if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0)
    exit(EXIT_FAILURE);

}

void rotate_image_helper(struct Image *image, long double angle_rad) 
{
  int i;
  long double x = 0; // stores x coordinate
  long double y = 0; // stores y coordinate

  for (i = 0; i < image->num_coordinates; i++) {
    x = image->x_coordinates[i];
    y = image->y_coordinates[i];
    image->x_coordinates[i] = (x * cos(angle_rad)) - (y * sin(angle_rad)); 
    image->y_coordinates[i] = (x * sin(angle_rad)) + (y * cos(angle_rad));
  }
}

/* moves an image a specified amount horizontally and vertically */
void translate_image(struct Image *image, long double x_move, 
		     long double y_move)
{
  int i = 0;

  for (i = 0; i < image->num_coordinates; i++) {
    image->x_coordinates[i] += x_move;
    image->y_coordinates[i] += y_move;
  }
}

/* calculates the coordinates of the centre of the ship */
void find_centre() 
{
  long double max_x = 0;
  long double max_y = 0;
  long double min_x = 0;
  long double min_y = 0;
  long double centre_x = 0;
  long double centre_y = 0;

  max_x = max_value(ship.x_coordinates, ship.num_coordinates);
  min_x = min_value(ship.x_coordinates, ship.num_coordinates);
  max_y = max_value(ship.y_coordinates, ship.num_coordinates);
  min_y = min_value(ship.y_coordinates, ship.num_coordinates);
  
  centre_x = (min_x + max_x) / 2;
  centre_y = (min_y + max_y) / 2;

  ship.centre_coord[0] = centre_x;
  ship.centre_coord[1] = centre_y;
  thrust.centre_coord[0] = centre_x;
  thrust.centre_coord[1] = centre_y;
}

/* calculates and returns the maximum value in an array */
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

/* calculates and returns the minimum value in an array */
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
