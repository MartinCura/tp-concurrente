/*
 * ProcesoRecepcionista.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#include <procesos/ProcesoMesasManager.h>
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

    FifoEscritura fifoNuevosComensalesEnMesa(ARCHIVO_FIFO_NUEVOS_COMENSALES_EN_MESA);
    fifoNuevosComensalesEnMesa.abrir();

    // oh dios demasiadas fifos

    char buffer[TAM_NUM_MESA+1] = "";

    while (!sigint_handler.getGracefulQuit()){

        int idMesa = -1;
        ssize_t bytesLeidos = 0;

        bytesLeidos = fifoMesasLibres.leer( static_cast<void*>(buffer),TAM_NUM_MESA );
        if (bytesLeidos > 0) {
            std::string msg = buffer;
            msg.resize( (unsigned long) bytesLeidos );
            idMesa = atoi(msg.c_str());
        }

        if (idMesa > -1) {
            std::string count_s = "0";
            /* Vemos si hay gente en el living */
            bytesLeidos = fifoLivingLec.leer( static_cast<void*>(buffer), 1);
            if (bytesLeidos > 0) {
                std::string mensaje = buffer;
                mensaje.resize ( (unsigned long) bytesLeidos );
                count_s = mensaje;
                Logger::log("INFO", RECP, getpid(), "Sacando comensales (" + mensaje + ") del living y ubicando en la mesa " + std::to_string(idMesa) + ".");
            } else {
                /* Si no hay gente en el living esperamos en la entrada */
                // Bloquea si todavía no hay más pedidos para cocinar
                bytesLeidos = fifoLlegadaCom.leer( static_cast<void*>(buffer), 1);
                if (bytesLeidos > 0) {
                    std::string mensaje = buffer;
                    mensaje.resize ( (unsigned long) bytesLeidos );
                    count_s = mensaje;
                    Logger::log("INFO", RECP, getpid(), "Ubicando a comensales (" + mensaje + ") en la mesa " + std::to_string(idMesa) + ".");
                }
            }
            /* Escribimos el id de la mesa y la cantidad de ocupantes */
            std::string mensaje = "";
            mensaje = ProcesoMesasManager::serializarIdMesa(idMesa) + "-" + count_s;
            fifoNuevosComensalesEnMesa.escribir(static_cast<const void*>(mensaje.c_str()), TAM_MAX_MSJ_RECP_PCM);
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

    fifoLlegadaCom.cerrar();
//    fifoLlegadaCom.eliminar(); //Este eliminar se hace en ProcesoGeneradorComensales
    fifoMesasLibres.cerrar();
//    fifoMesasLibres.eliminar();
    fifoLivingEsc.cerrar(); /* TODO debería haber un fifoLivingEsc.eliminar() en algún lado, pero sólo una vez. */
    //fifoLivingEsc.eliminar();
    fifoLivingLec.cerrar(); /* TODO debería haber un fifoLivingEsc.eliminar() en algún lado, pero sólo una vez. */
    //fifoLivingLec.eliminar();
    fifoNuevosComensalesEnMesa.cerrar();
//    fifoNuevosComensalesEnMesa.eliminar(); //Este eliminar se hace en ProcesoComensalesManager
    Logger::log("INFO", RECP, getpid(), "Proceso Recepcionista finalizado.");
    return 0;
}

ProcesoRecepcionista::~ProcesoRecepcionista() {
}
