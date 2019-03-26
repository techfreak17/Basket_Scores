#include "player.h"
#include "list.h"

int importFromFile(char * playerFileName, char * gameFileName, PtList _this);
void clearRecords(PtList _this);
void showFunction(PtList _this);
void printSortedNames(PtList playerList);
void printSortedDates(PtList playerList);
void printSortedNumberOfGames(PtList playerList);
PtList averageStatistics(PtList playerList);
void sortByAvg(PtList playerList);
PtList normalizeStatistics(PtList players);
void playersType(PtList list);
void checkType(PtList list);
void KMEANS(PtList list, int k, int maxIterations, double deltaError);
