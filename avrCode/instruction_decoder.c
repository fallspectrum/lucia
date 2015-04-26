#ifndef _H_INSTRUCTION_DECODER
#include "instruction_decoder.h"
#endif

#include "song_instructions.h"
#include "utils.h"
#include "effects.h"
#include "config.h"
#include "string.h"
#include "avr/pgmspace.h"



//Defines how much storage is available for each instruction to use
char instructionGroup1[sizeof(struct instruction_group) + ( (sizeof(struct instruction) + DATA_STORAGE_LENGTH)  * LEDCOUNT) ];
char instructionGroup2[sizeof(struct instruction_group) + ( (sizeof(struct instruction) + DATA_STORAGE_LENGTH) * LEDCOUNT) ];

char *newInstructionGroup = instructionGroup1;
char *oldInstructionGroup = instructionGroup2;



const char *dataPtr = song_instructions;


//TODO we got to make sure in the led editor that effect group
//data is no larger then the amount of space below.
char sramEffectDataBuffer[1024];
char *sramEffectDataPtr;

unsigned short effectGroupSizes[MAX_GROUP_COUNT]; 
unsigned short currentEffectGroupIndex;
void initDecoder() {
	currentEffectGroupIndex = 0;
	memset(&effectGroupSizes,0,sizeof(unsigned short) * MAX_GROUP_COUNT);
}



struct instruction_group * getLastGroup() {
	dataPtr -= (effectGroupSizes[currentEffectGroupIndex -1]);
	return getInstructionGroup();
}

struct instruction_group *getNextGroup() {
	const char *beginPtr = dataPtr;
	struct instruction_group *ig = getInstructionGroup();

	if(ig != 0) {
		effectGroupSizes[currentEffectGroupIndex] = dataPtr - beginPtr;
		currentEffectGroupIndex++;
	}
	else {
		ig = getLastGroup();
	}
	return ig;	
}

struct instruction_group *getPreviousGroup() {

        struct instruction_group *ig;
        if(currentEffectGroupIndex >= 2 ) {

			//Gets the size of the current effect group and subtracts
			//it from the data pointer

			//dataptr is pointing th the next effect to be played..

			//we subtract the size of the last group played, this points it back to
			//the last group to be blayed. We subtract 1 more group to play the group before the
			//previous
			currentEffectGroupIndex-=1;
	        dataPtr -= (effectGroupSizes[currentEffectGroupIndex] +effectGroupSizes[currentEffectGroupIndex -1]);
			ig = getInstructionGroup();
        }
		
		else 
		{ 
			const char *tmpPtr;
			if (currentEffectGroupIndex == 1) {
				dataPtr -= effectGroupSizes[currentEffectGroupIndex -1];
				currentEffectGroupIndex -= 1;
			}
			tmpPtr = dataPtr;
			ig = getInstructionGroup();
			dataPtr = tmpPtr;
		}		
        return ig;      
}

char readCharFromFlash(const char **data)
{
    
	char returnVal = pgm_read_byte(*data);
	//char returnVal = *(char *)*data;
    *data += sizeof(char);
    return returnVal;
}

short readShortFromFlash(const char **data)
{


	
    short returnVal = pgm_read_word(*data);
	//short returnVal = *(short *)*data;
    *data += sizeof(short);
    return returnVal;
}

void copyFromFlash(const char *src, char *dest, int length) {
	const char *srcPtr = src;
	char *destPtr  = dest;
	for(int i =0; i < length; i++) {
		*destPtr = pgm_read_byte(srcPtr);
		srcPtr++;
		destPtr++;
	}
}




//This functinos returns a pointer.
//this pointer points two 2 buffers

struct instruction_group * getInstructionGroup() {	
	
	
	sramEffectDataPtr = sramEffectDataBuffer;

	char beginPacket;
	//if we aren't aligned just return null
	//end of show
	beginPacket = readCharFromFlash(&dataPtr);
	if (beginPacket != 0xff) {
		return 0;
	}
	
	

	//flip the instruction group pointers
	//around, old is new and new is old
	char *tempPtr = newInstructionGroup;
	
	newInstructionGroup = oldInstructionGroup;
	oldInstructionGroup = tempPtr;


	//this points to the linked lists of 
	//a instruction group and multiple effects
	char *curPutPtr = newInstructionGroup;
	
	//read in new instruction group
	struct instruction_group *instructionGroup = (struct instruction_group *) curPutPtr;
	unsigned int aDt = (unsigned short) readShortFromFlash(&dataPtr);
	
	

	//README - SINCE THE TIME COUNTER VALUE MAY NOT REPRESENT 10MS
	//STEPS DO TO THE PICKING OF 0CR1A WE NEED A SCALING VALUE
	//SO THAT IT WILL BE
	instructionGroup->dt = aDt*TEN_MS_MULTIPLIER;
	
