/*
 * ProcesoRecepcionista.h
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_PROCESORECEPCIONISTA_H
#define TP_CONCURRENTE_PROCESORECEPCIONISTA_H

#include "Proceso.h"
#include <stdlib.h>
//#include "../modelo/ObjetosDeRestaurante.h"
//#include "../utils/MemoriaCompartida.h"

class ProcesoRecepcionista : public Proceso {
private:
//    int cantidadMesas;
//    std::vector<MemoriaCompartida<Mesa>> mesas;

    int ejecutarMiTarea();
//    void inicializarMesasCompartidas();
//    void destruirMesasCompartidas();

public:
    ProcesoRecepcionista(/*int cant_mesas*/);

    ~ProcesoRecepcionista();
};

#endif //TP_CONCURRENTE_PROCESORECEPCIONISTA_H
