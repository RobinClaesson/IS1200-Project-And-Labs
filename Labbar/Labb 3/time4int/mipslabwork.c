/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int prime = 1234567;
int mytime = 0x5957;
volatile int* portE = (volatile int *) 0xbf886110;
int timecount = 0;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void ) {

  timecount++;
  if (timecount == 10){
    timecount = 0;
    time2string( textstring, mytime );
    display_string( 3, textstring );
    display_update();
    tick( &mytime );
  }

  // Clear Timer 2 flag
  //if (IFS(0) & 0x100){
  IFS(0) &= 0xfffffeff;
  //}
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  volatile int* trisE = (volatile int *) 0xbf886100;

  //set leds as outputs and to 0 (bit 7-0)
  *trisE &= 0xfffff00;
  *portE &= 0xfffff09;

  // set bit 11-5 as input
  TRISDSET =  0xfe0;

  // set button 1 as input
  TRISFSET = 0x2;

  // timer 2 clear
  T2CON = 0x0;

  // set as ON
  T2CONSET = 1 << 15;

  // set the prescale to 256
  T2CONSET = 0x7 << 4;

  // set period register to 31250
  PR2 = 31250;

  IEC(0) = (1 << 8);
  IPC(2) = 4;

  enable_interrupt();

  return;
}

/* This function is called repetitively from the main program */
void labwork( void ) {
  prime = nextprime( prime );
  display_string( 0, itoaconv( prime ) );
  display_update();
}
