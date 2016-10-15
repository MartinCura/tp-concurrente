/*
 * ProcesoCocinero.h
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_PROCESOCOCINERO_H
#define TP_CONCURRENTE_PROCESOCOCINERO_H

#include "../modelo/Pedido.h"
#include "Proceso.h"
#include "../utils/fifos/FifoLectura.h"
#include "../utils/fifos/FifoEscritura.h"

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
