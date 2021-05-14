//
// Created by jossu on 11/05/2021.
//

#ifndef IFE2_FONCTIONS_H
#define IFE2_FONCTIONS_H


#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

int abs(int x)
{
    if(x>0)
    {
        return x;
    } else
    {
        return -x;
    }
}

int getint() {
    int n = fgetchar()-'0';
    fflush(stdin);
    return n;
}

void affGrille(Game *partie) {
    printf("  A B C D E F G H I J\n");
    for (int i = 0; i < 10; ++i) {
        printf("%d ",i);
        for (int j = 0; j < 10; ++j) {
            printf("%c ",*(partie->grille + 10 * i + j));
        }
        printf("\n");
    }
}

int* getBoat(Game *partie, int x, int y){
    int *bateau = NULL;
    int bx, by, bl, bh;
    for (int i = 0; i < 10; ++i) {
        bateau = partie->flotte + 10 * i;
        bl = *(bateau);
        bh = *(bateau+1);
        bx = *(bateau+2);
        by = *(bateau+3);

        if((by + (1-bh)*(bl-1)) >= y && by <= y && bx <= x && (bx + bh*(bl-1)) >= x)
        {
            if (bh)
            {
                return bateau + x - bx + 4;
            } else {
                return bateau + y - by + 4;
            }

        }

    }

    return NULL;
}

void checkSink(Game *partie) {

    int *bateau = NULL;
    int coule;

    for (int i = 0; i < 10; ++i) {

        bateau = partie->flotte + 10 * i;

        if (*bateau) {

            coule = 1;

            for (int j = 0; j < 6; ++j) {
                if (bateau[j + 4]) {
                    coule = 0;
                }
            }

            if (coule) {
                *bateau = 0;
                printf("Coule");
            }

        }



    }

}

void affFlotte(Game *partie) {
    int *bateau;
    for (int i = 0; i < 10; ++i) {
        bateau = partie->flotte + 10 * i;
        if (*bateau != 0)
        {
            printf("Bateau %d :\n",i+1);
            printf("Taille: %d\n",*bateau);
            printf("Orientiation : ");

            if (*(bateau+1)) {
                printf("Horizontal\n");
            } else {
                printf("Vertical\n");
            }

            printf("Coordonnees : (%d,%d)\n",*(bateau+2),*(bateau+3));

            printf("Etat : [ ");

            for (int j = 0; j < *bateau; ++j) {
                if (*(bateau+j+4))
                {
                    printf("_ ");
                } else {
                    printf("X ");
                }
            }

            printf("]\n\n");
        }
    }
}

void initGrid(char grille[100]) {
    for(int i = 0; i < 100; i++)
    {
        grille[i] = '_';
    }
}

void initFlotte(int flotte[100]) {
    for(int i = 0; i < 100; i++)
    {
        flotte[i] = 0;
    }
}

void affPlayer(Game *partie) {

    if(partie->mode == 1)
    {
        printf("Grille :\n");
        affGrille(partie);
    }
    printf("\nInventaire :\n");
    printf("Missiles simples : %d\n",partie->inventaire.missile_s);
    printf("Missiles d'artillerie : %d\n",partie->inventaire.missile_a);
    printf("Missiles tactiques : %d\n",partie->inventaire.missile_t);
    printf("Bombes : %d\n",partie->inventaire.missile_b);
    printf("\n");
    printf("Bateaux restants :\n\n");
    affFlotte(partie);
}

void initPlayer(Game *partie, int msimple, int mart, int mtact, int mbomb) {
    partie->inventaire.missile_s = msimple;
    partie->inventaire.missile_a = mart;
    partie->inventaire.missile_t = mtact;
    partie->inventaire.missile_b = mbomb;
    initGrid(partie->grille);
    initFlotte(partie->flotte);

}

void shoot_s(Game *partie, int x, int y)
{
    int *cible = getBoat(partie,x,y);
    if (cible == NULL) {
        if (partie->grille[10*y+x] != 'X')
        {
            partie->grille[10*y+x] = 'O';
        }
        printf("Echec\n");
    } else {
        *cible = 0;
        partie->grille[10*y+x] = 'X';
        printf("Touche\n");
    }

}

void shoot_a(Game *partie, int x, int y, int horizontal)
{
    int *cible = NULL;

    for (int i = 0; i < 10; ++i) {

        if (horizontal)
        {
            x = i;
        } else {
            y = i;
        }

        cible = getBoat(partie,x,y);

        if (cible == NULL) {
            if (partie->grille[10*y+x] != 'X')
            {
                partie->grille[10*y+x] = 'O';
            }
        } else {
            *cible = 0;
            partie->grille[10*y+x] = 'X';
            printf("Touche en %d,%d\n",x,y);
        }

    }


}

