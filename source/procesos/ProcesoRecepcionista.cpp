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

    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    FifoLectura fifoLlegadaCom(ARCHIVO_FIFO_LLEGADA_COM);
    fifoLlegadaCom.abrir();

    FifoLectura fifoMesasLibres(ARCHIVO_FIFO_MESAS_LIBRES);
    fifoMesasLibres.abrir(false);

    FifoLectura fifoLivingLec(ARCHIVO_FIFO_LIVING_COM);
    fifoLivingLec.abrir(false);

    FifoEscritura fifoLivingEsc(ARCHIVO_FIFO_LIVING_COM);
    fifoLivingEsc.abrir();

    char buffer[BUFFSIZE];
    while (!sigint_handler.getGracefulQuit()){
        int id_Mesa = -1;
        ssize_t bytesLeidos = fifoMesasLibres.leer( static_cast<void*>(buffer), 2);
        if (bytesLeidos > 0) {
            std::string msg = buffer;
            msg.resize(bytesLeidos);
            id_Mesa = atoi(msg.c_str());
            //Logger::log("INFO", RECP, getpid(), "Mesa disponible con id : " + std::to_string(id_Mesa) + ".");
        }

        if (id_Mesa > -1) {
            /* Vemos si hay gente en el living */
            bytesLeidos = fifoLivingLec.leer( static_cast<void*>(buffer), 1);
            if (bytesLeidos > 0) {
                std::string mensaje = buffer;
                mensaje.resize ( (unsigned long) bytesLeidos );
                Logger::log("INFO", RECP, getpid(), "Sacando comensales (" + mensaje + ") del living y ubicando en la mesa " + std::to_string(id_Mesa) + ".");
            } else {
                /* Si no hay gente en el living esperamos en la entrada */
                // Bloquea si todavía no hay más pedidos para cocinar
                bytesLeidos = fifoLlegadaCom.leer( static_cast<void*>(buffer), 1);
                if (bytesLeidos > 0) {
                    std::string mensaje = buffer;
                    mensaje.resize ( (unsigned long) bytesLeidos );
                    Logger::log("INFO", RECP, getpid(), "Ubicando a comensales (" + mensaje + ") en la mesa " + std::to_string(id_Mesa) + ".");
                }
            }
            ProcesoComensales* proc = new ProcesoComensales(id_Mesa);
            comensales.push_back(proc);
            try {
                proc->start();
            } catch (ProcesoTerminadoException &p) {
                SignalHandler::destruir();
                fifoLlegadaCom.cerrar();
                fifoMesasLibres.cerrar();
                fifoLivingEsc.cerrar(); /* TODO debería haber un fifoLivingEsc.eliminar() en algún lado, pero sólo una vez. */
                fifoLivingLec.cerrar(); /* TODO debería haber un fifoLivingEsc.eliminar() en algún lado, pero sólo una vez. */
                Logger::log("INFO", COMN, getpid(), "Comensales retirándose del restaurante.");
                throw ProcesoTerminadoException(p.pid);
            }
        } else {
            /* Como no hay mesas disponibles, las personas que lleguen pasarán al living */
            // Bloquea si todavía no hay más pedidos para cocinar
            bytesLeidos = fifoLlegadaCom.leer( static_cast<void*>(buffer), 1);
            if (bytesLeidos > 0) {
                std::string mensaje = buffer;
                mensaje.resize ( (unsigned long) bytesLeidos );
                fifoLivingEsc.escribir(static_cast<const void*>(mensaje.c_str()), mensaje.length());
                Logger::log("INFO", RECP, getpid(), "Ubicando a comensales (" + mensaje + ") en el living.");
            }
        }
    }
    SignalHandler::destruir();
    Logger::log("INFO", RECP, getpid(), "Proceso Recepcionista finalizado.");
    fifoLlegadaCom.cerrar();
    //fifoLlegadaCom.eliminar();
    fifoMesasLibres.cerrar();
    //fifoMesasLibres.eliminar();
    fifoLivingEsc.cerrar(); /* TODO debería haber un fifoLivingEsc.eliminar() en algún lado, pero sólo una vez. */
    fifoLivingEsc.eliminar();
    fifoLivingLec.cerrar(); /* TODO debería haber un fifoLivingEsc.eliminar() en algún lado, pero sólo una vez. */
    fifoLivingLec.eliminar();
    return 0;
}

ProcesoRecepcionista::~ProcesoRecepcionista() {
    for (unsigned i = 0; i < comensales.size(); i++)
        delete comensales[i];
}
