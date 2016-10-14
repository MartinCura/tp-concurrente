/*
 * ProcesoComensales.h
 *
 *  Created on: Oct 14, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_PROCESOCOMENSALES_H
#define TP_CONCURRENTE_PROCESOCOMENSALES_H

#include "Proceso.h"

class ProcesoComensales : public Proceso {
private:
    int ejecutarMiTarea();

    void loggear(std::string mensaje);

public:
    ProcesoComensales();

    ~ProcesoComensales();
};

#endif //TP_CONCURRENTE_PROCESOCOMENSALES_H
