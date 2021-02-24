#include <pic32mx.h>
#include <stdint.h>

#define OLED_VDD PORTFbits.RF6
#define OLED_VBATT PORTFbits.RF5
#define OLED_COMMAND_DATA PORTFbits.RF4
#define OLED_RESET PORTGbits.RG9

#define OLED_VDD_PORT PORTF
#define OLED_VDD_MASK 0x40
#define OLED_VBATT_PORT PORTF
#define OLED_VBATT_MASK 0x20
#define OLED_COMMAND_DATA_PORT PORTF
#define OLED_COMMAND_DATA_MASK 0x10
#define OLED_RESET_PORT PORTG
#define OLED_RESET_MASK 0x200



#define cbOledDispMax 512   //max number of bytes in display buffer
#define ccolOledMax   128   //number of display columns
#define crowOledMax   32    //number of display rows
#define cpagOledMax   4     //number of display memory pages
