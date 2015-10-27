#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUF 100
#define LINE_LEN 256

void reverse_file(FILE *input_file)
{
  int i;
  int n = 0;
  long int num_array[MAX_BUF];
  long int number = LINE_LEN + 1;
  char line[LINE_LEN + 1];
  
  for (i = 0; i < (sizeof(num_array)/sizeof(num_array[0])); i++) {
    num_array[i] = 0;
  }
  memset(line, 0, sizeof(line));

  while (fgets(line, sizeof(line), input_file) != NULL) {
    if (sscanf(line, "%ld", &number) != 1) {
      fprintf(stderr, "Failed to read number");
      exit(EXIT_FAILURE);
    }
   
    num_array[n] = number;
    n++;  
  }
  n -= 1;
  for (i = n; i >= 0; i--) {
    printf("%ld\n", num_array[i]);
    }
}

int main(int argc, char *argv[])
{
  FILE *input_file;
    
  input_file = fopen("input1.txt", "r");
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
