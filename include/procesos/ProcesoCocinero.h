/*
 * ProcesoCocinero.h
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_PROCESOCOCINERO_H
#define TP_CONCURRENTE_PROCESOCOCINERO_H

#include "Proceso.h"
#include "../modelo/Pedido.h"

class ProcesoCocinero : public Proceso {
private:
    int ejecutarMiTarea();

    void cocinar(Pedido pedido);
    void enviarPedidoAMozos(FifoEscritura fifo, Pedido pedido);

public:
    ProcesoCocinero();

    ~ProcesoCocinero();
};

#endif //TP_CONCURRENTE_PROCESOCOCINERO_H
