/* MINI-PROJETO 2 ATAD 2018
* Identificacao dos Alunos:
*
*      Numero: 170221078 | Nome: César Fontinha
*      Numero: 170221091 | Nome: Gonçalo Durães
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "program.h"
#include "list.h"

int equalsStringIgnoreCase(char str1[], char str2[]);
int isLoaded(int boolean);
void printCommandsMenu();
void exitProgram(PtList _this);


int main(int argc, char** argv) {
setlocale(LC_ALL, "Portuguese");

	PtList basket = listCreate(400);


	char command[21];
	int quit = 0, hasLoaded = 0;
	while (!quit) {

		printCommandsMenu();
		fgets(command, sizeof(command), stdin);

		command[strlen(command) - 1] = '\0';

		if (equalsStringIgnoreCase(command, "QUIT")) {
			quit = 1; 
		}
		else if (equalsStringIgnoreCase(command, "LOAD")) {

			char *playersName[21];
			char *gameName[21];

			if (hasLoaded) {
				printf("Já existem dados em memória, por favor faça CLEAR antes de fazer LOAD novamente\n");
			}
			else {
				printf_s("\nPor favor insira o nome do ficheiro de JOGADORES a abrir: \nResposta-> ");
				scanf_s("%[^\n]s", playersName, sizeof(playersName));
				getchar();


				printf_s("\nPor favor insira o nome do ficheiro de JOGOS a abrir: \nResposta-> ");
				scanf_s("%[^\n]s", gameName, sizeof(gameName));
				getchar();


				hasLoaded = importFromFile(playersName, gameName, basket);
			}
		}
		else if (equalsStringIgnoreCase(command, "CLEAR")) {
			if(isLoaded(hasLoaded)){
				clearRecords(basket);
				hasLoaded = 0;
			} else {
				printf("Por favor carregue os ficheiros primeiro! \n");
			}
		}
		else if (equalsStringIgnoreCase(command, "SHOW")) {
			if(isLoaded(hasLoaded)){
				showFunction(basket);
			} else {
				printf("Por favor carregue os ficheiros primeiro! \n");
			}
		}
		else if (equalsStringIgnoreCase(command, "SORT")) {
			int choice = 0;

			if (isLoaded(hasLoaded)) {
				printf_s("\n Selecione Opção: \n1 - Ordenar por Nome\n2 - Ordenar por data de nascimento\n3 - Ordenar por Número de jogos\nResposta-> ");
				scanf_s("%d", &choice);
				getchar();

				if (choice == 1) {
					printSortedNames(basket);
				}
				else if (choice == 2){
					printSortedDates(basket);
				}
				else if (choice == 3) {
					printSortedNumberOfGames(basket);
				}
				else {
					printf("Opção Inválida!");
				}
				
			} else {
				printf("Por favor carregue os ficheiros primeiro! \n");
			}
		}
		else if (equalsStringIgnoreCase(command, "AVG")) {
			if (isLoaded(hasLoaded)) {
				PtList tmp = averageStatistics(basket);
				sortByAvg(tmp);
				listPrint(tmp);
				free(tmp);
			} else {
				printf("Por favor carregue os ficheiros primeiro! \n");
			}
		}
		else if (equalsStringIgnoreCase(command, "NORM")) {
			if (isLoaded(hasLoaded)) {
				listPrint(normalizeStatistics(averageStatistics(basket)));
			} else {
				printf("Por favor carregue os ficheiros primeiro! \n");
			}
		}
		else if (equalsStringIgnoreCase(command, "TYPE")) {
			if (isLoaded(hasLoaded)) {
				playersType(basket);
			} else {
				printf("Por favor carregue os ficheiros primeiro \n");
			}
		}
		else if (equalsStringIgnoreCase(command, "CHECKTYPE")) {
			if (isLoaded(hasLoaded)) {
				checkType(averageStatistics(basket));
			} else {
				printf("Por favor carregue os ficheiros primeiro! \n");
			}
		}
		else if (equalsStringIgnoreCase(command, "KMEANS")) {
			if (isLoaded(hasLoaded)) {
				int k,maxIterations;
				double deltaError;

				printf_s("\nPor favor indique o número de Clusters desejados\nResposta-> ");
				scanf_s("%d", &k);
				getchar();

				printf_s("\nPor favor indique o número de Iterações desejadas\nResposta-> ");
				scanf_s("%d", &maxIterations);
				getchar();

				printf_s("\nPor favor indique o Delta Error associado\nResposta-> ");
				scanf_s("%lf", &deltaError);
				getchar();

				KMEANS(normalizeStatistics(averageStatistics(basket)),k,maxIterations,deltaError);
			} else {
				printf("Por favor carregue os ficheiros primeiro! \n");
			}
		}
		else {
			printf("Comando não encontrado.\n");
		}
	}

	exitProgram(basket);

	return (EXIT_SUCCESS);
}

int equalsStringIgnoreCase(char str1[], char str2[]) {

	return (_strcmpi(str1, str2) == 0);
}

int isLoaded(int boolean) {

	if(boolean == 1) {
	     return 1;		
	} else {
	     printf("SEM DADOS CARREGADOS!\n");
	     return 0;
	}
}

void printCommandsMenu() {
	printf("\n===================================================================================");
	printf("\n               MINI-PROJECT (Phase 2) : Basket Scores                              ");
	printf("\n===================================================================================");
	printf("\nA. Load files of Players and Games (LOAD).");
	printf("\nB. Delete the information contained (CLEAR).");
	printf("\nC. Print the information of the players (SHOW, SORT).");
	printf("\nD. Specific info about players' performance during the games (AVG, NORM, TYPE, CHECKTYPE).");
	printf("\nE. Advanced indicator (KMEANS)");
	printf("\nF. Exit (QUIT)\n\n");
	printf("COMMAND > ");
}

void exitProgram(PtList _this) {
	printf("Até uma próxima! :D ");
	free(_this);
}