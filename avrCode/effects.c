#include "effects.h"
#include "utils.h"
#include "config.h"


short blinkLedCounters[LEDCOUNT];


#define BLINK_LENGTH 18

void resetBlinkEffect(short ledNumber, short *data) {

	blinkLedCounters[(short) ledNumber] = 0;

	short theData[7];
	theData[0] = data[0];
	theData[1] = data[1];
	theData[2] = data[2];
	theData[3] = 0;
	theData[4] = 0;
	theData[5] = 0;
	theData[6] = BLINK_LENGTH; 
	resetEffect=TRUE;
	fadeBetweenColors(ledNumber,theData);
	resetEffect=FALSE;
}

void blinkEffect(short ledNumber, short *data)
{

	
	if (blinkLedCounters[(short) ledNumber] >= BLINK_LENGTH) {
		return;
	}
	short newEffectData[7];
	newEffectData[0] = data[0];
	newEffectData[1] = data[1];
	newEffectData[2] = data[2];
	newEffectData[3] = 0;
	newEffectData[4] = 0;
	newEffectData[5] = 0;
	newEffectData[6] = BLINK_LENGTH;
	
	fadeBetweenColors(ledNumber,newEffectData);
	blinkLedCounters[(short) ledNumber] += 1;
	
}




int currentStepLeds[LEDCOUNT];
int directionLeds[LEDCOUNT];

void resetFadeBetweenColorsEffect(short ledNumber, short*data) {
	resetEffect = TRUE;
	fadeBetweenColors(ledNumber,data);
	resetEffect = FALSE;
}


struct FadeBetweenColorsDataArgument {
	short beginRed;
	short beginGreen;
	short beginBlue;
	short endRed;
	short endGreen;
	short endBlue;
	short stepCount;
};

void fadeBetweenColors(short ledNumber, short *data)
{
//	struct FadeBetweenColorsDataArgument *dataArgument = (struct FadeBetweenColorsDataArgument *) data;
	short br = data[0];
	short bg = data[1];
	short bb = data[2];
	short er = data[3];
	short eg = data[4];
	short eb = data[5];

	short stepCount = data[6];
	
	static float savedColors[LEDCOUNT * 3];
	
	int *currentStep,*direction;
	currentStep = &currentStepLeds[(short)ledNumber];
	direction = &directionLeds[(short)ledNumber];

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
		*currentStep = 0;
		*direction = 1;	
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
	*cr = (float) br + (sr * (float) *currentStep );
	*cg = (float) bg + (sg * (float) *currentStep );
	*cb = (float) bb + (sb * (float) *currentStep );


	setRGBColor(ledNumber,(int) *cr,(int) *cg,(int) *cb);
	
	
	//if we are moving from beginning color
	//to end color
	if(*direction == 1) {
		*currentStep = *currentStep + 1;
		if(*currentStep > stepCount) {
			*direction = 0;
			*currentStep = stepCount;
		}
	}

	//we are moving from end color 
	//to beginning color
	else {
		*currentStep = *currentStep - 1;
		if(*currentStep < 0)  {
			*currentStep = 0;
			*direction = 1;
		}
	}
}



short pwmLedCounter[LEDCOUNT];
short pwmLedOn[LEDCOUNT];

void pwmEffect(short ledNumber, short *data) {
	
	short *curCounter = &pwmLedCounter[(short) ledNumber];
	short *curLedOn = &pwmLedOn[(short) ledNumber];
	
	short redColor = data[0];
	short greenColor = data[1];
	short blueColor = data [2];
	short onLength = data[3] -1;
	short offLength = data[4] -1;

	//if pwmLedOn is true
	if(*curLedOn == 1) {
		
		//Turn off the leds when on length
		//is longer then counter
		if(*curCounter >= onLength) {
			//we are turning led's off.
			*curLedOn = 0;
			*curCounter = 0;
			setRGBColor(ledNumber,0,0,0);
		}
		
		//set the rgb color 
		else  {
			setRGBColor(ledNumber,redColor,greenColor,blueColor);
			*curCounter += 1;
		}

	}


	//if pwmLedOn is off
	else if(*curLedOn == 0) {
		
		//Turn on the leds when on length
		//is longer then counter
		if(*curCounter >= offLength) {
			//we are turning led's off.
			*curLedOn = 1;
			*curCounter = 0;
 			setRGBColor(ledNumber,redColor,greenColor,blueColor);
		}
		
		//set the rgb color 
		else  {
			
			setRGBColor(ledNumber,0,0,0);
			*curCounter +=1;
		}

	}
	
}

