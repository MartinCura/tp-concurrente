//
// Created by martin on 16/10/16.
//

#ifndef TP_CONCURRENTE_PROCESOMESASMANAGER_H
#define TP_CONCURRENTE_PROCESOMESASMANAGER_H

#include "Proceso.h"
#include "../utils/MemoriaCompartida.h"

class ProcesoMesasManager : public Proceso {
private:
    int cantMesas;

    int ejecutarMiTarea();
    void lanzarMesasDisponiblesIniciales(FifoEscritura fifo);

public:
    ProcesoMesasManager(int cantMesas);
    void reset();
    ~ProcesoMesasManager();
};


#endif //TP_CONCURRENTE_PROCESOMESASMANAGER_H
