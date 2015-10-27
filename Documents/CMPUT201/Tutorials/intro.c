#include <stdio.h>

// Cannot have variables and functions with the same name

int main()
{
  // Declare variables at start of function
  int int_1, int_2; //Can declare multiple variables of same type on same line

  printf("I am alive! Beware.");
  getchar();
  /* getchar() keeps window from closing b/c program is not done yet
   * wait for user to hit enter 
   * gives user time to see the program run */

  printf("Please enter a number: " );
  scanf("%d", &int_1);
  /* scanf takes two variables:
   * 1) a format string that represents the value type that the user will enter
   *     -> % indicates values embedded in strings
   *     -> %d (read in int), %f (read in floats), %lf (read in doubles)
   * 2) &<variable_name>
   *     -> scanf requires the memory address of the variable to which you
   *        want to save your input file
   *     -> & in front of variable gives variable's location to scanf instead  
   *	    of the variable's value */
  printf("You entered %d. \n", int_1);
  /* printf takes the first argument after the string and treats it as though
   * it were of the type specified by the "format specifier"
   *   -> ex. treats int_1 as an integer (will truncate decimal numbers) */
  getchar();

  int_2 = 4 * 6;
  int_2 = int_2 + 5;
  printf("%d", int_2);
  getchar();
  return 0; // indicates everything went well
}
