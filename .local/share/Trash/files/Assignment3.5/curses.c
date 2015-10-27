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
#include <signal.h>
#include <sys/time.h>
#include "draw3.h"

void setup_curses()
{
  /* initializes the screen and should be called before any of the ncurses
   * library functions are used 
   *  -> the stdscr and curscr come into existance
   */
  initscr();
  
  /* if don't use cbreak(), curses will not read your input characters 
   * contiguously but will begin reading them only after a new line or an 
   * EOF is encountered
   * 
   * used so that characters are immediately available to your program
   */
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
