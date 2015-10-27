#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 128

struct Student {
    char name[MAX_NAME];
    int grade;
    struct Student *next;
};

void compute_average(struct Student *head);
void cleanup(struct Student *head);

int main() {

    char current_name[MAX_NAME];
    int current_grade, rval;
    struct Student *head, *temp;
    
    head = NULL;
    rval = scanf("%s %d", current_name, &current_grade);
   
    while (rval == 2) {
        temp = malloc(sizeof(struct Student));
	strncpy(temp->name, current_name, MAX_NAME);
	temp->grade = current_grade;
	temp->next = head;
	head = temp;

	rval = scanf("%s %d", current_name, &current_grade);
    }

    compute_average(head);
    cleanup(head);

    return(0);
} /* end of main */

void compute_average(struct Student *head) {

    struct Student *current;
    
    double total = 0;
    int count = 0;

    current = head;

    while (current != NULL) {
        total = total + (double)current->grade;
	count++;
	current = current->next;
    }

    if (count > 0)
        printf("The average is %g\n", total/(double)count);

} /* end of compute_average */

void cleanup(struct Student *head) {

    struct Student *temp;

    while (head != NULL) {
        temp = head->next;
	free(head);
	head = temp;
    }

} /* end of cleanup */
