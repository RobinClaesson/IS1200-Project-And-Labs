//2s about 900 000
//10s about 4 700 000

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

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
  if(n < 2)
  printf("Enter a integer larger than 1\n");

  else
  {
     bool* numbers = (bool*)malloc(sizeof(bool)*(n-1));
     for(int i = 2; i <= n; i++)
      numbers[i-2] = 1;

    for(int i = 2; i < sqrt(n); i++)
    {
        //print_number(i);
        if(numbers[i-2])
        {
            for(int j = i*i; j <= n; j += i)
            {
                numbers[j-2] = 0;
            }
        }
    }

    int primes = 0;
    int steps = 0;
    int sumSteps = 0;

    for(int i = 1; i < n-1; i++)
    {
      steps++;

      if(numbers[i]){
        sumSteps += steps;
        steps = 0;
        primes++;
      }
    }

    double avgSteps = (double)sumSteps/(primes);
    printf("primes: %d, sum: %d\n", primes, sumSteps);
    printf("Average steps between primes: %.2f\n", avgSteps);

    free(numbers);

  }
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
