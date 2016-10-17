/*
 * ProcesoMozo.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#include <poll.h>
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
    //// Marco como non-blocking para que no retenga la atención del mozo cuando no hay cosas esperando
    //fifoCocinado.setBlocking(false);
    FifoLectura fifoNuevosPedidos(ARCHIVO_FIFO_NUEVOS_PEDIDOS);
    fifoNuevosPedidos.abrir();

    std::vector<FifoLectura> fifos;
    fifos.push_back(fifoCocinado);
    fifos.push_back(fifoNuevosPedidos);

    //recibirNuevoPedido(fifoACocinar);//
    Restaurante::agregarGanancia(100);// TEST
    Restaurante::agregarPerdida(7);// TEST

    while (!sigint_handler.getGracefulQuit()){
        sleep(2);

        AccionMozo accion = esperarAccion(fifos);
        switch (accion) {
            case ENTREGAR_PEDIDO:
                recibirPedidosListos( fifoCocinado );
                break;
            case TOMAR_PEDIDO:
                recibirNuevoPedido(fifoNuevosPedidos, fifoACocinar); //cuando no esté hardcodeado TODO
                break;
            default:
                ;
        }
    }

    fifoACocinar.cerrar();
    fifoACocinar.eliminar();

    fifoCocinado.cerrar();
    fifoCocinado.eliminar();

    fifoNuevosPedidos.cerrar();
    fifoNuevosPedidos.eliminar();

    SignalHandler::destruir();
    Logger::log("INFO", MOZO, getpid(), "Proceso Mozo finalizado.");
    return 0;
}

AccionMozo ProcesoMozo::esperarAccion(std::vector<FifoLectura> fifos) {
    /* Suponer:
    *           Posición 0: fifo para los nuevos pedidos.
    *           Posición 1: fifo para los pedidos cocinados.
    */
    AccionMozo acciones[2] = {ENTREGAR_PEDIDO, TOMAR_PEDIDO}; // horrible
    struct pollfd fds[fifos.size()];
    for (unsigned i = 0; i < fifos.size(); i++) {
        fds[i] = {fifos[i].getfd(), POLLIN};
    }

    nfds_t tamFds = fifos.size(); // Actualizar si se agrega TODO

    int r = poll(fds, tamFds, 1000);//-1); // TODO Poner un timeout para que salgo o que quede esperando?

    if (r < 0 && errno != EINTR)
        perror( "poll()" );
    else {
        for (unsigned i = 0; i < fifos.size(); i++)
            if (fds[i].revents & POLLIN)
                return acciones[i];
        //if (fds[0].revents & POLLIN)
        //    return ENTREGAR_PEDIDO;
        //if (fds[1].revents & POLLIN)
        //    return TOMAR_PEDIDO;
        // Estar listo para otros TODO
    }
    return NADA;
}

void ProcesoMozo::recibirNuevoPedido(FifoLectura fifoNuevosPedidos, FifoEscritura fifoACocinar) {
    try {
        //Pedido pedido(10);// Pedido de grupo de comensales TODO HARDCODE
        char buffer[BUFFSIZE];
        ssize_t bytesLeidos = fifoNuevosPedidos.leer( static_cast<void*>(buffer),Pedido::TAM_MENSAJE );
        if (bytesLeidos > 0) {
            std::string mensaje = buffer;
            mensaje.resize( (unsigned long) bytesLeidos );
            Pedido pedidoRecibido = Pedido::deserializar(mensaje);
            Logger::log("INFO", MOZO, getpid(), "Pedido recibido de mesa " + std::to_string(pedidoRecibido.getNumMesa()) + ".");
            enviarPedidoACocinero( fifoACocinar, pedidoRecibido );
        }
    } catch (std::invalid_argument ex) {
        Logger::log("ERR", MOZO, getpid(), "Pasé un argumento inválido a creación de pedido");
    }
}

void ProcesoMozo::enviarPedidoACocinero(FifoEscritura fifo, Pedido pedido) {
    std::string mensaje = pedido.serializar();
    fifo.escribir( static_cast<const void*>(mensaje.c_str()),mensaje.length() );
    Logger::log("INFO", MOZO, getpid(), "Mozo entregando pedido de mesa " + std::to_string(pedido.getNumMesa()) + " a Cocinero.");
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
