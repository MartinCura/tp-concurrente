/*
 * ProcesoRecepcionista.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#include "../../include/procesos/ProcesoRecepcionista.h"

ProcesoRecepcionista::ProcesoRecepcionista(/*int cant_mesas*/) : Proceso() {
//    cantidadMesas = cant_mesas;
}
/*
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
*/
int ProcesoRecepcionista::ejecutarMiTarea() {
    //inicializarMesasCompartidas();

    Logger::log("INFO", RECP, getpid(), "Recepcionista esperando para atender...");

    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    FifoLectura fifoLlegadaCom(ARCHIVO_FIFO_LLEGADA_COM);
    fifoLlegadaCom.abrir();

    FifoLectura fifoMesasLibres(ARCHIVO_FIFO_MESAS_LIBRES);
    fifoMesasLibres.abrir();
    fifoMesasLibres.setBlocking(false);

    FifoEscritura fifoLivingEsc(ARCHIVO_FIFO_LIVING_COM);
    fifoLivingEsc.abrir();
    FifoLectura fifoLivingLec(ARCHIVO_FIFO_LIVING_COM);
    fifoLivingLec.abrir();
    fifoLivingLec.setBlocking(false);

    char buffer[BUFFSIZE];
    while (!sigint_handler.getGracefulQuit()){
        int id_Mesa = -1;
        ssize_t bytesLeidos = fifoMesasLibres.leer( static_cast<void*>(buffer), 2);
        if (bytesLeidos > 0) {
            std::string msg = buffer;
            msg.resize(bytesLeidos);
            id_Mesa = atoi(msg.c_str());
            Logger::log("INFO", RECP, getpid(), "Mesa disponible con id : " + std::to_string(id_Mesa) + ".");
        }

        if (id_Mesa > -1) {
            bytesLeidos = fifoLivingLec.leer( static_cast<void*>(buffer), 1);
            if (bytesLeidos > 0) {
                std::string mensaje = buffer;
                mensaje.resize ( (unsigned long) bytesLeidos );
                Logger::log("INFO", RECP, getpid(), "Ubicando a comensales (" + mensaje + ") en la mesa " + std::to_string(id_Mesa) + ".");
            } else {
                // Bloquea si todavía no hay más pedidos para cocinar
                bytesLeidos = fifoLlegadaCom.leer( static_cast<void*>(buffer), 1);
                if (bytesLeidos > 0) {
                    std::string mensaje = buffer;
                    mensaje.resize ( (unsigned long) bytesLeidos );
                    Logger::log("INFO", RECP, getpid(), "Ubicando a comensales (" + mensaje + ") en la mesa " + std::to_string(id_Mesa) + ".");
                }
            }
        } else {
            // Bloquea si todavía no hay más pedidos para cocinar
            bytesLeidos = fifoLlegadaCom.leer( static_cast<void*>(buffer), 1);
            if (bytesLeidos > 0) {
                std::string mensaje = buffer;
                mensaje.resize ( (unsigned long) bytesLeidos );
                fifoLivingEsc.escribir(static_cast<const void*>(mensaje.c_str()), mensaje.length());
                Logger::log("INFO", RECP, getpid(), "Ubicando a comensales (" + mensaje + ") en el living.");
            }
        }
/*
        // Bloquea si todavía no hay más pedidos para cocinar
        bytesLeidos = fifoLlegadaCom.leer( static_cast<void*>(buffer), 1);
        if (bytesLeidos > 0) {
            std::string mensaje = buffer;
            mensaje.resize ( (unsigned long) bytesLeidos );
            if (id_Mesa > -1)
                Logger::log("INFO", RECP, getpid(), "Ubicando a comensales (" + mensaje + ") en la mesa " + std::to_string(id_Mesa) + ".");
            else {
                fifoLivingEsc.escribir(static_cast<const void*>(mensaje.c_str()), mensaje.length());
                Logger::log("INFO", RECP, getpid(), "Atendiendo a un grupo de " + mensaje + ".");
            }
        }
*/
    }
    SignalHandler::destruir();
    Logger::log("INFO", RECP, getpid(), "Proceso Recepcionista finalizado.");
//    destruirMesasCompartidas();
    fifoLlegadaCom.cerrar();
    fifoMesasLibres.cerrar();
    fifoLivingEsc.cerrar(); /* TODO debería haber un fifoLivingEsc.eliminar() en algún lado, pero sólo una vez. */
    fifoLivingLec.cerrar(); /* TODO debería haber un fifoLivingEsc.eliminar() en algún lado, pero sólo una vez. */

    return 0;
}

ProcesoRecepcionista::~ProcesoRecepcionista() {

}
