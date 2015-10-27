int scanf(char *format,...); // reads from stdin
int fscanf(FILE *stream, char *format,...); // reads from file
int sscanf(char *str, char *format,...); // reads from string
/* ex. fscanf(fp, "%s%d", string, &integer)
 *  -> returns an integer (number of successfully matched input items) */ 

char fgets(char *str, int n, FILE *stream);
/*  -> str (pointer to an array of chars where the read string is stored)
 *  -> n (max number of characters to be read)
 *  -> stream (pointer to file object)
 *  -> stops when n-1 characters read, newline character read, or end-of-file
 *     is reached
 *  -> will return NULL if an error occurs (otherwise will return *str) */

int strncmp(char *str1, char *str2, size n);
/*  -> compare string pointed to by str1 to string pointed to by str2
 *  -> n: the max number of characters to be compared
 *  -> if return value < 0: str1 < str2
 *  -> if return value == 0: str1 = str2
 *  -> if return value > 0; str1 > str2 */
 
int isalnum(int c);
/*  -> checks if the passed character is alphanumeric
 *  -> returns nonzero value if c is a digit or a letter, else 0  */

char* strncpy(char *dest, char *src, size n);
/*  -> copies up to n characters from the string pointed to by src to dest
 *  -> dest (pointer to the destination array where the content is to be 
 *     copied) 
 *  -> n (number of characters to be copied from source)
 *  -> if length of src is less than n, the remainder of dest is padded with
 *     null bytes
 *  -> returns a pointer to the destination string dest */
    
void memset(void *s, int c, size n);
/* fills the first n bytes of memory area pointed to by s with the constant 
 * byte c
 *   -> ex. memset(line, 0, sizeof(line)) */

size strlen(char *str); 
// returns number of bytes in the string, excluding the terminating null byte

void qsort(void *base, size nitems, size size, int (*compar)
(const void *, const void*))
/* sorts an array */
/* base - pointer to first element of array to be sorted
 * nitems - # of elements in array 
 * size - size in bytes of each element in array
 * compar - fxn that compares two elements 
 */
     
void *memcpy(void *str1, const void *str2, size_t n) 
/* copies n characters from memory area str2 to memory area str1 */
/* str1 - pointer to destination where content is to be copied
 * str2 - pointer to source of data to be copies
 */

int getchar(void) // gets a character from stdin
int putchar(int char) // writes a character to stdout

int fputs(char *str, FILE *stream)
/* writes a string to the specified stream up to but not including the null 
 * character */

void *malloc(size_t size)
// allocates requested memory (in bytes) and returns a pointer to it

void *realloc(void *ptr, size_t size)
// resize the memory block pointed to by ptr that was previously allocated memory

#include <iostream>
using namespace std;
/* can write cout instead of std::cout */

class ClassA
{
 private:
  int dataMem;
 public: // code outside struct allowed to modify variables/fxns
  ClassA() { dataMem = 1; } // constructor 
  ClassA(int i) { dataMem = i; }
  ~ClassA() {}; // destructor

  int getNum() { return dataMem; }
};

int main(int argc, char**argv)
{
  Class A anA;
  Class A anotherA(2);
  
  cout << "Hello world " << anA.getNum();
  cout << " " << anotherA.getNum() << endl;
}

/* prints: Hello world 1 2 */


 








