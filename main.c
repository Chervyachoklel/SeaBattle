#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <windows.h>


typedef struct Point{
    int x,y; //координаты начала корабля нужны для определения убийства корабля
}Point;

typedef struct Cell{
    Point point; //начальные координаты корабля
    int status; //значение ячейки
    int length; //длина корабля
    char direction; //направление корабля
}Cell;

typedef struct Player{
    Cell ships[10][10];
    Cell hits[10][10]; //возможно можно int
}Player;

void clearMap(Cell map[][10]){//очищаю значения после создания игрока
    int i,j;
    for (i = 0; i < 10; i++){
        for (j = 0; j < 10; j++){
            map[i][j].status = 0;
            map[i][j].direction = 0;
            map[i][j].length = 0;
            map[i][j].point.x = -1;
            map[i][j].point.y = -1;
        }
    }
}

void clearStatus(Cell map[][10]){//после расстановки очищаю карту от лишних значений
    int i, j;
    for (i = 0; i < 10; i++){
        for(j = 0; j < 10; j++){
            if (map[i][j].status == 2){
                map[i][j].status = 0;
                map[i][j].direction = 0;
                map[i][j].length = 0;
                map[i][j].point.x = -1;
                map[i][j].point.y = -1;
            }
        }
    }
}

void printMap(Cell map[][10]){//выводит карту в консоль
    int size = 10;
    printf(" #0123456789#\n");
    int i,j;
    for (i = 0; i < size; i++){
        for (j = 0; j < size; j++){
            if (j == 0)
                printf("%d|",i);
            printf("%d", map[i][j].status);
            if (j == 9)
                printf("|");
        }
        printf("\n");
    }
    printf(" #----------#\n");
}

void printMaps(Cell ships[][10], Cell hits[][10]){//выводит две карты в консоль ships и hits
    int size = 10;
    printf(" #0123456789#  #0123456789#\n");
    int i,j;
    for (i = 0; i < size; i++){
        for (j = 0; j < 2 * size ; j++){
            if (j == 0)
                printf("%d|",i);
            if (j >= 0 && j <= 9)
                printf("%d", ships[i][j].status);
            if (j >=10 && j <= 19)
                printf("%d", hits[i][j-10].status); //костыль
            if (j == 9)
                printf("| %d|", i);
            if (j == 19)
                    printf("|");
        }
        printf("\n");
    }
    printf("\n");
}

int setShip(int ship, char dir, int x, int y, Cell map[][10]){//функция для установовки корабля
    int i,j;
    int shipDirH = 0, shipDirV = 0;
    if (dir == 'h'){
        shipDirH = ship - 1;
    }
    else shipDirV = ship - 1;

    if (x + shipDirH > 9 || y + shipDirV > 9)
        return 1;//не удалось поставить корабль тк корабль вышел за границы карты

    for (i = y; i <= y + shipDirV; i++){
        for (j = x; j <= x + shipDirH; j++){
            if (i >=0 && i < 10 && j >=0 && j<10){
                if (map[i][j].status != 0)
                    return 1;//не удалось поставить корабль тк рядом есть другой корабль
            }
        }
    }

    for (i = y - 1; i <= y + shipDirV + 1; i++){
        for (j = x - 1; j <= x + shipDirH + 1 ; j++){
            if (i >=0 && i < 10 && j >=0 && j<10){
                if ((dir == 'h' && i == y && j >= x && j<= x + shipDirH) || (dir == 'v' && j == x && i >= y && i <= y + shipDirV)){
                    map[i][j].status = 1;
                    map[i][j].point.x = x;
                    map[i][j].point.y = y;
                    map[i][j].direction = dir;
                    map[i][j].length = ship;
                }
                else map[i][j].status = 2;
            }
        }
    }
    return 0;// корабль удачно установлен
    /*
    в итоге примерно получится:
карта:   012345..
       0|022220
       1|021120      в клетку с начальными координатами x = 2 y = 1 установлен двухпалубный корабль
       2|022220      в клетках со значением 1 есть координаты начла корабля и его направление
       3|000000
       .
       .
     */
}

