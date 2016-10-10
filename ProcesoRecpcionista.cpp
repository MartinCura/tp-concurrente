/*
 * ProcesoRecepcionista.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#include "ProcesoRecepcionista.h"
#include <iostream>

ProcesoRecepcionista::ProcesoRecepcionista() : Proceso() {
}

void ProcesoRecepcionista::start() {
   pid_t id = fork();
   if (id == 0) {
        ejecutarMiTarea();
        throw ProcesoTerminadoException(getpid());
   } else {
       pid = id;
   }
}

int ProcesoRecepcionista::ejecutarMiTarea() {
    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    while (!sigint_handler.getGracefulQuit()){
        std::cout << "[" << getpid() << " Recp] Hola, soy un Recepcionista y voy a atender cada 2 segundos..." << std::endl;
        sleep(2);
    }
    SignalHandler::getInstance()->destruir();

    return 0;
}

ProcesoRecepcionista::~ProcesoRecepcionista() {

}
