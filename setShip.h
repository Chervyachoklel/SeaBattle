#ifndef SETSHIP_H
#define SETSHIP

#include "printMaps.h"
#include "time.h"

int setShip(int ship, int direction, int x, int y, Cell map[][10]);
void randomShip(Cell map[][10]);
void manually(Cell map[][10], Cell hits[][10]);

#endif // SETSHIP_H
