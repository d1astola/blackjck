#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAXCRD 15
#define END 60

struct deckGame {
	unsigned short 	deck[MAXCRD];
	unsigned short 	deckScore[MAXCRD];
	unsigned char 	allScore;
	unsigned char	aces; // Состояние колоды 0 - нет тузов 1 - есть тузы 2 - тузы 1 оч.
};

unsigned char countCards = 0;

void mixCards(unsigned short *);
void showCards(struct deckGame *);
void generateDeck(struct deckGame *, unsigned short *);
void addDeck(struct deckGame *, unsigned short *);
void checkWin(unsigned short, unsigned short);
void clearDeck(struct deckGame *);
bool AI(struct deckGame *);

/* Точка входа в программу */
void main(void) {
	struct 	deckGame playerDeck;
	struct 	deckGame AIDeck;
	char 	yn[3];

	while (1) {
		clearDeck(&playerDeck);
		clearDeck(&AIDeck);
		unsigned short randomDeck[52], i;
		i = 0;
		while (i < 52) {
			randomDeck[i] = i++;
		}

		/* Генерация карт на руках */
		mixCards(randomDeck);
		generateDeck(&playerDeck, randomDeck);
		generateDeck(&AIDeck, randomDeck);

		while (1) {
			printf("У вас на руках: \n");
			showCards(&playerDeck);
			printf("Очков: %d\n", playerDeck.allScore);
			if (playerDeck.allScore > 21) {
				printf("%d - перебор\n", playerDeck.allScore);
				break;
			}
			else if (playerDeck.allScore == 21) {
				printf("У вас 21\n");
				break;
			}
			else {
				putchar('\n');
				printf("Желаете взять еще одну карту? (y\\n): ");
				scanf("%s", yn);
				if (strcmp(yn, "y") == 0) {
					addDeck(&playerDeck, randomDeck);
				}
				else if (strcmp(yn, "n") == 0) {
					break;
				}
				else {
					printf("Недопустимый символ или строка!\n");
				}
			}
			if (AI(&AIDeck)) {
				addDeck(&AIDeck, randomDeck);
			}
		}

		while (AI(&AIDeck)) {
			addDeck(&AIDeck, randomDeck);
		}

		printf("У противника на руках: \n");
		showCards(&AIDeck);
		printf("Очков: %d\n", AIDeck.allScore);
		checkWin(AIDeck.allScore, playerDeck.allScore);

		printf("Желаете продолжить игру? (y\\n): ");
		scanf("%s", yn);
		if (strcmp(yn, "y") == 0) {
			countCards = 0;
			continue;
		}
		else if (strcmp(yn, "n") == 0) {
			break;
		}
		else {
			printf("Недопустимый символ или строка!\n");
		}
	}
	return;
}

void swapCards(unsigned short *, unsigned char, unsigned char);

/* Перемешивание колоды в начале */
void mixCards(unsigned short *deck) {
	srand(time(NULL));

	for (unsigned char i = 0; i < 52; i++) {
		int rnd = rand() % 52;
		if (i == rnd) {
			continue;
		}
		else {
			swapCards(deck, i, rnd);
		}
	}
	return;
}

/* Свап карт в колоде */
void swapCards(unsigned short *deck, unsigned char x, unsigned char y) {
	unsigned char tmp;
	tmp = deck[x];
	deck[x] = deck[y];
	deck[y] = tmp;
	return;
}

void score(struct deckGame *);

/* Генерация начальной колоды из 2-х карт */
void generateDeck(struct deckGame *deck, unsigned short *rndDeck) {
	deck->deck[0] = rndDeck[countCards++];
	deck->deck[1] = rndDeck[countCards++];
	deck->deck[2] = END;
	score(deck);
	return;
}

/* Добавляет карту в руку */
void addDeck(struct deckGame *deck, unsigned short *rndDeck) {
	unsigned char i = 0;
	while (deck->deck[i] != END) {git
		i++;
	}
	deck->deck[i] = rndDeck[countCards++];
	deck->deck[i + 1] = END;
	score(deck);
	return;
}

/* Интеллект на основе рандомизатора */
/* Возвращает false если считает что не стоит больше брать карт */
bool AI(struct deckGame *deck) {
	srand(time(NULL));
	if (deck->allScore > 20) {
		return false;
	}
	else {
		if (deck->allScore < 16) {
			return true;
		}
		else if (deck->allScore < 18) {
			if ((rand() % 10) > 6) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
}

/* Массив для подсчета очков карт */
unsigned char cardScore[13] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

/* Подсчет очков */
void score(struct deckGame *deck) {
	unsigned char valueCard;
	deck->allScore = 0;
	for (unsigned char i = 0; (valueCard = (deck->deck[i] / 4)) != 15; i++) {
		if (deck->aces == 2 && valueCard == 0) {
			deck->allScore += 1;
		}
		else if (valueCard == 0) {
			deck->aces = 1;
			deck->allScore += 11;
		}
		else {
			deck->allScore += cardScore[valueCard];
		}
	}
	if (deck->allScore > 21 && deck->aces == 1) {
		deck->aces = 2;
		score(deck);
	}
	return;
}

/* Вывод результатов игры */
void checkWin(unsigned short enemyScore, unsigned short playerScore) {
	if (enemyScore < 21 && playerScore < 21) {
		if (enemyScore > playerScore) {
			printf("Противник победил!\n");
		}
		else if (playerScore > enemyScore) {
			printf("Вы победили!\n");
		}
		else {
			printf("Ничья!\n");
		}
	}
	else {
		if (enemyScore < playerScore) {
			printf("Противник победил!\n");
		}
		else if (playerScore < enemyScore) {
			printf("Вы победили!\n");
		}
		else {
			printf("Ничья!\n");
		}
	}
	return;
}

/* Очищение структуры */
void clearDeck(struct deckGame *deck) {
	for (unsigned short i = 0; i < MAXCRD; i++) {
		deck->deck[i] = 0;
		deck->deckScore[i] = 0;
	}
	deck->allScore = 0;
	deck->aces = 0;
	return;
}

/* Выводит значение карт на экран */
void showCards(struct deckGame *deck) {
	unsigned char valueCard;
	for (unsigned char i = 0; (valueCard = (deck->deck[i] / 4)) != 15; i++) {
		if (valueCard > 0 && valueCard < 10) {
			printf("%d ", valueCard + 1);
		}
		else {
			switch(valueCard) {
				case 0:
					printf("Туз ");
					break;
				case 10:
					printf("Валет ");
					break;
				case 11:
					printf("Дама ");
					break;
				case 12:
					printf("Король ");
					break;
				default:
					printf("Ошибка! Неверное значение колоды!\n");
					break;
			}
		}
		valueCard = deck->deck[i] % 4;
		switch(valueCard) {
			case 0:
				printf("пики\n");
				break;
			case 1:
				printf("червы\n");
				break;
			case 2:
				printf("буби\n");
				break;
			case 3:
				printf("крести\n");
				break;
			default:
				printf("Ошибка! Неверное значение масти!\n");
				break;
		}
	}
	return;
}