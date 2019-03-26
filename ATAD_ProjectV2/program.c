#include <stdlib.h>
#include <stdio.h>
#include "map.h"
#include "list.h"
#include "cluster.h"
#include "program.h"

char** split(char *str, int numbFields, const char delim[]);
PtList listCopy(PtList listToCopy);
void switchPlayersPlace(PtList _this, Player player1, Player player2, int player1Idx, int player2Idx);
void sortByAvg(PtList playerList);
float getMvp(Player player);
float getTwoPointsMin(PtList playerList);
float getTwoPointsMax(PtList playerList);
float getThreePointsMin(PtList playerList);
float getThreePointsMax(PtList playerList);
float getAssistsMin(PtList playerList);
float getAssistsMax(PtList playerList);
float getBlocksMin(PtList playerList);
float getBlocksMax(PtList playerList);
float getFoulsMin(PtList playerList);
float getFoulsMax(PtList playerList);
float getNorm(float x, float min, float max);
void printTypeList(PtList sgList, PtList pgList, PtList asList);


int importFromFile(char * playerFileName, char * gameFileName, PtList _this) {
	FILE *fd;
	int err1 = fopen_s(&fd, playerFileName, "r");

	int size = 0;
	listSize(_this, &size);

	if (err1 != 0) {
		printf("Nao foi possivel abrir o ficheiro de jogadores %s \n", playerFileName);
		return 0;
	}

	char nextline[1024];

	int countPlayers = 0;

	while (fgets(nextline, sizeof(nextline), fd)) {
		if (strlen(nextline) < 1)
			continue;


		char **tokens = split(nextline, 5, ";");

		char name[40];
		char team[40];
		int id = 0, day = 0, month = 0, year = 0;
		char gender = "";

		id = atoi(tokens[0]);
		strcpy_s(name, sizeof(name), tokens[1]);
		strcpy_s(team, sizeof(team), tokens[2]);
		sscanf_s(tokens[3], "%d/%d/%d", &day, &month, &year);
		gender = tokens[4][0];

		listAdd(_this,size++, createPlayer(id, name, team, createDate(day, month, year), gender, createStatistics(0, 0, 0, 0, 0, 0)));

		free(tokens);
		countPlayers++;

	}

	int err2 = fopen_s(&fd, gameFileName, "r");

	if (err2 != 0) {
		printf("Nao foi possivel abrir o ficheiro de jogos %s \n", gameFileName);
		return 0;
	}

	int countGames = 0;

	while (fgets(nextline, sizeof(nextline), fd)) {
		if (strlen(nextline) < 1)
			continue;


		char **tokens = split(nextline, 7, ";");

		int idPlayer = 0, idGame = 0, twoPoints = 0, threePoints = 0, assists = 0, fouls = 0, blocks = 0;
		int gamesPlayed = 0;

		idPlayer = atoi(tokens[0]);
		idGame = atoi(tokens[1]);
		twoPoints = atoi(tokens[2]);
		threePoints = atoi(tokens[3]);
		assists = atoi(tokens[4]);
		fouls = atoi(tokens[5]);
		blocks = atoi(tokens[6]);

		Player tmp = createPlayer(0, "aesr!", "quack", createDate(1, 1, 95), 'k', createStatistics(0, 0, 0, 0, 0, 0));

		ptPlayer player = &tmp;

		for (int i = 0; i < size; i++) {
			listGet(_this, i, player);
			if (idPlayer == player->id) {
				player->statistic.gamesPlayed++;
				player->statistic.assists += assists;
				player->statistic.blocks += blocks;
				player->statistic.fouls += fouls;
				player->statistic.threePoints += threePoints;
				player->statistic.twoPoints += twoPoints;
				listSet(_this, i, *player, player);
			}
		}

		free(tokens);
		countGames++;
	}


	printf(" \nForam lidos %d jogadores e a informação sobre %d jogos.\n\n", countPlayers, countGames);
	fclose(fd);

	return 1;
}

