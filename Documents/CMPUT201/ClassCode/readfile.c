#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUF 256 // Max line size
#define MAX_LINES 32 /* Max number of lines */

struct aLine /* A structure for heterogeneous data */
{
  char line[MAX_BUF];
  int type;
};

void bufferFile(FILE *fp, struct aLine *theLines, int *currentLinePtr,
		int maxLines);
void classifyLine(struct aLine *theLines, int currentLine);
void printClass(struct aLine *theLines, int currentLine);

int main(int argc, char *argv[])
{
  int i;
  FILE *fp;
  /* Here, I avoid global variables */
  struct aLine theLines[MAX_LINES]; /* An array of structs */
  int currentLine;
  
  currentLine = 0;
  memset(theLines, 0, sizeof(theLines)); /* Initialize. Defensive. */
  for (i=1, i<argc; i++) {
    fp = fopen(argv[i], "r");
    if (fp == NULL) {
      printf("Could not open file %s\n", argv[i]);
    }
    else {
      /* currentLine is passed-by-reference/pointer */
      bufferFile(fp, theLines, &currentLine, MAX_LINES);
      fclose(fp);
    }
  }
  return(0);
}

void bufferFile(FILE *fp, struct aLine *theLines, int *curentLinePtr, 
		int maxLines)
{
  char *rbuf;
  
  /* Line-oriented */
  while ((*currentLinePtr < maxLines) && !feof(fp)) {
    rbuf = fgets(theLines[*currentLinePtr].line, MAX_BUF, fp);
    if (rbuf == NULL) {
      break;
    }
    printf("Read (%2d): %s",
	   *currentLinePtr, theLines[*currentLinePtr].line);
    classifyLine(theLines, *currentLinePtr);
    fflush(stdout);
    
    (*currentLinePtr)++;
  }
  
  /* Warn user if we exceed maximum number of lines buffered */
  if (*currentLinePtr >= maxLines) {
    printf("Warning: Exceeded %d lines of buffering. \n", *currentLinePtr);
  }
}

