//Clock cycle count used to time program
//The atmega128 runs at 16mhz
//A prescalar value of 8 is
//set defined during the main() function.
//This number below specifies how many
//sets of 8 clock cycles will the program 
//have until it's time to display the
//time effect. Setting this to too low
//of a value will not give succificent
//times for effect calculations, causing
//unpredictiable behavior
#define CLOCKCOUNT 0x0fa0;

//Amount of CLOCKCOUNT's needed
//to equal 10ms
#define TEN_MS_MULTIPLIER 10


//Maximum number of group counts
//TODO no checks are made against this value
//this is only to properly alloc an array
//used to hold each group's data size
#define MAX_GROUP_COUNT 255

//number of leds used
#define LEDCOUNT 12