void clearRecords(PtList _this) {
	if (_this == NULL) return;

	int size = 0;
	listSize(_this, &size);

	listClear(_this);

	printf("Foram apagados %d registos\n\n", size);
}


void showFunction(PtList _this) {
	listPrint(_this);
}




void printSortedNames(PtList playerList) {
	PtList toShow = listCopy(playerList);

	Player player1;
	Player player2;
	
	int size = 0;
	listSize(playerList, &size);

	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - i - 1; j++) {
			listGet(toShow, j, &player1);
			listGet(toShow, j + 1, &player2);

			if (strcmp(player1.name, player2.name) > 0) {
				switchPlayersPlace(toShow, player1, player2, j, j + 1);
			}
		}
	}

	listPrint(toShow);
	free(toShow);

}


void printSortedDates(PtList playerList) {
	PtList toShow = listCopy(playerList);

	Player player1;
	Player player2;

	int size = 0;
	listSize(playerList, &size);

	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - i - 1; j++) {
			listGet(toShow, j, &player1);
			listGet(toShow, j + 1, &player2);

			if (player1.birthDate.year == player2.birthDate.year) {

				if (player1.birthDate.month == player2.birthDate.month) {

					if (player1.birthDate.day == player2.birthDate.day) {

						if (strcmp(player1.name, player2.name) > 0) {
							switchPlayersPlace(toShow, player1, player2, j, j + 1);
						}

					}
					else if (player1.birthDate.day > player2.birthDate.day) {
						switchPlayersPlace(toShow, player1, player2, j, j + 1);
					}
				}
				else if (player1.birthDate.month > player2.birthDate.month) {
					switchPlayersPlace(toShow, player1, player2, j, j + 1);
				}
			}
			else if (player1.birthDate.year > player2.birthDate.year) {
						switchPlayersPlace(toShow, player1, player2, j, j + 1);
			}
			
		}
	}
	listPrint(toShow);
	free(toShow);
}

void printSortedNumberOfGames(PtList playerList) {
	PtList toShow = listCopy(playerList);

	Player player1;
	Player player2;

	int size = 0;
	listSize(playerList, &size);

	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - i - 1; j++) {
			listGet(toShow, j, &player1);
			listGet(toShow, j + 1, &player2);
			
			if (player1.statistic.gamesPlayed == player2.statistic.gamesPlayed) {
				if (strcmp(player1.name, player2.name) > 0) {
					switchPlayersPlace(toShow, player1, player2, j, j + 1);
				}
			}	
			else if (player1.statistic.gamesPlayed > player2.statistic.gamesPlayed) {
				switchPlayersPlace(toShow, player1, player2, j, j + 1);
			}
		}
	}

	listPrint(toShow);
	free(toShow);
}

PtList averageStatistics(PtList playerList)
{
	int size = 0, newListSize = 0;
	listSize(playerList, &size);

	PtList newList = listCreate(size);
	Player tmp;


	for (int i = 0; i < size; i++) {
		listGet(playerList, i, &tmp);
		if (tmp.statistic.gamesPlayed != 0) {

			tmp.statistic.twoPoints = tmp.statistic.twoPoints / tmp.statistic.gamesPlayed;
			tmp.statistic.threePoints = tmp.statistic.threePoints / tmp.statistic.gamesPlayed;
			tmp.statistic.assists = tmp.statistic.assists / tmp.statistic.gamesPlayed;
			tmp.statistic.blocks = tmp.statistic.blocks / tmp.statistic.gamesPlayed;
			tmp.statistic.fouls = tmp.statistic.fouls / tmp.statistic.gamesPlayed;

			listAdd(newList, newListSize++, tmp);
		}
	}

	return newList;
}

