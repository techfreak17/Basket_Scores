#include <stdlib.h>
#include <stdio.h>
#include "player.h"

Date createDate(int day, int month, int year)
{
	Date newDate;
	newDate.day = (day < 1) ? 1 : (day > 31 ? 31 : day);
	newDate.month = (month < 1) ? 1 : (month > 12 ? 12 : month);
	newDate.year = (year < 1995) ? 1995 : (year > 2018 ? 2018 : year);

	return newDate;
}

Statistics createStatistics(float twoPoints, float threePoints, float assists, float fouls, float blocks, unsigned int gamesPlayed)
{
	Statistics newStats;

	newStats.twoPoints = twoPoints;
	newStats.threePoints = threePoints;
	newStats.assists = assists;
	newStats.fouls = fouls;
	newStats.blocks = blocks;
	newStats.gamesPlayed = gamesPlayed;

	return newStats;
}

Player createPlayer(int id, char name[], char team[], Date birthDate, char gender, Statistics statistic)
{
	Player newPlayer;

	newPlayer.id = id;
	strcpy_s(newPlayer.name, sizeof(newPlayer.name), name);
	strcpy_s(newPlayer.team, sizeof(newPlayer.team), team);
	newPlayer.birthDate = birthDate;
	newPlayer.gender = gender;
	newPlayer.statistic = statistic;

	return newPlayer;
}


void printPlayer(ptPlayer _this){
	printf("%3d | %-19s | %-12s | %02d/%02d/%04d | %.2f\t| %.2f\t| %.2f\t| %.2f\t| %.2f\t| %d \n", 
		_this->id, _this->name, _this->team, _this->birthDate.day, _this->birthDate.month, _this->birthDate.year, 
		_this->statistic.twoPoints, _this->statistic.threePoints, _this->statistic.fouls, _this->statistic.assists,
		_this->statistic.blocks, _this->statistic.gamesPlayed);

}


