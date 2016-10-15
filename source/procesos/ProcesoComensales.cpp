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
    Logger::getInstance()->log("INFO", GCOM, getpid(), "Iniciando Generador de comensales...");

    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    Logger::getInstance()->log("INFO", GCOM, getpid(), "Hola, soy un generador de grupo de comensales y voy a generar grupos cada 4 segundos");
    while (!sigint_handler.getGracefulQuit()){
        Logger::getInstance()->log("INFO", GCOM, getpid(), "generando grupo de comensales: 4 (hardcodeo)...");

        sleep(4);
    }
    SignalHandler::destruir();
    Logger::getInstance()->log("INFO", GCOM, getpid(), "Cerrando...");
    return 0;
}

ProcesoComensales::~ProcesoComensales() {

}
