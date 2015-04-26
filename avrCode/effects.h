#ifndef _H_EFFECTS
#define _H_EFFECTS

//instruction 1, turn LED off
void ledOffEffect(short ledNumber, short *data);
void resetLedOffEffect(short ledNumber, short *data);


//Instruction 2
//data[0] = red
//data[1] = green
//data[2] = blue
void blinkEffect(short ledNumber, short *data);
void resetBlinkEffect(short ledNumber, short *data);

//Instruction 3 - PWM 
//data[0] = red
//data[1] = green
//data[2] = blue
//data[3] = onLength, 2 ms intervals
//data[4] = offLength 2 ms intervals
void pwmEffect(short ledNumber, short *data);
void resetPwmEffect(short ledNumber, short *data);


//Instruction 4
//data[0] = beginRed
//data[1] = beginBlue
//data[2] = beginGreen
//data[3] = endRed
//data[4] = endBlue
//data[5] = endGreen
//data[6] = stepCount (amount of 10ms steps for fade from one color to next)
void fadeBetweenColors(short ledNumber, short *data);
void resetFadeBetweenColorsEffect(short ledNumber, short*data);

//Instruction 5 - fade between colors with pwm
//data[0] = beginRed
//data[1] = beginGreen
//data[2] = beginBlue
//data[3] = endRed
//data[4] = endGreen
//data[5] = endBlue
//data[6] = colorChangeStepCount (amount of 10ms steps for fade from one color to next)
//data[7] = pwmOnCount (amount of 2ms intervals)
//data[8] = pwmOffCount (amount of 2ms intervals)
void fadeBetweenColorsWithPwm(short ledNumber, short *data);
void resetFadeBetweenColorsWithPwm(short ledNumber, short *data);


struct FadeBetweenColorsWithPwmEffectData {
	short beginRed;
	short beginGreen;
	short beginBlue;
	short endRed;
	short endGreen;
	short endBlue;
	short colorChangeStepCount;
	short pwmScalingDirection;
	short pwmChangeStepCount;
};
//Instruction 6 - fade between colors with scaling pwm
//data[0] = beginRed
//data[1] = beginGreen
//data[2] = beginBlue
//data[3] = endRed
//data[4] = endGreen
//data[5] = endBlue
//data[6] = colorChangeStepCount (amount of 10ms steps for fade from one color to next)
//data[7] = pwmScalingDirection (0 on to off, 1 off to one)
//data[8] = pwmChangeStepCount (In 10ms step intervals)
void fadeBetweenColorsWithScalingPwmEffect(short ledNumber, short *data);
void resetFadeBetweenColorsWithScalingPwmEffect(short ledNumber, short *data);

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
//data[11] = pwmChangeStepCount In (10ms step intervals)
void fadeBetween3ColorsWithScalingPwmEffect(short ledNumber, short *data);
void resetFadeBetween3ColorsWithScalingPwmEffect(short ledNumber, short *data);


//Instruction 8 - Light Beam
//data[0] - numberOfBeams
//data[1] - red color value
//data[2] - green color value
//data[3] - blue color value
//data[4] - time to display beem in 2ms
//Repeat format of last 4 values 
//until there are enough descriptions for
//numberOfBeams

void lightBeamEffect(short ledNumber, short *data);
void resetLightBeamEffect(short ledNumber, short *data);

//Instruction 9 - LED On Effect
//data[0] - red
//data[1] - green 
//data[2] - blue
void ledOnEffect(short ledNumber,short * data);
void resetLedOnEffect (short ledNumber, short *data);


#define PWM_SCALE_UP 1
#define PWM_SCALE_DOWN 0


#define DIRECTION_INCREASE 1
#define DIRECTION_DECREASE 0
//does nothing
void doNothingEffect(short led, short *data);
void resetDoNothingEffect(short led, short *data);
#endif

