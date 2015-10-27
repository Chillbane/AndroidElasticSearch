#include <stdio.h>

int main()
{
  int age;

  // Will return 1 if the comparison is true and 0 if the comparison is false
  printf("%d\n", 5 > 4);
  printf("%d\n", 2 == 1);
  printf("%d\n", !((1 < 3) && (2 < 5))); 
  /* NOT (!) returns 1 if input is FALSE and 0 if input is TRUE
   * AND (&&) returns 1 only if both inputs are TRUE  
   * OR (||) returns 1 if either of the inputs are TRUE */
  
  /* if (statement is TRUE)
   * {
   *   Execute all statements inside the body 
   * } */ 
  if (5 < 10) {
    printf("Five is less than ten.\n");
  }

  printf("Please enter your age: " );
  scanf("%d", &age);
  if (age < 100) {
      printf("You are pretty young!\n");
  }
  else if (age == 100) {
      printf("You are old.\n");
  }
  else {
      printf("You are really old.\n");
  }

  return 0;
}
