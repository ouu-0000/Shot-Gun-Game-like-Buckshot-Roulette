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
    sprintf(items[0].name, "����");
    sprintf(items[1].name, "���A��");
    sprintf(items[2].name, "�o�^�t���C�i�C�t");
    sprintf(items[3].name, "���ዾ");
    sprintf(items[4].name, "���");
    return;
}
void display(int turn, Player *players, Item *items) {
    printf("\n�v���C���[%d�̃^�[��------------------------------\n", turn + 1);
    printf("�v���C���[1�̃��C�t:\n");
    for(int i = 0; i < players[0].life; i++) {
        printf("��");
    }
    printf("\n�v���C���[2�̃��C�t:\n");
    for(int i = 0; i < players[1].life; i++) {
        printf("��");
    }
    printf("\n");
    printf("�v���C���[1�̃A�C�e��:\n");
    for(int i = 0; i < MAX_ITEM; i++) {
        printf("[%d]%s:%d\n", items[i].index, items[i].name,
               players[0].items[i]);
    }
    printf("\n");
    printf("�v���C���[2�̃A�C�e��:\n");
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
        printf("�������g����.\nHP��1��\n");
        players[turn].life++;
        break;
    case 1:
        printf("���A�����g����.\n��䰂��m�F����\n");
        tmp = shotGun(gun);
        if(tmp == 1) {
            printf("���e������.\n");
        } else {
            printf("��e������.\n");
        }
        break;
    case 2:
        printf("�o�^�t���C�i�C�t���g����.\n");
        *damage = 2;
        break;
    case 3:
        printf("���ዾ���g����.���̒e���m�F����.\n");
        *shot = shotGun(gun);
        *flagShot = 1;
        if(*shot == 1) {
            printf("���e������.\n");
        } else {
            printf("��e������.\n");
        }
        break;
    case 4:
        printf("������g����.���̑���̃^�[�����X�L�b�v����.\n");
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
            printf("�����[�h\n");
            // printf("���e%d��, ��e%d��\n", gun.bullets, gun.blanks);
            supplyItem(players);
        }
        printf("���e%d��, ��C%d��\n", gun.bullets, gun.blanks);
        display(turn, players, items);
        damage = 1;
        if(!(flagSkip == turn)) {
            while(1) {
                printf("�A�C�e�����g��.�g��Ȃ��Ȃ�n�����\n");
                printf("�A�C�e���̔ԍ������\n");
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
            printf("����Ɉ�����������(y/n)\n");
            scanf("%c%c", &input, &zz);
            if(flagShot == -1) {
                shot = shotGun(&gun);
            } else {
                flagShot = -1;
            }
            if(input == 'y') {
                if(shot == 1) {
                    changeTurn(&turn);
                    printf("�v���C���[%d�̓_���[�W���󂯂�.\n", turn + 1);
                    players[turn].life -= damage;
                    changeTurn(&turn);
                } else {
                    printf("��e������.�^�[���ڍs.\n",  turn + 1);
                }
            } else {
                if(shot == 1) {
                    printf("�v���C���[%d�̓_���[�W���󂯂�.\n", turn + 1);
                    players[turn].life -= damage;
                } else {
                    printf("��e������.�v���C���[%d�̓^�[�����X�L�b�v����.\n",  turn + 1);
                    changeTurn(&turn);
                }
            }
        } else {
            printf("������������Ă���A���������������Ƃ��ł��Ȃ�.\n");
			flagSkip = -1;
        }
        changeTurn(&turn);
    }
    if(players[0].life > 0) {
        printf("�v���C���[1�̏���\n");
    } else {
        printf("�v���C���[2�̏���\n");
    }

    return 0;
}