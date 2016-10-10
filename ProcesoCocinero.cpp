/*
 * ProcesoCocinero.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#include "ProcesoCocinero.h"

ProcesoCocinero::ProcesoCocinero() : Proceso() {
}

int ProcesoCocinero::ejecutarMiTarea() {
    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    while (!sigint_handler.getGracefulQuit()){
        std::cout << "[" << getpid() << " Coci] Hola, soy un Cocinero y voy a atender cada 3 segundos..." << std::endl;
        sleep(3);
    }
    SignalHandler::getInstance()->destruir();

    return 0;
}

ProcesoCocinero::~ProcesoCocinero() {

}
