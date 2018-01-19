#ifndef SHOT_H
#define SHOT_H

#include "printMaps.h"

void markKill(Cell ships[][10], Cell hits[][10], int y, int x);
int wasKill(Cell ships[][10], Cell hits[][10], int i, int j);
int shot(Cell ships[][10], Cell hits[][10], int x, int y);


#endif // SHOT_H
