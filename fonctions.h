//
// Created by jossu on 11/05/2021.
//

#ifndef IFE2_FONCTIONS_H
#define IFE2_FONCTIONS_H


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "structures.h"


int abs(int x) {
    if (x > 0) {
        return x;
    } else {
        return -x;
    }
}

int getint() {
    int n = fgetchar() - '0';
    fflush(stdin);
    return n;
}

void dispGrid(Game *partie) {
    printf("  0 1 2 3 4 5 6 7 8 9\n");
    for (int i = 0; i < 10; ++i) {
        printf("%d ", i);
        for (int j = 0; j < 10; ++j) {
            printf("%c ", *(partie->grid + 10 * i + j));
        }
        printf("\n");
    }
}

int *getBoat(Game *partie, int x, int y) {
    int *bateau = NULL;
    int bx, by, bl, bh;
    for (int i = 0; i < 10; ++i) {
        bateau = partie->fleet + 10 * i;
        bl = *(bateau);
        bh = *(bateau + 1);
        bx = *(bateau + 2);
        by = *(bateau + 3);

        if ((by + (1 - bh) * (bl - 1)) >= y && by <= y && bx <= x && (bx + bh * (bl - 1)) >= x) {
            if (bh) {
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

        bateau = partie->fleet + 10 * i;

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

void dispFleet(Game *partie) {
    int *bateau;
    for (int i = 0; i < 10; ++i) {
        bateau = partie->fleet + 10 * i;
        if (*bateau != 0) {
            printf("Bateau %d :\n", i + 1);
            printf("Taille: %d\n", *bateau);
            printf("Orientiation : ");

            if (*(bateau + 1)) {
                printf("Horizontal\n");
            } else {
                printf("Vertical\n");
            }

            //printf("Coordonnees : (%d,%d)\n",*(bateau+2),*(bateau+3));

            printf("Etat : [ ");

            for (int j = 0; j < *bateau; ++j) {
                if (*(bateau + j + 4)) {
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
    for (int i = 0; i < 100; i++) {
        grille[i] = '_';
    }
}

void initFleet(int *flotte) {
    for (int i = 0; i < 100; i++) {
        flotte[i] = 0;
    }
}

void dispPlayer(Game *partie) {

    dispFleet(partie);
    if (partie->mode == 1) {
        printf("Grille :\n");
        dispGrid(partie);
    }
    printf("\nInventaire :\n");
    printf("Missiles simples : %d\n", partie->missiles.simple);
    printf("Missiles d'artillerie : %d\n", partie->missiles.artillerie);
    printf("Missiles tactiques : %d\n", partie->missiles.tactique);
    printf("Bombes : %d\n", partie->missiles.bombe);
    printf("\n");
    printf("Bateaux restants :\n\n");
}

void initPlayer(Game *partie, int msimple, int mart, int mtact, int mbomb) {
    partie->missiles.simple = msimple;
    partie->missiles.artillerie = mart;
    partie->missiles.tactique = mtact;
    partie->missiles.bombe = mbomb;
    initGrid(partie->grid);
    initFleet(partie->fleet);

}

void shoot_s(Game *partie, int x, int y) {
    int *cible = getBoat(partie, x, y);
    if (cible == NULL) {
        if (partie->grid[10 * y + x] != 'X') {
            partie->grid[10 * y + x] = 'O';
        }
        printf("Echec\n");
    } else {
        *cible = 0;
        partie->grid[10 * y + x] = 'X';
        printf("Touche\n");
    }

}

void shoot_a(Game *partie, int x, int y) {
    int *cible = NULL;

    for (int i = 0; i < 10; ++i) {


        cible = getBoat(partie, x, i);

        if (cible == NULL) {
            if (partie->grid[10 * i + x] != 'X') {
                partie->grid[10 * i + x] = 'O';
            }
        } else {
            *cible = 0;
            partie->grid[10 * i + x] = 'X';
            printf("Touche en %d,%d\n", x, i);
        }

        cible = getBoat(partie, i, y);

        if (cible == NULL) {
            if (partie->grid[10 * y + i] != 'X') {
                partie->grid[10 * y + i] = 'O';
            }
        } else {
            *cible = 0;
            partie->grid[10 * y + i] = 'X';
            printf("Touche en %d,%d\n", i, y);
        }

    }


}

void shoot_t(Game *partie, int x, int y) {
    int *cible = NULL;
    int *bateau = NULL;

    cible = getBoat(partie, x, y);

    if (cible == NULL) {
        if (partie->grid[10 * y + x] != 'X') {
            partie->grid[10 * y + x] = 'O';
            printf("Echec\n");
        }
    } else {
        bateau = partie->fleet + (((cible - partie->fleet) / 10) * 10);
        for (int i = 0; i < *bateau; ++i) {
            partie->grid[10 * (*(bateau + 3) + (1 - *(bateau + 1)) * i) + (*(bateau + 2) + *(bateau + 1) * i)] = 'X';
        }
        *bateau = 0;
        printf("Coule\n");
    }


}

void shoot_b(Game *partie, int x0, int y0) {
    int x, y;
    int *cible = NULL;

    for (int i = -2; i < 3; ++i) {
        for (int j = -(2 - abs(i)); j < 3 - abs(i); ++j) {

            x = x0 + i;
            y = y0 + j;

            if (x >= 0 && x < 10 && y >= 0 && y < 10) {

                cible = getBoat(partie, x, y);

                if (cible == NULL) {
                    if (partie->grid[10 * y + x] != 'X') {
                        partie->grid[10 * y + x] = 'O';
                    }
                } else {
                    *cible = 0;
                    partie->grid[10 * y + x] = 'X';
                    printf("Touche en %d,%d\n", x, y);
                }
            }

        }

    }

}

int fire(Game *partie) {
    int x, y, missile;

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
            if (partie->missiles.simple == 0) {
                printf("Vous ne possedez plus de ce type de missile\n");
                return 0;
            }
            partie->missiles.simple--;
            shoot_s(partie, x, y);
            break;
        case 2:
            if (partie->missiles.artillerie == 0) {
                printf("Vous ne possedez plus de ce type de missile\n");
                return 0;
            }
            partie->missiles.artillerie--;
            shoot_a(partie, x, y);
            break;
        case 3:
            if (partie->missiles.tactique == 0) {
                printf("Vous ne possedez plus de ce type de missile\n");
                return 0;
            }
            partie->missiles.tactique--;
            shoot_t(partie, x, y);
            break;
        case 4:
            if (partie->missiles.bombe == 0) {
                printf("Vous ne possedez plus de ce type de missile\n");
                return 0;
            }
            partie->missiles.bombe--;
            shoot_b(partie, x, y);
            break;
        default:
            exit(0);

    }


    checkSink(partie);

    return 1;


}

void movement(Game *partie) {
    int *bateau;
    int sens, vitesse, x, y, valide;
    for (int i = 0; i < 10; ++i) {
        bateau = partie->fleet + 10 * i;
        valide = 1;
        if (*bateau) {
            sens = rand() % 2; //0 pour décroissant, 1 pour croissant
            vitesse = rand() % 3 + 1;

            for (int j = 1; j < vitesse + 1; ++j) {
                x = (bateau[2] + bateau[1] * (sens * (bateau[0] - 1) + (2 * sens - 1) * j));
                y = (bateau[3] + (1 - bateau[1]) * (sens * (bateau[0] - 1) + (2 * sens - 1) * j));
                if (!(x >= 0 && y >= 0 && x < 10 && y < 10 && getBoat(partie, x, y) == NULL)) {
                    valide = 0;
                }
            }

            if (valide && rand() % 10 > 7) {
                bateau[2] += vitesse * bateau[1] * (2 * sens - 1);
                bateau[3] += vitesse * (1 - bateau[1]) * (2 * sens - 1);
                printf("Le bateau %d s'est deplace de %d cases vers ", i + 1, vitesse);
                if (bateau[1] == 0 && sens == 0) {
                    printf("le haut\n");
                } else if (bateau[1] == 1 && sens == 0) {
                    printf("la gauche\n");
                } else if (bateau[1] == 0 && sens == 1) {
                    printf("le bas\n");
                } else if (bateau[1] == 1 && sens == 1) {
                    printf("la droite\n");
                }
            }


        }
    }
}

void endTurn(Game *partie) {

    int mort = 1;
    int *bateau;

    for (int i = 0; i < 10; ++i) {
        bateau = partie->fleet + 10 * i;

        if (*bateau) {
            mort = 0;
        }

    }

    if (mort) {
        printf("\n\n**** Victoire du joueur ! ****\n");
        exit(0);
    } else if (partie->missiles.simple + partie->missiles.artillerie + partie->missiles.tactique +
               partie->missiles.bombe == 0) {
        printf("\n\n**** Defaite du joueur ! ****\n");
        exit(0);
    }

    if (partie->mode == 3) {
        movement(partie);
    }
}

int addBoat(Game *partie, int *bateau, int l, int h, int x, int y) {
    for (int i = 0; i < l; ++i) {
        *(bateau + 4 + i) = 1;

        if (getBoat(partie, x + i * h, y + i * (1 - h)) != NULL || x + l * h > 10 || y + l * (1 - h) > 10) {

            return 0;
        }
    }

    *bateau = l;
    *(bateau + 1) = h;
    *(bateau + 2) = x;
    *(bateau + 3) = y;

    return 1;

}

void randFleet(Game *partie) {
    int effectif, taille, horizontal, x, y, n;
    int *bateau;
    effectif = 0;

    printf("Nombre de bateaux : ");
    n = getint();

    while (effectif < n) {

        bateau = partie->fleet + 10 * effectif;
        taille = rand() % 4 + 2;
        horizontal = rand() % 2;
        x = rand() % 10;
        y = rand() % 10;

        if (addBoat(partie, bateau, taille, horizontal, x, y)) {
            effectif++;
        }

    }
}

void createFleet(Game *partie) {
    int choix, effectif, taille, horizontal, x, y;
    int *bateau;
    effectif = 0;

    while (1) {
        printf("Choisissez une action :\n");
        printf("1: Ajouter bateau\n");
        printf("2: Terminer creation\n");
        choix = getint();
        if (choix == 2) {
            dispFleet(partie);
            return;
        }

        bateau = partie->fleet + 10 * effectif;

        printf("Entrez la taille du bateau : ");
        taille = getint();
        printf("Bateau horizontal : ");
        horizontal = getint();
        printf("Abcisse du bateau : ");
        x = getint();
        printf("Ordonnee du bateau : ");
        y = getint();

        if (addBoat(partie, bateau, taille, horizontal, x, y)) {
            effectif++;
        } else {
            printf("Echec de l'ajout\n");
        }

    }

}

void createGame(Game *partie) {

    int choix, difficulte;

    printf("Choisissez un mode de jeu\n");
    printf("1: Classic\n");
    printf("2: Blind\n");
    printf("3: Active\n");
    partie->mode = getint();

    printf("Choisissez une difficulte :\n");
    printf("1: Facile (10 missiles de chaque type)\n");
    printf("2: Moyen (3 missiles d'artillerie, 5 bombes, 5 missiles tactiques, 10 missiles simples)\n");
    printf("3: Difficile (1 missiles d'artillerie, 2 bombes, 4 missiles tactiques, 15 missiles simples)\n");
    difficulte = getint();

    switch (difficulte) {
        case 1:
            initPlayer(partie, 10, 10, 10, 10);
            break;
        case 2:
            initPlayer(partie, 10, 3, 5, 5);
            break;
        case 3:
            initPlayer(partie, 15, 1, 4, 2);
            break;
        default:
            printf("Choix incorrect");
            exit(0);

    }

    printf("Mode de creation de la fleet :\n");
    printf("1: Automatique\n");
    printf("2: Manuelle\n");
    choix = getint();

    if (choix == 2) {
        createFleet(partie);
    } else {
        randFleet(partie);
    }

}

void saveGame(Game *partie) {
    FILE *fichier = NULL;

    fichier = fopen("sauvegarde.txt", "w+");

    if (fichier == NULL) {
        printf("ERROR");
        exit(0);
    }

    for (int i = 0; i < 100; ++i) {
        fputc(partie->fleet[i] + '0', fichier);
    }

    for (int i = 0; i < 100; ++i) {
        fputc(partie->grid[i], fichier);
    }

    fputc(partie->mode + '0', fichier);
    fprintf(fichier, " %d", partie->missiles.simple);
    fprintf(fichier, " %d", partie->missiles.artillerie);
    fprintf(fichier, " %d", partie->missiles.tactique);
    fprintf(fichier, " %d", partie->missiles.bombe);


    fclose(fichier);

}

void loadGame(Game *partie) {
    FILE *fichier = NULL;

    fichier = fopen("sauvegarde.txt", "r");

    for (int i = 0; i < 100; ++i) {
        partie->fleet[i] = fgetc(fichier) - '0';
    }

    for (int i = 0; i < 100; ++i) {
        partie->grid[i] = fgetc(fichier);
    }

    fscanf(fichier, "%d", &partie->mode);
    fscanf(fichier, " %d %d %d %d", &partie->missiles.simple, &partie->missiles.artillerie,
           &partie->missiles.tactique, &partie->missiles.bombe);

    fclose(fichier);
}

void gameMenu(Game *partie) {
    int choix, fin;
    fin = 0;
    dispPlayer(partie);
    printf("Choisissez une action :\n");
    printf("1: Tirer\n");
    printf("2: Sauvegarder et quitter\n");
    printf("3: Quitter\n");
    choix = getint();

    while (!fin) {
        if (choix == 1) {
            fin = fire(partie);
        } else if (choix == 2) {
            saveGame(partie);
            exit(0);
        } else {
            exit(0);
        }
    }


    endTurn(partie);

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
            loadGame(partie);
            break;
        case 3:
            exit(0);
        default:
            printf("ERROR CHOICE\n");
            exit(0);
    }

}


#endif //IFE2_FONCTIONS_H