void sortByAvg(PtList playerList) {

	Player player1;
	Player player2;

	int size = 0;
	listSize(playerList, &size);

	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - i - 1; j++) {
			listGet(playerList, j, &player1);
			listGet(playerList, j + 1, &player2);

			if (getMvp(player1) < getMvp(player2)) {
				switchPlayersPlace(playerList, player1, player2, j, j + 1);
			}
		}
	}
}

PtList normalizeStatistics(PtList playerList)
{
	int size = 0, newListSize = 0;
	float twoPointsMin = getTwoPointsMin(playerList);
	float twoPointsMax = getTwoPointsMax(playerList);
	float threePointsMin = getThreePointsMin(playerList);
	float threePointsMax = getThreePointsMax(playerList);
	float assistsMin = getAssistsMin(playerList);
	float assistsMax = getAssistsMax(playerList);
	float blocksMin = getBlocksMin(playerList);
	float blocksMax = getBlocksMax(playerList);
	float foulsMin = getFoulsMin(playerList);
	float foulsMax = getFoulsMax(playerList);

	listSize(playerList, &size);

	PtList newList = listCreate(size);
	Player tmp;

	for (int i = 0; i < size; i++) {
		listGet(playerList, i, &tmp);
	
		tmp.statistic.twoPoints = getNorm(tmp.statistic.twoPoints,twoPointsMin,twoPointsMax);
		tmp.statistic.threePoints = getNorm(tmp.statistic.threePoints,threePointsMin,threePointsMax);
		tmp.statistic.assists = getNorm(tmp.statistic.assists,assistsMin,assistsMax);
		tmp.statistic.blocks = getNorm(tmp.statistic.blocks,blocksMin,blocksMax);	
		tmp.statistic.fouls = getNorm(tmp.statistic.fouls,foulsMin,foulsMax);

		listAdd(newList, newListSize++, tmp);
		}
	return newList;
	}

void playersType(PtList list) {
	if (list == NULL) return;

	PtList newList = listCopy(averageStatistics(list));

	int size = 0, sgListSize = 0, pgListSize = 0, asListSize = 0;
	listSize(newList, &size);

	float mediaPoints = 0, mediaAssistsBlocks = 0;

	PtList sgList = listCreate(size);
	PtList pgList = listCreate(size);
	PtList asList = listCreate(size);

	for (int i = 0; i < size; i++) {
		Player tmp;
		listGet(newList, i, &tmp);

		mediaPoints += tmp.statistic.twoPoints + tmp.statistic.threePoints;
		mediaAssistsBlocks += tmp.statistic.blocks + tmp.statistic.assists;
	}

	mediaPoints = mediaPoints / size;
	mediaAssistsBlocks = mediaAssistsBlocks / size;

	for (int j = 0; j < size; j++) {
		Player tmp;
		listGet(newList, j, &tmp);

		if (tmp.statistic.twoPoints + tmp.statistic.threePoints > mediaPoints && tmp.statistic.blocks + tmp.statistic.assists < mediaAssistsBlocks) {
			listAdd(sgList, sgListSize++, tmp);
		}
		else if (tmp.statistic.twoPoints + tmp.statistic.threePoints < mediaPoints && tmp.statistic.blocks + tmp.statistic.assists > mediaAssistsBlocks) {
			listAdd(pgList, pgListSize++, tmp);
		}
		else if (tmp.statistic.twoPoints + tmp.statistic.threePoints + tmp.statistic.blocks + tmp.statistic.assists > mediaPoints + mediaAssistsBlocks) {
			listAdd(asList, asListSize++, tmp);
		}

	}

	printTypeList(sgList, pgList, asList);
	free(sgList);
	free(pgList);
	free(asList);
}

