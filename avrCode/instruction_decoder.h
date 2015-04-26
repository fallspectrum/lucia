#ifndef _H_INSTRUCTION_DECODER
#define _H_INSTRUCTION_DECODER
#include "utils.h"

struct effect_info{
	void (*effectFunc)(short, short *);
	void (*effectResetFunc)(short, short *);
    short *effectData;
	char tenMsTriggerOnly;
};

struct instruction {
    char ledNumber;
	char effectNumber;
	struct effect_info effectInfo;
    struct instruction *nextInstruction;
	
};

struct instruction_group {
    //time difference between last effect and
    //this one
    unsigned int dt;
	struct instruction *firstInstruction;
};


struct instruction_group getInstruction();




#define DATA_STORAGE_LENGTH 30

extern char instructionGroup1[sizeof(struct instruction_group) + ( (sizeof(struct instruction) + DATA_STORAGE_LENGTH)  * LEDCOUNT) ];
extern char instructionGroup2[sizeof(struct instruction_group) + ( (sizeof(struct instruction) + DATA_STORAGE_LENGTH) * LEDCOUNT) ];


extern char *newInstructionGroup;
extern char *oldInstructionGroup;


void initDecoder();

//Use these functions to get the next/previous
//effect group data
struct instruction_group *getNextGroup();
struct instruction_group *getPreviousGroup();

//This should only be called inside instruction_decoder.c
struct instruction_group * getInstructionGroup();
struct instruction_group * getLastGroup();
#endif
