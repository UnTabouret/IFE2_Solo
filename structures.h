//
// Created by jossu on 11/05/2021.
//

#ifndef IFE2_STRUCTURES_H
#define IFE2_STRUCTURES_H


typedef struct Inventory Inventory;

typedef struct Game Game;

struct Inventory {

    int missile_s, missile_a, missile_b, missile_t;

};

struct Game {


    int flotte[100];

    char grille[100];

    int mode;

    Inventory inventaire;


};

#endif //IFE2_STRUCTURES_H
