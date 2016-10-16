/*
 * ProcesoMozo.h
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_PROCESOMOZO_H
#define TP_CONCURRENTE_PROCESOMOZO_H

#include "Proceso.h"
#include "../utils/fifos/FifoEscritura.h"
#include "../utils/fifos/FifoLectura.h"
#include "../modelo/Pedido.h"

class ProcesoMozo : public Proceso {
private:
    int ejecutarMiTarea();

    void recibirNuevoPedido(FifoEscritura fifo);
    void enviarPedidoACocinero(FifoEscritura fifo, Pedido pedido);
    void recibirPedidosListos(FifoLectura fifo);
    void entregarPedido(Pedido pedido);

public:
    ProcesoMozo();

    ~ProcesoMozo();
};

#endif //TP_CONCURRENTE_PROCESOMOZO_H
