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
    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    loggear("Hola, soy un Recepcionista y voy a atender cada 3 segundos");
    while (!sigint_handler.getGracefulQuit()){
        loggear("atendiendo...");
        sleep(3);
    }
    SignalHandler::getInstance()->destruir();

    return 0;
}

void ProcesoRecepcionista::loggear(std::string mensaje) {
    std::cout << "[" << getpid() << " Recp] " << mensaje << std::endl;
}

ProcesoRecepcionista::~ProcesoRecepcionista() {

}