void checkType(PtList list) {
	if (list == NULL) return;

	int size = 0;
	listSize(list, &size);

	PtMap map = mapCreate(size);

	Player tmp;
	float avg2Points = 0, avgAll2Points = 0, avg3Points = 0, avgAll3Points = 0, avgAssists = 0, avgAllAssists = 0, avgBlocks = 0, avgAllBlocks = 0, avgFaults = 0, avgAllFaults = 0;

	for (int i = 0; i < size; i++) {
		listGet(list, i, &tmp);
		avgAll2Points += tmp.statistic.twoPoints;
		avgAll3Points += tmp.statistic.threePoints;
		avgAllAssists += tmp.statistic.assists;
		avgAllBlocks += tmp.statistic.blocks;
		avgAllFaults += tmp.statistic.fouls;
	}

	avgAll2Points = avgAll2Points / size;
	avgAll3Points = avgAll3Points / size;
	avgAllAssists = avgAllAssists / size;
	avgAllBlocks = avgAllBlocks / size;
	avgAllFaults = avgAllFaults / size;


	for (int i = 0; i < size; i++) {
		listGet(list, i, &tmp);
		avg2Points = tmp.statistic.twoPoints;
		avg3Points = tmp.statistic.threePoints;
		avgAssists = tmp.statistic.assists;
		avgBlocks = tmp.statistic.blocks;
		avgFaults = tmp.statistic.fouls;

		char *type = associateType(tmp, avgAll2Points + avgAll3Points, avgAllAssists + avgAllBlocks);

		mapPut(map, tmp.id, *createToValue(type, avg2Points, avgAll2Points, avg3Points, avgAll3Points, avgAssists, avgAllAssists, avgBlocks, avgAllBlocks, avgFaults, avgAllFaults));
	}

	mapPrint(map);
	free(map);
}

void KMEANS(PtList list, int k, int maxIterations, double deltaError)
{
	
	runAlgorithm(list, k, maxIterations, deltaError);

}

char** split(char *str, int numbFields, const char delim[]) {
	char **tokens = malloc(sizeof(char*) * numbFields);

	int index = 0;
	char *next_token = NULL;

	char *token = strtok_s(str, delim, &next_token);
	while (token) {
		tokens[index++] = token;
		token = strtok_s(NULL, delim, &next_token);
	}

	return tokens;
}

PtList listCopy(PtList listToCopy) {
	int size = 0;
	listSize(listToCopy, &size);

	PtList toShow = listCreate(size);
	Player tmp;

	for (int i = 0; i < size; i++) {
		listGet(listToCopy, i, &tmp);
		listAdd(toShow, i, tmp);
	}

	return toShow;
}

void switchPlayersPlace(PtList _this, Player player1, Player player2, int player1Idx, int player2Idx) {
	Player tmp = player1;
	listSet(_this, player1Idx, player2, &player1);
	listSet(_this, player2Idx, player1, &tmp);
}

float getMvp(Player player) {
	return (3 * (player.statistic.threePoints)) + (2 * (player.statistic.twoPoints)) + player.statistic.assists + (2 * (player.statistic.blocks)) - (3 * (player.statistic.fouls));
}

float getTwoPointsMin(PtList playerList) {
	float cought = 0;
	int size = 0;
	listSize(playerList, &size);

	Player tmp;

	listGet(playerList, 0, &tmp);
	cought = tmp.statistic.twoPoints;

	for (int i = 1; i < size; i++) {
		listGet(playerList, i, &tmp);
		if (cought > tmp.statistic.twoPoints) {
			cought = tmp.statistic.twoPoints;
		}
	}

	return cought;
}

float getTwoPointsMax(PtList playerList) {
	float cought = 0;
	int size = 0;
	listSize(playerList, &size);

	Player tmp;

	listGet(playerList, 0, &tmp);
	cought = tmp.statistic.twoPoints;

	for (int i = 1; i < size; i++) {
		listGet(playerList, i, &tmp);
		if (cought < tmp.statistic.twoPoints) {
			cought = tmp.statistic.twoPoints;
		}
	}

	return cought;
}

