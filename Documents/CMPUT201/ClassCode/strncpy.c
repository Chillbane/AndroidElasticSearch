/* Simple test program to understand strncpy() */
#include <stdio.h>
#include <string.h>

#define __USE_GNU 
#define LEN 32

int main(int argc, char *argv[])
{
  char *source = "hello";
  char dest[LEN];

  /* Setup */
  memset(dest, 'a', LEN); 
  dest[LEN-1] = 0;
  printf("s (%2d): %s|| d (%2d): %s|| d[30] = '%c'\n", 
	 (int)strlen(source), source,
	 (int)strlen(dest), dest, dest[30]);
  // %2d = print as a decimal integer with a width of at least 2 wide
  
  /* Correct usage, careful of semantics */
  strncpy(dest, source, LEN);
  printf("s (%2d): %s|| d (%2d): %s|| d[30] = '%c'\n",
	 (int)strlen(source), source,
	 (int)strlen(dest), dest, dest[30]);
  // dest[30] = NULL byte (will print nothing)
  
  /* Will cause segmentation fault
   * src is less than n -> rest of dest padded with null bytes, but null bytes
   *                       far exceed the length of dest (overruns buffer  */ 
  strncpy(dest, source, 100000);
  return(0);
}
