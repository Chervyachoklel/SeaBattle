#include "shot.h"
#include "printMaps.h"

void markKill(Cell ships[][MAX_SIZE], Cell hits[][MAX_SIZE], int y, int x){//помечаем оставшиеся клетки вокруг корабля и клетки убитого корабля
    hits[y][x].status = KILLED;
    ships[y][x].status = KILLED;//помечаем убитый корабль на двух картах
    int i,j;
    for (i = y - 1; i <= y+1; i++)
        for (j = x - 1; j <= x + 1; j++)
            if (inMap(i,j) == 1)
                if(ships[i][j].status == EMPTY){
                        ships[i][j].status = MISS;
                        hits[i][j].status = MISS;
                }
}

int wasKill(Cell ships[][MAX_SIZE], Cell hits[][MAX_SIZE], int i, int j){
    int m,x,y;
    x = ships[j][i].point.x;
    y = ships[j][i].point.y;
    if (ships[j][i].direction == HORIZONTALLY){//для горизонтального корабля
        for (m = x; m < x + ships[j][i].length; m++){
                if(ships[y][m].status == SHIP)
                    return 0;//не убит т.к. у горизонтального корабля ещё есть целые клетки
                }
        }
    if (ships[j][i].direction == VERTICALLY){//для вертикального корабля
        for (m = y; m < y + ships[j][i].length; m++){
                if(ships[m][x].status == SHIP)
                    return 0;//не убит т.к. у вертикального корабля еще есть целые клетки
                }
        }
    //если программа прошла предыдущие условия, значит у корабля не осталось целых клеток т.е. корабль убит
    if (ships[j][i].direction == HORIZONTALLY){//для горизонтального корабля
        for (m = x; m < x + ships[j][i].length; m++){
                if(ships[y][m].status == INJURED){
                    markKill(ships, hits, y, m);// помечаем все оставшиеся клетки вокруг корабля
                }
            }
        }
    if (ships[j][i].direction == VERTICALLY){//для вертикального корабля
        for (m = y; m < y + ships[j][i].length; m++){
                if(ships[m][x].status == INJURED){
                    markKill(ships, hits, m, x);// помечаем все оставшиеся клетки вокруг корабля
                }
            }
        }
    return 1;//убит
}

int shot(Cell ships[][MAX_SIZE], Cell hits[][MAX_SIZE], int x, int y){
    if (ships[y][x].status == SHIP){//попали в корабль
        hits[y][x].status = INJURED;//
        ships[y][x].status = INJURED;//подбитый корабль
        if(wasKill(ships,hits, x, y) == TRUE){//проверяем был ли убит корабль
            return 0;//убил
        }
        return 1;//попал
    }else {
        hits[y][x].status = MISS;//отмечаем клетку в которой нет корабля
        ships[y][x].status = MISS;
        return 2;//промах
    }
}

