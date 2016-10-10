/*
 * ProcesoMozo.h
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_PROCESOMOZO_H
#define TP_CONCURRENTE_PROCESOMOZO_H

#include "Proceso.h"
#include <iostream>

class ProcesoMozo : public Proceso {
private:
    int ejecutarMiTarea();

public:
    ProcesoMozo();

    void start();

    ~ProcesoMozo();
};

#endif //TP_CONCURRENTE_PROCESOMOZO_H
