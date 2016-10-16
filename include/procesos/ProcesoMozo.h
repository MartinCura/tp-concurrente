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
    void enviarPedidoACocinero(Pedido pedido);
    void tomarPedidosComensales();
    void cobrarAComensales();
    void entregarPedidosAComensales();

    void loggear(std::string mensaje);

public:
    ProcesoMozo();

    ~ProcesoMozo();
};

#endif //TP_CONCURRENTE_PROCESOMOZO_H
