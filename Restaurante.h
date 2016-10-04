//
// Created by martin on 27/09/16.
//

#ifndef TP_CONCURRENTE_RESTAURANTE_H
#define TP_CONCURRENTE_RESTAURANTE_H

#include <iostream>
#include <list>
#include "Plato.h"

class Restaurante {
private:
    int cantDeRecepcionistas,
        cantMozos,
        cantMesas;
    std::list<Plato> listPlatos;

public:
    Restaurante();

    bool inicializar();

    void run();

    ~Restaurante();
};


#endif //TP_CONCURRENTE_RESTAURANTE_H
