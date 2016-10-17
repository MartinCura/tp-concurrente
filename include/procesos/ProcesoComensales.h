/*
 * ProcesoComensales.h
 *
 *  Created on: Oct 14, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_PROCESOCOMENSALES_H
#define TP_CONCURRENTE_PROCESOCOMENSALES_H

#include "Proceso.h"
#include "../modelo/Pedido.h"
#include "../utils/Utils.h"

class ProcesoComensales : public Proceso {
private:
    int id_Mesa;
    int ejecutarMiTarea();

public:
    ProcesoComensales(int idMesa);

    ~ProcesoComensales();
};

#endif //TP_CONCURRENTE_PROCESOCOMENSALES_H
