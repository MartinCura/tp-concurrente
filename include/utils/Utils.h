/*
 * Constantes.h
 *
 *  Created on: Oct 15, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_UTILS_H
#define TP_CONCURRENTE_UTILS_H

#include <string>
#include <sstream>

static std::string int_to_string(int number) {
    std::string result;
    std::ostringstream convert;
    convert << number;
    result = convert.str();
    return result;
}

#endif //TP_CONCURRENTE_UTILS_H
