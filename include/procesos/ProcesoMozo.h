/*
 * ProcesoMozo.h
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_PROCESOMOZO_H
#define TP_CONCURRENTE_PROCESOMOZO_H

#include "Proceso.h"
#include "../modelo/Pedido.h"
#include "../utils/Constantes.h"

enum AccionMozo {
    NADA,
    TOMAR_PEDIDO,
    ENTREGAR_PEDIDO
};

class ProcesoMozo : public Proceso {
private:
    int ejecutarMiTarea();

    AccionMozo esperarAccion(std::vector<FifoLectura> fifos);
    void recibirNuevoPedido(FifoLectura fifoNuevosPedidos, FifoEscritura fifoCocinero);
    void enviarPedidoACocinero(FifoEscritura fifo, Pedido pedido);
    void recibirPedidosListos(FifoLectura fifo);
    void entregarPedido(Pedido pedido);

public:
    ProcesoMozo();

    ~ProcesoMozo();
};

#endif //TP_CONCURRENTE_PROCESOMOZO_H
