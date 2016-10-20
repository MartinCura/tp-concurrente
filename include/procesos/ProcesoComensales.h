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
#include <modelo/ObjetosDeRestaurante.h>
#include <procesos/ProcesoMesasManager.h>


class ProcesoComensales : public Proceso {
private:
    int id_mesa;
    int count;
    Semaforo semaforoComer;
    int ejecutarMiTarea();
    Pedido recibirPedido();
    void comer(Pedido pedido);

    MemoriaCompartida<struct MesasConPedidos> shmPedidos;

public:
    ProcesoComensales(Semaforo &sem, int idMesa, int count);

    ~ProcesoComensales();
};

#endif //TP_CONCURRENTE_PROCESOCOMENSALES_H