void resetPwmEffect(short ledNumber, short *data) {
	pwmLedCounter[(short) ledNumber] = 0;
	pwmLedOn[(short) ledNumber] = 1;
	//turn on the led
	setRGBColor(ledNumber,data[0],data[1],data[2]);
}


short fadeWithPwm10msCounter[LEDCOUNT];
static struct rgbColors fadeWithPwm10msCounterColorStructs[LEDCOUNT];
void fadeBetweenColorsWithPwm(short ledNumber, short *data) {
	
	
	//data[0] = beginRed
	//data[1] = begingreen
	//data[2] = beginblue
	//data[3] = endRed
	//data[4] = endBlue
	//data[5] = endGreen
	//data[6] = colorChangeStepCount
	//data[7] = pwmOnCount
	//data[8] = pwmOffCount
	
	
	struct rgbColors *colorInfo = &fadeWithPwm10msCounterColorStructs[(short) ledNumber];
	//First call the fadeBetweenColors effect.
	//This shoul only happen every 10 ms!
	if(fadeWithPwm10msCounter[(short) ledNumber] >= TEN_MS_MULTIPLIER) {
		fadeBetweenColors( (short) ledNumber, data);
		
		getRGBColor((short) ledNumber, colorInfo);

		fadeWithPwm10msCounter[(short) ledNumber] = 0;
	}
	else {
		fadeWithPwm10msCounter[(short) ledNumber] += 1;
	}

	//now turn on/off the led if necessary

	short pwmData[] = {
		(short) colorInfo->red,
		(short) colorInfo->green,
		(short) colorInfo->blue,
		data[7],
		data[8],
	};

	//call pwm effect 
	pwmEffect(ledNumber,pwmData);

	
	
}

void resetFadeBetweenColorsWithPwm(short ledNumber, short *data) {
	pwmLedCounter[(short) ledNumber] = 0;
	pwmLedOn[(short) ledNumber] = 1;
	fadeWithPwm10msCounter[(short) ledNumber] = 0;

	//NOTE resetPwmEffect DOES NOT DEPEND ON DATA ATM SO IT's OK!
	resetPwmEffect(ledNumber,data);
	//NOTE IF ARGUMENTS FOR FADE   BETWEEN COLORS EFFECT CHANGES
	//THEN THIS WILL NOT WORK!
	
	resetFadeBetweenColorsEffect(ledNumber,data);
	
	struct rgbColors *colorInfo = &fadeWithPwm10msCounterColorStructs[(short) ledNumber];
	getRGBColor((short) ledNumber, colorInfo);
}



struct PwmScaleData {
	short onCount;
	short offCount;	
	short curTimeCounter;
	short pwmFinished;
	int stepCount;
} pwmScaleData[LEDCOUNT];



void fadeBetweenColorsWithScalingPwmEffect(short ledNumber, short *data) {
	short direction = data[7];
	short stepCount = pwmScaleData[ledNumber].stepCount;

	//see if we are done scaling 
	if(pwmScaleData[ledNumber].pwmFinished == TRUE) {
		//check if we need to display a color
		if(pwmScaleData[ledNumber].curTimeCounter >= TEN_MS_MULTIPLIER) {
			
			//if direction is UP then use the fadeBetweenColor effect
			if(direction == PWM_SCALE_UP) {
				fadeBetweenColors(ledNumber,data);
			}

			//if direction is DOWN then turn led off
			if(direction == PWM_SCALE_DOWN) {
				setRGBColor(ledNumber,0,0,0);
			}

			pwmScaleData[ledNumber].curTimeCounter = 0;	
		}
		
		pwmScaleData[ledNumber].curTimeCounter++;
		return;
	}

	else if(pwmScaleData[ledNumber].curTimeCounter >= stepCount) {
		pwmScaleData[ledNumber].curTimeCounter = 0;
		
		//check the scaling directoin
		if(direction == PWM_SCALE_UP) {
			if(pwmScaleData[ledNumber].offCount > 2) {
				//keep decreasing the off count
				pwmScaleData[ledNumber].offCount -=2;
				
			} 
			else {
				//keep increasing the On count until we get to 32
				if(pwmScaleData[ledNumber].onCount < 32) {
					pwmScaleData[ledNumber].onCount += 2;

					if(pwmScaleData[ledNumber].onCount == 32) {
						pwmScaleData[ledNumber].pwmFinished = TRUE;
					}
				}
			}
 		} 

		//check the scaling directoin
		else if(direction == PWM_SCALE_DOWN) {
			if(pwmScaleData[ledNumber].onCount > 2) {
				//keep decreasing the on count
				pwmScaleData[ledNumber].onCount -=2;
				
			} 
			else {
				//keep increasing the off count until we get to 32
				if(pwmScaleData[ledNumber].offCount < 32) {
					pwmScaleData[ledNumber].offCount += 2;

					if(pwmScaleData[ledNumber].offCount == 32) {
						pwmScaleData[ledNumber].pwmFinished = TRUE;
					}
				}
			}
 		} 

	} 
	else {
		pwmScaleData[ledNumber].curTimeCounter ++;
	}

	short newData[9];
	copy_short_data(newData,data,7);
	newData[7] = pwmScaleData[ledNumber].onCount;
	newData[8] = pwmScaleData[ledNumber].offCount;
	fadeBetweenColorsWithPwm(ledNumber,newData);
	

}

