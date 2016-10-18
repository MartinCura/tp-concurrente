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

    FifoLectura fifoNuevosPedidos(ARCHIVO_FIFO_NUEVOS_PEDIDOS);
    fifoNuevosPedidos.abrir();

    std::vector<FifoLectura> vectorFifos;
    vectorFifos.push_back(fifoCocinado);
    vectorFifos.push_back(fifoNuevosPedidos);

    //recibirNuevoPedido(fifoACocinar);//
    Restaurante::agregarGanancia(100);// TEST
    Restaurante::agregarPerdida(7);// TEST

    while (!sigint_handler.getGracefulQuit()){
        sleep(2);

        AccionMozo accion = esperarAccion(vectorFifos);
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
    AccionMozo acciones[2] = { ENTREGAR_PEDIDO, TOMAR_PEDIDO }; // horrible
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
        char buffer[TAM_PEDIDO+1];
        ssize_t bytesLeidos = fifoNuevosPedidos.leer( static_cast<void*>(buffer),TAM_PEDIDO );
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
    char buffer[TAM_PEDIDO+1];

    // NO bloquea si todavía no hay pedidos para entregar
    ssize_t bytesLeidos = fifo.leer( static_cast<void*>(buffer),TAM_PEDIDO );

    if (bytesLeidos > 0) {
        std::string mensajeDePedido = buffer;
        mensajeDePedido.resize( (unsigned long) bytesLeidos );

        Pedido pedidoAEntregar = Pedido::deserializar(mensajeDePedido);

        entregarPedido(pedidoAEntregar);
        Logger::log("INFO", MOZO, getpid(), "Entregó pedido.");
    }
}

void ProcesoMozo::entregarPedido(Pedido pedido) {
    //int numMesaPedido = pedido.getNumMesa();
    //pid_t pid = pedido.getPid();
    //pedido.serializar();
    // TODO
}

ProcesoMozo::~ProcesoMozo() {

}
