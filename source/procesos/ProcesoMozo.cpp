/*
 * ProcesoMozo.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#include "../../include/procesos/ProcesoMozo.h"
#include "../../include/modelo/Restaurante.h"

ProcesoMozo::ProcesoMozo() : Proceso() {
}

int ProcesoMozo::ejecutarMiTarea() {
    Logger::log("INFO", MOZO, getpid(), "Mozo esperando a atender...");

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

    recibirNuevoPedido(fifoACocinar);//
    Restaurante::agregarGanancia(100);// TEST
    Restaurante::agregarPerdida(7);// TEST

    while (!sigint_handler.getGracefulQuit()){
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
    Logger::log("INFO", MOZO, getpid(), "Proceso Mozo finalizado.");
    return 0;
}

void ProcesoMozo::recibirNuevoPedido(FifoEscritura fifo) {
    try {
        Pedido pedido(10);// Pedido de grupo de comensales TODO HARDCODE
        Logger::log("INFO", MOZO, getpid(), "Mozo tomando pedido.");
        enviarPedidoACocinero( fifo, pedido );
    } catch (std::invalid_argument ex) {
        Logger::log("ERR", MOZO, getpid(), "Pasé un argumento inválido a creación de pedido");
    }
}

void ProcesoMozo::enviarPedidoACocinero(FifoEscritura fifo, Pedido pedido) {
    std::string mensaje = pedido.serializar();
    fifo.escribir( static_cast<const void*>(mensaje.c_str()),mensaje.length() );
    Logger::log("INFO", MOZO, getpid(), "Mozo entregando pedido a Cocinero.");
}

void ProcesoMozo::recibirPedidosListos(FifoLectura fifo) {
    char buffer[BUFFSIZE];

    // NO bloquea si todavía no hay pedidos para entregar
    ssize_t bytesLeidos = fifo.leer( static_cast<void*>(buffer),Pedido::TAM_MENSAJE );

    if (bytesLeidos > 0) {
        std::string mensajeDePedido = buffer;
        mensajeDePedido.resize( (unsigned long) bytesLeidos );

        Pedido pedidoAEntregar = Pedido::deserializar(mensajeDePedido);

        entregarPedido(pedidoAEntregar);
        Logger::log("INFO", MOZO, getpid(), "Entregó pedido.");
    }
}

void ProcesoMozo::entregarPedido(Pedido pedido) {
    int numMesaPedido = pedido.getNumMesa();

    //pedido.serializar();
    // TODO
}

ProcesoMozo::~ProcesoMozo() {

}
