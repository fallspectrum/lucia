#include <avr/io.h>
#include <avr/interrupt.h>

#include "utils.h"

char outputCompareBitmask = 1 << OCF1A;

int abs(int number) {
	int returnVal;
	if(number < 0) {
		returnVal = number * -1;
	}
	else {
		returnVal = number;
	}
	return returnVal;
}



//THIS SHOULD BE CALLED TO SET CORRECT COLORS
//FOR EACH LED! THE INNER LEDS INSIDE THE CASING
//AREN'T IN RGB ORDER!
void setRGBColor(int led, int red, int green, int blue) {
	
	//this is used to switch palm and thumb, software patch
	if(led >= 6) {
		if(led == 6) {
			led = 11;		
		}
		
		else {
			led--;
		}
	}


	
	short *pwmPtr = &ledPWMArray[led * 3];
	pwmPtr[0] = red;
	pwmPtr[2] = green;
	pwmPtr[1] = blue;



}

void getRGBColor(int led, struct rgbColors* aColorsStruct) {

	//this is used to switch palm and thumb, software patch
	if(led >= 6) {
		if(led == 6) {
			led = 11;		
		}
		
		else {
			led--;
		}
	}

	



	short *pwmPtr = &ledPWMArray[led * 3];
	aColorsStruct->red = pwmPtr[0];
	aColorsStruct->green = pwmPtr[2];
	aColorsStruct->blue = pwmPtr[1];
	
	
	
}

//outputs colors for leds using octobrite
//outputs the appropiate colors for each led
void displayColors() {

int pwmIndex;
	int brightness;
	int pwmCount;
	int curBrightnessBit;

	pwmCount = LEDCOUNT * 3;
	for(pwmIndex = pwmCount -1;pwmIndex >= 0; pwmIndex --) {
	
		brightness = ledPWMArray[pwmIndex];

		//we want the high brightness for the bits
		brightness = brightness << 4;
		if(brightness & 0x08) {
			brightness |= 0x0E;
		}

		for(curBrightnessBit=12; curBrightnessBit > 0; curBrightnessBit--) {

		
			//get the brightness bit
			unsigned char outputBit  = (brightness >> (curBrightnessBit - 1)) & 0x1;

	 
			if(outputBit == 1 ) {
				PINHIGH(BDI);
			}
			else {
				PINLOW(BDI);
			}

	
			//generate clock signal
			PINHIGH(BCI);

			PINLOW(BCI);
		}
		
		//We cleaned up the octrobright a bit, so we skipped a few connections
		if(pwmIndex == 18) {
			PINLOW(BDI);
			//for(int i =0; i < 105; i++) {
			for(int i =0; i < 72; i++) {
				PINHIGH(BCI);
				PINLOW(BCI);

			}
		}
	}
	//move shift register contents to LED
	//pwm contents
	PINHIGH(BLI);
	PINLOW(BLI);
}




//led effects

//Fades back and forth between to colors
//If esetEffect is not true then
//old colors are used

/*
void fadeBetweenColors(int ledNumber, //led number to apply effect to
					   int br, //begin red color
					   int bg, //begin green color
					   int bb, //begin blue color
					   int er, //end red color
					   int eg, //end green color
					   int eb, //end blue color
					   unsigned int stepCount //number of steps from being to end color
					   ) 
*/





void red60BPM() {
	int counter = 0;

	setRGBColor(0,0,0,0);
	setRGBColor(1,0,0,0);
	displayColors();

	setRGBColor(0,100,0,0);
	setRGBColor(1,100,0,0);
	while(counter < 50) {
		waitForTimer();
		counter++;
	}
	displayColors();
	
	setRGBColor(0,0,0,0);
	setRGBColor(1,0,0,0);	

	counter = 0;
	setRGBColor(0,0,0,0);
	setRGBColor(1,0,0,0);
	while(counter < 50 ) {
		waitForTimer();
		counter++;
	}
	displayColors();
}

void waitForTimer()  {
	//wait for 2ms to pass and then 
	//output led data
	//this flag is 0 when no match 
	//occured
	while(!(outputCompareBitmask & TIFR)) {
	} 
	TIFR = (1 << OCF1A);
}

void copy_data(char *dest, char *src, short length) {
	short i;
	for(i = 0; i < length; i ++){
		dest[i] = src[i];
	}
}


void copy_short_data(short *dest, short *src, short count) {
	short i;
	for(i = 0; i < count; i ++){
		dest[i] = src[i];
	}
}