void resetFadeBetweenColorsWithScalingPwmEffect(short ledNumber, short *data) 
{
	
	//data[0] = beginRed
	//data[1] = begingreen
	//data[2] = beginblue
	//data[3] = endRed
	//data[4] = endGreen
	//data[5] = endBlue
	//data[6] = colorChangeStepCount
	//data[7] = pwmScalingDirection (0 on to off, 1 off to one)
	//data[8] = pwmChangeStepCount In Ms

	//set initial conditions for effcect
	int direction = data[7];
	if(direction == PWM_SCALE_UP) {
		pwmScaleData[ledNumber].onCount = 0;
		pwmScaleData[ledNumber].offCount = 32;
	}

	else if(direction == PWM_SCALE_DOWN) {
		pwmScaleData[ledNumber].onCount = 32;
		pwmScaleData[ledNumber].offCount = 2;
	}
	pwmScaleData[ledNumber].curTimeCounter = 0;
	pwmScaleData[ledNumber].stepCount = data[8] * TEN_MS_MULTIPLIER;

	short newData[9];
	copy_short_data(newData,data,7);
	newData[7] = pwmScaleData[ledNumber].onCount;
	newData[8] = pwmScaleData[ledNumber].offCount;
	resetFadeBetweenColorsWithPwm(ledNumber,newData);

	//TODO: This is a bit of a hack. LED flashes green for a second
	//so we just shut it off at first.
	setRGBColor(ledNumber,0,0,0);

}


//Instruction 7 - fade between 3 colors with scaling pwm
//data[0] = beginRed
//data[1] = beginGreen
//data[2] = beginBlue
//data[3] = middleRed
//data[4] = middleGreen
//data[5] = middleBlue
//data[6] = endRed
//data[7] = endGreen
//data[8] = endBlue
//data[9] = colorChangeStepCount
//data[10] = pwmScalingDirection (0 on to off, 1 off to one)
//data[11] = pwmChangeStepCount In 10ms step intervals

struct FadeBetween3ColorsWithScalingPwmEffectArgument {
	short beginRed;
	short beginGreen;
	short beginBlue;
	short middleRed;
	short middleGreen;
	short middleBlue;
	short endRed;
	short endGreen;
	short endBlue;
	short colorChangeStepCount;
	short pwmScalingDirection; //(0 on to off, 1 off to one)
	short pwmChangeStepCount; // In 10ms step intervals
};

struct FadeBetween3ColorsWithScalingPwmEffectData {
	short colorIndicator;
	short direction;
	short colorReset10msTick;
	short tenMsCounter;
	short twoMsCounter;
	short *colorBeginPtr;
	short *colorEndPtr;
} fadeBetween3ColorsWithScalingPwmEffectData[LEDCOUNT];

