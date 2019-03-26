#pragma once
#include "list.h"

typedef struct toValue {
	char type[21];
	float avg2Points;
	float avgAll2Points;
	float avg3Points;
	float avgAll3Points;
	float avgAssists;
	float avgAllAssists;
	float avgBlocks;
	float avgAllBlocks;
	float avgFaults;
	float avgAllFaults;
}ToValue;

typedef struct toValue *PtValue;


PtValue createToValue(char type[], float avg2Points, float avgAll2Points, float avg3Points, float avgAll3Points, 
					  float avgAssists, float avgAllAssists, float avgBlocks, float avgAllBlocks, float avgFaults, float avgAllFaults);
char * associateType(Player player, float mediaPoints, float mediaAssistsBlocks);
void printValue(ToValue value);
