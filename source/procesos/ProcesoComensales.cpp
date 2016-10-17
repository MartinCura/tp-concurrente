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
    Logger::log("INFO", COMN, getpid(), "Iniciando proceso de comensales...");

    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    while (!sigint_handler.getGracefulQuit()){

    }
    SignalHandler::destruir();

    Logger::log("INFO", COMN, getpid(), "Proceso de comensales finalizado.");
    return 0;
}

ProcesoComensales::~ProcesoComensales() {

}
