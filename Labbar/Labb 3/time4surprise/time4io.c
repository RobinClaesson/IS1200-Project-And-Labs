#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw(void);
int getbtns(void);

int getsw (){
  // return value of the 4  on-board switches
  return (PORTD &= 0xf00) >> 8;
}

int getbtns(){
  // return the value of 3 of the on-board buttons
  return ((PORTD &= 0xe0) >> 4) | ((PORTF &= 0x2) >> 1);
}
