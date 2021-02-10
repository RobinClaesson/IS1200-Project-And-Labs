//2s about 900 000
//10s segment fault after 2095000? Stack full?
//https://stackoverflow.com/questions/3771154/how-to-declare-and-use-huge-arrays-of-1-billion-integers-in-c/3771162

#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COLUMNS 6

void print_number(int n) {
  static int calls = 0;
  if (calls  ==  COLUMNS){
    printf("\n");
    calls = 0;
  }

  printf("%d\t", n);

  calls++;
}

void print_sieves(const int n)
{
    if(n < 2)
    printf("Enter a integer larger than 1\n");

    else
    {
       bool numbers[n-1];
       for(int i = 2; i <= n; i++)
        numbers[i-2] = 1;

      for(int i = 2; i < sqrt(n); i++)
      {
          if(numbers[i-2])
          {
              for(int j = i*i; j <= n; j += i)
              {
                  numbers[j-2] = 0;
              }
          }
      }


      for(int i = 0; i < n-1; i++)
        if(numbers[i])
          print_number(i+2);

    }

}


// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){

  clock_t t;
   t = clock();
   printf("Timer starts\n");



  if(argc == 2)
    print_sieves(atoi(argv[1]));
  else
    printf("Please state an interger number.\n");

    printf("\n");

    printf("Timer ends \n");
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
    printf("The program took %f seconds to execute\n", time_taken);
  return 0;
}
