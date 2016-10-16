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

    if (BUFFSIZE < Pedido::TAM_MENSAJE + 1) {
        perror("BUFFSIZE muy chico");
        exit(1);
    }

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
    // Marco como non-blocking para que no retenga la atención del mozo cuando no hay cosas esperando
    fifoCocinado.setBlocking(false);

    Logger::getInstance()->log("INFO", MOZO, getpid(), "Hola, soy un Mozo y voy a atender cada 2 segundos...");

    recibirNuevoPedido(fifoACocinar);//

    while (!sigint_handler.getGracefulQuit()){
        Logger::getInstance()->log("INFO", MOZO, getpid(), "atendiendo...");
        sleep(2);

        // TODO Revisar forma de hacerlo no bloqueante
        recibirPedidosListos( fifoCocinado );

        // recibirNuevoPedido(); cuando no esté hardcodeado TODO
    }

    fifoACocinar.cerrar();
    fifoACocinar.eliminar();

    fifoCocinado.cerrar();
    fifoCocinado.eliminar();

    SignalHandler::destruir();
    Logger::getInstance()->log("INFO", MOZO, getpid(), "Cerrando Mozo...");
    return 0;
}

void ProcesoMozo::recibirNuevoPedido(FifoEscritura fifo) {
    try {
        Pedido pedido(10);// Pedido de grupo de comensales TODO HARDCODE
        enviarPedidoACocinero( fifo, pedido );

    } catch (std::invalid_argument ex) {
        Logger::getInstance()->log("ERR", MOZO, getpid(), "Pasé un argumento inválido a creación de pedido");
    }
}

void ProcesoMozo::enviarPedidoACocinero(FifoEscritura fifo, Pedido pedido) {
    std::string mensaje = pedido.serializar();
    //loggear("mensaje: " + mensaje);
    fifo.escribir( static_cast<const void*>(mensaje.c_str()),mensaje.length() );
}

void ProcesoMozo::recibirPedidosListos(FifoLectura fifo) {
    char buffer[BUFFSIZE];

    // NO bloquea si todavía no hay pedidos para entregar
    ssize_t bytesLeidos = fifo.leer( static_cast<void*>(buffer),Pedido::TAM_MENSAJE );

    if (bytesLeidos > 0) {
        std::string mensajeDePedido = buffer;
        mensajeDePedido.resize( (unsigned long) bytesLeidos );
        //Logger::getInstance()->log("INFO", MOZO, getpid(), "Entrego pedido: " + mensajeDePedido);
        Pedido pedidoAEntregar = Pedido::deserializar(mensajeDePedido);

        entregarPedido(pedidoAEntregar);
    }
}

void ProcesoMozo::entregarPedido(Pedido pedido) {
    int numMesaPedido = pedido.getNumMesa();

    //pedido.serializar();
    // TODO
}

ProcesoMozo::~ProcesoMozo() {

}
