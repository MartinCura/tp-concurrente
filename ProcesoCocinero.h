/*
 * ProcesoCocinero.h
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_PROCESOCOCINERO_H
#define TP_CONCURRENTE_PROCESOCOCINERO_H

#include "Proceso.h"

class ProcesoCocinero : public Proceso {
private:
    int ejecutarMiTarea();

public:
    ProcesoCocinero();

    ~ProcesoCocinero();
};

#endif //TP_CONCURRENTE_PROCESOCOCINERO_H
