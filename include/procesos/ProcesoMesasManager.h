//
// Created by martin on 16/10/16.
//

#ifndef TP_CONCURRENTE_PROCESOMESASMANAGER_H
#define TP_CONCURRENTE_PROCESOMESASMANAGER_H

#include "Proceso.h"
#include "../utils/MemoriaCompartida.h"
//#include "../utils/Semaforo.h"

class ProcesoMesasManager : public Proceso {
private:
    unsigned cantMesas = 1;
    std::vector<struct Mesa> vMesas;
    Semaforo semaforoMesasListas;

    int ejecutarMiTarea();
    void lanzarMesasDisponiblesIniciales(FifoEscritura fifo);
    void registrarPedidoEnUnaMesa(FifoLectura);

public:
    ProcesoMesasManager(unsigned cantMesas, Semaforo &sem);
    void reset(); // TODO: Por qué aparece como nunca usado? SIGUSR1_Handler lo usa...

    static std::string serializarIdMesa(int num_mesa);

    int stop__();
    void vaciar();

    ~ProcesoMesasManager();
};


#endif //TP_CONCURRENTE_PROCESOMESASMANAGER_H
