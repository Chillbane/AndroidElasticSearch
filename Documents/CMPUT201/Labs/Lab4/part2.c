#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUF 200
#define LINE_LEN 256

void reverse_file(FILE *input_file)
{
  int i;
  int n = 0;
  long int *num_array;
  long int number = LINE_LEN + 1;
  char line[LINE_LEN + 1];
  
  memset(line, 0, sizeof(line));
  
  // Get size of array from first number
  fgets(line, sizeof(line), input_file);
  sscanf(line, "%ld", &number);
  num_array = (long int*)malloc(number * sizeof(long int));

  if (num_array == NULL) {
    fprintf(stderr, "Error: Out of memory\n");
    exit(EXIT_FAILURE);
  }

  // Input numbers into array
  while (fgets(line, sizeof(line), input_file) != NULL) {
    sscanf(line, "%ld", &number);
    num_array[n] = number;
    n++;  
  }
  
  // Print numbers by reading array in reverse
  n -= 1;
  for (i = n; i >= 0; i--) {
    printf("%ld\n", num_array[i]);
    }
  
  free(num_array);
}

int main(int argc, char *argv[])
{
  FILE *input_file;
    
  input_file = fopen("input2.txt", "r");
  if (input_file == NULL) {
    fprintf(stderr, "Could not open file");
    exit(EXIT_FAILURE);
  }
  else {
    reverse_file(input_file);
    if (fclose(input_file) == EOF) {
      fprintf(stderr, "Could not close file");
      exit(EXIT_FAILURE);
    }
  }
  return 0;
}
