/*
 * ProcesoRecepcionista.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#include "../../include/procesos/ProcesoRecepcionista.h"

ProcesoRecepcionista::ProcesoRecepcionista() : Proceso() {
}

int ProcesoRecepcionista::ejecutarMiTarea() {
    Logger::log("INFO", RECP, getpid(), "Recepcionista esperando para atender...");

    Semaforo semaforoCom = _semaforos[SEMAFORO_COM_RECP-1];
    Semaforo semaforoRec = _semaforos[SEMAFORO_RECP_COM-1];

    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    FifoLectura fifoLlegadaCom(ARCHIVO_FIFO_LLEGADA_COM);
    fifoLlegadaCom.abrir();
    
    char buffer[BUFFSIZE];
    while (!sigint_handler.getGracefulQuit()){
        // Bloquea si todavía no hay más pedidos para cocinar
        ssize_t bytesLeidos = fifoLlegadaCom.leer( static_cast<void*>(buffer), 10);
        if (bytesLeidos > 0) {
            std::string mensaje = buffer;
            mensaje.resize ( (unsigned long) bytesLeidos );
            Logger::log("INFO", RECP, getpid(), "Atendiendo a un grupo de " + mensaje + ".");
        }
        //sleep(2);
        semaforoRec.v();
    }
    SignalHandler::destruir();
    Logger::log("INFO", RECP, getpid(), "Proceso Recepcionista finalizado.");

    fifoLlegadaCom.cerrar();

    return 0;
}

ProcesoRecepcionista::~ProcesoRecepcionista() {

}
