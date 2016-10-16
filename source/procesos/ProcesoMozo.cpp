/*
 * ProcesoMozo.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#include "../../include/procesos/ProcesoMozo.h"

ProcesoMozo::ProcesoMozo() : Proceso() {
}

int ProcesoMozo::ejecutarMiTarea() {
    Logger::log("INFO", MOZO, getpid(), "Mozo esperando a atender...");

    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    // Abro fifo de escritura para pedidos al cocinero
    // Se bloquea hasta que aparezca el cocinero
    FifoEscritura fifoACocinar ( ARCHIVO_FIFO_COCINAR );
    fifoACocinar.abrir();

    // Abro fifo de escritura para pedidos ya cocinados y para entregar
    // Se bloquea hasta que aparezca el cocinero
    FifoLectura fifoCocinado ( ARCHIVO_FIFO_COCINADO );
    fifoCocinado.abrir();

    while (!sigint_handler.getGracefulQuit()){
        Logger::log("INFO", MOZO, getpid(), "Mozo atendiendo...");
        sleep(2);

        // Recibir pedidos cocinados por cocinero
        // Si hay, entregar a comensales correctos
        // TODO: Cómo chequear cola sin bloquear? Semáforos? Locks?

        try {
            Pedido pedido(10);// Pedido de grupo de comensales TODO HARDCODE
            enviarPedidoACocinero( fifoACocinar, pedido );

        } catch (std::invalid_argument ex) {
            Logger::log("ERR", MOZO, getpid(), "Pasé un argumento inválido a creación de pedido");
        }
    }

    fifoACocinar.cerrar();
    fifoACocinar.eliminar();

    fifoCocinado.cerrar();
    fifoCocinado.eliminar();

    SignalHandler::destruir();
    Logger::log("INFO", MOZO, getpid(), "Proceso Mozo finalizado.");
    return 0;
}

void ProcesoMozo::enviarPedidoACocinero(FifoEscritura fifo, Pedido pedido) {
    std::string mensaje = pedido.serializar();
    //loggear("mensaje: " + mensaje);
    fifo.escribir(static_cast<const void*>(mensaje.c_str()),mensaje.length() );
}

ProcesoMozo::~ProcesoMozo() {

}
