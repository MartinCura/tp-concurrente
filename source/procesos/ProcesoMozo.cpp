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

    // Abro fifo de lectura para nuevos pedidos de comensales
    // Se bloquea hasta que aparezca algún comensal // TODO creo
    FifoLectura fifoNuevosPedidos(ARCHIVO_FIFO_NUEVOS_PEDIDOS);
    fifoNuevosPedidos.abrir();

    // Abro fifo de escritura para pedidos al cocinero
    // Se bloquea hasta que aparezca el cocinero
    FifoEscritura fifoACocinar ( ARCHIVO_FIFO_COCINAR );
    fifoACocinar.abrir();

    // Abro fifo de escritura para pedidos ya cocinados y para entregar
    // Se bloquea hasta que aparezca el cocinero
    FifoLectura fifoCocinado ( ARCHIVO_FIFO_COCINADO );
    fifoCocinado.abrir();

    // Abro fifo de escritura para pasar pedidos al PMM
    FifoEscritura fifoPedidosMesas ( ARCHIVO_FIFO_SALDOS_MESA );
    fifoPedidosMesas.abrir();

    std::vector<FifoLectura> vectorFifos;
    vectorFifos.push_back(fifoCocinado);
    vectorFifos.push_back(fifoNuevosPedidos);

    while (!sigint_handler.getGracefulQuit()){
        sleep(2);

        AccionMozo accion = esperarAccion(vectorFifos);
        switch (accion) {
            case ENTREGAR_PEDIDO:
                recibirPedidosListos( fifoCocinado, fifoPedidosMesas );
                break;
            case TOMAR_PEDIDO:
                recibirNuevoPedido( fifoNuevosPedidos, fifoACocinar );
                break;
            default:
                ;
        }
    }

    fifoACocinar.cerrar();  // Se elimina en ProcesoCocinero
    fifoCocinado.cerrar();  // Se elimina en ProcesoCocinero
    fifoNuevosPedidos.cerrar(); // TODO: Dónde se elimina???
    fifoPedidosMesas.cerrar(); // Se elimina en PMM

    SignalHandler::destruir();
    Logger::log("INFO", MOZO, getpid(), "Proceso Mozo finalizado.");
    return 0;
}

AccionMozo ProcesoMozo::esperarAccion(std::vector<FifoLectura> fifos) {
    /* Suponer:
    *           Posición 0: fifo para los pedidos cocinados.
    *           Posición 1: fifo para los nuevos pedidos.
    */
    AccionMozo acciones[2] = { ENTREGAR_PEDIDO, TOMAR_PEDIDO };

    struct pollfd fds[fifos.size()];
    for (unsigned i = 0; i < fifos.size(); i++) {
        fds[i] = {fifos[i].getfd(), POLLIN};
    }

    nfds_t tamFds = fifos.size();

    int r = poll(fds, tamFds, -1); // Sin timeout

    if (r < 0 && errno != EINTR)
        perror( "poll()" );
    else {
        for (unsigned i = 0; i < fifos.size(); i++)
            if (fds[i].revents & POLLIN)
                return acciones[i];
    }
    return NADA;
}

void ProcesoMozo::recibirNuevoPedido(FifoLectura fifoNuevosPedidos, FifoEscritura fifoACocinar) {
    try {
        char buffer[TAM_PEDIDO+1] = "";
        ssize_t bytesLeidos = fifoNuevosPedidos.leer( static_cast<void*>(buffer),TAM_PEDIDO );
        if (bytesLeidos > 0) {
            std::string mensaje = buffer;
            mensaje.resize( (unsigned long) bytesLeidos );
            Pedido pedidoRecibido = Pedido::deserializar(mensaje);
            Logger::log("INFO", MOZO, getpid(),
                        "Pedido recibido de mesa " + std::to_string(pedidoRecibido.getNumMesa()) + ".");
            enviarPedidoACocinero( fifoACocinar, pedidoRecibido );
        }
    } catch (std::invalid_argument ex) {
        Logger::log("ERR", MOZO, getpid(), "Pasé un argumento inválido a creación de pedido");
    }
}

void ProcesoMozo::enviarPedidoACocinero(FifoEscritura fifo, Pedido pedido) {
    std::string mensaje = pedido.serializar();
    fifo.escribir( static_cast<const void*>(mensaje.c_str()),mensaje.length() );
    Logger::log("INFO", MOZO, getpid(),
                "Mozo entregando pedido de mesa " + std::to_string(pedido.getNumMesa()) + " a Cocinero.");
}

void ProcesoMozo::recibirPedidosListos(FifoLectura fifoCocinado, FifoEscritura fifoGastosMesa) {
    char buffer[TAM_PEDIDO+1] = "";

    // NO bloquea si todavía no hay pedidos para entregar
    ssize_t bytesLeidos = fifoCocinado.leer( static_cast<void*>(buffer),TAM_PEDIDO );

    if (bytesLeidos > 0) {
        std::string mensajeDePedido = buffer;
        mensajeDePedido.resize( (unsigned long) bytesLeidos );

        Pedido pedidoAEntregar = Pedido::deserializar(mensajeDePedido);

        entregarPedido(pedidoAEntregar, fifoGastosMesa);
        Logger::log("INFO", MOZO, getpid(), "Entregó pedido.");
    }
}

void ProcesoMozo::entregarPedido(Pedido pedido, FifoEscritura fifoGastosMesa) {
    contabilizarPedido(pedido, fifoGastosMesa);

    pedido.serializar();
    // TODO enviarle al comensal el pedido
}


/**
 * Método que pasa al PMM usando un FIFO para sumar el costo del pedido a la mesa.
 * @param pedido
 */
void ProcesoMozo::contabilizarPedido(Pedido pedido, FifoEscritura fifoGastosMesa) {
    std::string mensaje = pedido.serializar();
    fifoGastosMesa.escribir( static_cast<const void*>(mensaje.c_str()),mensaje.length() );
    Logger::log("INFO", MOZO, getpid(),
                "Mozo registrando pedido de mesa " + std::to_string(pedido.getNumMesa()) + " con PMM.");
}

ProcesoMozo::~ProcesoMozo() {

}
