// Header Files
#include <stdio.h>
#include <string.h>  // For strncmp(), etc.
#include <stdlib.h>  // For exit()
#include <ctype.h> // For isalnum()

// Macros (definitions)
#define MIN_BUF 256
#define MAX_BUF 2048

// Global Variables
char Buffer[MAX_BUF];
char Word[MIN_BUF];
char NextWord[MIN_BUF];

// Function prototypes (for defining functions after main())
void parseFile(FILE *fp, char *fname);

int main(int argc, char *argv[])
{
  /* argc (argument count) -> program receives number of arguments in argc
   *    -> ex. typing "cat file" in the terminal to call the cat program to
   *       read a file has 2 arguments (argument name counts as one) 
   * argv (argument vector) -> program receives the vector of arguments 
   *    -> ex. argument vector would contain [cat, file, null]
   *        -> last element is a null pointer
   * int main() is used if you do not need to process command line arguments */
  
  int i;
  FILE *fp;
  
  for (i = 1; i < argc; i++) { // index starts at 1 because 0 is file name
    fp = fopen(argv[i], "r");
    if ( fp == NULL ) {
      printf("Could not open file %s\n", argv[i]); 
      // %s expects corresponding argument to be of type char *
      exit(-1); // exit with a non-zero value indicates program error
    } 
    else {
      parseFile(fp, argv[i]);
      fclose(fp);
    }
  }
  
  return 0;
}

void parseFile(FILE *fp, char *fname)
{
  int rval;
  
  // read first word
  rval = fscanf(fp, "%s", Word); // %s reads until whitespace (one word)
  /*  Stores first word in Word
   *
   *  fscanf returns an integer (the number of successfully matched input items)
   *   -> Will return 1 if it successfuly reads one word  */

  if ( rval != 1 ) {
    printf("Failed to read first word\n");
    exit(-1);
  }
  
  /* char *fgets(char *str, int n, FILE *stream)
   *  -> str (pointer to an array of chars where the read string is stored)
   *  -> n (max number of characters to be read)
   *  -> stream (pointer to file object)
   *  -> stops when n-1 characters read, newline character read, or end-of-file
   *     is reached
   *  -> will return NULL if an error occurs (otherwise will return *str) */

  while ( ! feof(fp) ) {
    rval = fscanf(fp, "%s", NextWord);
    if ( rval != 1 ) {
      continue;
    }
     
    /* int strncmp(char *str1, char *str2, size n)
     *  -> compare string pointed to by str1 to string pointed to by str2
     *  -> n: the max number of characters to be compared (compares the first n
     *     bytes of str1 and str2)
     *  -> if return value < 0: str1 < str2
     *  -> if return value == 0: str1 = str2
     *  -> if return value > 0; str1 > str2 */
 
    if ( strncmp(Word, NextWord, MIN_BUF) == 0 ) {
      printf("In file %s, repeated word: %s %s\n", fname, Word, NextWord);
      
      // print out context info to help reader find word
      // first letter must be alpha-numeric

      /* int isalnum(int c)
       *  -> checks if the passed character is alphanumeric
       *  -> returns nonzero value if c is a digit or a letter, else 0  */

      if ( isalnum(Word[0]) ) {
	fgets(Buffer, MAX_BUF, fp); // gets the line with double word
	printf("Context: %s\n", Buffer);
	printf("Re-run program after fixing!\n");
      }
      
    }
    
    /* char *strncpy(char *dest, char *src, size n)
     *  -> copies up to n characters from the string pointed to by src to dest
     *  -> dest (pointer to the destination array where the content is to be 
     *     copied)
     *  -> src (string to be copied) 
     *  -> n (number of characters to be copied from source) */

    strncpy(Word, NextWord, MIN_BUF); // Moves NextWord to Word
  }
}
