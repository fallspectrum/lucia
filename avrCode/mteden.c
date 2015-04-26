//Matches at number of 10ms - 1
#include "utils.h"
void intro1() {
	int timeCounter;
	
	timeCounter = 0;
	resetEffect = TRUE;
	fadeBetweenColors(0,100,0,0,0,0,100,600);
	fadeBetweenColors(1,100,0,0,0,0,100,600);
	resetEffect= FALSE;

	while(timeCounter < 600) {
		fadeBetweenColors(0,100,0,0,0,0,100,600);
		fadeBetweenColors(1,100,0,0,0,0,100,600);
		waitForTimer();

		//display colors
		displayColors();
		timeCounter++;
	}

	timeCounter = 0;
	resetEffect = TRUE;
	fadeBetweenColors(0,100,0,100,0,100,0,500);
	fadeBetweenColors(1,100,0,100,0,100,0,500);
	resetEffect = FALSE;
	while(timeCounter < 550) {
		waitForTimer();

		//display colors
		displayColors();
		timeCounter++;
		fadeBetweenColors(0,100,0,100,0,100,0,500);
		fadeBetweenColors(1,100,0,100,0,100,0,500);
	}
}


void intro2() {
	int timeCounter;
	
	timeCounter = 0; 
	resetEffect = TRUE;
	fadeBetweenColors(0,100,0,0,0,0,100,550);
	fadeBetweenColors(1,100,0,0,0,0,100,550);
	resetEffect= FALSE;

	while(timeCounter < 550) {
		fadeBetweenColors(0,100,0,0,0,0,100,550);
		fadeBetweenColors(1,100,0,0,0,0,100,550);
		waitForTimer();

		//display colors
		displayColors();
		timeCounter++;
	}

	timeCounter = 0;
	resetEffect = TRUE;
	fadeBetweenColors(0,100,0,100,0,100,0,500);
	fadeBetweenColors(1,100,0,100,0,100,0,500);
	resetEffect = FALSE;
	while(timeCounter < 600) {
		waitForTimer();

		//display colors
		displayColors();
		timeCounter++;
		fadeBetweenColors(0,100,0,100,0,100,0,500);
		fadeBetweenColors(1,100,0,100,0,100,0,500);
	}
	setRGBColor(0,255,0,0);
	setRGBColor(1,255,0,0);
	waitForTimer();
	displayColors();

}

void beat1() {
	int timeCounter;
	timeCounter = 0;
	resetEffect = TRUE;
	
	fadeBetweenColors(1,255,0,0,0,0,0,20);
	setRGBColor(0,0,0,0);
	resetEffect = FALSE;
	
	while(timeCounter<20) {
		displayColors();
		timeCounter++;
		fadeBetweenColors(1,255,0,0,0,0,0,19);
		waitForTimer();
	}

	//chill for 100 ms
	timeCounter = 0;
	while(timeCounter < 10) {
		timeCounter++;
		waitForTimer();
	}
	
	timeCounter = 0;
	resetEffect = TRUE;
	setRGBColor(1,0,0,0);  
	fadeBetweenColors(0,255,0,0,0,0,0,20);
	resetEffect = FALSE;
	
	while(timeCounter<20) {
		displayColors();
		timeCounter++;
		fadeBetweenColors(0,255,0,0,0,0,0,19);
		waitForTimer();
	}


	//chill for 100 ms
	timeCounter = 0;
	while(timeCounter < 20) {
		timeCounter++;
		waitForTimer();
	}


	timeCounter=0;
	fadeBetweenColors(1,255,0,0,0,0,0,20);
	setRGBColor(0,0,0,0);
	resetEffect = FALSE;
	
	while(timeCounter<20) {
		displayColors();
		timeCounter++;
		fadeBetweenColors(1,255,0,0,0,0,0,19);
		waitForTimer();
	}

	//chill for 100 ms
	timeCounter = 0;
	while(timeCounter < 0) {
		timeCounter++;
		waitForTimer();
	}
	
	timeCounter = 0;
	resetEffect = TRUE;
	setRGBColor(1,0,0,0);  
	fadeBetweenColors(0,255,0,0,0,0,0,20);
	resetEffect = FALSE;
	
	while(timeCounter<20) {
		displayColors();
		timeCounter++;
		fadeBetweenColors(0,255,0,0,0,0,0,19);
		waitForTimer();
	}

	//chill for 100 ms
	timeCounter = 0;
	while(timeCounter < 25) {
		timeCounter++;
		waitForTimer();
	}

}
