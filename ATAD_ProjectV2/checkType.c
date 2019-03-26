#include <stdio.h>
#include "checkType.h"


PtValue createToValue(char type[], float avg2Points, float avgAll2Points, float avg3Points, float avgAll3Points, float avgAssists, float avgAllAssists, float avgBlocks, float avgAllBlocks, float avgFaults, float avgAllFaults)
{
	PtValue value = (PtValue)malloc(sizeof(ToValue));

	value->avg2Points = avg2Points;
	value->avg3Points = avg3Points;
	value->avgAssists = avgAssists;
	value->avgBlocks = avgBlocks;
	value->avgFaults = avgFaults;
	value->avgAll2Points = avgAll2Points;
	value->avgAll3Points = avgAll3Points;
	value->avgAllAssists = avgAllAssists;
	value->avgAllBlocks = avgAllBlocks;
	value->avgAllFaults = avgAllFaults;
	strcpy_s(value->type, sizeof(value->type), type);

	return value;
}


char * associateType(Player player, float mediaPoints, float mediaAssistsBlocks) {
	if (player.statistic.twoPoints + player.statistic.threePoints > mediaPoints && player.statistic.blocks + player.statistic.assists < mediaAssistsBlocks) {
		return "Shooting-Guard";
	}
	else if (player.statistic.twoPoints + player.statistic.threePoints < mediaPoints && player.statistic.blocks + player.statistic.assists > mediaAssistsBlocks) {
		return "Point-Guard";
	}
	else if (player.statistic.twoPoints + player.statistic.threePoints + player.statistic.blocks + player.statistic.assists > mediaPoints + mediaAssistsBlocks) {
		return "All-Star";
	}
	else {
		return " ";
	}
}

void printValue(ToValue value) {
	printf("%-20s   %.2f    %.2f    %.2f    %.2f    %.2f    %.2f    %.2f    %.2f    %.2f    %.2f  \n ", value.type,
		value.avg2Points, value.avgAll2Points, value.avg3Points, value.avgAll3Points, value.avgFaults, value.avgAllFaults,
		value.avgAssists, value.avgAllAssists, value.avgBlocks, value.avgAllBlocks);
}


