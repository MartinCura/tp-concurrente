/*
 * ProcesoComensales.cpp
 *
 *  Created on: Oct 14, 2016
 *      Author: emanuel
 */
#include <poll.h>
#include "../../include/procesos/ProcesoComensales.h"

ProcesoComensales::ProcesoComensales() : Proceso() {
}

int ProcesoComensales::ejecutarMiTarea() {
    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    loggear("Hola, soy un generador de grupo de comensales y voy a generar grupos cada 4 segundos");
    while (!sigint_handler.getGracefulQuit()){
        loggear("...generando grupo de comensales: 4 (hardcodeo)...");

        //TODO: hardcodeo un hacerpedido para probar que le llegue un pedido al mozo.
        hacerPedido();

        sleep(4);
    }
    SignalHandler::getInstance()->destruir();

    return 0;
}

/**
 * Metodo que revisa la fifo de pedidos nuevos de comensales.
 *
 */
void ProcesoComensales::hacerPedido() {
    //Abro FIFO de escritura para hacer pedidos al mozo.
    FifoEscritura fifoNuevosPedidos(ARCHIVO_FIFO_NUEVOS_PEDIDOS_COMENSALES);
    fifoNuevosPedidos.abrir();

    loggear("Abrio fifoescritura comensales");
    loggear("Comensal realizando pedido");

    Pedido pedido;
    pedido.num_mesa=2;
    std::string mensaje = pedido.serializar();
    fifoNuevosPedidos.escribir( static_cast<const void*>(mensaje.c_str()),mensaje.length() );
    loggear("Pedido Enviado: " + mensaje);//

    fifoNuevosPedidos.cerrar();
}

void ProcesoComensales::loggear(std::string mensaje) {
    std::cout << "[" << getpid() << " Comens] " << mensaje << std::endl;
}

ProcesoComensales::~ProcesoComensales() {

}
