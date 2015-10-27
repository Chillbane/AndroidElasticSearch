#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <curses.h>

void setup_curses();
void unset_curses();
FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);

int main()
{
  const char executable_name[] = "java -jar Sketchpad.jar";
  FILE *executable = NULL;
  double x1 = 100;
  double y1 = 100;
  double x2 = 150;
  double y2 = 150;
  int c = 0;

  executable = popen(executable_name, "w");
  if (executable == NULL) {
    exit(EXIT_FAILURE);
  }

  setup_curses();

  move(5, 10);
  printw("Press any key to start.");
  refresh();
  c = getch();

  nodelay(stdscr, true);
  erase();

  while(1) // infinite loop
  {
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n",
	    lround(x1), lround(y1), lround(x2), lround(y2));
    c = getch();
    fprintf(executable, "eraseSegment %ld %ld %ld %ld\n",
	    lround(x1), lround(y1), lround(x2), lround(y2));

    if (c != ERR)
    {
      // in asn3, won't need to do any printing to screen.
      // instead, will rotate figure on left or right arrow keys, and
      // initiate thrust when space bar is pressed.
      if (c == KEY_DOWN) {
        y1 += 1;
	y2 += 1;
      }
      else if (c == KEY_LEFT) {
        x1 -= 1;
	x2 -= 1;
      }
      else if (c == KEY_RIGHT) {
        x1 += 1;
	x2 += 1;
      }
      else if (c == KEY_UP) {
        y1 -= 1;
	y2 -= 1;
      }
      else if (c == 'q') {
        break;
      }
    }
  }

  // must do this or else Terminal will be unusable
  // (if there are problems, it's not that big a deal though ... just
  // close the Terminal, and open a new one.)
  unset_curses();

  fprintf(executable, "end\n");
  pclose(executable);

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
