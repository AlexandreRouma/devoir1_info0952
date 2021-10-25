#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "schelling.h"
#include "schelling_gif.h"


int main(int argc, char const *argv[]) {
    int height = 50;
    int width = 50;
    int pixelSize = 10;
    double probRed = 0.4;
    double probBlue = 0.4;
    double satisRatio = 0.7;
    const char filename_tmp[] = "simu.gif";
    const char *filename = filename_tmp;

    // Analyse des arguments
    for (int i = 1; i < argc; i+=2) {
        if (argc - i == 1)
            break;

        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--height")) {
            height = atoi(argv[i+1]);
            if (height <= 0) {
                fprintf(stderr, "Error: 'height' argument is not valid.\n");
                exit(1);
            }
            continue;
        }

        if (!strcmp(argv[i], "-w") || !strcmp(argv[i], "--width")) {
            width = atoi(argv[i+1]);
            if (width <= 0) {
                fprintf(stderr, "Error: 'width' argument is not valid.\n");
                exit(1);
            }
            continue;
        }

        if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--probred")) {
            probRed = atof(argv[i+1]);
            if (probRed < 0. || (probRed == 0. && !(!strcmp(argv[i+1], "0") || !strcmp(argv[i+1], "0.") || !strcmp(argv[i+1], "0.0")))) {
                fprintf(stderr, "Error: 'probred' argument is not valid.\n");
                exit(1);
            }
            continue;
        }

        if (!strcmp(argv[i], "-b") || !strcmp(argv[i], "--probblue")) {
            probBlue = atof(argv[i+1]);
            if (probBlue < 0. || (probBlue == 0. && !(!strcmp(argv[i+1], "0") || !strcmp(argv[i+1], "0.") || !strcmp(argv[i+1], "0.0")))) {
                fprintf(stderr, "Error: 'probblue' argument is not valid.\n");
                exit(1);
            }
            continue;
        }

        if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--satisratio")) {
            satisRatio = atof(argv[i+1]);
            if (satisRatio < 0. || (satisRatio == 0. && !(!strcmp(argv[i+1], "0") || !strcmp(argv[i+1], "0.") || !strcmp(argv[i+1], "0.0")))) {
                fprintf(stderr, "Error: 'satisratio' argument is not valid.\n");
                exit(1);
            }
            continue;
        }

        if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--pixelsize")) {
            pixelSize = atoi(argv[i+1]);
            if (pixelSize <= 0) {
                fprintf(stderr, "Error: 'pixelsize' argument is not valid.\n");
                exit(1);
            }
            continue;
        }

        if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--filename"))
            filename = argv[i+1];
    }

    // Seed de 'rand' fixée
    srand(time(NULL));

    // Création de la structure de Schelling
    Schelling *s = schellingInit(height, width, probRed, probBlue, satisRatio);
    if (!s)
        exit(1);

    // Initialisation du Gif
    schellingGIF *gif = createGif(s, filename, pixelSize);
    addFrame(s, gif, 1);

    // Simulation du modèle de Schelling
    int nbUnsatis;
    for (int i = 0; i < 1000; i++) {
        // Transition
        nbUnsatis = schellingOneStep(s);
        printf("Step %3d: Number of unsatisfieds = %d\n", i, nbUnsatis);

        // Ajout d'une nouvelle frame au Gif
        addFrame(s, gif, 10);

        // Si aucun insatisfait, le modèle a convergé
        if (nbUnsatis == 0) {
            printf("Converged in %d steps.\n", i);
            break;
        }
    }

    // Fermeture du Gif et Libération de la mémoire
    closeGif(gif);
    schellingFree(s);

    return 0;
}
