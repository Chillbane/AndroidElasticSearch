// Starter file for 201 lab exam
// References http://www.lemoda.net/c/array-sort/ for qsort

// You may add other includes here.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// You may add global variables, declarations and/or
// definitions, struct type definitions, macros or function declarations
// here

//e.g.

#define MAX_BUF 80
#define LINE_LEN 256
#define TRUE 1
#define FALSE 0

void do_section1();
void do_section2();
void do_section3();
void do_section4();

// do not modify this main function in any way ... don't touch it.
// main checks for some errors, for your convenience - it may make 
// debugging easier.
int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "error: required argument not given\n");
    fprintf(stderr, 
            "       usage example: ./labexam 1 < input_file > output_file\n"); 
    exit(EXIT_FAILURE);
  }

  int which_section;
  int ret_val = sscanf(argv[1], "%d", &which_section); 
  if (ret_val != 1)
  {
    fprintf(stderr, "error: argument is not a number, %s\n",
                    argv[1]);
    exit(EXIT_FAILURE);
  }
 
  switch(which_section)
  {
    case 1:
      do_section1();
      break;
    case 2:
      do_section2();
      break;
    case 3:
      do_section3();
      break;
    case 4:
      do_section4();
      break;
    default:
      fprintf(stderr, "error: argument is not between 1 and 4, %s\n",
                      argv[1]);
      exit(EXIT_FAILURE);
  }
  
}
 
// Add your code below this line.  If needed, create other helper functions.
//============================================================================


void do_section1()
{
  char line[LINE_LEN + 1];
  int i; // index for iterating through characters in line

  memset(line, 0, sizeof(line));

  while (fgets(line, sizeof(line), stdin) != NULL) {
    for (i = 0; i < (LINE_LEN + 1); i++) {
      if (line[i] == '\n') {
	printf("\n");
	break;
      }

      // removes redundant lines
      else if ((isblank(line[i]) == TRUE) && (isblank(line[i-1]) == TRUE)) {
	continue;
      }
      
      // removes punctuation
      else if (ispunct(line[i]) == FALSE) {
	printf("%c", line[i]);
      }
    }
  }
}
 

void do_section2()
{
  char line[LINE_LEN + 1];
  int i; // index for iterating through characters in line

  memset(line, 0, sizeof(line));

  while (fgets(line, sizeof(line), stdin) != NULL) {
    for (i = 0; i < (LINE_LEN + 1); i++) {
      if (line[i] == '\n') {
	printf("\n");
	break;
      }

      // removes redundant lines
      else if ((isblank(line[i]) == TRUE) && (isblank(line[i-1]) == TRUE)) {
	continue;
      }

      // separates word into next line
      else if (isblank(line[i]) == TRUE) {
	printf("\n");
      }
      
      else {
	printf("%c", line[i]);
      }
    }
  }
}
  
void do_section3()
{
  /* char line[LINE_LEN + 1];
  int i; // index for iterating through characters in line

  memset(line, 0, sizeof(line));

  while (fgets(line, sizeof(line), stdin) != NULL) {
    for (i = 0; i < (LINE_LEN + 1); i++) {
      if (line[i] == '\n') {
	break;
      }
  */
}

void do_section4()
{
  char line[LINE_LEN + 1];
  int i; // index for iterating through characters in line
  int n = 0; // index for characters in name array
  char string_set[MAX_BUF][MAX_BUF];
  char name[MAX_BUF];
  int ptr_i = 0;
  
  memset(line, 0, sizeof(line));
  memset(string_set, 0, sizeof(string_set));
  memset(name, 0, sizeof(name));

  while (fgets(line, sizeof(line), stdin) != NULL) {
    for (i = 0; i < (LINE_LEN + 1); i++) {
      if (line[i] == '\n') {
	strncpy(string_set[ptr_i], name, MAX_BUF);
	memset(name, 0, sizeof(name));
	n = 0;
	break;
      } 
      else if (isalpha(line[i])) {
	name[n] = line[i];
	n += 1;
      }
      else {
	strncpy(string_set[ptr_i], name, MAX_BUF);
	ptr_i += 1;
	memset(name, 0, sizeof(name));
	n = 0;
      }
    }
    printf("%s\n", string_set[3]);
  }
}



