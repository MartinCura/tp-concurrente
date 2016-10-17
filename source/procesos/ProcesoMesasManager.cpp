//
// Created by martin on 16/10/16.
//

#include <utils/MemoriaCompartida.h>
#include <modelo/ObjetosDeRestaurante.h>
#include <modelo/Restaurante.h>
#include "procesos/ProcesoMesasManager.h"

ProcesoMesasManager::ProcesoMesasManager(int cantMesas) : Proceso() {
    this->cantMesas = cantMesas;
}

void ProcesoMesasManager::lanzarMesasDisponiblesIniciales(FifoEscritura fifo) {
    for (unsigned i = 0; i < cantMesas; i++) {
        std::string let = std::to_string(i);
        fifo.escribir(static_cast<const void*>(let.c_str()), 2);
    }
}

int ProcesoMesasManager::ejecutarMiTarea() {
    Logger::log("INFO", PMM_, getpid(), "Iniciando Proceso Mesas Manager...");
    FifoEscritura fifoMesasLibres(ARCHIVO_FIFO_MESAS_LIBRES);
//    FifoLectura fifoMesasLiberadas(ARCHIVO_FIFO_MESAS_LIBERADAS);
    fifoMesasLibres.abrir();
//    fifoMesasLiberadas.abrir();

    lanzarMesasDisponiblesIniciales(fifoMesasLibres);

    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    while (!sigint_handler.getGracefulQuit()) {
        char letra = 'a';
        for (unsigned i = 0; i < cantMesas; ++i) {
            try {
                MemoriaCompartida<Mesa> shmMesa(ARCHIVO_SHM_MESA, letra);
                struct Mesa mesa = shmMesa.leer();

                if (!mesa.ocupada && mesa.gastado != 0) {
                    Restaurante::agregarGanancia(mesa.gastado);
                    mesa.gastado = 0;
                    shmMesa.escribir(mesa);
                    // TODO Actualizar mesas[numMesa] en Restaurante de alguna manera...
                }
                letra++;

            } catch (std::string &mensaje) {
                //Logger::log("ERR", REST, getpid(), "No se pudo acceder a una mesa. " + mensaje); TODO
                continue;
            }
        }
    }
    SignalHandler::destruir();
    Logger::log("INFO", PMM_, getpid(), "Proceso Mesas Manager finalizado.");
    fifoMesasLibres.cerrar();
    fifoMesasLibres.eliminar();
    return 0;
}

void ProcesoMesasManager::reset() {
    char letra = 'a';
    int cantMesas = 10;//
    for (unsigned i = 0; i < cantMesas; ++i) {
        try {
            MemoriaCompartida<Mesa> shmMesa(ARCHIVO_SHM_MESA, letra);
            struct Mesa mesa = shmMesa.leer();

            if (mesa.gastado != 0) {
                Restaurante::agregarPerdida(mesa.gastado);
                mesa.gastado = 0;
                shmMesa.escribir(mesa);
                // TODO Actualizar mesas[numMesa] en Restaurante de alguna manera...
            }
            letra++;

        } catch (std::string &mensaje) {
            Logger::log("ERR", PMM_, getpid(), "No se pudo acceder a una mesa. " + mensaje);
            continue;
        }
    }
}

ProcesoMesasManager::~ProcesoMesasManager() {
}