void randomShip(Cell map[][10]){//функция рандомной расстановки кораблей
    int ships[] = {0,4,3,2,1};//сейчас 4 - однопалубных 3 - двухпалубных 2 - трехпалубных и 1 - четырехбалубный
                            //можно изменить количество кораблей
    while(ships[1] != 0 || ships[2] !=0 || ships[3] != 0 || ships[4] != 0){// цикл прервется когда все корабли закончатся
        int ship = 1 + rand() % 4;// рандомно выбираем кораль по его длине
        if (ship >= 1 && ship <= 4 && ships[ship] > 0){
                char dir;
                int z = rand() % 2;
                if (z == 0)
                    dir = 'h';
                else dir = 'v';//рандомно выбираем направление корабля
                int x,y;
                x = rand()%10;
                y = rand()%10;//рандомно выбираем начальные координаты корабля
                int e = setShip(ship, dir, x, y, map);//устанавливаем корабль
                if (e == 0)
                    ships[ship]--;//если корабль успешно установлен, то -1 корабль данной длины

            }

        }
}

void manually(Cell map[][10]){//расстановка кораблей вручную
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int ships[] = {0,4,3,2,1};
    char dir;
    int x = 0, y = 0, e = 0, ship = 0;

    while(ships[1] != 0 || ships[2] !=0 || ships[3] != 0 || ships[4] != 0){
        //printf("У вас: %d - однопалубных кораблей %d - двухпалубных кораблей %d - трехпалубных кораблей %d - четырехпалубных кораблей\n",ships[1],ships[2],ships[3],ships[4]);
        printf("Выберите корабль:\n");
        scanf("%d", &ship);
        if (ship >= 1 && ship <= 4 && ships[ship] > 0){ // если есть такой корабль и он длиной от 1 до 4, то продолжаем ввод
                printf("Выберите ориентацию корабля: h - горизонтальная, v - вертикальная\n");
                dir = getche(); //направление корабля
                printf("\n");
                printf("Выберите клетку: x y\n");
                scanf("%d",&x); //крашится
                scanf("%d",&y);
                /*char z;
                z = getche();
                x = z - '0';
                printf(" ");
                z = getche();
                y = z - '0';*/
                e = setShip(ship, dir, x, y, map);// 1 - не удалось поставить корабль 2 - корабль установлен
                if (e == 0){
                    ships[ship]--;
                }
                else{
                    printf("Не удалось поставить корабль\n");
                    system("pause");
                }
                printMap(map);
            }
        else {
            printf("Такого коробля нет\n");
            system("pause");
            continue;
        }
    }
}

int inMap(int x, int y){//проверяем находится ли точка в пределах карты
    if (x >= 0 && x <=9 && y>=0 && y <= 9)
        return 1;//Точка в пределах карты
    else return 0;
}

void hitting(Cell hits[][10],int x, int y){//помечаем клетки в которых точно нет корабля
    if (inMap(y-1,x-1)==1){                // 202
        hits[y-1][x-1].status = 2;         // 030  2 - это клетки в которых нет корабля
    }                                      // 202  3 - это клетка в которую мы сейчас попали
    if (inMap(y-1,x+1)==1){
        hits[y-1][x+1].status = 2;
    }
    if (inMap(y+1,x-1)==1){
        hits[y+1][x-1].status = 2;
    }
    if (inMap(y+1,x+1)==1){
        hits[y+1][x+1].status = 2;
    }

}

void markKill(Cell ships[][10], Cell hits[][10], int i, int j){//помечаем оставшиеся клетки вокруг корабля и клетки убитого корабля
    hits[i][j].status = 4;
    ships[i][j].status = 4;//помечаем убитый корабль на двух картах
    if (inMap(i-1,j) == 1){
        if (hits[i-1][j].status == 0){
            hits[i-1][j].status = 2;
        }
    }
    if (inMap(i+1,j) == 1){
        if (hits[i+1][j].status == 0){
            hits[i+1][j].status = 2;
        }
    }
    if (inMap(i,j-1) == 1){
        if (hits[i][j-1].status == 0){
            hits[i][j-1].status = 2;
        }
    }
    if (inMap(i,j+1) == 1){
        if (hits[i][j+1].status == 0){
            hits[i][j+1].status = 2;
        }
    }
}