void shoot_t(Game *partie, int x, int y)
{
    int *cible = NULL;
    int *bateau = NULL;

    cible = getBoat(partie,x,y);

    if (cible == NULL) {
        if (partie->grille[10*y+x] != 'X')
        {
            partie->grille[10*y+x] = 'O';
            printf("Echec\n");
        }
    } else {
        bateau = partie->flotte + (((cible - partie->flotte)/10) * 10);
        for (int i = 0; i < *bateau; ++i) {
            partie->grille[10 * (*(bateau+3) + (1 - *(bateau+1)) * i ) + (*(bateau+2) + *(bateau+1) * i )] = 'X';
        }
        *bateau = 0;
        printf("Coule\n");
    }




}

void shoot_b(Game *partie, int x0, int y0)
{
    int x,y;
    int *cible = NULL;

    for (int i = -2; i < 3; ++i) {
        for (int j = -(2 - abs(i)); j < 3- abs(i); ++j) {

            x = x0 +i;
            y = y0 + j;

            if (x >= 0 && x < 10 && y >= 0 && y < 10) {

                cible = getBoat(partie, x, y);

                if (cible == NULL) {
                    if (partie->grille[10 * y + x] != 'X') {
                        partie->grille[10 * y + x] = 'O';
                    }
                } else {
                    *cible = 0;
                    partie->grille[10 * y + x] = 'X';
                    printf("Touche en %d,%d\n", x, y);
                }
            }

        }

    }

}

int tirer(Game *partie) {
    int x,y,missile,direction;

    printf("Choisissez un missile :\n");
    printf("1: Simple\n");
    printf("2: Artillerie\n");
    printf("3: Tactique\n");
    printf("4: Bombe\n");
    missile = getint();

    printf("\nEntrez x : ");
    x = getint();
    printf("Entrez y : ");
    y = getint();

    switch (missile) {
        case 1:
            shoot_s(partie,x,y);
            break;
        case 2:
            printf("Horizontal ? ");
            direction = getint();
            shoot_a(partie,x,y,direction);
            break;
        case 3:
            shoot_t(partie,x,y);
            break;
        case 4:
            shoot_b(partie,x,y);
            break;
        default:
            exit(0);
            break;

    }


    checkSink(partie);

    return 1;



}

void finTour(Game *partie){

    int mort = 1;
    int *bateau;

    for (int i = 0; i < 10; ++i) {
        bateau = partie->flotte + 10 * i;

        if (*bateau) {
            mort = 0;
        }

    }

    if (mort)
    {
        printf("\n\n**** Victoire du joueur ! ****\n");
        exit(0);
    } else {
        return;
    }
}

void createFleet(Game *partie)
{
    int choix, effectif, taille, horizontal, x, y;
    int *bateau;
    effectif = 0;
    initPlayer(partie,10,0,0,0);

    while (1)
    {
        printf("Choisissez une action :\n");
        printf("1: Ajouter bateau\n");
        printf("2: Terminer creation\n");
        choix = getint();
        if (choix == 2)
        {
            affFlotte(partie);
            return;
        }

        bateau = partie->flotte + 10 * effectif;

        printf("Entrez la taille du bateau : ");
        taille = getint();
        printf("Bateau horizontal : ");
        horizontal = getint();
        printf("Abcisse du bateau : ");
        x = getint();
        printf("Ordonnee du bateau : ");
        y = getint();


        for (int i = 0; i < taille; ++i) {
            *(bateau+4+i) = 1;

            if (getBoat(partie,x + i*horizontal ,y+ i * (1-horizontal)) != NULL || x + taille*horizontal > 10 || y + taille *(1-horizontal) > 10) {

                exit(0);
            }
        }

        *bateau = taille;
        *(bateau+1) = horizontal;
        *(bateau+2) = x;
        *(bateau+3) = y;

        effectif++;


    }

}

void createGame(Game *partie) {

    printf("Choisissez un mode de jeu : ");
    partie->mode = getint();

    printf("Creation de la flotte :\n");
    createFleet(partie);

}

void gameMenu(Game *partie){
    int choix,fin;
    fin = 0;
    affPlayer(partie);
    printf("Choisissez une action :\n");
    printf("1: Tirer\n");
    printf("2: Sauvegarder\n");
    choix = getint();

    while (!fin) {
        if (choix == 1) {
            fin = tirer(partie);
        } else if (choix == 2) {
            exit(0);
            // sauvegarder(partie);
        } else {
            printf("\nERROR CHOICE\n");
            exit(0);
        }
    }


    finTour(partie);

}

void mainMenu(Game *partie) {
    int choix;
    printf("Choisissez une action :\n");
    printf("1: Nouvelle partie\n");
    printf("2: Charger une partie\n");
    printf("3: Quitter\n");
    choix = getint();

    switch (choix) {
        case 1:
            createGame(partie);
            break;
        case 2:
            //loadGame();
        case 3:
            exit(0);
            break;
        default:
            printf("ERROR CHOICE\n");
            exit(0);
            break;
    }

}


#endif //IFE2_FONCTIONS_H
