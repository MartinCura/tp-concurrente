//
// Created by martin on 27/09/16.
//

#ifndef TP_CONCURRENTE_PLATO_H
#define TP_CONCURRENTE_PLATO_H

#include <string>

class Plato {
private:
    std::string nombre;
    float precio;

public:
    std::string get_nombre();
    float get_precio();
};


#endif //TP_CONCURRENTE_PLATO_H
