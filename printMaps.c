#include "printMaps.h"
#include "windows.h"



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
    if (x == EMPTY){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Black));
        printf("•");
    }
    if (x == SHIP){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Green));
        printf("o");
    }
    if (x == MISS){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Magenta));
        printf("o",x);
    }
    if (x == INJURED){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Yellow));
        printf("o",x);
    }
    if (x == KILLED){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Red));
        printf("o",x);
    }
}



void clearMap(Cell map[][MAX_SIZE]){//очищаю значения после создания игрока
    int i,j;
    for (i = 0; i < MAX_SIZE; i++){
        for (j = 0; j < MAX_SIZE; j++){
            map[i][j].status = EMPTY;
            map[i][j].direction = 0;
            map[i][j].length = 0;
            map[i][j].point.x = -1;
            map[i][j].point.y = -1;
        }
    }
}

void clearStatus(Cell map[][MAX_SIZE]){//после расстановки очищаю карту от лишних значений
    int i, j;
    for (i = 0; i < MAX_SIZE; i++){
        for(j = 0; j < MAX_SIZE; j++){
            if (map[i][j].status == MISS){
                map[i][j].status = EMPTY;
                map[i][j].direction = -1;
                map[i][j].length = 0;
                map[i][j].point.x = -1;
                map[i][j].point.y = -1;
            }
        }
    }
}
int inMap(int x, int y){//проверяем находится ли точка в пределах карты
    if (x >= MIN_SIZE && x < MAX_SIZE && y >= MIN_SIZE && y < MAX_SIZE)
        return TRUE;//Точка в пределах карты
    else return FALSE;
}



void printMaps(Cell ships[][MAX_SIZE], Cell hits[][MAX_SIZE]){//выводит карту кораблей и карту выстрелов
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("  #abcdefghij#   #abcdefghij#\n");
    int i,j;
    for (i = 0; i < MAX_SIZE; i++){
        for (j = 0; j < 2 * MAX_SIZE; j++){
            SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | White));
            if (j == 0)
                if (i+1 != MAX_SIZE)
                    printf(" %d|", i+1);
                else printf("%d|", i+1);
            if (j >= MIN_SIZE && j < MAX_SIZE)
                printColor(ships[i][j].status);
                SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | White));
            if (j >= MAX_SIZE && j < 2 * MAX_SIZE)
                printColor(hits[i][j-MAX_SIZE].status);
                SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | White));
            if (j == MAX_SIZE-1)
                if (i+1 != MAX_SIZE)
                    printf("|  %d|", i+1);
                else printf("| %d|", i+1);
            if (j == 2 * MAX_SIZE - 1)
                    printf("|");
        }
        printf("\n");
    }
    printf("\n\n");
}

void refreshMaps(Cell ships[MAX_SIZE][MAX_SIZE], Cell hits[MAX_SIZE][MAX_SIZE], int score_human, int score_comp){
    system("cls");
    printf("Вы убили: %d Противник убил: %d\n", score_human, score_comp);//количество потопленных кораблей
    printMaps(ships, hits);//вывод карты кораблей и карты попаданий
}