void fadeBetween3ColorsWithScalingPwmEffect(short ledNumber, short *data) {
	struct FadeBetween3ColorsWithScalingPwmEffectArgument *dataArgument = (struct FadeBetween3ColorsWithScalingPwmEffectArgument *) data;

	//This is a bit of a hack. this counter is going to update
	//every 10ms by fadBetweenColorsWithScalingPwmEffect() 
	//just saving some memory space
	if( fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].twoMsCounter >= TEN_MS_MULTIPLIER) {

		//if we reached the point where color fade is about to reset direction...
		if(fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].tenMsCounter >=
			fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorReset10msTick) {
	
		
			struct FadeBetweenColorsDataArgument resetData;

			//if we are going to the next color upwards...
			if(fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].direction == DIRECTION_INCREASE) {
				fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorIndicator++;
				
				if(fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorIndicator == 1) {


					fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorBeginPtr = &dataArgument->middleRed;
					fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorEndPtr = &dataArgument->endRed;
					resetData.beginRed = dataArgument->middleRed;
					resetData.beginGreen = dataArgument->middleGreen;
					resetData.beginBlue = dataArgument->middleBlue;

					resetData.endRed = dataArgument->endRed;
					resetData.endGreen = dataArgument->endGreen;
					resetData.endBlue = dataArgument->endBlue;

				}

				//we reached the 3rd color, time to go backwards
				else {
					fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].direction = DIRECTION_DECREASE;
					fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorIndicator = 1;
					

					fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorBeginPtr = &dataArgument->endRed;
					fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorEndPtr = &dataArgument->middleRed;
					
					
					resetData.beginRed = dataArgument->endRed;
					resetData.beginGreen = dataArgument->endGreen;
					resetData.beginBlue = dataArgument->endBlue;

					resetData.endRed = dataArgument->middleRed;
					resetData.endGreen = dataArgument->middleGreen;
					resetData.endBlue = dataArgument->middleBlue;	
				}

			}

			else if (fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].direction == DIRECTION_DECREASE) {
				fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorIndicator--;
				if(fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorIndicator == 0) {


					fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorBeginPtr = &dataArgument->middleRed;
					fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorEndPtr = &dataArgument->beginRed;
					
					resetData.beginRed = dataArgument->middleRed;
					resetData.beginGreen = dataArgument->middleGreen;
					resetData.beginBlue = dataArgument->middleBlue;

					resetData.endRed = dataArgument->beginRed;
					resetData.endGreen = dataArgument->beginGreen;
					resetData.endBlue = dataArgument->beginBlue;

				}
				 
				//we reached the end, now we are going back up
				else {
					fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorIndicator = 0;
					fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].direction = DIRECTION_INCREASE;
					fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorBeginPtr = &dataArgument->beginRed;
					fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorEndPtr = &dataArgument->middleRed;
					
					resetData.beginRed = dataArgument->beginRed;
					resetData.beginGreen = dataArgument->beginGreen;
					resetData.beginBlue = dataArgument->beginBlue;

					resetData.endRed = dataArgument->middleRed;
					resetData.endGreen = dataArgument->middleGreen;
					resetData.endBlue = dataArgument->middleBlue;
				}

			}
			
			//reset the fade color effect
			/*
			resetData.beginRed = 255;
			resetData.beginGreen = 0;
			resetData.beginBlue = 0;
			resetData.endRed =0;
			resetData.endGreen = 255;
			resetData.endBlue = 0;
			resetData.stepCount = dataArgument->colorChangeStepCount;
			*/
			

			//TODO this is a bit of a hack, for some reason it flashes the end
			//color withought the below group of code
			struct rgbColors curColors;
			getRGBColor(ledNumber,&curColors);
			resetData.beginRed = curColors.red;
			resetData.beginGreen = curColors.green;
			resetData.beginBlue  = curColors.blue;
			
			
			resetFadeBetweenColorsEffect(ledNumber, (short *)&resetData);
			fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].tenMsCounter = 0;

		}

		else 
		{
			fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].tenMsCounter+=1;
		}
		fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].twoMsCounter = 0;;

	}
	else 
	{
			fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].twoMsCounter+=1;
	}
	struct FadeBetweenColorsWithPwmEffectData sData;
	sData.beginRed = fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorBeginPtr[0];
	sData.beginGreen = fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorBeginPtr[1];
	sData.beginBlue = fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorBeginPtr[2];
	sData.endRed = fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorEndPtr[0];
	sData.endGreen = fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorEndPtr[1];
	sData.endBlue = fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorEndPtr[2];
	sData.colorChangeStepCount = dataArgument->colorChangeStepCount;
	sData.pwmScalingDirection = dataArgument->pwmScalingDirection;
	sData.pwmChangeStepCount = dataArgument->pwmChangeStepCount;
	
	fadeBetweenColorsWithScalingPwmEffect(ledNumber, (short *)&sData);

	/*
	//TODO this is a hack. the led flashes brightly at the end color for some
	//reason...
	if(fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].tenMsCounter == 0 && 
	fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].direction == DIRECTION_INCREASE) {
		setRGBColor(ledNumber,sData.beginRed,sData.beginGreen,sData.beginBlue);
	}
	else if(fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].tenMsCounter == 0 && 
	fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].direction == DIRECTION_DECREASE) {
		setRGBColor(ledNumber,0,0,0);
	}
	*/
	
	
}

