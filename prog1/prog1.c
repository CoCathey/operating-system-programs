// Prog 1
// Creighton Cathey

// Libraries Included in program
#include <stdio.h>

// Main function 
int main(void) {

  // Initialize variables 
  int a, b, sum;

  // User input of first variable
  printf("Enter the first integer: ");
  scanf("%d", &a);

  // User input of second variable
  printf("Enter the second integer: ");
  scanf("%d", &b);

  // Sum of two variables
  sum = a+b;

  // if a and b are same sign and sum is different sign overflow occurred
  if ((a >= 0 && b >= 0 && sum == 0) || (a <= 0 && b <= 0 && sum > 0)) {
    printf("Overflow occurred.\n");

  // otherwise overflow did not occur
  } else {
    printf("Overflow did not occur. \nThe sum is %d.\n", sum);
  }

  return 0;
}



