#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
  char *buf = malloc(7);
  strcpy( buf, "012345" );
  printf("buf: %s\n", buf );
  
  free(buf);
  return 0;
}
