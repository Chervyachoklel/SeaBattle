#include "setShip.h"
#include "printMaps.h"
#include "time.h"


int setShip(int ship, int direction, int x, int y, Cell map[][MAX_SIZE]){//функци€ дл€ установовки корабл€

    int i,j;
    int shipDirH = 0, shipDirV = 0;
    if (direction == HORIZONTALLY){
        shipDirH = ship - 1;
    }
    else shipDirV = ship - 1;

    if (inMap(x + shipDirH, y + shipDirV) == FALSE)
        return 1;//не удалось поставить корабль тк корабль вышел за границы карты

    for (i = y; i <= y + shipDirV; i++){
        for (j = x; j <= x + shipDirH; j++){
            if (i >= MIN_SIZE && i < MAX_SIZE && j >= MIN_SIZE && j < MAX_SIZE){
                if (map[i][j].status != EMPTY)
                    return 1;//не удалось поставить корабль тк р€дом есть другой корабль
            }
        }
    }

    for (i = y - 1; i <= y + shipDirV + 1; i++){
        for (j = x - 1; j <= x + shipDirH + 1 ; j++){
            if (i >= MIN_SIZE && i < MAX_SIZE && j >= MIN_SIZE && j < MAX_SIZE){
                if ((direction == HORIZONTALLY && i == y && j >= x && j<= x + shipDirH) || (direction == VERTICALLY && j == x && i >= y && i <= y + shipDirV)){
                    map[i][j].status = SHIP;
                    map[i][j].point.x = x;
                    map[i][j].point.y = y;
                    map[i][j].direction = direction;
                    map[i][j].length = ship;
                }
                else map[i][j].status = MISS;
            }
        }
    }
    return 0;// корабль удачно установлен
    /*
    в итоге примерно получитс€:
карта:   012345..
       0|022220
       1|021120      в клетку с начальными координатами x = 2 y = 1 установлен двухпалубный корабль
       2|022220      в клетках со значением 1 есть координаты начла корабл€ и его направление
       3|000000
       .
       .
     */
}

void randomShip(Cell map[][MAX_SIZE]){//функци€ рандомной расстановки кораблей
    int ships[] = {0,4,3,2,1};//сейчас 4 - однопалубных 3 - двухпалубных 2 - трехпалубных и 1 - четырехбалубный
    while(ships[1] != 0 || ships[2] !=0 || ships[3] != 0 || ships[4] != 0){// цикл прерветс€ когда все корабли закончатс€
        int ship = 1 + rand() % 4;// рандомно выбираем кораль по его длине
        if (ship >= 1 && ship <= 4 && ships[ship] > 0){
                int direction = rand() % 2;//рандомно выбираем направление корабл€
                int x, y;
                x = rand() % MAX_SIZE;
                y = rand() % MAX_SIZE;//рандомно выбираем начальные координаты корабл€
                int e = setShip(ship, direction, x, y, map);//устанавливаем корабль
                if (e == 0)
                    ships[ship]--;//если корабль успешно установлен, то -1 корабль данной длины
            }
        }
    clearStatus(map);//оставл€ем на карте только корабли
}

void manually(Cell map[][MAX_SIZE], Cell hits[][MAX_SIZE]){//расстановка кораблей вручную
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int ships[] = {0,4,3,2,1};
    int direction;
    char dir, s[10], a[10];
    int x = 0, y = 0, e = 0, ship = 0;
    while(ships[1] != 0 || ships[2] !=0 || ships[3] != 0 || ships[4] != 0){
        system("cls");
        printMaps(map, hits);
        printf("” вас: однопалубных - %d, двухпалубных - %d, трЄхпалубных - %d, четырЄхпалубных - %d\n",ships[1],ships[2],ships[3],ships[4]);
        printf("¬ыберите корабль:\n");
        scanf("%s", &s);
        ship = atoi(s);
        if (strlen(s) > 1 || ship == 0){
            printf("Ќужно ввести одну цифру!\n");
            system("pause");
            continue;
        }
        if (ship >= 1 && ship <= 4 && ships[ship] > 0){ // если есть такой корабль и он длиной от 1 до 4, то продолжаем ввод
                if (ship != 1){
                        printf("¬ыберите ориентацию корабл€: h - горизонтальна€, v - вертикальна€\n");
                        dir = getche(); //направление корабл€
                        if (dir == 'h')
                            direction = HORIZONTALLY;
                        else if (dir == 'v') direction = VERTICALLY;
                        else direction = -1;
                        printf("\n");
                        }
                else direction = HORIZONTALLY;
                if (direction != HORIZONTALLY && direction != VERTICALLY){
                    printf("Ќеверно выбранно направление\n");
                    system("pause");
                    continue;
                }
                printf("¬ведите координаты:\n");
                scanf("%s",&s);
                strncpy(a,s,1);
                int sz = strlen(s);
                memmove(s,s+1,sz-1);
                s[sz-1] = 0;
                y = atoi(s) - 1;
                x = *a - 'a';
                if (inMap(x,y) != TRUE){
                    printf("Ќеверные координаты\n");
                    system("pause");
                    continue;
                }
                e = setShip(ship, direction, x, y, map);// 1 - не удалось поставить корабль 2 - корабль установлен
                if (e == 0){
                    ships[ship]--;
                }
                else{
                    printf("Ќе удалось поставить корабль\n");
                    system("pause");
                }
            }
        else {
            printf("“акого коробл€ нет\n");
            system("pause");
            continue;
        }
    }
    clearStatus(map);//оставл€ем на карте только корабли
}
