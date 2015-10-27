#include <stdio.h>

/* return-type function_name (arg_type arg1, ..., arg_type argN)
 *   -> fxn can return nothing (void as return-type) */

int multiply(int x, int y)
{
  return x * y;
}

int main()
{ 
  int num_1, num_2, product;

  printf("Please enter two numbers separated by a space: ");
  scanf("%d%d", &num_1, &num_2);
 
  product = multiply(num_1, num_2);
  printf("The product of %d and %d is %d.\n", num_1, num_2, product);
  
  return 0;
}
