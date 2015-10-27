#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>

int main()
{
  initscr(); // initialize curses library

  move(5, 15);
  attron(A_BOLD); // boldface
  printw("Macbeth");
  attroff(A_BOLD);
  refresh();
  getch();

  move(8, 15);
  attron(A_STANDOUT); // highlighting
  printw("Thunder and Lightning");
  attroff(A_STANDOUT);
  refresh();
  getch();
  
  move(10, 10);
  printw("When shall we three meet again");
  move(11, 23); 
  printw("In thunder, lightning, or in rain?");
  move(13, 10);
  printw("When the hurlyburly's done,");
  move(14, 23);
  printw("When the battle's lost and won.");
  refresh();
  getch();

  move(LINES - 1, COLS - 1); // move cursor to bottom-right corner of screen
  refresh();
  getch();
  
  endwin(); // resets the curses library
  exit(EXIT_SUCCESS);
}
