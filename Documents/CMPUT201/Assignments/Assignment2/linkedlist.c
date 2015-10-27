/* name:                Carin Li
 * ONE Card number:     1392213
 * Unix id              cbli
 * lecture section:     B1
 * instructor's name    Jacqueline Smith
 * lab section          H01 
 * TA's name            Nicolas Barriga
 *
 * REFERENCES: linkedlist.c (class code)
 *
 * This program contains code that implements a linked list. The linked list 
 * is implemented by using the Node struct which stores the image name in a 
 * char array, coordinates of points in two dynamically-allocated double 
 * arrays (x and y respectively), the total number of pairs of coordinates as 
 * an integer, and a pointer to the next node.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "memwatch.h"
#include "draw2.h"

struct Node *head = NULL; /* Definition */

void add_node(struct Node *node) 
{
   node->next = head;
   head = node;
} 

struct Node *find_node(char *key) 
{
    struct Node *current = head;

    while (current != NULL) {
      if (strncmp(current->name, key, IMAGE_NAME_LEN + 1) == 0) {	
	return current;
      }
      current = current->next;
    }
    return NULL;
} 

void delete_node(char *key)
{
  struct Node *prev = NULL;
  struct Node *current = head;

  if (head == NULL) return;
  
  while (current != NULL) {
    if (strncmp(current->name, key, IMAGE_NAME_LEN + 1) == 0) {	
      break;
    }
    prev = current; 
    current = current->next;
  }
  
  // Delete first node
  if (prev == NULL) {
    head = current->next;
  }
  else {
    prev->next = current->next;
  }
  if (current != NULL) {
    free(current->x_coordinates);
    free(current->y_coordinates);
    free(current);
  }
}

void cleanup(void) 
{
    struct Node *current = head;
    struct Node *temp = NULL;

    while (current != NULL) {
        temp = current->next;
	free(current->x_coordinates);
	free(current->y_coordinates);
	free(current);
	current = temp;
    }
} 