float getThreePointsMin(PtList playerList) {
	float cought = 0;
	int size = 0;
	listSize(playerList, &size);

	Player tmp;

	listGet(playerList, 0, &tmp);
	cought = tmp.statistic.threePoints;

	for (int i = 1; i < size; i++) {
		listGet(playerList, i, &tmp);
		if (cought > tmp.statistic.threePoints) {
			cought = tmp.statistic.threePoints;
		}
	}

	return cought;
}

float getThreePointsMax(PtList playerList) {
	float cought = 0;
	int size = 0;
	listSize(playerList, &size);

	Player tmp;

	listGet(playerList, 0, &tmp);
	cought = tmp.statistic.threePoints;

	for (int i = 1; i < size; i++) {
		listGet(playerList, i, &tmp);
		if (cought < tmp.statistic.threePoints) {
			cought = tmp.statistic.threePoints;
		}
	}

	return cought;
}

float getAssistsMin(PtList playerList) {
	float cought = 0;
	int size = 0;
	listSize(playerList, &size);

	Player tmp;

	listGet(playerList, 0, &tmp);
	cought = tmp.statistic.assists;

	for (int i = 1; i < size; i++) {
		listGet(playerList, i, &tmp);
		if (cought > tmp.statistic.assists) {
			cought = tmp.statistic.assists;
		}
	}

	return cought;
}

float getAssistsMax(PtList playerList) {
	float cought = 0;
	int size = 0;
	listSize(playerList, &size);

	Player tmp;

	listGet(playerList, 0, &tmp);
	cought = tmp.statistic.assists;

	for (int i = 1; i < size; i++) {
		listGet(playerList, i, &tmp);
		if (cought < tmp.statistic.assists) {
			cought = tmp.statistic.assists;
		}
	}

	return cought;
}

float getBlocksMin(PtList playerList) {
	float cought = 0;
	int size = 0;
	listSize(playerList, &size);

	Player tmp;

	listGet(playerList, 0, &tmp);
	cought = tmp.statistic.blocks;

	for (int i = 1; i < size; i++) {
		listGet(playerList, i, &tmp);
		if (cought > tmp.statistic.blocks) {
			cought = tmp.statistic.blocks;
		}
	}

	return cought;
}

float getBlocksMax(PtList playerList) {
	float cought = 0;
	int size = 0;
	listSize(playerList, &size);

	Player tmp;

	listGet(playerList, 0, &tmp);
	cought = tmp.statistic.blocks;

	for (int i = 1; i < size; i++) {
		listGet(playerList, i, &tmp);
		if (cought < tmp.statistic.blocks) {
			cought = tmp.statistic.blocks;
		}
	}

	return cought;
}

float getFoulsMin(PtList playerList) {
	float cought = 0;
	int size = 0;
	listSize(playerList, &size);

	Player tmp;

	listGet(playerList, 0, &tmp);
	cought = tmp.statistic.fouls;

	for (int i = 1; i < size; i++) {
		listGet(playerList, i, &tmp);
		if (cought > tmp.statistic.fouls) {
			cought = tmp.statistic.fouls;
		}
	}

	return cought;
}

float getFoulsMax(PtList playerList) {
	float cought = 0;
	int size = 0;
	listSize(playerList, &size);

	Player tmp;

	listGet(playerList, 0, &tmp);
	cought = tmp.statistic.fouls;

	for (int i = 1; i < size; i++) {
		listGet(playerList, i, &tmp);
		if (cought < tmp.statistic.fouls) {
			cought = tmp.statistic.fouls;
		}
	}

	return cought;
}

float getNorm(float x, float min, float max) {
	return ((x - min) / (max - min));
}


void printTypeList(PtList sgList, PtList pgList,PtList asList) {
	printf("Tipo Shooting - Guard\n");
	listPrint(sgList);
	printf("\n");

	printf("Tipo Point - Guard\n");
	listPrint(pgList);
	printf("");

	printf("Tipo All - Star\n");
	listPrint(asList);
	printf("\n");
}

