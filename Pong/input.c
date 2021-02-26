#include <pic32mx.h>

//-----------------------------------------------
// Global variable
//-----------------------------------------------
int btn_state = 0, old_btn_state = 0;
int swt_state = 0, old_swt_state = 0;

//-----------------------------------------------
// Init and get values from chipkit
//-----------------------------------------------
void input_init(){
  //set buttons 2-4 and the switches as inputs
  TRISDSET =  0xfe0;

  //set button 1 as input
  TRISFSET = 0x2;
}

// returns value of the 4  on-board switches
int get_switches(){
  return (PORTD &= 0xf00) >> 8;
}

// returns the value of 4 of the on-board buttons
int get_buttons(){
  return ((PORTD &= 0xe0) >> 4) | ((PORTF &= 0x2) >> 1);
}

//-----------------------------------------------
// Update function
//-----------------------------------------------
void update_input()
{
    old_btn_state = btn_state;
    old_swt_state = swt_state;

    btn_state = get_buttons();
    swt_state = get_switches();
}

//-----------------------------------------------
// Get button input
//-----------------------------------------------
//Button 1
int btn1_down()//Is down
{
    return btn_state & 0x1;
}
int btn1_pressed() //Single press
{
    return (btn_state & 0x1) && (~old_btn_state & 0x1);
}

//Button 2
int btn2_down() //Is down
{
    return btn_state & 0x2;
}
int btn2_pressed() //Single press
{
    return (btn_state & 0x2) && (~old_btn_state & 0x2);
}

//Button 3
int btn3_down()//Is down
{
    return btn_state & 0x4;
}
int btn3_pressed()//Single Press
{
    return (btn_state & 0x4) && (~old_btn_state & 0x4);
}

//Button 4
int btn4_down()//Is down
{
    return btn_state & 0x8;
}
int btn4_pressed()//Single Press
{
    return (btn_state & 0x8) && (~old_btn_state & 0x8);
}

//-----------------------------------------------
// Get switch input
//-----------------------------------------------

//Switch 1
int swt1_on() //Is on
{
    return swt_state & 0x1;
}
int swt1_toggl()//Toggled on
{
    return (swt_state & 0x1) && (~old_swt_state & 0x1);
}

//Switch 2
int swt2_on() //Is on
{
    return swt_state & 0x1;
}
int swt2_toggl()//Toggled on
{
    return (swt_state & 0x2) && (~old_swt_state & 0x2);
}

//Switch 3
int swt3_on() //Is on
{
    return swt_state & 0x1;
}
int swt3_toggl()//Toggled on
{
    return (swt_state & 0x4) && (~old_swt_state & 0x4);
}

//Switch 4
int swt4_on() //Is on
{
    return swt_state & 0x1;
}
int swt4_toggl()//Toggled on
{
    return (swt_state & 0x4) && (~old_swt_state & 0x4);
}