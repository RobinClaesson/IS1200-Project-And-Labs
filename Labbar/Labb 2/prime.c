/*
 prime.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>

int is_prime(int n){
  /*
  Loops through every number snaller than n from 2 and up
  If there is no remainder after deviding with i then n is not a prime
  If no number that has no remainder then n is a prime
  */
  if(n < 2)
    return 0;

  for(int i = 2; i < n; i++)
    if(n % i == 0)
      return 0;

  return 1;
}

int main(void){
  printf("%d\n", is_prime(11));  // 11 is a prime.      Should print 1.
  printf("%d\n", is_prime(383)); // 383 is a prime.     Should print 1.
  printf("%d\n", is_prime(987)); // 987 is not a prime. Should print 0.
}
