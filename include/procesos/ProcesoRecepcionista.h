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

    void loggear(std::string mensaje);

public:
    ProcesoRecepcionista();

    ~ProcesoRecepcionista();
};

#endif //TP_CONCURRENTE_PROCESORECEPCIONISTA_H
