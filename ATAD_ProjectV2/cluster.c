#include "cluster.h"
#include "list.h"
#include <time.h>
#include <math.h>

PtCluster clusterCreate(PtList list);
Player getRandomObservation(PtList list);
long double getEucledianDistance(Player player, PtCluster cluster);
long double getError(Player player, PtCluster cluster);
void clearClustersLists(PtCluster clusters[], int clusterSize);
void assign(PtList list, PtCluster clusters[], int clusterSize);
void reCalculate(PtCluster clusters[], int clusterSize);
long double calculateError(PtCluster clusters[], int clusterSize);
void clusterPrint(PtCluster clusters[], int clusterSize);
void freeClusters(PtCluster clusters[], int clusterSize);


typedef struct cluster {
	float meanTwoPoints;
	float meanThreePoints;
	float meanAssists;
	float meanFouls;
	float meanBlocks;
	PtList members;
} Cluster;


PtCluster clusterCreate(PtList list)
{
	int size;
	listSize(list, &size);

	PtCluster newCluster = (PtCluster)malloc(sizeof(Cluster));
	Player player = getRandomObservation(list);
	newCluster->meanAssists = player.statistic.assists;
	newCluster->meanBlocks = player.statistic.blocks;
	newCluster->meanFouls = player.statistic.fouls;
	newCluster->meanThreePoints = player.statistic.threePoints;
	newCluster->meanTwoPoints = player.statistic.twoPoints;
	newCluster->members = listCreate(size);
	return newCluster;
}

void runAlgorithm(PtList list, unsigned int k, unsigned int maxIterations, long double deltaError)
{
	PtCluster clusters[10];
	int iterationNumber = 1, clusterSize = 0;
	long double prevError = 0;
	long double iterationError = 0;

	time_t t;


	for (clusterSize; clusterSize < k; clusterSize++) {
		srand((unsigned)time(&t) + clusterSize);
		clusters[clusterSize] = clusterCreate(list);
	}

	do {
		if (iterationNumber > 1) {
			prevError = iterationError;
		}

		clearClustersLists(clusters, clusterSize);

		assign(list, clusters, clusterSize);

		reCalculate(clusters, clusterSize);

		iterationError = calculateError(clusters, clusterSize);

		iterationNumber++;

	} while (iterationNumber<maxIterations && abs(prevError - iterationError)> deltaError);

	printf("\nBest Error: %lf\n\n", iterationError);
	clusterPrint(clusters, clusterSize);
	freeClusters(clusters, clusterSize);
}

Player getRandomObservation(PtList list) {

	int size, rank;
	listSize(list, &size);
	rank = rand() % size;
	Player player;
	listGet(list, rank, &player);
	return player;
}

long double getEucledianDistance(Player player, PtCluster cluster) {
	return sqrtl(
		powl(player.statistic.assists - cluster->meanAssists, 2) +
		powl(player.statistic.blocks - cluster->meanBlocks, 2) +
		powl(player.statistic.fouls - cluster->meanFouls, 2) +
		powl(player.statistic.twoPoints - cluster->meanTwoPoints, 2) +
		powl(player.statistic.threePoints - cluster->meanThreePoints, 2)
	);
}

long double getError(Player player, PtCluster cluster) {
	return powl(getEucledianDistance(player, cluster), 2);
}

void clearClustersLists(PtCluster clusters[], int clusterSize) {
	for (int i = 0; i < clusterSize; i++) {
		listClear(clusters[i]->members);
	}

}

void assign(PtList list, PtCluster clusters[], int clusterSize) {
	int size = 0, clusterListSize = 0, clusterNumber = 0;
	listSize(list, &size);
	Player toAssign;
	long double toAssignDistance = 0, tmpDistance = 0;



	for (int i = 0; i < size; i++) {
		listGet(list, i, &toAssign);
		toAssignDistance = getEucledianDistance(toAssign, clusters[clusterNumber = 0]);
		for (int j = 1; j < clusterSize; j++) {

			tmpDistance = getEucledianDistance(toAssign, clusters[j]);

			if (toAssignDistance > tmpDistance) {
				toAssignDistance = tmpDistance;
				clusterNumber = j;
			}
		}
		listSize(clusters[clusterNumber]->members, &clusterListSize);
		listAdd(clusters[clusterNumber]->members, clusterListSize++, toAssign);
	}
}

void reCalculate(PtCluster clusters[], int clusterSize) {
	double twoPoints = 0, threePoints = 0, assists = 0, blocks = 0, fouls = 0;
	int count = 0, clusterListSize = 0;
	Player tmp;

	for (int i = 0; i < clusterSize; i++) {
		listSize(clusters[i]->members, &clusterListSize);
		for (int j = 0; j < clusterListSize; j++) {
			listGet(clusters[i]->members, j, &tmp);

			twoPoints += tmp.statistic.twoPoints;
			threePoints += tmp.statistic.threePoints;
			assists += tmp.statistic.assists;
			blocks += tmp.statistic.blocks;
			fouls += tmp.statistic.fouls;
			count++;
		}

		clusters[i]->meanAssists = assists / count;
		clusters[i]->meanBlocks = blocks / count;
		clusters[i]->meanFouls = fouls / count;
		clusters[i]->meanThreePoints = threePoints / count;
		clusters[i]->meanTwoPoints = twoPoints / count;

		twoPoints = threePoints = fouls = blocks = assists = count = 0;

	}
}

long double calculateError(PtCluster clusters[], int clusterSize) {
	long double error = 0;
	int clusterListSize = 0;
	Player tmp;

	for (int i = 0; i < clusterSize; i++) {
		listSize(clusters[i]->members, &clusterListSize);
		for (int j = 0; j < clusterListSize; j++) {
			listGet(clusters[i]->members, j, &tmp);
			error += getError(tmp, clusters[i]);
		}
	}
	return error;
}

void clusterPrint(PtCluster clusters[], int clusterSize) {

	for (int i = 0; i < clusterSize; i++) {
		printf("Cluster Número %d - Valores do Centróide:\n\n\tTwo Points ->\t%lf\n\tThree Points ->\t%lf\n\tAssists ->\t%lf\n\tBlocks ->\t%lf\n\tFouls ->\t%lf\n\nLista de observações do Cluster:\n\n",
			i + 1, clusters[i]->meanTwoPoints, clusters[i]->meanThreePoints, clusters[i]->meanAssists, clusters[i]->meanBlocks, clusters[i]->meanFouls);
		listPrint(clusters[i]->members);
	}
}

void freeClusters(PtCluster clusters[], int clusterSize) {
	for (int i = 0; i < clusterSize; i++) {
		free(clusters[i]);
	}
}