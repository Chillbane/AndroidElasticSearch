/* name:                Carin Li
 * ONE Card number:     1392213
 * Unix id              cbli
 * lecture section:     B1
 * instructor's name    Jacqueline Smith
 * lab section          H01
 * TA's name            Nicolas Barriga
 *
 * REFERENCES: timer.c (Class Code)
 *
 * This .c file contains functions that handle the timer. The timer sends
 * a signal every 0.05 seconds.
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

/* will store alarm signal to indicate that we want it blocked */
sigset_t block_mask_g;

void timer_start()
{
  sigset_t old_mask;
  /* initializes the signal set given by *set to empty, with all signals */
  sigemptyset(&block_mask_g);

  /* SIGALRM (this signal indicates expiration of a timer)
   */
  sigaddset(&block_mask_g, SIGALRM);

  // set up signal handler
  /* *********************************************************** */

  // will store information for our alarm
  struct sigaction handler;

  /* set up handler struct */
  
  // handle_timeout is a function called when signal (alarm) is sent
  //  -> (timer expires)
  /* specifies the action to be associated with SIGALRM */
  handler.sa_handler = handle_timeout;

  /* specifies signals which should be blocked during execution of signal 
   * handler
   */
  sigemptyset(&handler.sa_mask);

  /* specifies a set of flags which modify the behaviour of the signal */
  handler.sa_flags = 0;
 
  // sets up a signal handler as specified in handler struct
  // when SIGALRM occurs, handle_timeout will be called
  if (sigaction(SIGALRM, &handler, NULL) == -1)
    exit(EXIT_FAILURE);

  // set up timer
  /* *********************************************************** */
  
  // sets up values for timer (intervals b/t sending SIGALRM)
  // times in microseconds 

  struct itimerval timer;
  
    struct timeval time_delay;
    time_delay.tv_sec = 0;
    time_delay.tv_usec = 50000; // ship redrawn every 50 000 microseconds
    
    timer.it_interval = time_delay;

    struct timeval start;
    start.tv_sec = 0;
    start.tv_usec = 50000;

    timer.it_value = start;

  // will execute repeatedly, sending SIGALRM every timer.it_interval 
  // microseconds, whose initial value is timer.it_value
  if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
    exit(EXIT_FAILURE);
  
  // SHIP CONTROLS
  /* ***************************************************************** */

  int c = 0;

  // infinite loop 
  while (1) {
    // get keyboard key from player
    c = getch();

    if (c != ERR) {
      // if left arrow key, rotate ship CCW by 10 degrees
      if (c == KEY_LEFT) {
	rotate_image(-10);
      }
      // if right arrow key, rotate ship CW by 10 degrees
      else if (c == KEY_RIGHT) {
	rotate_image(10);
      }
      // if space bar, apply thrust
      else if (c == ' ') {
	apply_thrust = TRUE;
      }
      // if 'q', exit the program
      else if (c == 'q') {
	game_done = TRUE;
        break;
      }
    }
  }
 
  // block SIGALRM
  /* ***************************************************************** */
  if (sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) < 0)
    exit(EXIT_FAILURE);
  return;
}
    
// called when SIGALRM is sent
// could be called for other signals, so signal parameter indicates which one
void handle_timeout(int signal)
{
  sigset_t old_mask;
  int c = 0;

  if (signal == SIGALRM) {

    // gets timer, puts it into timer 
    struct itimerval timer;
    if (getitimer(ITIMER_REAL, &timer) == -1)
      exit(EXIT_FAILURE);

    // block SIGALRM
    /* ***************************************************************** */
    if (sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) < 0)
      exit(EXIT_FAILURE);

    fall_due_to_grav();
    
    // game is done when the ship touches the landscape
    if (is_intersect() == TRUE) {
      game_done = TRUE;
    }

    // timer stops when game is done
    if (game_done == TRUE) {
      timer.it_interval.tv_usec = 0;
      timer.it_value.tv_usec = 0;
   
      nodelay(stdscr, false);
      erase();
      move(5, 10);
      printw("Left arrow rotates counterclockwise, right arrow clockwise,");
      move(6, 10);
      printw("space for thrust, 'q' to quit");
      move(7, 15);

      /* check that ship is descending sufficiently slowly 
       * and that the piece of land the ship contacted is flat */
      if ((y_velocity <= 20) && (landscape.y_coordinates[land_i] == 
				 landscape.y_coordinates[land_i+1]))  {
	printw("LANDED!!!");
      }
      else {
	printw("CRASHED!!! %Lf", y_velocity);
      }

      /* wait for keyboard input from user before exiting game */
      while (c != 'q') {
	c = getch();
      }
     
      unset_curses();
      fprintf(executable, "end\n");
      if (pclose(executable) == -1) {
	exit(EXIT_FAILURE);
      }
      exit(EXIT_SUCCESS);
    }
    // unblock SIGALRM
    /* ***************************************************************** */
    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0)
      exit(EXIT_FAILURE);

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) 
      exit(EXIT_FAILURE);
  }
}
