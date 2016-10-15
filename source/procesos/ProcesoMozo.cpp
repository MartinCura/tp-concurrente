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
    Logger::getInstance()->log("INFO", MOZO, getpid(), "Iniciando Mozo...");

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

    Logger::getInstance()->log("INFO", MOZO, getpid(), "Hola, soy un Mozo y voy a atender cada 2 segundos...");

    while (!sigint_handler.getGracefulQuit()){
        Logger::getInstance()->log("INFO", MOZO, getpid(), "atendiendo...");
        sleep(2);

        // Recibir pedidos cocinados por cocinero
        // Si hay, entregar a comensales correctos
        // TODO: Cómo chequear cola sin bloquear? Semáforos? Locks?

        try {
            Pedido pedido(10);// Pedido de grupo de comensales TODO HARDCODE
            enviarPedidoACocinero( fifoACocinar, pedido );

        } catch (std::invalid_argument ex) {
            Logger::getInstance()->log("ERR", MOZO, getpid(), "Pasé un argumento inválido a creación de pedido");
        }
    }

    fifoACocinar.cerrar();
    fifoACocinar.eliminar();

    fifoCocinado.cerrar();
    fifoCocinado.eliminar();

    SignalHandler::getInstance()->destruir();
    Logger::getInstance()->log("INFO", MOZO, getpid(), "Cerrando Mozo...");
    return 0;
}

void ProcesoMozo::enviarPedidoACocinero(FifoEscritura fifo, Pedido pedido) {
    std::string mensaje = pedido.serializar();
    //loggear("mensaje: " + mensaje);
    fifo.escribir(static_cast<const void*>(mensaje.c_str()),mensaje.length() );
}

ProcesoMozo::~ProcesoMozo() {

}
