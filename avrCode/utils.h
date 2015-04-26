#ifndef _H_UTILS
#define _H_UTILS

#include "config.h"

//contains pwm values for each led's color
short ledPWMArray[LEDCOUNT * 3];


//PIN INFO
//BRITELITE CONNECTIONS FOR PORT A

#define BDI 0
#define BLI 1
#define BEI 2
#define BCI 3

//PIN UTILS
#define PINLOW(a) PORTA &= !(1 << a) //_delay_ms(10); 
#define PINHIGH(a) PORTA |= 1<< a //_delay_ms(10);



//Some effects keep persitent data
//To start the effect from scratch set this to true
char resetEffect;


int abs(int number);

//waits for the counter 
//to get to 10ms
void waitForTimer();



//sends pwm buffer to octobrite
void displayColors();

//led effects 
struct rgbColors {
	int red;
	int green;
	int blue;
};
void setRGBColor(int led, int red, int green, int blue);
void getRGBColor(int led, struct rgbColors* savedColors);


//MAX BRIGHTNESS SUPPORTED BY OCTOBRITE
#define MAXBRIGHTNESS 0b111111111111
#define MINBRIGHTNESS 0b0


void red60BPM();


//copies bytes from src to dest
void copy_data(char *dest, char *src, short length);


//copies short values from src to dest
void copy_short_data(short *dest, short *src, short count);
#define TRUE 1
#define FALSE 0
#endif

