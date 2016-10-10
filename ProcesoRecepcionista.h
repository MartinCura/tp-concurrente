/*
 * ProcesoRecepcionista.h
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_PROCESORECEPCIONISTA_H
#define TP_CONCURRENTE_PROCESORECEPCIONISTA_H

#include "Proceso.h"

class ProcesoRecepcionista : public Proceso {
private:
    int ejecutarMiTarea();

public:
    ProcesoRecepcionista();

    void start();

    ~ProcesoRecepcionista();
};

#endif //TP_CONCURRENTE_PROCESORECEPCIONISTA_H
