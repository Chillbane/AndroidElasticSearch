#include <curses.h> // import curses library
#include <stdio.h>
#include <stdlib.h>

/* WINDOW */
/* structure that holds information on each window that you create
 *  -> representation of the window in memory
 *  -> any change that you make to it will be displayed on screen only if you
 *     display the window using an explicit command refresh()
 */

/* stdscr */
/* standard screen
 *  -> what will be displayed by the next refresh() call
 */

/* curscr */
/* current state of the screen */

/* wrefresh(WINDOW *) */
/* displays the contents of a window on the screen
 *  -> takes WINDOW pointer as an argument
 */

/* refresh() */
/* displays the contents of the stdscr
 *  -> equivalent to wrefresh(stdscr) 
 */

/* newwin(int, int, int, int) */
/* used to assign memory from the heap
 *  -> 1st parameter: # of rows the window will have
 *  -> 2nd parameter: # of columns the window will have
 *  -> 3rd parameter: y-coordinate of first row
 *  -> 4th parameter: x-coordinate of first column
 *  -> a zero as the first/second parameter -> max # of rows/columns is taken
 */

/* delwin(WINDOW *) */
/* frees memory allocated uwing newwin() -> every newwin() should have a 
 *                                          corresponding delwin()
 */

/* wmove(WINDOW *, int, int) */
/* moves cursor to a particular location in the WINDOW
 *  -> relative to the top left cell of the WINDOW in question
    -> 2nd parameter: y-coordinate of location to move to
    -> 3rd parameter: x-coordinate of location to move to
    -> move(y, x) is equivalent to wmove(stdscr, y, x)
 */

/* wprintw(WINDOW *, char *) */
/* used to add text to a WINDOW
 *   -> 2nd parameter: text string
 *   -> printw(char *) is equivalent to wprintw(stdscr, char *)
 */

/* werase(WINDOW *) */
/* used to erase all contents in the WINDOW by printing blanks over all text
 *   -> faster than wclear(WINDOW *) which performs the same fxn differently
 */

/* getch() */
/* obtains a character input -> can be used to detect arrow keys 
 *   -> wgetch(WINDOW *) does the same thing except it uses the particular
 *      window passed to it
 *   
 *  int c;
 *  keypad(stdscr, TRUE); 
 *  // accepts all keys (enables keypad of user's terminal)
 * 
 *  c = getch();
 *  if (c == KEY_UP) exit(1);
 *    -> exit if UP arrow key is pressed 
 */

/* start_color() */
/* used to initalize all the colours and should be called before initializing
 * any colour pair (a number that stands for a selected foreground and 
 * background colour and can be applied to any window)
 */

/* init_pair(int, ??, ??) */
/* used to initialize colour pairs 
 *  -> there are 7 pre-defined colours to use:
    -> COLOR_WHITE, COLOR_BLACK, COLOR_BLUE, COLOR_GREEN, COLOR_YELLOW
       COLOR_RED, and COLOR_MAGENTA available in both light and dark shades
    -> thus 14 colours in total
    -> background can only have the darker shades
    -> 1st argument: pair number
    -> 2nd argument: foreground colour
    -> 3rd argument: background colour
 */






// brief example of using curses.
// man 3 ncurses for introductory man page, and man 3 function name
// for more information on that function.

void setup_curses();
void unset_curses();

int main()
{

  setup_curses();

  move(5, 10);
  printw("Press any key to start.");
  refresh();
  int c = getch();

  nodelay(stdscr, true); // return ERR if key input is not ready
  erase(); // writes blanks to every screen location

  move(5, 10);
  printw("Press arrow keys, 'q' to quit.");
  refresh();

  c = getch();

  while(1) // infinite loop
  {
    if (c != ERR)
    {
      // in asn3, won't need to do any printing to screen.
      // instead, will rotate figure on left or right arrow keys, and
      // initiate thrust when space bar is pressed.
      erase();
      move(5,10);
      printw("Press arrow keys, 'q' to quit.");
      move(6, 10);
      if (c == KEY_DOWN)
        printw("down key pressed");
      else if (c == KEY_LEFT)
        printw("left key pressed");
      else if (c == KEY_RIGHT) 
        printw("right key pressed");
      else if (c == KEY_UP)
        printw("up key pressed");
      else if (c == 'q')
        break;
      refresh();

    }

    c = getch();

  }

  // must do this or else Terminal will be unusable
  // (if there are problems, it's not that big a deal though ... just
  // close the Terminal, and open a new one.)
  unset_curses();

  exit(EXIT_SUCCESS);
}

void setup_curses()
{
  // use return values.  see man pages.  likely just useful for error
  // checking (NULL or non-NULL, at least for init_scr)

  /* initializes the screen and should be called before any of the ncurses
   * library functions are used 
   *  -> the stdscr and curscr come into existance
   */
  initscr();
  
  /* if don't use cbreak(), curses will not read your input characters contiguously
   * but will begin reading them only after a new line or an EOF is encountered
   * 
   * used so that characters are immediately available to your program
   */
  cbreak(); 
  noecho(); // characters typed by user will not show up on the screen
  
  // needed for cursor keys (even though says keypad)
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
