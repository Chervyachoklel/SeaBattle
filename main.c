/*
Code::Blocks 16.01
GNU GCC Compiler
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <Windows.h>

enum ConsoleColor {
    Black = 0,
    Green = 2,
    Red = 4,
    Magenta = 5,
    Yellow = 14,
    White = 15
};
void printColor(int x){//вывод цветных символов
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (x == 0){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Black));
        printf("•");
    }
    if (x == 1){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Green));
        printf("o");
    }
    if (x == 2){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Magenta));
        printf("o",x);
    }
    if (x == 3){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Yellow));
        printf("o",x);
    }
    if (x == 4){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Red));
        printf("o",x);
    }
    return 0;
}

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
    Cell hits[10][10];
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

void printMaps(Cell ships[][10], Cell hits[][10]){//выводит карту кораблей и карту выстрелов
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int size = 10;
    printf("  #abcdefghij#   #abcdefghij#\n");
    int i,j;
    for (i = 0; i < size; i++){
        for (j = 0; j < 2 * size ; j++){
            SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | White));
            if (j == 0)
                if (i+1 != 10)
                    printf(" %d|",i+1);
                else printf("%d|",i+1);
            if (j >= 0 && j <= 9)
                printColor(ships[i][j].status);
                SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | White));
            if (j >=10 && j <= 19)
                printColor(hits[i][j-10].status);
                SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | White));
            if (j == 9)
                if (i+1 != 10)
                    printf("|  %d|", i+1);
                else printf("| %d|", i+1);
            if (j == 19)
                    printf("|");
        }
        printf("\n");
    }
    printf("\n\n");
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

void manually(Cell map[][10], Cell hits[][10]){//расстановка кораблей вручную
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int ships[] = {0,4,3,2,1};
    char dir, s[10], a[10];
    int x = 0, y = 0, e = 0, ship = 0;
    while(ships[1] != 0 || ships[2] !=0 || ships[3] != 0 || ships[4] != 0){
        system("cls");
        printMaps(map, hits);
        printf("У вас: однопалубных - %d, двухпалубных - %d, трёхпалубных - %d, четырёхпалубных - %d\n",ships[1],ships[2],ships[3],ships[4]);
        printf("Выберите корабль:\n");
        scanf("%s",&s);
        ship = atoi(s);
        if (strlen(s) > 1 || ship == 0){
            printf("Нужно ввести одну цифру!\n");
            system("pause");
            continue;
        }
        if (ship >= 1 && ship <= 4 && ships[ship] > 0){ // если есть такой корабль и он длиной от 1 до 4, то продолжаем ввод
                if (ship != 1){
                        printf("Выберите ориентацию корабля: h - горизонтальная, v - вертикальная\n");
                        dir = getche(); //направление корабля
                        printf("\n");
                        }
                else dir = 'h';
                if (dir != 'h' && dir != 'v'){
                    printf("Неверно выбранно направление\n");
                    system("pause");
                    continue;
                }
                printf("Введите координаты:\n");
                scanf("%s",&s);
                strncpy(a,s,1);
                int sz = strlen(s);
                memmove(s,s+1,sz-1);
                s[sz-1] = 0;
                y = atoi(s) - 1;
                x = *a - 'a';
                if (inMap(x,y) != 1){
                    printf("Неверные координаты\n");
                    system("pause");
                    continue;
                }
                e = setShip(ship, dir, x, y, map);// 1 - не удалось поставить корабль 2 - корабль установлен
                if (e == 0){
                    ships[ship]--;
                }
                else{
                    printf("Не удалось поставить корабль\n");
                    system("pause");
                }
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

void markKill(Cell ships[][10], Cell hits[][10], int y, int x){//помечаем оставшиеся клетки вокруг корабля и клетки убитого корабля
    hits[y][x].status = 4;
    ships[y][x].status = 4;//помечаем убитый корабль на двух картах
    int i,j;
    for (i = y - 1; i <= y+1; i++)
        for (j = x - 1; j <= x + 1; j++)
            if (inMap(i,j) == 1)
                if(ships[i][j].status == 0){
                        ships[i][j].status = 2;
                        hits[i][j].status = 2;
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
                    markKill(ships, hits, y, m);// помечаем все оставшиеся клетки вокруг корабля
                }
            }
        }
    if (ships[j][i].direction == 'v'){//для вертикального корабля
        for (m = y; m < y + ships[j][i].length; m++){
                if(ships[m][x].status == 3){
                    markKill(ships, hits, m, x);// помечаем все оставшиеся клетки вокруг корабля
                }
            }
        }
    return 1;//убит
}

int shot(Cell ships[][10], Cell hits[][10], int x, int y){
    if (ships[y][x].status == 1){//попали в корабль
        hits[y][x].status = 3;//
        ships[y][x].status = 3;//подбитый корабль
        if(wasKill(ships,hits, x, y) == 1){//проверяем был ли убит корабль
            return 0;//убил
        }
        return 1;//попал
    }else {
        hits[y][x].status = 2;//отмечаем клетку в которой нет корабля
        ships[y][x].status = 2;
        return 2;//промах
    }
}

typedef struct Recommend{// координаты в которые рекомендуется стрелять боту
    Point point;
}Recommend;

void clearRec(Recommend rec[4]){//очистить рекомендуемые координаты
    int i;
    for (i = 0; i < 4; i++){
        rec[i].point.x = -1;
        rec[i].point.y = -1;
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);//для корректного изображения русских букв в консоле
    srand(time(NULL));//чтобы нормально работал рандом

    Player human;//создаем игрока человека
    clearMap(human.hits);//обнуляем значения карты
    clearMap(human.ships);

    while(1){
        system("cls");
        printf("Как расставить корабли?\n 1 - случайно \n 2 - вручную\n\n\nПримеры ввода координат: a1, g10\n\n");
        int n;
        scanf("%d", &n);
        if (n == 1){
            randomShip(human.ships);//расставляем корабли рандомно
            clearStatus(human.ships);//оставляем на карте только корабли
            printMaps(human.ships,human.ships);//выводим карту кораблей и карту попаданий
            break;
        }
        else if (n == 2){
            manually(human.ships, human.hits);//расставляем корабли вручную
            system("cls");
            clearStatus(human.ships);//оставляем на карте только корабли
            printMaps(human.ships, human.hits);
            break;
        }
        else{
           printf("Некорректный ввод\n");
           system("pause");
        }
    }
    Player comp;//создаем бота
    clearMap(comp.hits);//обнуляем значения карты
    clearMap(comp.ships);
    randomShip(comp.ships);// рандомно расставляем корабли
    clearStatus(comp.ships);// оставляем на карте только корабли
    int x,y;//координаты точки которые вводит человек
    int flag;//для выстрела 0-убил 1-попал 2-промах
    int xc,yc;//координаты выстрела компьютера
    int hit = 0;//было ли попадание у компьютера
    int j = 0;//переменная для прохода по рекомендуемым клеткам
    Recommend recommend[4];//создаем рекомендуемые клетки для стрельбы бота
    clearRec(recommend);//очищаем значения
    int turn = 1;//1 - ходит человек, 2 - ходит компьютер
    int mode = 1;//для бота: 1 - рандомный выстрел 2 - добивание
    int win1 = 0,win2 = 0;//считаем количество убитых кораблей
    while(win1 < 10 && win2 < 10){
        if (turn == 1){
            system("cls");//очищаем конслоль
            printf("Вы убили: %d Противник убил: %d\n", win1, win2);//количество потопленных кораблей
            printMaps(human.ships, human.hits);//вывод карты кораблей и карты попаданий
            printf("Введите координаты:\n");
            char s[10], a[10];
            scanf("%s", &s);
            strncpy(a, s, 1);
            int sz = strlen(s);
            memmove(s, s+1, sz-1);
            s[sz-1] = 0;//удаляем первый символ
            y = atoi(s) - 1;
            x = *a - 'a';
            if (inMap(x, y) != 1 || sz > 3){
                printf("Неверно введены координаты\n");
                system("pause");
                continue;
            }
            if (human.hits[y][x].status != 0){
                printf("Невозможно выстрелить\n");
                system("pause");
                continue;
            }
            flag = shot(comp.ships, human.hits, x, y);//производим выстрел
            if (flag == 0){
                system("cls");
                win1++;//увеличиваем количество потопленных кораблей у игрока
                printf("Вы убили: %d Противник убил: %d\n", win1, win2);
                printMaps(human.ships, human.hits);
                printf("Убил!\n");
                system("pause");
            }
            if (flag == 1){
                system("cls");
                printf("Вы убили: %d Противник убил: %d\n", win1, win2);
                printMaps(human.ships, human.hits);
                printf("Попал!\n");
                system("pause");
            }
            if (flag == 2){
                system("cls");
                printf("Вы убили: %d Противник убил: %d\n", win1, win2);
                printMaps(human.ships, human.hits);
                printf("Промах!\n");
                turn = 2;// предоставляем ход компьютеру т.к. мы не попали
                system("pause");
            }
        }
        else {//ход пк
            if (mode == 1){//рандомный выстрел
                xc = rand() % 10;//еще не было попадания поэтому выбираем рандомные координаты
                yc = rand() % 10;
            }
            else {//mode == 2 добивание
                if (hit != 0){
                    if (recommend[j].point.x == -1){//проверяем есть ли значения у рекомендуемой точки
                        j++;   //если нет, то переходим к следующей точке
                        continue;
                    }
                    xc = recommend[j].point.x;
                    yc = recommend[j].point.y;
                    j++;
                    if (inMap(xc,yc) != 1){//если координаты не в пределах карты то переходим к следующей рекомендуемой координате
                        continue;
                    }
                }
            }
            if (comp.hits[yc][xc].status != 0){//если компьютер стреляет по уже помеченной клетке
                continue;//выбираются другие координаты
            }
            flag = shot(human.ships, comp.hits, xc, yc);//производим выстрел
            if (flag == 0){
                win2++;//Компьютер убил корабль
                clearRec(recommend);
                j = 0;  //обнулем переменную для прохода по рекомендуемым точкам
                mode = 1; // переходим в режим рандомной стрельбы
                hit = 0; //обунуляем значения попадний
                system("cls");
                printf("Вы убили: %d Противник убил: %d\n", win1, win2);
                printMaps(human.ships, human.hits);
                printf("Противник убил ваш корабль!: %c%d\n", xc + 'a', yc + 1);
                system("pause");
            }
            if (flag == 1){//компьютер попал
                if (hit == 1){//если до этого уже было одно попадание
                    int k;
                    clearRec(recommend);//очищаем от предыдущих значений
                    /* Пример:
                            01234567
                          0|00000000
                          1|00000000 3 - клетка в которую попали
                          2|0rr33rr0 r - рекомендуемые точки для стрельбы
                          3|00000000
                          4|00000000

                    */
                    if (inMap(xc+1,yc) == 1){
                            if(comp.hits[yc][xc+1].status == 3){//если справа предыдущее поподание
                               recommend[0].point.x = xc - 1;
                               recommend[0].point.y = yc;
                               recommend[1].point.x = xc + 2;
                               recommend[1].point.y = yc;
                               recommend[2].point.x = xc - 2;
                               recommend[2].point.y = yc;
                               recommend[3].point.x = xc + 3;
                               recommend[3].point.y = yc;

                            }

                    }
                    if (inMap(xc-1,yc) == 1){
                        if(comp.hits[yc][xc-1].status == 3){//если слева предыдущее попадание
                               recommend[0].point.x = xc - 2;
                               recommend[0].point.y = yc;
                               recommend[1].point.x = xc + 1;
                               recommend[1].point.y = yc;
                               recommend[2].point.x = xc - 3;
                               recommend[2].point.y = yc;
                               recommend[3].point.x = xc + 2;
                               recommend[3].point.y = yc;
                            }
                    }
                    if (inMap(xc,yc+1) == 1){
                        if(comp.hits[yc+1][xc].status == 3){//если снизу предыдущее попадание
                               recommend[0].point.x = xc;
                               recommend[0].point.y = yc + 2;
                               recommend[1].point.x = xc;
                               recommend[1].point.y = yc - 1;
                               recommend[2].point.x = xc;
                               recommend[2].point.y = yc - 2;
                               recommend[3].point.x = xc;
                               recommend[3].point.y = yc + 3;

                            }
                    }
                    if (inMap(xc,yc-1) == 1){
                        if(comp.hits[yc-1][xc].status == 3){//если сверху предыдущее попадание
                               recommend[0].point.x = xc;
                               recommend[0].point.y = yc - 2;
                               recommend[1].point.x = xc;
                               recommend[1].point.y = yc + 1;
                               recommend[2].point.x = xc;
                               recommend[2].point.y = yc - 3;
                               recommend[3].point.x = xc;
                               recommend[3].point.y = yc + 2;
                            }
                    }
                    j = 0;//обнулем переменную для прохода по рекомендуемым точкам
                    hit = 2;//было уже два или больше попаданий
                }
                if (hit == 0){//если еще не было попаданий
                    int f = 1, r , arr[4], i;
                    arr[0] = rand() % 4;
                    while (f < 4){
                        M1: arr[f] = rand() % 4;
                        for (i = 0; i < f; i++){
                            if(arr[f] == arr[i])
                                goto M1;
                        }
                        f++;
                    }//генерируем массив из 4 различных чисел от 0 до 3 чтобы рекомендуемые клетки выбирались случайно
                     /*   Пример:
                            0123456
                          0|0000000
                          1|00r0000 3 - клетка в которую попали
                          2|0r3r000 r - рекомендуемые клетки для стрельбы
                          3|00r0000
                          4|0000000
                     */
                    if (inMap(xc + 1, yc) == 1){
                        recommend[arr[0]].point.x = xc + 1;
                        recommend[arr[0]].point.y = yc;
                    }
                    if (inMap(xc - 1, yc) == 1){
                        recommend[arr[1]].point.x = xc - 1;
                        recommend[arr[1]].point.y = yc;
                    }
                    if (inMap(xc, yc + 1) == 1){
                        recommend[arr[2]].point.x = xc;
                        recommend[arr[2]].point.y = yc + 1;
                    }
                    if (inMap(xc, yc - 1) == 1){
                        recommend[arr[3]].point.x = xc;
                        recommend[arr[3]].point.y = yc - 1;
                    }
                    hit = 1;//было одно попадание

                }
                mode = 2;
                system("cls");
                printf("Вы убили: %d Противник убил: %d\n", win1, win2);
                printMaps(human.ships, human.hits);
                printf("Противник попал: %c%d\n", xc+'a', yc+1);
                system("pause");
            }
            if (flag == 2){
                    system("cls");
                    printf("Вы убили: %d Противник убил: %d\n", win1, win2);
                    printMaps(human.ships, human.hits);
                    printf("Противник промахнулся\n");
                    printf("Ваш ход\n");
                    turn = 1;
                    system("pause");
                    system("cls");
            }
        }
    }
    system("cls");
    if (win1 == 10){
        printf("Вы победили!\n");
    }
    if (win2 == 10){
        printf("Вы проиграли!\n");
    }
    return 0;
}
