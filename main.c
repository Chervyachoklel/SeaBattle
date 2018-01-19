#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <windows.h>


typedef struct Point{
    int x,y; //���������� ������ ������� ����� ��� ����������� �������� �������
}Point;

typedef struct Cell{
    Point point; //��������� ���������� �������
    int status; //�������� ������
    int length; //����� �������
    char direction; //����������� �������
}Cell;

typedef struct Player{
    Cell ships[10][10];
    Cell hits[10][10]; //�������� ����� int
}Player;

void clearMap(Cell map[][10]){//������ �������� ����� �������� ������
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

void clearStatus(Cell map[][10]){//����� ����������� ������ ����� �� ������ ��������
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

void printMap(Cell map[][10]){//������� ����� � �������
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

void printMaps(Cell ships[][10], Cell hits[][10]){//������� ��� ����� � ������� ships � hits
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
                printf("%d", hits[i][j-10].status); //�������
            if (j == 9)
                printf("| %d|", i);
            if (j == 19)
                    printf("|");
        }
        printf("\n");
    }
    printf("\n");
}

int setShip(int ship, char dir, int x, int y, Cell map[][10]){//������� ��� ����������� �������
    int i,j;
    int shipDirH = 0, shipDirV = 0;
    if (dir == 'h'){
        shipDirH = ship - 1;
    }
    else shipDirV = ship - 1;

    if (x + shipDirH > 9 || y + shipDirV > 9)
        return 1;//�� ������� ��������� ������� �� ������� ����� �� ������� �����

    for (i = y; i <= y + shipDirV; i++){
        for (j = x; j <= x + shipDirH; j++){
            if (i >=0 && i < 10 && j >=0 && j<10){
                if (map[i][j].status != 0)
                    return 1;//�� ������� ��������� ������� �� ����� ���� ������ �������
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
    return 0;// ������� ������ ����������
    /*
    � ����� �������� ���������:
�����:   012345..
       0|022220
       1|021120      � ������ � ���������� ������������ x = 2 y = 1 ���������� ������������ �������
       2|022220      � ������� �� ��������� 1 ���� ���������� ����� ������� � ��� �����������
       3|000000
       .
       .
     */
}

void randomShip(Cell map[][10]){//������� ��������� ����������� ��������
    int ships[] = {0,4,3,2,1};//������ 4 - ������������ 3 - ������������ 2 - ������������ � 1 - ���������������
                            //����� �������� ���������� ��������
    while(ships[1] != 0 || ships[2] !=0 || ships[3] != 0 || ships[4] != 0){// ���� ��������� ����� ��� ������� ����������
        int ship = 1 + rand() % 4;// �������� �������� ������ �� ��� �����
        if (ship >= 1 && ship <= 4 && ships[ship] > 0){
                char dir;
                int z = rand() % 2;
                if (z == 0)
                    dir = 'h';
                else dir = 'v';//�������� �������� ����������� �������
                int x,y;
                x = rand()%10;
                y = rand()%10;//�������� �������� ��������� ���������� �������
                int e = setShip(ship, dir, x, y, map);//������������� �������
                if (e == 0)
                    ships[ship]--;//���� ������� ������� ����������, �� -1 ������� ������ �����

            }

        }
}

void manually(Cell map[][10]){//����������� �������� �������
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int ships[] = {0,4,3,2,1};
    char dir;
    int x = 0, y = 0, e = 0, ship = 0;

    while(ships[1] != 0 || ships[2] !=0 || ships[3] != 0 || ships[4] != 0){
        //printf("� ���: %d - ������������ �������� %d - ������������ �������� %d - ������������ �������� %d - ��������������� ��������\n",ships[1],ships[2],ships[3],ships[4]);
        printf("�������� �������:\n");
        scanf("%d", &ship);
        if (ship >= 1 && ship <= 4 && ships[ship] > 0){ // ���� ���� ����� ������� � �� ������ �� 1 �� 4, �� ���������� ����
                printf("�������� ���������� �������: h - ��������������, v - ������������\n");
                dir = getche(); //����������� �������
                printf("\n");
                printf("�������� ������: x y\n");
                scanf("%d",&x); //��������
                scanf("%d",&y);
                /*char z;
                z = getche();
                x = z - '0';
                printf(" ");
                z = getche();
                y = z - '0';*/
                e = setShip(ship, dir, x, y, map);// 1 - �� ������� ��������� ������� 2 - ������� ����������
                if (e == 0){
                    ships[ship]--;
                }
                else{
                    printf("�� ������� ��������� �������\n");
                    system("pause");
                }
                printMap(map);
            }
        else {
            printf("������ ������� ���\n");
            system("pause");
            continue;
        }
    }
}

int inMap(int x, int y){//��������� ��������� �� ����� � �������� �����
    if (x >= 0 && x <=9 && y>=0 && y <= 9)
        return 1;//����� � �������� �����
    else return 0;
}

void hitting(Cell hits[][10],int x, int y){//�������� ������ � ������� ����� ��� �������
    if (inMap(y-1,x-1)==1){                // 202
        hits[y-1][x-1].status = 2;         // 030  2 - ��� ������ � ������� ��� �������
    }                                      // 202  3 - ��� ������ � ������� �� ������ ������
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

void markKill(Cell ships[][10], Cell hits[][10], int i, int j){//�������� ���������� ������ ������ ������� � ������ ������� �������
    hits[i][j].status = 4;
    ships[i][j].status = 4;//�������� ������ ������� �� ���� ������
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
    if (ships[j][i].direction=='h'){//��� ��������������� �������
        for (m = x; m < x + ships[j][i].length; m++){
                if(ships[y][m].status == 1)
                    return 0;//�� ���� �.�. � ��������������� ������� ��� ���� ����� ������
                }
        }
    if (ships[j][i].direction=='v'){//��� ������������� �������
        for (m = y; m < y + ships[j][i].length; m++){
                if(ships[m][x].status == 1)
                    return 0;//�� ���� �.�. � ������������� ������� ��� ���� ����� ������
                }
        }
    //���� ��������� ������ ���������� �������, ������ � ������� �� �������� ����� ������ �.�. ������� ����
    if (ships[j][i].direction == 'h'){//��� ��������������� �������
        for (m = x; m < x + ships[j][i].length; m++){
                if(ships[y][m].status == 3){
                    markKill(ships,hits,y,m);// �������� ��� ���������� ������ ������ �������
                }
            }
        }
    if (ships[j][i].direction == 'v'){//��� ������������� �������
        for (m = y; m < y + ships[j][i].length; m++){
                if(ships[m][x].status == 3){
                    markKill(ships,hits,m,x);// �������� ��� ������ ������ ������������ ������� 2
                }
            }
        }
    return 1;//����
}

int shot(Cell ships[][10], Cell hits[][10], int x, int y){
    if (ships[y][x].status == 1){//������ � �������
        hitting(hits, x, y);//�������� ������ � ������� ����� ��� �������
        hits[y][x].status = 3;//
        ships[y][x].status = 3;//�������� �������
        if(wasKill(ships,hits, x, y) == 1){//��������� ��� �� ���� �������
            return 0;//����
        }
        return 1;//�����
    }else {
        hits[y][x].status = 2;//�������� ������ � ������� ��� �������
        return 2;//������
    }
}

typedef struct Recommend{// ���������� � ������� ������������� �������� ����
    Point point[4];
}Recommend;


void clearRec(Recommend rec){//�������� ������������� ����������
    int i;
    for (i = 0; i < 4; i++){
        rec.point[i].x = -1;
        rec.point[i].y = -1;
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);//��� ����������� ����������� ������� ���� � �������
    srand(time(NULL));//����� ������ ������� �����

    Player human;//������� ������ ��������
    clearMap(human.hits);//�������� �������� �����
    clearMap(human.ships);
    printf("��� ���������� �������?\n 1 - �������� \n 2 - �������\n");

    while(1){
        int n;
        scanf("%d", &n);
        if (n == 1){
            randomShip(human.ships);//����������� ������� ��������
            clearStatus(human.ships);//��������� �� ����� ������ �������
            printMaps(human.ships,human.ships);//������� ����� �������� � ����� ���������
            break;
        }
        else if (n == 2){
            manually(human.ships);//����������� ������� �������
            system("cls");
            clearStatus(human.ships);//��������� �� ����� ������ �������
            printMaps(human.ships, human.hits);
            break;
        }
        else printf("������������ ����\n");
    }
    Player comp;//������� ����
    clearMap(comp.hits);//�������� �������� �����
    clearMap(comp.ships);
    randomShip(comp.ships);// �������� ����������� �������
    clearStatus(comp.ships);// ��������� �� ����� ������ �������
    //printMaps(comp.ships,comp.hits);
    int x,y;//���������� ����� ������� ������ �������
    int flag;//
    int xc,yc,hit = 0,j = 0;
    Recommend recommend;//������� ������������� ������ ��� �������� ����
    clearRec(recommend);//������� ��������
    printf("rec: x: %d",recommend.point[0].x);
    system("pause");
    int turn = 1;//1 - ����� �������, 2 - ����� ���������
    int mode = 1;//��� ����: 1 - ��������� ������� 2 - ���������
    int win1 = 0,win2 = 0;
    while(win1 < 10 && win2 < 10){
        if (turn == 1){
            printf("Human:\n");
            printMaps(human.ships,human.hits);
            printf("Comp:\n");
            printMaps(comp.ships,comp.hits);
            printf("win1: %d win2: %d\n",win1,win2);
            printf("������� ����������\n");
            scanf("%d%d",&x,&y);
            if (human.hits[y][x].status != 0){
                printf("���������� ���������\n");
                continue;
            }
            flag = shot(comp.ships,human.hits,x,y);
            if (flag == 0){
                win1++;
                printf("����\n");
            }
            if (flag == 1){
                printf("�����\n");
            }
            if (flag == 2){
                printf("������\n");
                turn = 2;
            }
        }
        else {//��� ��
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
                            if(comp.hits[yc][xc+1].status == 3){//���� ������ ���������� ���������
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
                        if(comp.hits[yc][xc-1].status == 3){//���� ����� ���������� ���������
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
                        if(comp.hits[yc+1][xc].status == 3){//���� ����� ���������� ���������
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
                        if(comp.hits[yc-1][xc].status == 3){//���� ������ ���������� ���������
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
        printf("�� ��������\n");
    }
    if (win2 == 10){
        printf("�� ���������\n");
    }
    return 0;
}
