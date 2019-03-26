#pragma once
#include "list.h"

struct cluster;

typedef struct cluster *PtCluster;

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

