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
#include <vector>
#include <string.h>


static int generarRandom(int tope) {
    srand((unsigned int) time(0));
    return rand() % tope + 1;
}

static void dormir(int tiempo) {
    clock_t start = clock();
    double duration = 0;
    while (duration < tiempo) {
        duration = (clock() - start) / (double)CLOCKS_PER_SEC;
    }
}

static std::vector<std::string> split(std::string line, const char* separator) {
	std::vector<std::string> vec;
    char * dup = strdup(line.c_str());
    char * token = strtok(dup, separator);
    while(token != NULL){
        vec.push_back(std::string(token));
        token = strtok(NULL, separator);
    }
    free(dup);
	return vec;
}

#endif //TP_CONCURRENTE_UTILS_H
