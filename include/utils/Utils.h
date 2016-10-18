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
#include <string>
#include <string.h>

class Utils {
public:
    static int generarRandom(int tope);
//    static void dormir(int tiempo);
    static std::vector<std::string> split(std::string line, const char* separator);
};


#endif //TP_CONCURRENTE_UTILS_H
