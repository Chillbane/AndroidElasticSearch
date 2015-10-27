/* name:                Carin Li
 * ONE Card number:     1392213
 * Unix id              cbli
 * lecture section:     B1
 * instructor's name    Jacqueline Smith
 * lab section          H01 
 * TA's name            Nicolas Barriga
 *
 * REFERENCES: finddw.c and parse.c (class code)
 *
 * This program reads in an input file, identifies the keyword at the start of
 * the line, counts how often each keyword occurs, and prints the statistics
 * to stdout. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUF 256

// Prints occurrence of each keyword in the input file (stored in key_counts) 
void print_report(int *key_counts)
{
  printf("%d Image definition(s)\n", key_counts[0]);
  printf("%d lineTo command(s) within Figures\n", key_counts[1]);
  printf("%d End command(s)\n", key_counts[2]);
  printf("%d print command(s)\n", key_counts[3]);
  printf("%d draw command(s)\n", key_counts[4]);
  printf("%d translate command(s)\n", key_counts[5]);
  printf("%d child command(s)\n", key_counts[6]);
  printf("%d comment(s)\n", key_counts[7]);
}

/* Reads the file one line at a time and counts the occurrence of each keyword
 *
 * Assumes that each line starts with one of the following 8 keywords:
 * Image, lineTo, End, print, draw, translate, child, # 
 * 
 * key_counts is an array of integers that holds the counts for each keyword 
 * (the indices store counts of the keywords in the above specified order) */
void parse_file(FILE *fp)
{
  int i;
  int key_counts[8]; 
  char line[BUF];
  char first_word[BUF];

  // Sets key_counts array to all zeros
  for (i = 0; i < 8; i++) {
    key_counts[i] = 0;
  }

  while (fgets(line, sizeof(line), fp)!= NULL) {
    if (sscanf(line, "%s", first_word) != 1) {
      fscanf(stderr, "Failed to read first word\n");
      exit(EXIT_FAILURE);
    }
   
    else if (strncmp(first_word, "Image", 5) == 0) {
      key_counts[0]++;
    }
    else if (strncmp(first_word, "lineTo", 6) == 0) {
      key_counts[1]++;
    }
    else if (strncmp(first_word, "End", 3) == 0) {
      key_counts[2]++;
    }
    else if (strncmp(first_word, "print", 5) == 0) {
      key_counts[3]++;
    }
    else if (strncmp(first_word, "draw", 4) == 0) {
      key_counts[4]++;
    }
    else if (strncmp(first_word, "translate", 9) == 0) {
      key_counts[5]++;
    }
    else if (strncmp(first_word, "child", 5) == 0) {
      key_counts[6]++;
    }
    else if (strncmp(first_word, "#", 1) == 0) {
      key_counts[7]++;
    }
  }
  print_report(key_counts);
}

// Prints start-up message and sends file to be parsed 
int main(int argc, char *argv[])
{
  FILE *fp;

  if (argc == 2) { // Program will only run with a single input file
    
    // Prints start-up message
    printf("%s started on ", argv[0]); 
    fflush(stdout);
    system("date");
    printf("Input file: %s\n", argv[1]);

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
      fprintf(stderr, "Could not open file %s.\n", argv[1]);
      exit(EXIT_FAILURE);
    }
    else {
      parse_file(fp);
      if (fclose(fp) == EOF) {
	fprintf(stderr, "Could not close file %s.\n", argv[1]);
	exit(EXIT_FAILURE);
      }
    }
  }
  else {
    printf("A single input file is needed.\n");
  }
    
  return 0;
}
