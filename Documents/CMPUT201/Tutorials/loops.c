#include <stdio.h>

int main()
{   
  int y;    

  /* for (variable initialization; condition; variable update) {
   *   Code to execute while condition is TRUE
   * }
   *   -> variable initialization (declare a variable and give it a value or
   *      give a value to an already existing variable)
   *   -> condition (while the conditional expression is true, the loop should
   *      should continue to repeat itself)  
   *   -> variable update (how to change variable) 
   *   -> every section may be empty but semicolons still need to be present */

  for (int x = 0; x <= 5; x++) { // ++ -> increase/increment by one
    printf("For loop: %d\n", x);
  }

  /* while (condition) {
   *   Code to execute while condition is TRUE
   * } */

  y = 6;
  while (y != 11) {
    printf("While loop: %d\n", y);
    y++;
  }

  return 0;
}

