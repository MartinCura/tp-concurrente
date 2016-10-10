/*
 * ProcesoMozo.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#include "ProcesoMozo.h"

ProcesoMozo::ProcesoMozo() : Proceso() {
}

void ProcesoMozo::start() {
   pid_t id = fork();
   if (id == 0) {
       ejecutarMiTarea();
       throw ProcesoTerminadoException(getpid());
   } else {
       pid = id;
   }
}

int ProcesoMozo::ejecutarMiTarea() {
    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    while (!sigint_handler.getGracefulQuit()){
        std::cout << "[" << getpid() << " Mozo] Hola, soy un Mozo y voy a atender cada 2 segundos..." << std::endl;
        sleep(2);
    }
    SignalHandler::getInstance()->destruir();

    return 0;
}

ProcesoMozo::~ProcesoMozo() {

}
