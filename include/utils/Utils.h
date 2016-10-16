/*
 * Constantes.h
 *
 *  Created on: Oct 15, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_UTILS_H
#define TP_CONCURRENTE_UTILS_H

#include <stdlib.h>
#include <time.h>

static int generarRandom(int tope) {
    srand((unsigned int) time(0));
    int result = rand() % tope + 1;
    return result;
}

static void dormir(int tiempo) {
    clock_t start = clock();
    double duration = 0;
    while (duration < tiempo) {
        duration = (clock() - start) / (double)CLOCKS_PER_SEC;
    }
}

#endif //TP_CONCURRENTE_UTILS_H
