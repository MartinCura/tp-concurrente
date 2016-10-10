/*
 * ProcesoRecepcionista.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#include "ProcesoRecepcionista.h"

ProcesoRecepcionista::ProcesoRecepcionista() : Proceso() {
}

int ProcesoRecepcionista::ejecutarMiTarea() {
    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    while (!sigint_handler.getGracefulQuit()){
        std::cout << "[" << getpid() << " Recp] Hola, soy un Recepcionista y voy a atender cada 3 segundos..." << std::endl;
        sleep(3);
    }
    SignalHandler::getInstance()->destruir();

    return 0;
}

ProcesoRecepcionista::~ProcesoRecepcionista() {

}
