/*
 * ProcesoMozo.h
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_PROCESOMOZO_H
#define TP_CONCURRENTE_PROCESOMOZO_H

#include "Proceso.h"

class ProcesoMozo : public Proceso {
private:
    int ejecutarMiTarea();

public:
    ProcesoMozo();

    ~ProcesoMozo();
};

#endif //TP_CONCURRENTE_PROCESOMOZO_H
