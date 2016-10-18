/*
 * ProcesoComensales.cpp
 *
 *  Created on: Oct 14, 2016
 *      Author: emanuel
 */

#include "../../include/procesos/ProcesoComensales.h"

ProcesoComensales::ProcesoComensales(int id_Mesa, int count) : Proceso() {
    this->id_Mesa = id_Mesa;
    this->count = count;
}

int ProcesoComensales::ejecutarMiTarea() {
    Logger::log("INFO", COMN, getpid(), "Iniciando proceso de comensales...");

    SIGINT_Handler sigint_handler;
    SIGTERM_Handler sigterm_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
    SignalHandler::getInstance()->registrarHandler(SIGTERM, &sigterm_handler);

    FifoEscritura fifoNuevosPedidos(ARCHIVO_FIFO_NUEVOS_PEDIDOS);
    fifoNuevosPedidos.abrir();

    char buffer[BUFFSIZE];

    while (!sigint_handler.getGracefulQuit()) {
        Pedido pedido(id_Mesa); /* TODO debería poder mandarse el id_Mesa y el pid de este proceso. */
        pedido.agregarPlato(1,1);
        std::string mensaje = pedido.serializar();
        fifoNuevosPedidos.escribir( static_cast<const void*>(mensaje.c_str()),mensaje.length() );
        Logger::log("INFO", COMN, getpid(), "Esperando que un mozo tome nuestro pedido.");
        //sigterm_handler.executeNext(); /* Se bloquea hasta que le llegue su pedido */
        Logger::log("INFO", COMN, getpid(), "Llegó nuestro pedido. Comiendo...");
        break;
    }
    SignalHandler::destruir();
    sigterm_handler.eliminarSemaforo();
    fifoNuevosPedidos.cerrar();
    //fifoNuevosPedidos.eliminar(); Dejar comentado...porque deja procesos sueltos
    Logger::log("INFO", COMN, getpid(), "Comensales retirándose del restaurante.");
    return 0;
}

ProcesoComensales::~ProcesoComensales() {

}
