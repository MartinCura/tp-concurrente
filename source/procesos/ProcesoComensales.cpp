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
    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    loggear("Hola, soy un generador de grupo de comensales y voy a generar grupos cada 4 segundos");
    while (!sigint_handler.getGracefulQuit()){
        loggear("...generando grupo de comensales: 4 (hardcodeo)...");
        sleep(4);
    }
    SignalHandler::getInstance()->destruir();

    return 0;
}

void ProcesoComensales::loggear(std::string mensaje) {
    std::cout << "[" << getpid() << " Comens] " << mensaje << std::endl;
}

ProcesoComensales::~ProcesoComensales() {

}