int wasKill(Cell ships[][10], Cell hits[][10], int i, int j){
    int m,x,y;
    x = ships[j][i].point.x;
    y = ships[j][i].point.y;
    if (ships[j][i].direction=='h'){//для горизонтального корабля
        for (m = x; m < x + ships[j][i].length; m++){
                if(ships[y][m].status == 1)
                    return 0;//не убит т.к. у горизонтального корабля ещё есть целые клетки
                }
        }
    if (ships[j][i].direction=='v'){//для вертикального корабля
        for (m = y; m < y + ships[j][i].length; m++){
                if(ships[m][x].status == 1)
                    return 0;//не убит т.к. у вертикального корабля еще есть целые клетки
                }
        }
    //если программа прошла предыдущие условия, значит у корабля не осталось целых клеток т.е. корабль убит
    if (ships[j][i].direction == 'h'){//для горизонтального корабля
        for (m = x; m < x + ships[j][i].length; m++){
                if(ships[y][m].status == 3){
                    markKill(ships,hits,y,m);// помечаем все оставшиеся клетки вокруг корабля
                }
            }
        }
    if (ships[j][i].direction == 'v'){//для вертикального корабля
        for (m = y; m < y + ships[j][i].length; m++){
                if(ships[m][x].status == 3){
                    markKill(ships,hits,m,x);// помечаем все клетки вокруг потопленного корабля 2
                }
            }
        }
    return 1;//убит
}

int shot(Cell ships[][10], Cell hits[][10], int x, int y){
    if (ships[y][x].status == 1){//попали в корабль
        hitting(hits, x, y);//помечаем клетки в которых точно нет корабля
        hits[y][x].status = 3;//
        ships[y][x].status = 3;//подбитый корабль
        if(wasKill(ships,hits, x, y) == 1){//проверяем был ли убит корабль
            return 0;//убил
        }
        return 1;//попал
    }else {
        hits[y][x].status = 2;//отмечаем клетку в которой нет корабля
        return 2;//промах
    }
}

typedef struct Recommend{// координаты в которые рекомендуется стрелять боту
    Point point[4];
}Recommend;


