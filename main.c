
#include "fonctions.h"
int main() {

    Game partie;
    srand(time(NULL));

    mainMenu(&partie);

    while (1) {
        gameMenu(&partie);
        printf("\n\nTour suivant\n\n");
    }
}
