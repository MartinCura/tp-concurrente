//
// Created by martin on 26/09/16.
//

#ifndef TP_CONCURRENTE_COCINERO_H
#define TP_CONCURRENTE_COCINERO_H


#include <queue>
#include <string>
#include "Empleado.h"
#include "../modelo/Plato.h"

class Cocinero: public Empleado {
private:
    std::queue<Plato> colaCocinar; // TODO: Reemplazar por fifo

public:
    void Cocinar();
};


#endif //TP_CONCURRENTE_COCINERO_H
