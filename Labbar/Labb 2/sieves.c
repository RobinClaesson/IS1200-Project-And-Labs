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

void print_sieves(const int n)
{
    int arrayLength = n-2;
    int numbers[arrayLength];

    for(int i = 0; i <= arrayLength; i++){
      numbers[i] = i + 2;
    }

    //select first prime
    int selected = 0;
    while(selected != -1)
    {
      //Printing current selected prime
      print_number(numbers[selected]);

      //Marking multiples of selected
      for(int i = selected + numbers[selected]; i < arrayLength; i += numbers[selected])
        numbers[i] = -1; //-1 marks as not prime

      //Finds next prime in array if there is any
      int i = selected + 1;
      selected = -1;
      while (selected == -1 && i < arrayLength) {

        if(numbers[i] != -1)
          selected = i;

          i++;
      }
    }

}


// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
  if(argc == 2)
    print_sieves(atoi(argv[1]));
  else
    printf("Please state an interger number.\n");

    printf("\n");
  return 0;
}