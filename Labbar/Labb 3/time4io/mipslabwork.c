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

int mytime = 0x5957;
volatile int* portE = (volatile int *) 0xbf886110;


char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  volatile int* trisE = (volatile int *) 0xbf886100;

  //set leds as outputs and to 0 (bit 7-0)
  *trisE &= 0xfffff00;
  *portE &= 0xfffff09;

  //set bit 11-5 as input
  TRISDSET =  0xfe0;

  //set button 1 as input
  TRISFSET = 0x2;

  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  delay( 1000 );

  int buttons = getbtns();

  if (buttons){

    int switches = getsw();
    // button 1
    if (buttons & 0x1){
      mytime &= 0xfff0;
      mytime |= switches;
    }

    // button 2
    if (buttons & 0x2){
      mytime &= 0xff0f;
      mytime |= switches << 4;
    }

    // button 3
    if (buttons & 0x4){
      mytime &= 0xf0ff;
      mytime |= switches << 8;
    }

    //button 4
    if (buttons & 0x8){
      mytime &= 0x0fff;
      mytime |= switches << 12;
    }
  }

  time2string( textstring, mytime );
  //sprintf(textstring, "Updates = %d", updates);
  display_string( 3, textstring );
  display_update();
  tick( &mytime );
  display_image(96, icon);

  //
  if((*portE & 0xff) == 0xff)
    *portE &= 0xfffff00;
  else
    (*portE)++;
}
