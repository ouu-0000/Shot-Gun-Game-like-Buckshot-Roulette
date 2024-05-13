#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_ITEM 5
typedef struct player {
    int index;
    int life;
    int items[MAX_ITEM];
} Player;
typedef struct item {
    int index;
    char name[20];
} Item;
typedef struct gun {
    int blanks;
    int bullets;
} Gun;
void initializePlayer(Player *players) {
    for(int i = 0; i < 2; i++) {
        players[i].index = i;
        players[i].life = 6;
        for(int j = 0; j < MAX_ITEM; j++) {
            players[i].items[j] = 0;
        }
    }
    return;
}
void initializeItem(Item *items) {
    for(int i = 0; i < MAX_ITEM; i++) {
        items[i].index = i;
    }
    sprintf(items[0].name, "煙草");
    sprintf(items[1].name, "発泡酒");
    sprintf(items[2].name, "バタフライナイフ");
    sprintf(items[3].name, "虫眼鏡");
    sprintf(items[4].name, "手錠");
    return;
}
void display(int turn, Player *players, Item *items) {
    printf("\nプレイヤー%dのターン------------------------------\n", turn + 1);
    printf("プレイヤー1のライフ:\n");
    for(int i = 0; i < players[0].life; i++) {
        printf("●");
    }
    printf("\nプレイヤー2のライフ:\n");
    for(int i = 0; i < players[1].life; i++) {
        printf("●");
    }
    printf("\n");
    printf("プレイヤー1のアイテム:\n");
    for(int i = 0; i < MAX_ITEM; i++) {
        printf("[%d]%s:%d\n", items[i].index, items[i].name,
               players[0].items[i]);
    }
    printf("\n");
    printf("プレイヤー2のアイテム:\n");
    for(int i = 0; i < MAX_ITEM; i++) {
        printf("[%d]%s:%d\n", items[i].index, items[i].name,
               players[1].items[i]);
    }
    return;
}
void changeTurn(int *turn) {
    if(*turn == 0) {
        *turn = 1;
    } else {
        *turn = 0;
    }
    return;
}
void reloadGun(Gun *gun) {
    gun->blanks = rand() % 4 + 1;
    gun->bullets = rand() % 4 + 1;
}
int shotGun(Gun *gun) {
    int shot = rand() % (gun->blanks + gun->bullets);
    if(shot < gun->bullets) {
        gun->bullets--;
        return 1;
    } else {
        gun->blanks--;
        return 0;
    }
}
void useItem(int index, int turn, Player *players, Gun *gun, int *damage,
             int *flagShot, int *flagSkip, int *shot) {
    int tmp = 0;
    switch(index) {
    case 0:
        printf("煙草を使った.\nHPを1回復\n");
        players[turn].life++;
        break;
    case 1:
        printf("発泡酒を使った.\n薬莢を確認する\n");
        tmp = shotGun(gun);
        if(tmp == 1) {
            printf("実弾だった.\n");
        } else {
            printf("空弾だった.\n");
        }
        break;
    case 2:
        printf("バタフライナイフを使った.\n");
        *damage = 2;
        break;
    case 3:
        printf("虫眼鏡を使った.次の弾を確認する.\n");
        *shot = shotGun(gun);
        *flagShot = 1;
        if(*shot == 1) {
            printf("実弾だった.\n");
        } else {
            printf("空弾だった.\n");
        }
        break;
    case 4:
        printf("手錠を使った.次の相手のターンをスキップする.\n");
        changeTurn(&turn);
        *flagSkip = turn;
        changeTurn(&turn);
        break;
    default:
        break;
    }
    return;
}
void supplyItem(Player *players) {
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 4; j++) {
            players[i].items[rand() % MAX_ITEM]++;
        }
    }
    return;
}
int main() {
    Player players[2];
    Item items[MAX_ITEM];
    Gun gun = {0, 0};
    srand((unsigned)time(NULL));
    int turn = 0;
    int shot = 0;
    char input, zz = 0;
    int damage = 1;
    int flagShot = -1;
    int flagSkip = -1;

    initializePlayer(players);
    initializeItem(items);
    printf("click enter key to game start\n");
    while(getchar() != '\n')
        ;
    while(players[0].life > 0 && players[1].life > 0) {
        if(gun.blanks == 0 && gun.bullets == 0) {
            reloadGun(&gun);
            printf("リロード\n");
            // printf("実弾%d発, 空弾%d発\n", gun.bullets, gun.blanks);
            supplyItem(players);
        }
        printf("実弾%d発, 空砲%d発\n", gun.bullets, gun.blanks);
        display(turn, players, items);
        damage = 1;
        if(!(flagSkip == turn)) {
            while(1) {
                printf("アイテムを使う.使わないならnを入力\n");
                printf("アイテムの番号を入力\n");
                scanf("%c%c", &input, &zz);
                if(players[turn].items[input - '0'] > 0) {
                    players[turn].items[input - '0'] -= 1;
                    useItem(input - '0', turn, &players[turn], &gun, &damage,
                            &flagShot, &flagSkip, &shot);
                    display(turn, players, items);
                }
                if(input == 'n') {
                    break;
                }
            }
            printf("相手に引き金を引く(y/n)\n");
            scanf("%c%c", &input, &zz);
            if(flagShot == -1) {
                shot = shotGun(&gun);
            } else {
                flagShot = -1;
            }
            if(input == 'y') {
                if(shot == 1) {
                    changeTurn(&turn);
                    printf("プレイヤー%dはダメージを受けた.\n", turn + 1);
                    players[turn].life -= damage;
                    changeTurn(&turn);
                } else {
                    printf("空弾だった.ターン移行.\n",  turn + 1);
                }
            } else {
                if(shot == 1) {
                    printf("プレイヤー%dはダメージを受けた.\n", turn + 1);
                    players[turn].life -= damage;
                } else {
                    printf("空弾だった.プレイヤー%dはターンをスキップした.\n",  turn + 1);
                    changeTurn(&turn);
                }
            }
        } else {
            printf("手錠をかけられており、引き金を引くことができない.\n");
			flagSkip = -1;
        }
        changeTurn(&turn);
    }
    if(players[0].life > 0) {
        printf("プレイヤー1の勝利\n");
    } else {
        printf("プレイヤー2の勝利\n");
    }

    return 0;
}