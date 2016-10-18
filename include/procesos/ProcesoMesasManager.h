//
// Created by martin on 16/10/16.
//

#ifndef TP_CONCURRENTE_PROCESOMESASMANAGER_H
#define TP_CONCURRENTE_PROCESOMESASMANAGER_H

#include "Proceso.h"
#include "../utils/MemoriaCompartida.h"

class ProcesoMesasManager : public Proceso {
private:
    unsigned cantMesas = 1;
    std::vector<struct Mesa> vMesas;

    int ejecutarMiTarea();
    void lanzarMesasDisponiblesIniciales(FifoEscritura fifo);

public:
    ProcesoMesasManager(unsigned cantMesas);
    void reset();

    static std::string serializarIdMesa(int num_mesa);

    ~ProcesoMesasManager();
};


#endif //TP_CONCURRENTE_PROCESOMESASMANAGER_H
