#include <avr/io.h>
#include <avr/interrupt.h>

#include "utils.h"
#include "instruction_decoder.h"
#include "effects.h"
#include "config.h"
#include "string.h"


//#include "mteden.h"

#define BITMASK_BACKWARD 0x01
#define BITMASK_FORWARD 0x02

void doEffects(struct effect_info *ledInstructions);

 int main(void) {


	//Set all pins of port G to input
	DDRG=0x00;

	//enable pull-up resistors 
	PORTG = 0xff;

	
	int switchStates = 0x00;


	
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

	//resetEffect = TRUE;


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
	//OCR1A = 0x4E20;
	//OCR1A = 0x07d0;
	OCR1A = CLOCKCOUNT;
	//OCR1A = 0x0fa0;
	
	
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
	
	
	//Start the timer
	TCCR1B |= ((1 << CS11));

	//keep on looping...

	//play the song here..
/*
	while(1){
		red60BPM();
	}
*/
	/*
	//test blink effect
	
	*/	




//Test full brightness of all leds via manual control of TLC5947
//outputs colors for leds using octobrite
//outputs the appropiate colors for each led
//BEGIN

int pwmIndex;
int brightness;
int pwmCount;
int curBrightnessBit;

pwmCount = LEDCOUNT * 3;
/*
int dummyCounter = 0;
for(pwmIndex = pwmCount -1;pwmIndex >= 0; pwmIndex --) {

	brightness = 0xffff;

	//we want the high brightness for the bits
	brightness = brightness << 4;

	for(curBrightnessBit=12; curBrightnessBit > 0; curBrightnessBit--) {

		//ALWAYS HAVE HIGH OUTPUT FOR BIT, MAX PWM

		for(int breakVar=0;breakVar<1000;breakVar++) {
			dummyCounter++;
		}
		PINHIGH(BDI);
		for(int breakVar=0;breakVar<1000;breakVar++) {
						dummyCounter++;
		}
		PINHIGH(BCI);
		for(int breakVar=0;breakVar<1000;breakVar++) {
						dummyCounter++;
		}
		PINLOW(BCI);
		for(int breakVar=0;breakVar<1000;breakVar++) {
						dummyCounter++;
		}
	}

	
}
//move shift register contents to LED
//pwm contents
PINHIGH(BLI);
for(int breakVar=0;breakVar<1000;breakVar++) {
			dummyCounter++;
}
PINLOW(BLI);
while(1) {
}
//END
*/




/*
	for (int i =0; i < LEDCOUNT; i++) {


				setRGBColor(i,255,255,255);
		
	}
	displayColors();
	while(1) {
		waitForTimer();
	}

*/


/*
	//
	//light beam test
	short data[] = {
	2,
	255,0,0,30,
	0,0,255,30,
	255,255,0,30,};
	for(int i=0; i < LEDCOUNT; i++) {
		resetLightBeamEffect(i,data);
	}
	short tenMsCounter =0;
	while(1) {
			for(int i =0; i < LEDCOUNT; i++) {
				lightBeamEffect(i,data);
			}
			displayColors();
			waitForTimer();

	
	}
	*/	
	
	//test time sync
	/*
	short off = 0;
	while(1) {
		for(int i = 0; i < (100 * TEN_MS_MULTIPLIER); i++) {
			waitForTimer();
		}
		if(off) {
			setRGBColor(1,0,0,0);
		}
		else {
			setRGBColor(1,255,0,0);
		}
		displayColors();
		off = !off;
	}
	*/
	
	/*
	//test for pwm 
	char led1[] = {
	0xff,0x00, //red
	0x00,0x00, //green
	0x00,0x00, //blue
	0xf4,0x01, //on time
	0xf4,0x01}; //off time
	resetPwmEffect(0,(short *)led1);
	resetPwmEffect(1,(short *)led1);
	//setRGBColor(1,255,0,0);
	displayColors();
	int aCount = 0;
	while(aCount < 488){
		aCount++;
		pwmEffect(0,(short *)led1);
		pwmEffect(1,(short *)led1);
		waitForTimer();
		displayColors();
	}
	resetPwmEffect(1,(short *)led1);
	while(1) {
		pwmEffect(0,(short *)led1);
		pwmEffect(1,(short *)led1);
		waitForTimer();
		displayColors();
	}
*/	

/*
	
	//test for scaling pwm effect
	short led0[] = {
	255,0,0,
	0,0,255,
	1500,
	PWM_SCALE_UP,
	150,};

	 
	resetFadeBetweenColorsWithScalingPwmEffect(0,led0);
	displayColors();
	while(1){
		fadeBetweenColorsWithScalingPwmEffect(0,led0);
		waitForTimer();
		displayColors();
	}
	
	*/

	



	//fade between colors test
   /*
	short testData[7] = {255,0,00,0,255,0,100};
	
	int ledTest;
	for(ledTest = 0; ledTest < LEDCOUNT; ledTest++) {
		resetEffect = TRUE;
		fadeBetweenColors(ledTest,testData);
		resetEffect = FALSE;
	}	 
	displayColors();
	while (1) {
		for(ledTest = 0; ledTest < LEDCOUNT; ledTest++) {
			fadeBetweenColors(ledTest,testData);
		}
		for(int i=0; i < TEN_MS_MULTIPLIER; i++) {
			waitForTimer();
		}
		displayColors();
	}

	*/
	


	/*
	//test for fade between colors with pwm effect
	short testData[] = {
		255, //red begin
		0, //green begin
		0, //blue begin
		0, //red end
		0, //green end
		255, //blue end
		100, //1 second (100 10ms...)
		10, //on length PWM
		10, //off length PWM
	};
	
 	resetFadeBetweenColorsWithPwm(0,testData);
	while(1) {
		fadeBetweenColorsWithPwm(0,testData);
		waitForTimer();
		displayColors();
	}
	*/

	/*
	//test fade between 3 colors with pwm scaling
	//test for fade between colors with pwm effect
	short testData[] = {
		255,0,0,
		0,255,0,
		0,0,255,
		100,
		1,
		25,
	};
	resetFadeBetween3ColorsWithScalingPwmEffect(0,testData);
	while(1) {
		fadeBetween3ColorsWithScalingPwmEffect(0,testData);
		waitForTimer();
		displayColors();
	}
	*/

	initDecoder();

	struct instruction_group *groupNew;
	
	//holds function pointer and data pointer for each led
	struct effect_info ledInstructions[LEDCOUNT];

	//set each led instruction to do nothing at first
	short i;
	for(i=0; i < LEDCOUNT; i++) {
		ledInstructions[i].effectFunc = ledOffEffect;
		ledInstructions[i].effectResetFunc = resetLedOffEffect;
		ledInstructions[i].effectData = 0;
		ledInstructions[0].tenMsTriggerOnly = TRUE;
	}

	//setup functionPointers	
	short deltaTimer;
	
	
/*
	short effectData[] = {
	255,0,0,
	0,255,0,
	100,
	1,
	25};

	ledInstructions[0].effectFunc = fadeBetweenColorsWithScalingPwmEffect;
	ledInstructions[0].effectResetFunc = resetFadeBetweenColorsWithScalingPwmEffect;
	ledInstructions[0].effectData = effectData;
	ledInstructions[0].tenMsTriggerOnly = FALSE;
	ledInstructions[0].effectResetFunc(0,effectData);
	while(1) {
		doEffects(ledInstructions);
	//	waitForTimer();
		displayColors();
	}
*/


	groupNew = getNextGroup();

	//used to track is held down first so we can 
	//straddle correctly
	int firstHeld = -1;

	while(1) {

		//setup new effects
		struct instruction *tmpInstruction;
		tmpInstruction = groupNew->firstInstruction;
		char ledSet[LEDCOUNT];
		memset(&ledSet,0,sizeof(char) * LEDCOUNT);
		do {
			short ledNumber = (short) tmpInstruction->ledNumber;
			ledSet[ledNumber] = 1;
			ledInstructions[ledNumber].effectFunc = tmpInstruction->effectInfo.effectFunc;
			ledInstructions[ledNumber].effectData = tmpInstruction->effectInfo.effectData;
			ledInstructions[ledNumber].tenMsTriggerOnly = tmpInstruction->effectInfo.tenMsTriggerOnly;
			tmpInstruction->effectInfo.effectResetFunc(ledNumber,ledInstructions[ledNumber].effectData);
			tmpInstruction = tmpInstruction->nextInstruction;
		} while (tmpInstruction != 0);
		
		//all leds are turned off it not explicity set
		for(int i=0; i < LEDCOUNT; i++) {
			if(ledSet[i] == 0) {
				ledInstructions[i].effectFunc = ledOffEffect;
				ledInstructions[i].effectData = NULL;
				ledInstructions[i].tenMsTriggerOnly = TRUE;
			}
		}


		deltaTimer = 0;
				
		int justReleasedMask = 0;
		//keep doing current effect until forward button is pressed
		while(1) {
			
			//because of switch bouncing we need
			//to wait unti 10ms intervals
			//to check a switch's state
			if(deltaTimer >= TEN_MS_MULTIPLIER) {
				//check if forward button is pressed down
				//because of pullup resistors 
				//0 is on and 1 is off
				//FIXED: WAS USING PORT B, BUT CAUSED A ISSUE WHILE PROGRAMMING
				//BECAUSE IT USED THE SAME PORT. SO, DON'T USE PORT B!
				if((~PING) & BITMASK_FORWARD) {
					//make sure the forward button isn't already
					//pressed down
					if((switchStates & BITMASK_FORWARD) == 0 ) {
						switchStates = switchStates | BITMASK_FORWARD;
						
						if(firstHeld < 0) {
							firstHeld  = BITMASK_FORWARD;
						}

						groupNew = getNextGroup();
						
						break;
					}
				}
				
				//forward button is not being pressed down
				else {

					//if the forward button was just released and
					//the back button is sitll being pressed, move backwards
					if(switchStates & BITMASK_FORWARD) {
						justReleasedMask |= BITMASK_FORWARD;
					}
					switchStates = switchStates & (~BITMASK_FORWARD);
					
				}
				
				if((~PING) & BITMASK_BACKWARD) {
					if((switchStates & BITMASK_BACKWARD)== 0) {
						switchStates = switchStates | BITMASK_BACKWARD;
						groupNew = getPreviousGroup();

						if(firstHeld < 0) {
							firstHeld = BITMASK_BACKWARD;
						}

						break;
					}
				}
				else {
					
					//if the back button was just released and
					//the foward button is being pressed, move foward
					if(switchStates & BITMASK_BACKWARD) {
						justReleasedMask |= BITMASK_BACKWARD;
					}
					switchStates = switchStates & (~BITMASK_BACKWARD);
				}

				
				if( justReleasedMask & BITMASK_FORWARD) {
					if(firstHeld == BITMASK_BACKWARD) {
						if(switchStates & BITMASK_BACKWARD) {
							groupNew = getPreviousGroup();
							justReleasedMask ^= BITMASK_FORWARD;
							break;
						}
					}
				}

				else if( justReleasedMask & BITMASK_BACKWARD)  {
					if( firstHeld == BITMASK_FORWARD)  {
						if(switchStates & BITMASK_FORWARD) {

							groupNew = getNextGroup();
							justReleasedMask ^= BITMASK_BACKWARD;
							break;
						}
					}
				}
				
				deltaTimer = 0;
			}

			if(switchStates == 0) {
				firstHeld = -1;
			}

			doEffects(ledInstructions);
			waitForTimer();
			displayColors();
			deltaTimer++;
		}
	}

	//if no more effects then
	//just keep using the last one
	while(1) {
		doEffects(ledInstructions);
		waitForTimer();
		displayColors();
	}


	return 1;
}




/**
*Preforms the set instruction for each led
*/
void doEffects(struct effect_info *ledInstructions) {
	static int timerScalingCounter = 0;
	int i;
	short resetScalingCounter  = FALSE;
	//call each effect function
	for(i = 0; i < LEDCOUNT; i++){	
		if (ledInstructions[i].tenMsTriggerOnly == TRUE) {
			if (timerScalingCounter  >=  TEN_MS_MULTIPLIER) {
				ledInstructions[i].effectFunc(i,ledInstructions[i].effectData);
				resetScalingCounter = TRUE;
			}
		}
		else {
			ledInstructions[i].effectFunc(i,ledInstructions[i].effectData);
		}
	}
	
	if(resetScalingCounter == TRUE) {
		timerScalingCounter = 0;
	}
	else {
		timerScalingCounter++;
	}
}


