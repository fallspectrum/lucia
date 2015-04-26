#include "util/delay.h"
#include "avr/io.h"

#include "utils.h"

//BRITELITE CONNECTIONS FOR PORT A

#define BDI 0
#define BLI 1
#define BEI 2
#define BCI 3


#define MAXBRIGHTNESS 0b111111111111
#define MINBRIGHTNESS 0b0
#define PINLOW(a) PORTA &= !(1 << a) //_delay_ms(10); 
#define PINHIGH(a) PORTA |= 1<< a //_delay_ms(10);


#define TRUE 1
#define FALSE 0

//number of leds used
#define LEDCOUNT 2

//contains pwm values for each led's color
int ledPWMArray[LEDCOUNT * 3];

//Some effects keep persitent data
//To start the effect from scratch set this to true
int resetEffect;


//outputs the appropiate colors for each led
void displayColors() {

	int pwmIndex;
	int brightness;
	int pwmCount;
	int curBrightnessBit;

	pwmCount = LEDCOUNT * 3;
	for(pwmIndex = 0;pwmIndex < pwmCount; pwmIndex ++) {
	
		brightness = ledPWMArray[pwmIndex];
		for(curBrightnessBit=12; curBrightnessBit > 0; curBrightnessBit--) {

		
			//get the brightness bit
			unsigned char outputBit  = (brightness >> curBrightnessBit) & 0x1;
	 
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
	}
	//move shift register contents to LED
	//pwm contents
	PINHIGH(BLI);
	PINLOW(BLI);
}


//THIS SHOULD BE CALLED TO SET CORRECT COLORS
//FOR EACH LED! THE INNER LEDS INSIDE THE CASING
//AREN'T IN RGB ORDER!
void setRGBColor(int led, int red, int green, int blue) {
	int *pwmPtr = &ledPWMArray[led * 3];
	pwmPtr[0] = red;
	pwmPtr[1] = blue;
	pwmPtr[2] = green;
}


//Fades back and forth between to colors
//If esetEffect is not true then
//old colors are used
void fadeBetweenColors(int ledNumber, //led number to apply effect to
					   int br, //begin red color
					   int bg, //begin green color
					   int bb, //begin blue color
					   int er, //end red color
					   int eg, //end green color
					   int eb, //end blue color
					   unsigned int stepCount //number of steps from being to end color
					   ) {
	
	static float savedColors[LEDCOUNT * 3];
	
	static int currentStep;
	
	static int direction;

	float *cr;
	float *cg;
	float *cb;
	int ledIndex;
	
	//set up pointers for local color references
	ledIndex = (ledNumber * 3);
	cr = &savedColors[ledIndex];
	cg = &savedColors[ledIndex + 1];
	cb = &savedColors[ledIndex + 2];
	
	
	//set current colors to beginning colors
	//on reset
	if(resetEffect == TRUE) { 
		*cr = br;
		*cg = bg;
		*cb = bb;
		currentStep = 0;
		direction = 1;
	
	}
	
	
	//get the differences between end and beginning color
	float dr = er - br;
	float dg = eg - bg;
	float db = eb - bb;

	//get the amount of change
	//for each color
	float sr = dr / (float)stepCount;
	float sg = dg / (float)stepCount;
	float sb = db / (float)stepCount;

	//calculate the current value for each color
	*cr = (float) br + (sr * (float) currentStep );
	*cg = (float) bg + (sg * (float) currentStep );
	*cb = (float) bb + (sb * (float) currentStep );


	setRGBColor(ledNumber,(int) *cr,(int) *cg,(int) *cb);
	
	
	//if we are moving from beginning color
	//to end color
	if(direction == 1) {
		currentStep++;
		if(currentStep > stepCount) {
			direction = 0;
			currentStep = stepCount;
		}
	}

	//we are moving from end color 
	//to beginning color
	else {
		currentStep--;
		if(currentStep < 0)  {
			currentStep = 0;
			direction = 1;
		}
	}
}

int main(void) {

	//all pins to output
	DDRA=0xff;

	
	//set all pins low
	PORTA = 0x00;


	//bring enable pin to HIGH
	//PINHIGH(BEI);

	//Bring enable to low
	PINLOW(BEI);
	
	//lets turn off led.
	unsigned char turnOn;
	turnOn = 0;
	char l1;
	l1 = 1; 


	//reset all the effects at the beginning...

	resetEffect = TRUE;
	//keep on looping...
	while(1) {

		//for each led output the color
		fadeBetweenColors(0,0,0,100,100,0,100,200);
		fadeBetweenColors(1,0,0,100,100,0,100,200);
		//fadeBetweenColors(1,0,0,MAXBRIGHTNESS / 10,MAXBRIGHTNESS /10,0,MAXBRIGHTNESS /10,100);
		//setRGBColor(0,0x00,0,0x0f);
		//ledPWMArray[0] = MAXBRIGHTNESS;
		//ledPWMArray[1] = MAXBRIGHTNESS;
		//ledPWMArray[2] = 0;
		displayColors();
		resetEffect = FALSE;
		_delay_ms(10); 
		//check if we should do the next function
	}
	return 1;
}




