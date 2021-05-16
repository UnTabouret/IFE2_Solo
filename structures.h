//
// Created by jossu on 11/05/2021.
//

#ifndef IFE2_STRUCTURES_H
#define IFE2_STRUCTURES_H


typedef struct Inventory Inventory;

typedef struct Game Game;

struct Inventory {

    int simple, artillerie, bombe, tactique;

};

struct Game {


    int fleet[100];

    char grid[100];

    int mode;

    Inventory missiles;


};

#endif //IFE2_STRUCTURES_H
