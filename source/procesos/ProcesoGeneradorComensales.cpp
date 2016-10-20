/*
 * ProcesoGeneradorComensales.cpp
 *
 *  Created on: Oct 16, 2016
 *      Author: emanuel
 */

#include "../../include/procesos/ProcesoGeneradorComensales.h"

ProcesoGeneradorComensales::ProcesoGeneradorComensales() : Proceso() {
}

int ProcesoGeneradorComensales::ejecutarMiTarea() {
    Logger::log("INFO", GCOM, getpid(), "Iniciando Generador de comensales...");

    SIGINT_Handler sigint_handler;
    SIGTERM_Handler sigterm_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
    SignalHandler::getInstance()->registrarHandler(SIGTERM, &sigterm_handler);

    FifoEscritura fifoLlegadaCom ( ARCHIVO_FIFO_LLEGADA_COM );
    fifoLlegadaCom.abrir();
    kill(getpid(), SIGTERM);
    while (!sigint_handler.getGracefulQuit()){
        if (sigterm_handler.isAvailible()) {
            /* Tiempo que transcurre hasta que llegue el próximo grupo */
            unsigned tiempoEspera = (unsigned) Utils::generarRandom(10);
            sleep(tiempoEspera);
            //dormir(tiempoEspera); /* TODO Es un "slepp" sin sleep asi que vale */
            /* Cantidad de comensales en el grupo que llegó */
            int cantComensales = Utils::generarRandom(MAX_COMENSALES_EN_GRUPO);
            std::string num_s = std::to_string(cantComensales);

            Logger::log("INFO", GCOM, getpid(), "Llegó a la puerta un grupo de " + num_s + ".");
            int result = fifoLlegadaCom.escribir(static_cast<const void*>(num_s.c_str()), num_s.length());
            if (result < 0)
                break;
        }
    }
    SignalHandler::destruir();

    fifoLlegadaCom.cerrar();
    fifoLlegadaCom.eliminar();

    Logger::log("INFO", GCOM, getpid(), "Proceso de llegada de comensales finalizado.");
    return 0;
}

ProcesoGeneradorComensales::~ProcesoGeneradorComensales() {

}
