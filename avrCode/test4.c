#include "util/delay.h"
#include "avr/io.h"
//BRITELITE CONNECTIONS FOR PORT A

#define BDI 0
#define BLI 1
#define BEI 2
#define BCI 3


#define MAXBRIGHTNESS 0b111111111111
#define MINBRIGHTNESS 0b0
#define PINLOW(a) PORTA &= !(1 << a); //_delay_ms(10); 
#define PINHIGH(a) PORTA |= 1<<a; //_delay_ms(10);



void setBrightness(unsigned int brightness) {

	int curBrightnessBit;

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


void setRGBColor(unsigned int red, unsigned int green, unsigned int blue) {
	setBrightness(red);
	setBrightness(blue);
	setBrightness(green);
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
	int i;
	unsigned char turnOn;
	turnOn = 0;
	char l1;
	l1 = 1; 


	unsigned int brightness1 = 0;
	unsigned int brightness2 = MAXBRIGHTNESS;
	unsigned int brightness3 =0;
	
	unsigned int *brightnessIncrease = &brightness1;
	unsigned int *brightnessDecrease = &brightness2;
	while(1) {

		
		setRGBColor(brightness1,0,brightness2);
		setRGBColor(0,brightness1,brightness2);


		//move shift register contents to LED 
		//pwm contents
		PINHIGH(BLI);
		PINLOW(BLI);


		l1++;
		if(l1> 3) {
			l1 = 1;
		}
		

		//update colors
		
		
		
		if(*brightnessDecrease > 10) {
			*brightnessDecrease -= 10;
			*brightnessIncrease += 10;
		}
		else {
			unsigned int *tempPtr;
			tempPtr = brightnessDecrease;
			brightnessDecrease= brightnessIncrease;
			brightnessIncrease = tempPtr;
			
			*brightnessIncrease = 0;
			*brightnessDecrease= MAXBRIGHTNESS;
			
			
		}

		
		
		_delay_ms(10);
	}
 

	//keep on looping...
	while(1) {
	}
	return 1;
}





