/* name:                Carin Li
 * ONE Card number:     1392213
 * Unix id              cbli
 * lecture section:     B1
 * instructor's name    Jacqueline Smith
 * lab section          H01 
 * TA's name            Nicolas Barriga
 *
 * REFERENCES: test_curses.c (Class code)
 *
 * This .c file contains functions that handles the curses library
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
#include "lander.h"

void setup_curses()
{
  /* initializes the screen and should be called before any of the ncurses
   * library functions are used 
   */
  initscr();
  cbreak(); 
  noecho(); // characters typed by user will not show up on the screen
 
  keypad(stdscr, true);
}

void unset_curses()
{
  keypad(stdscr, false);
  nodelay(stdscr, false);
  nocbreak();
  echo();
  
  // resets the screen and should always be called before exiting the program
  endwin();
}
