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
    Logger::getInstance()->log("INFO", RECP, getpid(), "Iniciando Recepcionista...");

    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    Logger::getInstance()->log("INFO", RECP, getpid(), "Hola, soy un Recepcionista y voy a atender cada 3 segundos");

    while (!sigint_handler.getGracefulQuit()){
        Logger::getInstance()->log("INFO", RECP, getpid(), "atendiendo...");

        sleep(3);
    }
    SignalHandler::getInstance()->destruir();
    Logger::getInstance()->log("INFO", RECP, getpid(), "Cerrando...");

    return 0;
}

ProcesoRecepcionista::~ProcesoRecepcionista() {

}
