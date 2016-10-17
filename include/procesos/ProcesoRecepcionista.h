/*
 * ProcesoRecepcionista.h
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_PROCESORECEPCIONISTA_H
#define TP_CONCURRENTE_PROCESORECEPCIONISTA_H

#include "ProcesoComensales.h"
#include <stdlib.h>

class ProcesoRecepcionista : public Proceso {
private:
    std::vector<ProcesoComensales*> comensales;
    int ejecutarMiTarea();

public:
    ProcesoRecepcionista();

    ~ProcesoRecepcionista();
};

#endif //TP_CONCURRENTE_PROCESORECEPCIONISTA_H
