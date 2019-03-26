#pragma once

typedef struct date {
	int day, month, year;
} Date;

typedef struct statistics {
	float twoPoints;
	float threePoints;
	float assists;
	float fouls;
	float blocks;
	int gamesPlayed;
}Statistics;

typedef struct player {
	int id;
	char name[50];
	char team[50];
	Date birthDate;
	char gender;
	Statistics statistic;
} Player;

typedef Date* ptDate;
typedef Player * ptPlayer;
typedef Statistics * ptStatistics;

Date createDate(int day, int month, int year);
Statistics createStatistics(float twoPoints, float threePoints, float assists, float fouls, float blocks, unsigned int gamesPlayed);
Player createPlayer(int id, char name[], char team[], Date birthDate, char gender, Statistics statistics);

void printPlayer(ptPlayer _this);
