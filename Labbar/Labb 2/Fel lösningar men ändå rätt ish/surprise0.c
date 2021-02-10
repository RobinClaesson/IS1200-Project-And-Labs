//2s about 900 000
//10s about 4 700 000

#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6

void print_number(int n) {
  static int calls = 0;

  if (calls  ==  COLUMNS){
    printf("\n");
    calls = 0;
  }

  printf("%u\t", n);

  calls++;
}

void print_sieves_avrg(const int n)
{
    //Allocate array
    int arrayLength = n-1;
    int* numbers = (int*)malloc(sizeof(int) * arrayLength);

    int distSum = 0;
    int numOfPrimes = 1;

    //Fills Array from 2 to n
    for(int i = 0; i < arrayLength; i++){
      numbers[i] = i + 2;
    }

    int selected = 0;
    while(selected != -1)
    {
      //Printing current selected prime
      //print_number(numbers[selected]);

      //Marking multiples of selected
      for(int i = selected + numbers[selected]; i < arrayLength; i += numbers[selected])
        numbers[i] = -1; //-1 marks as not prime

      //Finds next prime in array if there is any
      int i = selected + 1;
      selected = -1;
      int steps = 1;
      while (selected == -1 && i < arrayLength) {

        if(numbers[i] != -1)
          {
            selected = i;

            numOfPrimes++;
            distSum += steps;
          }

          steps++;
          i++;
      }
    }

    //printf("np: %d | ds: %d\n", numOfPrimes, distSum);
    //Calculate distance
    double avgDist = (double)distSum/(numOfPrimes-1);

    printf("Average distance between primes = %.2f\n", avgDist);


    //Dealocate array
    free(numbers);
}


// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
  if(argc == 2)
    print_sieves_avrg(atoi(argv[1]));
  else
    printf("Please state an interger number.\n");

    printf("\n");
  return 0;
}
