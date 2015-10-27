// Starter file for 201 lab exam 
// REFERENCES: Class and Lab coe (specifically slies on qsort)
// REFERENCES: http://stackoverflow.com/questions/20251946/qsort-array-of-strings-in-descending-order

// You may add other includes here.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// You may add global variables, declarations and/or
// definitions, struct type definitions, macros or function declarations
// here

// e.g.
#define NAME_LEN 32
#define LINE_LEN 1000
#define MAX_BUF 1000
#define TRUE 1
#define FALSE 0

struct Pair {
  char name[NAME_LEN + 1];
  int score_stored;
};

void do_section1();
void do_section2();
void do_section3();
int compare_ints(const void *p, const void *q);
int find_pair_index(struct Pair *pairs, int total_structs, int score);

// do not modify this main function in any way ... don't touch it.
// main checks for some errors, for your convenience - it may make 
// debugging easier.
int main(int argc, char *argv[]) {
    if (argc != 2) {
      fprintf(stderr, "error: required argument not given\n");
      fprintf(stderr, "       usage example: ./labexam 1 < input_file > output_file\n"); 
      exit(EXIT_FAILURE);
    }

    int which_section;
    int ret_val = sscanf(argv[1], "%d", &which_section); 
    if (ret_val != 1) {
        fprintf(stderr, "error: argument is not a number, %s\n",  argv[1]);
    exit(EXIT_FAILURE);
    } 

    switch(which_section) {
        case 1:
            do_section1();
            break;
        case 2:
            do_section2();
            break;
        case 3:
            do_section3();
            break;
        default:
            fprintf(stderr, "error: argument is not between 1 and 4, %s\n", argv[1]);
            exit(EXIT_FAILURE);
    }
  
} /* end of main */
 
// Add your code below this line.  If needed, create other helper functions.
//============================================================================


void do_section1() {
  // Add code here that does what is needed for Section 1.  Free up all
  // memory allocated (if you have allocated memory).  You may call
  // other functions you create.
  
  char line[LINE_LEN + 1];
  int i = 0; // index for iterating through characters in line
  int comment_done = TRUE; // flag

  memset(line, 0, sizeof(line));

  while (fgets(line, sizeof(line), stdin) != NULL) {
    while (i < LINE_LEN + 1) {
      if (line[i] == '\n') {
	if (comment_done == TRUE) {
	  printf("\n");
	}
	break;
      } 
      else if ((line[i] == '/') && (line[i+1] == '*')) {
	comment_done = FALSE;
      }
      else if ((line[i] == '*') && (line[i+1] == '/')) {
	i += 1;
	comment_done = TRUE;
      }
      else if (comment_done == TRUE) {
	printf("%c", line[i]);
      }
      i += 1;
    }
    i = 0;
  }
} /* end of do_section1 */
 
void do_section2() {
  // Add code here that does what is needed for Section 2.  Free up all
  // memory allocated (if you have allocated memory).  You may call
  // other functions you create.
 
  char line[LINE_LEN + 1];
  int i; // index for iterating through characters in line
  int num_removed = 0;

  memset(line, 0, sizeof(line));

  while (fgets(line, sizeof(line), stdin) != NULL) {
    for (i = 0; i < (LINE_LEN + 1); i++) {
      if (line[i] == '\n') {
	printf(",%d\n", num_removed);
	num_removed = 0;
	break;
      }
      
      // only prints character if not punctuation
      else if (ispunct(line[i]) == FALSE) {
	printf("%c", line[i]);
      }
      
      // counts punctuation removed
      else {
	num_removed += 1;
      }
    }
  }
} /* end of do_section2 */
  
void do_section3() {
  // Add code here that does what is needed for Section 3.  Free up all
  // memory allocated (if you have allocated memory).  You may call
  // other functions you create.

  struct Pair pairs[MAX_BUF];
  char line[LINE_LEN + 1];
  char name[NAME_LEN + 1]; // stores name
  int scores[MAX_BUF]; // array of scores to sort
  int i; // index for iterating
  int score = 0; // stores score
  int struct_i = 0; // index through array of structs
  int score_i = 0; // index through scores aray
  int total_structs = 0;

  // Initialize
  memset(line, 0, sizeof(line));
  memset(name, 0, sizeof(name));

  while (fgets(line, sizeof(line), stdin) != NULL) {
    sscanf(line, "%s%d", name, &score);

    // Store score into scores array for sorting
    scores[score_i] = score;
    score_i += 1;

    // Store name and score into struct for later retrieval
    strncpy(pairs[struct_i].name, name, MAX_BUF);
    pairs[struct_i].score_stored = score;
    struct_i += 1;
  }

  qsort(scores, score_i, sizeof(scores[0]), compare_ints); // sort scores
  total_structs = struct_i;

  // Print to stdout
  for (i = 0; i < total_structs; i++) {
    struct_i = find_pair_index(pairs, total_structs, scores[i]);
    printf("%d,%s\n", pairs[struct_i].score_stored, pairs[struct_i].name);
  }

} /* end of do_section3 */

int compare_ints(const void *p, const void *q) 
{
  return -(*((int*)p) - *((int*)q)); // multiply by -1 to get descending order
}


int find_pair_index(struct Pair *pairs, int total_structs, int score) 
{
  int struct_i = 0; // index to iterate through pairs struct
  
  for (; struct_i < total_structs; struct_i++) {
    //printf("SCORE GIVEN: %d\n", score);
    //printf("SCORE IN STRUCT: %d\n", pairs[struct_i].score);
    if (pairs[struct_i].score_stored == score) {
      return struct_i;
    }
  }
  return 0;
}
