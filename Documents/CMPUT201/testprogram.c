#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  char str[50];
  int len;

  memset(str, 0, sizeof(str));
  strcpy(str, "To infinity and beyond");

  len = strlen(str);
  printf("Length of %s is %d\n", str, len);
  
  return 0;
}
