#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>

#define LEN 256

int main()
{
  char name[LEN + 1];
  char password[LEN + 1];
  const char *real_password = "jediknight";
  int i = 0;

  memset(name, 0, sizeof(name));
  memset(password, 0, sizeof(password));

  initscr(); // initialize curses library
  
  move(5, 10);
  printw("Please login:");
  move(7, 10);
  printw("Username: ");
  getstr(name);
  move(8, 10);
  printw("Password: ");
  refresh();

  noecho();

  while (i < LEN) {
    password[i] = getch();
    if (password[i] == '\n') break;
    move(8, 20 + i);
    addch('*');
    refresh();
    i++;
  }
  
  echo();

  move(11, 10);
  if (strncmp(real_password, password, strlen(real_password)) == 0) {
    printw("Access granted");
  }
  else {
    printw("Access denied");
  }
  refresh();
  getch();

  endwin(); // resets the curses library
  exit(EXIT_SUCCESS);
}
