#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memset
#include <assert.h>

struct Node {
  int payload;
  struct Node *next;
};

struct Node *insert_node(struct Node *head, int data);
void print_list(struct Node *head);
void find_node(struct Node *head, int data);
struct Node *delete_node(struct Node *head, int data);
void cleanup(struct Node *head);

int main()
{
  struct Node *head = NULL;
  
  head = insert_node(head, 1);
  head = insert_node(head, 2);
  head = insert_node(head, 3);
  print_list(head);
  find_node(head, 2);
  head = delete_node(head, 2);
  print_list(head);

  cleanup(head);

  return(0);
}

void print_list(struct Node *head) 
{
  struct Node *current = head;
  
  while (current != NULL) {
    printf("%d -> ", current->payload);
    current = current->next;
  }
  printf("\n");
}

struct Node *insert_node(struct node *head, int data) 
{
  struct Node *newNode;
  
  // malloc() reserves memory and returns a pointer to that piece of memory
  newNode = malloc(sizeof(struct Node)); 
  /* when using malloc(), be careful to use the right expression
   * sizeof(struct Node) == 8 (pointer)
   * sizeof(struct Node*) == 4 (int) */

  assert(newNode != NULL); // abort the program if assertion is false
  memset(newNode, 0, sizeof(struct Node));

  newNode->payload = data;
  /* the arrow operator (->)
   *   - newNode->payload = 1 is the same as (*newNode).payload = 1 */

  /* Insert at head of list */
  newNode->next = head;
  head = newNode;

  return head;
}

void find_node(struct Node *head, int data) 
{
  struct Node *current = head;
  
  while (current != NULL) {
    if (current->payload == data) {
      printf("Found %d\n", data);
      return;
    }
    current = current->next;
  }
  printf("%d not found\n", data);
}

int *delete_node(struct Node *head, int data)
{
  struct Node *current = head;
  struct Node *prev = NULL;

  while (current != NULL) {
    if (current->payload == data) {
      break;
    }
    prev = current;
    current = current->next;
  }
  
  // Delete first node in list
  if (current == head) {
    head = current->next;
  }
  else if (prev != NULL) {
    prev->next = current->next;
  }

  free(current);
  return head;
}

void cleanup(struct Node *head) 
{
  struct Node *current = head;
  struct Node *temp = NULL;
  
  while (current != NULL) {
    temp = current->next;
    free(current);
    current = temp;
  }
}
 
