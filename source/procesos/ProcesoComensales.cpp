/*
 * ProcesoComensales.cpp
 *
 *  Created on: Oct 14, 2016
 *      Author: emanuel
 */

#include "../../include/procesos/ProcesoComensales.h"

ProcesoComensales::ProcesoComensales() : Proceso() {
}

int ProcesoComensales::ejecutarMiTarea() {
    Logger::log("INFO", GCOM, getpid(), "Iniciando Generador de comensales...");

    Semaforo semaforoCom = _semaforos[SEMAFORO_COM_RECP-1];
    Semaforo semaforoRec = _semaforos[SEMAFORO_RECP_COM-1];

    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    FifoEscritura fifoLlegadaCom ( ARCHIVO_FIFO_LLEGADA_COM );
    fifoLlegadaCom.abrir();

    while (!sigint_handler.getGracefulQuit()){
        /* Tiempo que transcurre hasta que llegue el próximo grupo */
        int tiempoEspera = generarRandom(10);
        sleep(tiempoEspera);
        //dormir(tiempoEspera); /* TODO Es un "slepp" sin sleep asi que vale */
        /* Cantidad de comensales en el grupo que llegó */
        int cantComensales = generarRandom(MAX_COMENSALES_EN_GRUPO);
        std::string num_s = int_to_string(cantComensales);

        Logger::log("INFO", GCOM, getpid(), "Llegó un grupo de comensales de " + num_s);
        semaforoRec.p();
        fifoLlegadaCom.escribir(static_cast<const void*>(num_s.c_str()), num_s.length());
    }
    SignalHandler::destruir();

    fifoLlegadaCom.cerrar();
    fifoLlegadaCom.eliminar();

    Logger::log("INFO", GCOM, getpid(), "Proceso de llegada de comensales finalizado.");
    return 0;
}

ProcesoComensales::~ProcesoComensales() {

}
