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
 * This program reads in an input file, identifies the command at the start of
 * the line, and processes the line accordingly. Image data will be stored,
 * manipulated, sent to Sketchpad for drawing, and written to stdout.
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

/* signal - software interrupt delivered to a process 
 *   -> e.g. expiration of a timer or alarm 
 */

/* signal handler - fxn called using signal or sigaction to tell the operating 
 *                  system to call when a signal arrives 
 */

/* sigset_t */
/* data type used to represent a signal set */

/* will store alarm signal to indicate that we want it blocked */
sigset_t block_mask_g;

int timer_start()
{
  /* int sigemptyset(sigset_t *set) */
  /* initializes the signal set given by *set to empty, with all signals
   * excluded from the set
   *  -> always returns zero
   */
  sigemptyset(&block_mask_g);

  
  /* int sigaddset(sigset_t *set, int signmum) */
  /* adds the signal signum to the signal set set
   *  -> only modifies set (doesn't block/unblock any signals)
   *  -> returns 0 on success and -1 on failure
   */

  /* Macro: int SIGALRM 
   *  -> this signal indicates expiration of a timer
   */
  sigaddset(&block_mask_g, SIGALRM);

  // set up signal handler
  /* *********************************************************** */

  // will store information for our alarm
  struct sigaction handler;

  /* set up handler struct */
  
  // handle_timeout is a function called when signal (alarm) is sent
  //  -> (timer expires)
  /* sa_handler */
  /* specifies the action to be associated with SIGALRM */
  handler.sa_handler = handle_timeout;

  // empties sa_mask, meaning no signals will be blocked when signal_handler
  // (handle_timeout) is executing
  /* sa_mask */
  /* specifies signals which should be blocked during execution of signal 
   * handler
   */
  sigemptyset(&handler.sa_mask);

  // if non-zero, indicates various options (which are not used)
  /* sa_flags */
  /* specifies a set of flags which modify the behaviour of the signal */
  handler.sa_flags = 0;
 
  // sets up a signal handler as specified in handler struct
  // when SIGALRM occurs, handle_timeout will be called
  /* int sigaction(int signum, const struct sigaction *act, struct sigaction 
   *               *oldact);
   *   -> changes signal action
   *   -> signum (specifies signal e.g. SIGALRM)
   *   -> returns 0 on success, -1 on error
   */
  if (sigaction(SIGALRM, &handler, NULL) == -1)
    exit(EXIT_FAILURE);

  // set up timer
  /* *********************************************************** */
  
  // sets up values for timer (intervals b/t sending SIGALRM)
  // times in microseconds 

  /* struct itimerval {
   *   struct timeval it_interval; 
   *   struct timeval it_value;
   * };
   *
   * struct timeval {
   *   time_t tv_sec; // seconds
   *   suseconds_t tv_usec; // microseconds
   * };
   */

  struct itimerval timer;
  
    struct timeval time_delay;
    time_delay.tv_sec = 0;
    time_delay.tv_usec = 50000; // ship redrawn every 50 000 microseconds
    
    timer.it_interval = time_delay;

    // value for initial time (before 1st alrm is triggered)
    struct timeval start;
    start.tv_sec = 0;
    start.tv_usec = 50000;

    timer.it_value = start;

  // will execute repeatedly, sending SIGALRM every timer.it_interval 
  // microseconds, whose initial value is timer.it_value
  /* int setitimer(int which, const struct itimerval *new_value, 
   *               struct itimerval *old_value);
   *   -> set value of interval timer
   *   -> ITIMER_REAL (decrements in real time, and delivers SIGALRM upon 
   *                  expiration)
   *   -> timers decrement from it_value to zero, generate a signal, and reset
   *      to it_interval
   */
  if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
    exit(EXIT_FAILURE);
  
  // SHIP CONTROLS
  /* ***************************************************************** */

  int c = 0;
  // infinite loop 
  while (1) {
    c = getch();

    if (c != ERR) {
      if (c == KEY_LEFT) {
	rotate_image(-10);
      }
      else if (c == KEY_RIGHT) {
	rotate_image(10);
      }
      else if (c == ' ') {
	thrust_ship();
      }
      else if (c == 'q') {
	game_done = TRUE;
        break;
      }
    }
  }
  unset_curses();

  fprintf(executable, "end\n");
  if (pclose(executable) == -1) {
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
    
// called when SIGALRM is sent
// could be called for other signals, so signal parameter indicates which one
void handle_timeout(int signal)
{
  sigset_t old_mask;
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

    // timer stops when game is done
    if (game_done == TRUE) {
      timer.it_interval.tv_usec = 0;
      timer.it_value.tv_usec = 0;
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
