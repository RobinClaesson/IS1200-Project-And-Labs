#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  if(argc == 2) {

    int i = atoi(argv[1]);
    int increment = 1;
    while(1)
    {
      volatile int a[i];
      a[0] = 10;
      printf("%d\n", i);
      i+= increment;
    }

    }
  else
    printf("Please state interger number.\n");

    printf("\n");
  return 0;
}
