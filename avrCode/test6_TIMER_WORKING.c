#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

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

void function waitForTimer()  {
	//wait for 10ms to pass and then 
	//output led data
	//this flag is 0 when no match 
	//occured
	while(!(outputCompareBitmask & TIFR)) {
	} 
	TIFR = (1 << OCF1A);
}


int main(void) {

	//set all pins of port A to output
	DDRA=0xff;

	
	//set all PORTA to low
	PORTA = 0x00;


	//bring enable pin to HIGH to
	//disable leds on octrobrite
	//PINHIGH(BEI);

	//Bring BEI pin of octobrite 
	//to enable  leds on board
	PINLOW(BEI);
	
	//lets turn off led.
	unsigned char turnOn;
	turnOn = 0;
	char l1;
	l1 = 1; 


	//reset all the effects at the beginning...

	resetEffect = TRUE;


	//setup clock to generate flag
	//every 10ms
	//16MHz * 10ms = 160000 main clock cycles
	//Setting a 16 bit prescalar
	//16000 / 65536 =  2.4414...
	//Lowest prescalar divide amount is 8 

	//disable all interrupts
	//_CLI();

	//TCCR1B
	//NOTE: THIS START THE TIMER!
	//MAKE SURE TO SET WANTED SETTINGS FIRST
	//Groups are marked right to left
	//0b[0][0][0][00][010]
	//Bits 2:0	Set clock source of timer 1 to use
	//			prescalar value of 8
	//
	//Bits 4:3 	Waveform Generation Mode 4 (CTC)
	//
	//Bit  5:5 	Reserved, must be 0
	//
	//Bit  6:6 	Input Capture Edge Select, use
	//			falling edge
	//Bit  7:7	Set Input capture noise to off
	TCCR1B |= (1 << WGM12);
	
	
	
	//TTCCR1A
	//0b[00][00][00][00]
	//Bits 7:2	Disable compare output for
	//			all channels
	//
	//Bits 1:0 Set timer/counter mode to 4 (CTC)
	TCCR1A = (1 << COM1A0);
	

	//SET output compare register to match every
	//10ms. (16Mhz /8 = 2Mhz * 10ms = 20000)
	OCR1A = 0x4E20;
	
	
	
	
	//TCCR1c
	//0b[000][00000]
	 
	//Bits 7:5  Disable force compare for
	//			channels a-c
	//Bits 4:0 	Reserved. Must be 0.

	//TIMSK
	//Timer/Counter interrupt mask
	//0b00000000
	//Mask all timer interrupts

	//ETIMSK
	//Extended timer/counter interrupt mask
	//0b00000000
	//Mask all timer/counter interrupts


	

	//reenable all interrupts
	//_SEI();

	
	//Calculate Output 1 compare flag
	//bitmask
	char outputCompareBitmask = 1 << OCF1A;
	
	//Do first effect...
	fadeBetweenColors(0,100,0,0,0,0,100,100);
	fadeBetweenColors(1,100,0,0,0,0,100,100);
	displayColors();
	resetEffect = FALSE;

	
	
	
	int timeCounter =0;
	//Start the timer
	TCCR1B |= ((1 << CS11));

	//keep on looping...
	while(1) {
		
		//Matches at number of 10ms - 1
		if(timeCounter < 4999) {
			fadeBetweenColors(0,100,0,0,0,0,100,100);
			fadeBetweenColors(1,100,0,0,0,0,100,100);
			waitForTimer();
			displayColors();
			timeCounter++;
		}
				
		//Get the next effect ready...
		//fadeBetweenColors(0,0,0,100,100,0,100,1000);
		//fadeBetweenColors(1,0,0,100,100,0,100,1000);
		//fadeBetweenColors(1,0,0,MAXBRIGHTNESS / 10,MAXBRIGHTNESS /10,0,MAXBRIGHTNESS /10,100);
		//setRGBColor(0,0x00,0,0x0f);
		


		//display colors
		
		
		//reset flag
		
		
	}
	return 1;
}




