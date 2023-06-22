//Spring 2023
//CSC 3304 Section 2
//Lab Assignment 1
//

// Creighton Cathey 

//
//Background: this file implements a simple singly-linked list structure.
//Dynamic memory is simulated with an array and helper functions are already
//written to initialize memory and obtain a pointer to unused memory which you
//can use to allocate a new List element.
//
//When allocating (or deallocating) an element you MUST modify its status
//field. Set it to USED or UNUSED as appropriate or else the global listMemory
//state will become corrupt. Speaking of that global, DO NOT directly modify it
//or write any new functions that modify it or access it. It should only be
//used via pointers allocated from the findUnusedMem function.
//
//Instructions: fill in the 4 blank function definitions at the end of the file
//

#include <stdio.h>

//Define macros for the listMemory array's total size and to represent usage
#define LIST_MEM_SIZE 20
#define UNUSED 0
#define USED 1

//Defines a singly-linked list structure
struct List {
	struct List *nextp;
	int data;
	int status;
};

//Function declarations
void initListMem(void);
void printListMem(void);
void testFunction1(void);
void testFunction2(void);
struct List *findUnusedMem(void);
struct List *newElement(int data);
void appendData(struct List *listp, int data);
void deleteElementByValue(struct List *listp, int data);
int findMax(struct List *listp);
void printList(struct List *listp);
void deleteHead(struct List **list_pp);

//Global variable for listMemory (DO NOT ACCESS THIS DIRECTLY)
struct List listMemory[LIST_MEM_SIZE];


//Main
int main (void)
{
	//This is a test function I wrote to demonstrate list behavior
	testFunction1();

	//This is blank; fill it in if you want to do your own testing
	//testFunction2();

	return 0;
} //End Main


//DO NOT MODIFY
void testFunction1(void)
{

	//Declare two new lists;
	struct List *testList1p = NULL;
	struct List *testList2p = NULL;

	int max;

	//Begin with a lineskip to create same space below the prompt
	printf("\n");

	//Initialize list memory (also erases existing lists)
	initListMem();

	//Initialize a new List
	testList1p = newElement(10);

	//Return immediately if out of listMemory;
	if (testList1p == NULL) {
		printf("Could not create testList1: out of memory\n");
		printf("\n");
		return;
	}

	//Append 4 new elements to test list 1
	appendData(testList1p, 20);
	appendData(testList1p, 30);
	appendData(testList1p, 99);
	appendData(testList1p, 50);

	//Print the list
	printf("Printing testList1: ");
	printList(testList1p);
	printf("\n");

	//Find the max value in the list and print it	
	max = findMax(testList1p);
	printf("The maximum element of testList1 is: %d\n", max);
	printf("\n");
	
	//Delete the max valued element
	deleteElementByValue(testList1p, max);
	printf("Deleting first element with data %d\n", max);
	printf("\n");

	printf("Constructing a new list testList2...\n");
	//Initialize another list and assign it to testList2
	testList2p = newElement(-10);
	if (testList2p == NULL) {
		printf("Could not create testList2: out of memory\n");
		printf("\n");
		return;
	}

	appendData(testList2p, -20);
	appendData(testList2p, -30);
	
	//Print testList and testList2 to verify that their data is accurate	
	printf("\n");
	printf("Printing testList1: ");
	printList(testList1p);
	printf("\n");
	printf("Printing testList2: ");
	printList(testList2p);
	printf("\n");

	printf("Deleting head of testList1\n");
	deleteHead(&testList1p);
	printf("Printing testList1: ");
	printList(testList1p);
	printf("\n");

	//Print listMem as well to see how its data compares to the lists
	printListMem();
	printf("\n");
}

//Implement your own test function here if you wish
void testFunction2(void)
{
	return;
}


//Initializes list memory by assigning 0 to each listMem[n].status which
//indicates that it is not in use. This erases any existing lists and
//deallocates any used memory. Call at the start of any testing function.
void initListMem(void) {
	for (int i=0; i < LIST_MEM_SIZE; i++)
		listMemory[i].status = UNUSED;
	return;
}

//Print raw data values from listMemory (regardless of whether or not the data
//is allocated). This is a testing function you can use if desired.
void printListMem(void) {
	printf("List Memory Dump: ");
	for (int i=0; i < LIST_MEM_SIZE; i++)
		printf ("%d ", listMemory[i].data);
	printf("\n");
}

//Scan the listMem array and return a pointer to the first unused element
//(this is not an efficient way to manage memory)
struct List *findUnusedMem(void) {
	for (int i=0; i<LIST_MEM_SIZE; i++)
		if (listMemory[i].status == UNUSED) return &listMemory[i];

	//Returning NULL signifies that no memory is available
	return NULL;
}

//printList: takes a List* as parameter and prints the data element
//of each member of the list.
void printList(struct List *listp)
{
	while (listp != NULL)
	{
		printf("%d ", listp->data);
		listp = listp->nextp;
	}
	printf("\n");
}

//Create a new list in the first available memory location and return a
//pointer to the new list.
struct List *newElement(int data) {

	struct List *headp = findUnusedMem();
	if (headp != NULL)
	{
		headp->nextp = NULL;
		headp->data = data;
		headp->status = USED;
		return headp;
	}
	else
	{
		printf("Error: out of memory; cannot create new List\n");
		return NULL;
	}
}

/****************************************************************************/
                     /*** FILL IN THE FUNCTIONS BELOW ***/
/****************************************************************************/


//appendData: takes a pointer to a list, creates a new element and appends it
//to the end of the list; also sets its data field to the data parameter.
//appendData: takes a pointer to a list and appends a new element to the end of

//the list and sets its data field to the data parameter.

void appendData(struct List *listp, int data)
{
//create a pointer we can use to walk through the list
struct List *currentp = listp;
//begin or continue looping through the list
while (currentp != NULL)
{
//Check the next pointer to determine if we've reached the end
if (currentp->nextp == NULL)
{
//Add a new list element to the end of the list
currentp->nextp = newElement(data);
return;
}
//Update currentp to point to the next element
currentp = currentp->nextp;
}
}

//Walk through the list recording the maximum int data encountered. Return
//the max (not a pointer to the element; but the max int value encountered)
int findMax(struct List *listp)
{
int max = listp->data;
while (listp->nextp != NULL)
{
listp = listp->nextp;
if (listp->data > max)
max = listp->data;
}
return max;
}

//Ignoring the list head, delete the first element encountered in the list
//with data matching the parameter data then return. To fully remove the element
//you must set its status to UNUSED in addition to relinking the list. Also set
//its data field to 0.
void deleteElementByValue(struct List *listp, int data)
{
struct List *prevp = NULL;
while (listp != NULL && listp->data != data)
{
prevp = listp;
listp = listp->nextp;
}
if (listp != NULL)
{
if (prevp != NULL)
prevp->nextp = listp->nextp;
listp->status = UNUSED;
listp->data = 0;
}
}

// Delete the list head replacing it with the second element of the list, or else
//NULL if there is no second element. This takes the address-of a list pointer
//as parameter. Remember to set the old head's status to UNUSED and data to 0
//Hint: you'll need two derefences to access the head members, but you can't
//use the -> operator twice in a row.
void deleteHead(struct List **headpp)
{
	struct List *temp;
	
	if (*headpp == NULL) return;

	temp = *headpp;

	*headpp = (*headpp)->nextp;

	temp->nextp = NULL;

	temp->status = UNUSED;
	temp->data = 0;
}

