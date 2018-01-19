#ifndef BOT_H
#define BOT_H

#include "shot.h"
#include "printMaps.h"

typedef struct Recommend{// координаты в которые рекомендуется стрелять боту
    Point point;
}Recommend;

typedef struct Bot{
    Recommend recommend[4];
    Player maps;
    int score;
    int mode;//1 - рандомный выстрел, 2 - добивание
    int hit;//было ли попадание у бота
    int iterator;
} Bot;

void clearRec(Bot *bot);//очистить рекомендуемые координаты
void botFirstHit(Bot *bot, int xc, int yc);
void botSecondHit(Bot *bot, int xc, int yc, Cell hits[MAX_SIZE][MAX_SIZE]);
void botKilled(Bot *bot);
Point botSelectsPoint(Bot *bot);
Bot createBot();
#endif // BOT_H
