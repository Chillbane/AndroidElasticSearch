#include <stdio.h>
#include <stdlib.h>
int main() {
  int **ia;
  int ix1, ix2;
  ia = malloc( 20 * sizeof(int*) );
  for( ix1 = 0; ix1 < 20; ix1++ )
    ia[ix1] = malloc( 10 * sizeof(int) );
  for( ix1 = 0; ix1 < 20; ix1++ )
    for( ix2 = 0; ix2 < 10; ix2++ )
      ia[ix1][ix2] = 10;
  
  for( ix1 = 0; ix1 < 20; ix1++ ) {
    free(ia[ix1]);
  }
  free(ia);
  return 0;
}