	//curPutPtr is caluclated first
	//if you try to displace by instructionGroup +  sizeof(struct instruction_group)
	//it calculates it wrong....
	curPutPtr += sizeof(struct instruction_group);
	instructionGroup->firstInstruction = (struct instruction *) curPutPtr;

	


	
	//set instruction pointer to new location...
	struct instruction *instructionPointer = (struct instruction *) (curPutPtr);	
	do {
		//get the led number
		instructionPointer->ledNumber = readCharFromFlash(&dataPtr);

		//get the effect number
		instructionPointer->effectNumber = readCharFromFlash(&dataPtr);

		//set the pointer to the effect data
		instructionPointer->effectInfo.effectData = (short *)sramEffectDataPtr;

		short effectDataLength=0;
		//README
		//WHEN A EFFECT CHANGES IT'S REQUIRED LENGTH OF DATA
		//UPDATE THE OFFSETS BELOW!
	
		//Effect number 0 is do nothing
		if(instructionPointer->effectNumber == 0 ) {
			instructionPointer->effectInfo.effectFunc = doNothingEffect;
			instructionPointer->effectInfo.effectFunc = resetDoNothingEffect;
			instructionPointer->effectInfo.tenMsTriggerOnly=TRUE;
		}

		//Effect number 2 is blink
		else if(instructionPointer->effectNumber == 2 ) {
			instructionPointer->effectInfo.effectFunc = blinkEffect;
			instructionPointer->effectInfo.effectResetFunc = resetBlinkEffect;
			instructionPointer->effectInfo.tenMsTriggerOnly=TRUE;
			effectDataLength =  6; //two bytes per color, 3 colors
			
			
		}

		//Effect number 3 is PWM
		else if(instructionPointer->effectNumber == 3) {
			instructionPointer->effectInfo.effectFunc = pwmEffect;
			instructionPointer->effectInfo.effectResetFunc = resetPwmEffect;
			effectDataLength =  10; //6 color bytes, 4 timing bytes
			instructionPointer->effectInfo.tenMsTriggerOnly=FALSE;
		}

		//Effect 4 is fade between colors
		else if(instructionPointer->effectNumber == 4 ) {
			instructionPointer->effectInfo.tenMsTriggerOnly=TRUE;
			instructionPointer->effectInfo.effectFunc = fadeBetweenColors;
			instructionPointer->effectInfo.effectResetFunc = resetFadeBetweenColorsEffect;
			effectDataLength =  14; //two bytes per color, 6 colors, 2 for step count
			
		}

		//Effect 5 is fade between colors with pwm
		else if (instructionPointer->effectNumber == 5) {
			instructionPointer->effectInfo.effectFunc = fadeBetweenColorsWithPwm;
			instructionPointer->effectInfo.effectResetFunc = resetFadeBetweenColorsWithPwm;
			effectDataLength =  18; //two bytes per color, 6 colors, 2 for step count
			instructionPointer->effectInfo.tenMsTriggerOnly=FALSE;
		}
		//Instruction 6 - fade between colors with scaling pwm
		else if(instructionPointer->effectNumber == 6) {
			instructionPointer->effectInfo.effectFunc = fadeBetweenColorsWithScalingPwmEffect;
			instructionPointer->effectInfo.effectResetFunc = resetFadeBetweenColorsWithScalingPwmEffect;
			effectDataLength =  18; 
			instructionPointer->effectInfo.tenMsTriggerOnly=FALSE;
		}

		//Instruction 7 - fade between 3 colors with scaling pwm
		else if(instructionPointer->effectNumber == 7) {
			instructionPointer->effectInfo.effectFunc = fadeBetween3ColorsWithScalingPwmEffect;	
			instructionPointer->effectInfo.effectResetFunc = resetFadeBetween3ColorsWithScalingPwmEffect;
			effectDataLength =  24;
			instructionPointer->effectInfo.tenMsTriggerOnly=FALSE;


		}

		//Instrction 8 - Light beem with multiple leds
		else if(instructionPointer->effectNumber == 8) {
		

			instructionPointer->effectInfo.effectFunc = lightBeamEffect;
			instructionPointer->effectInfo.effectResetFunc = resetLightBeamEffect;
			short beamCount = readShortFromFlash(&dataPtr);
			dataPtr -= 2;
			effectDataLength =  (8 * (beamCount)) + 2;
			instructionPointer->effectInfo.tenMsTriggerOnly=FALSE;


		}
		
		//Instruction 9 - Led ON Effect
		else if (instructionPointer->effectNumber == 9) {
			instructionPointer->effectInfo.effectFunc = ledOnEffect;
			instructionPointer->effectInfo.effectResetFunc = resetLedOnEffect;;
			effectDataLength =  6;
			instructionPointer->effectInfo.tenMsTriggerOnly=TRUE;
		}
		
		copyFromFlash(dataPtr,sramEffectDataPtr,effectDataLength);
		sramEffectDataPtr += effectDataLength;
		dataPtr+=effectDataLength;


		//If *dataPtr=0xff we are at the end of the packet!
		if(pgm_read_byte(dataPtr) == 0xff) {
			instructionPointer->nextInstruction = 0;
			break;
		}


		else {
			instructionPointer->nextInstruction = instructionPointer + 1;
			instructionPointer++;
		}
	}
	while (pgm_read_byte(dataPtr) != 0xff);
	dataPtr++;
	return instructionGroup;
}
