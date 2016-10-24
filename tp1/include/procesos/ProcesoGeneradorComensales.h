/*
 * ProcesoGeneradorComensales.h
 *
 *  Created on: Oct 16, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_PROCESOGENERADORCOMENSALES_H
#define TP_CONCURRENTE_PROCESOGENERADORCOMENSALES_H

#include "Proceso.h"
#include "../utils/Utils.h"

class ProcesoGeneradorComensales : public Proceso {
private:
    int ejecutarMiTarea();

public:
    ProcesoGeneradorComensales();

    ~ProcesoGeneradorComensales();
};

#endif //TP_CONCURRENTE_PROCESOGENERADORCOMENSALES_H