void clearRec(Recommend rec){//очистить рекомендуемые координаты
    int i;
    for (i = 0; i < 4; i++){
        rec.point[i].x = -1;
        rec.point[i].y = -1;
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);//для корректного изображения русских букв в консоле
    srand(time(NULL));//чтобы рандом работал лучше

    Player human;//создаем игрока человека
    clearMap(human.hits);//обнуляем значения карты
    clearMap(human.ships);
    printf("Как расставить корабли?\n 1 - случайно \n 2 - вручную\n");

    while(1){
        int n;
        scanf("%d", &n);
        if (n == 1){
            randomShip(human.ships);//расставляем корабли рандомно
            clearStatus(human.ships);//оставляем на карте только корабли
            printMaps(human.ships,human.ships);//выводим карту кораблей и карту попаданий
            break;
        }
        else if (n == 2){
            manually(human.ships);//расставляем корабли вручную
            system("cls");
            clearStatus(human.ships);//оставляем на карте только корабли
            printMaps(human.ships, human.hits);
            break;
        }
        else printf("Некорректный ввод\n");
    }
    Player comp;//создаем бота
    clearMap(comp.hits);//обнуляем значения карты
    clearMap(comp.ships);
    randomShip(comp.ships);// рандомно расставляем корабли
    clearStatus(comp.ships);// оставляем на карте только корабли
    //printMaps(comp.ships,comp.hits);
    int x,y;//координаты точки которые вводит человек
    int flag;//
    int xc,yc,hit = 0,j = 0;
    Recommend recommend;//создаем рекомендуемые клетки для стрельбы бота
    clearRec(recommend);//очищаем значения
    printf("rec: x: %d",recommend.point[0].x);
    system("pause");
    int turn = 1;//1 - ходит человек, 2 - ходит компьютер
    int mode = 1;//для бота: 1 - рандомный выстрел 2 - добивание
    int win1 = 0,win2 = 0;
    while(win1 < 10 && win2 < 10){
        if (turn == 1){
            printf("Human:\n");
            printMaps(human.ships,human.hits);
            printf("Comp:\n");
            printMaps(comp.ships,comp.hits);
            printf("win1: %d win2: %d\n",win1,win2);
            printf("Введите координаты\n");
            scanf("%d%d",&x,&y);
            if (human.hits[y][x].status != 0){
                printf("Невозможно выстелить\n");
                continue;
            }
            flag = shot(comp.ships,human.hits,x,y);
            if (flag == 0){
                win1++;
                printf("Убил\n");
            }
            if (flag == 1){
                printf("Попал\n");
            }
            if (flag == 2){
                printf("Промах\n");
                turn = 2;
            }
        }
        else {//ход пк
            if (mode == 1){
                xc = rand() % 10;
                yc = rand() % 10;
            }
            else {//mode == 2
                if (hit != 0){
                    if (recommend.point[j].x == -1){
                        j++;
                        continue;
                    }
                    xc = recommend.point[j].x;
                    yc = recommend.point[j].y;
                    j++;
                    if (inMap(xc,yc)==0){
                        continue;
                    }
                }
            }

            if (comp.hits[yc][xc].status != 0){

                continue;
            }
            flag = shot(human.ships, comp.hits, xc, yc);
            if (flag == 0){
                win2++;
                int i;
                for (i = 0; i < 4; i++){
                    recommend.point[i].x = -1;
                    recommend.point[i].y = -1;
                }
                j = 0;
                mode = 1;
                hit = 0;
            }
            if (flag == 1){
                if (hit == 1){
                    int k;
                    clearRec(recommend);
                    if (inMap(xc+1,yc) == 1){
                            if(comp.hits[yc][xc+1].status == 3){//если справа предыдущее поподание
                                recommend.point[0].x = xc-1;
                               recommend.point[0].y = yc;
                               recommend.point[1].x = xc+2;
                               recommend.point[1].y = yc;
                               recommend.point[2].x = xc-2;
                               recommend.point[2].y = yc;
                               recommend.point[3].x = xc+3;
                               recommend.point[3].y = yc;

                            }

                    }
                    if (inMap(xc-1,yc) == 1){
                        if(comp.hits[yc][xc-1].status == 3){//если слева предыдущее попадание
                               recommend.point[0].x = xc-2;
                               recommend.point[0].y = yc;
                               recommend.point[1].x = xc+1;
                               recommend.point[1].y = yc;
                               recommend.point[2].x = xc-3;
                               recommend.point[2].y = yc;
                               recommend.point[3].x = xc+2;
                               recommend.point[3].y = yc;
                            }
                    }
                    if (inMap(xc,yc+1) == 1){
                        if(comp.hits[yc+1][xc].status == 3){//если снизу предыдущее попадание
                                recommend.point[0].x = xc;
                               recommend.point[0].y = yc + 2;
                               recommend.point[1].x = xc;
                               recommend.point[1].y = yc - 1;
                               recommend.point[2].x = xc;
                               recommend.point[2].y = yc - 2;
                               recommend.point[3].x = xc;
                               recommend.point[3].y = yc + 3;

                            }
                    }
                    if (inMap(xc,yc-1) == 1){
                        if(comp.hits[yc-1][xc].status == 3){//если сверху предыдущее попадание
                                recommend.point[0].x = xc;
                               recommend.point[0].y = yc - 2;
                               recommend.point[1].x = xc;
                               recommend.point[1].y = yc + 1;
                               recommend.point[2].x = xc;
                               recommend.point[2].y = yc - 3;
                               recommend.point[3].x = xc;
                               recommend.point[3].y = yc + 2;
                            }
                    }
                    j = 0;
                    hit = 2;
                }
                if (hit == 0){
                    if (inMap(xc+1,yc) == 1){
                        recommend.point[0].x = xc + 1;
                        recommend.point[0].y = yc;
                    }
                    if (inMap(xc-1,yc) == 1){
                        recommend.point[1].x = xc - 1;
                        recommend.point[1].y = yc;
                    }
                    if (inMap(xc,yc+1) == 1){
                        recommend.point[2].x = xc;
                        recommend.point[2].y = yc + 1;
                    }
                    if (inMap(xc,yc-1) == 1){
                        recommend.point[3].x = xc;
                        recommend.point[3].y = yc - 1;
                    }
                    hit = 1;

                }

                mode = 2;//2
            }
            if (flag == 2){

                    turn = 1;//1
            }
            /*printf("xc: %d yc %d\n",xc,yc);
            printf("hits %d\n",hit);
            printf("win2: %d\n",win2);
            printf("x0: %d y0: %d x1: %d y1: %d x2: %d y2: %d x3: %d y3: %d \n",recommend.point[0].x,recommend.point[0].y,recommend.point[1].x,recommend.point[1].y,recommend.point[2].x,recommend.point[2].y,recommend.point[3].x,recommend.point[3].y);
            printMaps(comp.ships, comp.hits);
            */
            system("pause");
        }
    }
    if (win1 == 10){
        printf("Вы победили\n");
    }
    if (win2 == 10){
        printf("Вы проиграли\n");
    }
    return 0;
}
