// Creighton Cathey 
// Spring 2023
// CSC 3304 Section 2
// Homework Assignment 4
//
// Background: this file implements a simple singly-linked list structure.
//Dynamic memory is simulated with an array and helper functions are already
//written to initialize memory and obtain a pointer to unused memory which you
//can use to allocate a new List element.
//
// Remove this artificial memory management code and replace with calls to
//malloc() and free()
//

#include <stdio.h>
#include <stdlib.h> // added for malloc and free

struct List {
  int data;
  struct List * next;
};

//Function declarations
void testFunction1(void);
void testFunction2(void);
struct List * newElement(int data);
void insertAtHead(struct List ** list, struct List * element);
void deleteHead(struct List ** list);
void deleteElementByValue(struct List ** list, int data);
int findMax(struct List * list);
void printList(const struct List * list);
void deleteList(struct List ** list);

//Main
int main(void) {
  //This is a test function to demonstrate list behavior
  testFunction1();

  //This is blank; fill it in if you want to do your own testing
  //testFunction2();

  return 0;
} //End Main

//DO NOT MODIFY
void testFunction1(void) {
  //Declare variables
  struct List * testList1 = NULL;
  int max;

  //Begin with a lineskip to create same space below the prompt
  printf("\n");

  //Initialize a new List
  testList1 = newElement(10);

  //Return immediately if out of memory;
  if (testList1 == NULL) {
    printf("Could not create testList1: out of memory\n");
    printf("\n");
    return;
  }

  //Append 4 new elements to test list 1
  insertAtHead( & testList1, newElement(20));
  insertAtHead( & testList1, newElement(30));
  insertAtHead( & testList1, newElement(99));
  insertAtHead( & testList1, newElement(50));

  //Print the list
  printf("Printing testList1: ");
  printList(testList1);
  printf("\n");

  //Find the max value in the list and print it    
  max = findMax(testList1);
  printf("The maximum element of testList1 is: %d\n", max);
  printf("\n");

  //Delete the max valued element
  deleteElementByValue( & testList1, max);
  printf("Deleting first element with data %d\n", max);
  printf("\n");

  printf("Deleting head of testList1\n");
  deleteHead( & testList1);
  printf("Printing testList1: ");
  printList(testList1);
  printf("\n");

  //Delete list
  deleteList( & testList1);
}

//Implement your own test function here if you wish
void testFunction2(void) {
  return;
}

//Created a new list in the first available memory location and return a
//pointer to the new list.
struct List * newElement(int data) {
  struct List * element = (struct List * ) malloc(sizeof(struct List));
  if (element == NULL) {
    fprintf(stderr, "Error: Unable to allocate memory for new element.\n");
    return NULL;
  }
  element -> data = data;
  element -> next = NULL;
  return element;
}

//Insert element at head of list
void insertAtHead(struct List ** list, struct List * element) {
  if (element == NULL) {
    fprintf(stderr, "Error: Element is NULL. Cannot insert at head.\n");
    return;
  }
  element -> next = * list;
  * list = element;
}

//Delete the list head replacing it with the second element of the list, or else
//NULL if there is no second element. This takes the address-of a list pointer
//as parameter. Remember to set the old head's status to UNUSED and data to 0
//Hint: you'll need two derefences to access the head members, but you can't
//use the -> operator twice in a row.
void deleteHead(struct List ** list) {
  struct List * temp;

  if ( * list == NULL) {
    fprintf(stderr, "Error: List is empty. Cannot delete head.\n");
    return;
  }

  temp = * list;
  * list = ( * list) -> next;
  free(temp);
}

//Delete the max valued element
void deleteElementByValue(struct List ** list, int data) {
  struct List * current = * list;
  struct List * previous = NULL;

  while (current != NULL) {
    if (current -> data == data) {
      if (previous == NULL) {
        * list = current -> next;
      } else {
        previous -> next = current -> next;
      }
      free(current);
      return;
    }
    previous = current;
    current = current -> next;
  }
  fprintf(stderr, "Error: Element with value %d not found.\n", data);
}

//Walk through the list recording the maximum int data encountered. Return
//the max (not a pointer to the element; but the max int value encountered)
int findMax(struct List * list) {
  int max = list -> data;
  while (list != NULL) {
    if (list -> data > max) {
      max = list -> data;
    }
    list = list -> next;
  }
  return max;
}

//Print raw data values from listMemory (regardless of whether or not the data
//is allocated). This is a testing function you can use if desired.
void printList(const struct List * list) {
  while (list != NULL) {
    printf("%d -> ", list -> data);
    list = list -> next;
  }
  printf("NULL\n");
}

//Delete the entire list, freeing the allocated memory for each node
void deleteList(struct List ** list) {
  struct List * current = * list;
  struct List * temp;
  while (current != NULL) {
    temp = current;
    current = current -> next;
    free(temp);
  }
  * list = NULL;
}
