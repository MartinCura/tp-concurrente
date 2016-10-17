//
// Created by martin on 16/10/16.
//

#include <utils/MemoriaCompartida.h>
#include <modelo/ObjetosDeRestaurante.h>
#include <modelo/Restaurante.h>
#include "procesos/ProcesoMesasManager.h"

ProcesoMesasManager::ProcesoMesasManager() : Proceso() {
}

int ProcesoMesasManager::ejecutarMiTarea() {
    int cantMesas = 1000; //hardcodeo

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
    return 0;
}

void ProcesoMesasManager::reset() {
    char letra = 'a';
    int cantMesas = 1000;//
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
            Logger::log("ERR", REST, getpid(), "No se pudo acceder a una mesa. " + mensaje);
            continue;
        }
    }
}

ProcesoMesasManager::~ProcesoMesasManager() {
}
