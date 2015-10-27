#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

int main()
{
  initscr(); // initialize curses library

  move(5, 15); // moves cursor to point (5, 15)
  printw("Hello World"); 
  refresh();
  getch(); // gets character before exit

  endwin(); // resets the curses library
  exit(EXIT_SUCCESS);
}
