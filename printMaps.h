#ifndef PRINTMAPS_H
#define PRINTMAPS_H

#include "windows.h"

#define EMPTY 0
#define SHIP 1
#define MISS 2
#define INJURED 3
#define KILLED 4
#define MAX_SIZE 10
#define MIN_SIZE 0

enum Direction{
    HORIZONTALLY = 0,
    VERTICALLY = 1
};

enum ConsoleColor;
void printColor(int x);

typedef struct Point{
    int x,y; //координаты начала корабля нужны для определения убийства корабля
}Point;

typedef struct Cell{
    Point point; //начальные координаты корабля
    int status; //значение ячейки
    int length; //длина корабля
    int direction; //направление корабля 0 - горизонтальное 1 - вертикальное
}Cell;

typedef struct Player{
    Cell ships[MAX_SIZE][MAX_SIZE];
    Cell hits[MAX_SIZE][MAX_SIZE];
    int score;
}Player;



void clearMap(Cell map[][MAX_SIZE]);
void clearStatus(Cell map[][MAX_SIZE]);
int inMap(int x, int y);

void printMaps(Cell ships[][MAX_SIZE], Cell hits[][MAX_SIZE]);
void refreshMaps(Cell ships[MAX_SIZE][MAX_SIZE], Cell hits[MAX_SIZE][MAX_SIZE], int score_human, int score_comp);

#endif // PRINTMAPS_H
