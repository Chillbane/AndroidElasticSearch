#include <stdio.h>

int main()
{
  int c; 
  char file_name[] = "input.txt";
  FILE *fp; // file pointer

  fp = fopen(file_name, "r"); // read mode
  
  printf("The contents of %s file are:\n", file_name);
  if (fp) {
    while ((c = getc(fp)) != EOF) { 
      /* getc -> gets the next character from the specified stream (ex. file
       *         pointer) and advances the position indicator for the stream)
       *      -> returns character read cast to an int, an EOF (end of file 
       *         code), or error */
      putchar(c);
      /* putchar -> writes a character to stdout (default: the screen) */
    }
  }

  fclose(fp);

  return 0;
}
