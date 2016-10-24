//
// Created by martin on 17/10/16.
//

#include "../../include/utils/Utils.h"


int Utils::generarRandom(int tope) {
    srand((unsigned int) time(0));
    return rand() % tope + 1;
}

//static void dormir(int tiempo) {
//    clock_t start = clock();
//    double duration = 0;
//    while (duration < tiempo) {
//        duration = (clock() - start) / (double)CLOCKS_PER_SEC;
//    }
//}

std::vector<std::string> Utils::split(std::string line, const char* separator) {
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