void resetFadeBetween3ColorsWithScalingPwmEffect(short ledNumber, short *data) {
	//resetFadeBetweenColorsWithScalingPwmEffect
	struct FadeBetweenColorsWithPwmEffectData resetData;
	struct FadeBetween3ColorsWithScalingPwmEffectArgument *dataArgument = (struct FadeBetween3ColorsWithScalingPwmEffectArgument *)data;
	resetData.beginRed = dataArgument->beginRed;
	resetData.beginGreen = dataArgument->beginGreen;
	resetData.beginBlue = dataArgument->beginBlue;
	resetData.endRed = dataArgument->middleRed;
	resetData.endGreen = dataArgument->middleGreen;
	resetData.endBlue = dataArgument->middleBlue;
	resetData.colorChangeStepCount = dataArgument->colorChangeStepCount;
	resetData.pwmScalingDirection = dataArgument->pwmScalingDirection;
	resetData.pwmChangeStepCount = dataArgument ->pwmChangeStepCount;
	resetFadeBetweenColorsWithScalingPwmEffect(ledNumber, (short *)&resetData);


	fadeBetween3ColorsWithScalingPwmEffectData[ledNumber] . colorIndicator = 0;
	fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorReset10msTick = 
		resetData.colorChangeStepCount;
	fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].direction = DIRECTION_INCREASE;
	fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].tenMsCounter = 0;
	
	fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorBeginPtr = &dataArgument->beginRed;
	fadeBetween3ColorsWithScalingPwmEffectData[ledNumber].colorEndPtr = &dataArgument->middleRed;


}

void ledOnEffect(short ledNumber,short *data) {
	setRGBColor(ledNumber,data[0],data[1],data[2]);
}

void resetLedOnEffect (short ledNumber, short *data) {
}

void ledOffEffect(short ledNumber, short *data) {
	setRGBColor(ledNumber,0,0,0);
}
void resetLedOffEffect(short ledNumber, short *data) {
}



//this effect simply returns 
void doNothingEffect(short led, short *data) {
	return;
}

void resetDoNothingEffect(short ledNumber, short *data) {
	return;
}



struct LightBeamEffectDataArgument1 {
	short beamColorCount;
};

struct LightBeamEffectDataArgument2 {
	short red;
	short green;
	short blue;
	short onTime;
};

struct LightBeamEffectSavedData {
	short currentMsTime;
	short currentBeamIndex;
} lightBeamSavedData[LEDCOUNT];


void lightBeamEffect(short ledNumber, short *data) {
	//check beam effect time
	
	struct LightBeamEffectSavedData *curSavedBeam = &lightBeamSavedData[ledNumber];
	
	struct LightBeamEffectDataArgument2 *firstBeamArgument = 
		(struct LightBeamEffectDataArgument2  *) &data[1];


	short currentBeamIndex = curSavedBeam->currentBeamIndex;
	
	

	if(curSavedBeam->currentMsTime > 
			firstBeamArgument[currentBeamIndex].onTime) {
			curSavedBeam->currentBeamIndex+=1;
		if(curSavedBeam->currentBeamIndex >= data[0]) {
			curSavedBeam->currentBeamIndex = 0;
		}
		curSavedBeam->currentMsTime = 0;
					
	}

	else {
		curSavedBeam->currentMsTime++;
	}
	
	//display the current beam value
	setRGBColor( 
		ledNumber,
		firstBeamArgument[curSavedBeam->currentBeamIndex].red,
		firstBeamArgument[curSavedBeam->currentBeamIndex].green,
		firstBeamArgument[curSavedBeam->currentBeamIndex].blue
	);

}
void resetLightBeamEffect(short ledNumber, short *data) {
	lightBeamSavedData[ledNumber].currentMsTime = 0;
	lightBeamSavedData[ledNumber].currentBeamIndex = 0;
}	
