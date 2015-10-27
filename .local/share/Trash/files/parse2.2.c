#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "parse.h"

char program_name[PROG_LEN + 1];
const char exec_name[] = "java -jar Sketchpad.jar -d";
int shiftX; 
int shiftY;
char arg2_str[LINE_LEN + 1];
char arg3_str[LINE_LEN + 1];

int main(int argc, char* argv[])
{
  FILE* input;
  FILE* executable;

  memset(program_name, 0, PROG_LEN + 1); // fills program_name array with zeros
  // copies argv[0] (program name) to the variable program_name
  strncpy(program_name, argv[0], PROG_LEN);

  if (argc != 4) {
    fprintf(stderr, "Usage: %s input-file shiftX shiftY\n", program_name);
    exit(1);
  }

  input = fopen(argv[1], "r");

  if (input == NULL) {
    fprintf(stderr, "Could not open input file %s\n", argv[1]);
    exit(1);
  }

  /*** Add code to extract the shift values from the command line ***/
  /*** arguments and set the shift values shiftX and shiftY ***/
  strncpy(arg2_str, argv[2], LINE_LEN);
  strncpy(arg3_str, argv[3], LINE_LEN);
  printf("%s %s\n", arg2_str, arg3_str);

  executable = popen(exec_name, "w");
  if (executable == NULL)
    fprintf(stderr, "Could not open pipe %s\n", exec_name);

  convert(input, executable);

  pclose(executable);
  fclose(input);

  return EXIT_SUCCESS;
}

// Read from input, convert, and send the rows to executable 
void convert(FILE* input, FILE* executable)
{
  char command[LINE_LEN + 1];
  char line[LINE_LEN + 1];
  int x1, x2, y1, y2;

  memset(line, 0, LINE_LEN + 1);
  memset(command, 0, LINE_LEN + 1);
  while (fgets(line, LINE_LEN + 1, input) != NULL)
  {
    if (strncmp(line, "drawSegment", 11) == 0) {
      fprintf(executable, "%s", line);  // print the original line

      /*** Add code to shift the X and Y coordinates respectively by shiftX
           and shiftY (i.e. add shiftX to X and shiftY to Y) and draw 
           the shifted line ***/
      sscanf(line, "%s%d%d%d%d", command, &x1, &y1, &x2, &y2);
      x1 += shiftX;
      x2 += shiftX;
      y1 += shiftY;
      y2 += shiftY;
      fprintf(executable, "%s %d %d %d %d\n", command, x1, y1, x2, y2);
    }
    else 
      fprintf(executable, "%s", line);

    memset(line, 0, LINE_LEN + 1);
    memset(command, 0, LINE_LEN + 1);
  }
}
