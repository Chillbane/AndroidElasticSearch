#include <stdio.h>
#include <stdlib.h> /* For exit() */

extern FILE *popen(const char *command, const char *type);
extern int pclose(FILE *stream);
/* declaring extern for function prototypes is unnecessary (it's redundant)
 * use only if fxns and variables are defined in other source files and
 * for which not header file is provied */

int main(int argc, char *argv[])
{
  FILE *fppo;
  
  fppo = popen("grep hello.txt", "w");
  if (fppo == NULL) {
    printf("Error\n");
    exit(-1);
  }
  
  fprintf(fppo, "greetings\n");
  fprintf(fppo, "hello\n");
  fprintf(fppo, "goodbye\n");
  pclose(fppo);
  
  return(0);
}

