/*
 * ProcesoRecepcionista.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#include "../../include/procesos/ProcesoRecepcionista.h"

ProcesoRecepcionista::ProcesoRecepcionista(int cant_mesas) : Proceso() {
    cantidadMesas = cant_mesas;
}

void ProcesoRecepcionista::inicializarMesasCompartidas() {
    char letra = 'A';
    for (unsigned i = 0; i < cantidadMesas; i++) {
        try {
            MemoriaCompartida<Mesa> mesa(ARCHIVO_SHM_MESA, letra);
            mesas.push_back(mesa);
            letra++;
        } catch (std::string &mensaje) {
            Logger::log("ERR", RECP, getpid(), "No se pudo crear la mesa con id " + std::to_string(i+1) + ". " + mensaje);
            continue;
        }
    }
}

void ProcesoRecepcionista::destruirMesasCompartidas() {
    for (unsigned i = 0; i < mesas.size(); i++)
        mesas[i].liberar();
}

int ProcesoRecepcionista::ejecutarMiTarea() {
    inicializarMesasCompartidas();

    Logger::log("INFO", RECP, getpid(), "Recepcionista esperando para atender...");

    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    FifoLectura fifoLlegadaCom(ARCHIVO_FIFO_LLEGADA_COM);
    fifoLlegadaCom.abrir();

    FifoEscritura fifoLivingEsc();
    FifoLectura fifoLivingLec();

    char buffer[BUFFSIZE];
    while (!sigint_handler.getGracefulQuit()){
        // Bloquea si todavía no hay más pedidos para cocinar
        ssize_t bytesLeidos = fifoLlegadaCom.leer( static_cast<void*>(buffer), 1);
        if (bytesLeidos > 0) {
            std::string mensaje = buffer;
            mensaje.resize ( (unsigned long) bytesLeidos );
            Logger::log("INFO", RECP, getpid(), "Atendiendo a un grupo de " + mensaje + ".");
        }
    }
    SignalHandler::destruir();
    Logger::log("INFO", RECP, getpid(), "Proceso Recepcionista finalizado.");
    destruirMesasCompartidas();
    fifoLlegadaCom.cerrar();

    return 0;
}

ProcesoRecepcionista::~ProcesoRecepcionista() {

}
