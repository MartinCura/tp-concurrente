/*
 * ProcesoCocinero.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#include "../../include/procesos/ProcesoCocinero.h"

ProcesoCocinero::ProcesoCocinero() : Proceso() {
}

int ProcesoCocinero::ejecutarMiTarea() {
    Logger::log("INFO", CHEF, getpid(), "Cocinero esperando pedidos.");

    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    // Abro fifo de lectura para pedidos al cocinero
    // Se bloquea si todavía no ha habido mozos
    FifoLectura fifoACocinar ( ARCHIVO_FIFO_COCINAR );
    fifoACocinar.abrir();

    // Abro fifo de escritrua para pedidos ya cocinados
    // Se bloquea si todavía no ha habido mozos
    FifoEscritura fifoCocinado ( ARCHIVO_FIFO_COCINADO );
    fifoCocinado.abrir();

    char buffer[TAM_PEDIDO + 1] = "";

    while (!sigint_handler.getGracefulQuit()) {
        sleep(3);

        // Bloquea si todavía no hay más pedidos para cocinar
        ssize_t bytesLeidos = fifoACocinar.leer( static_cast<void*>(buffer),TAM_PEDIDO );
        if (bytesLeidos > 0) {
            Logger::log("INFO", CHEF, getpid(), "Recibiendo un nuevo pedido.");
            std::string mensaje = buffer;
            mensaje.resize((unsigned long) bytesLeidos);
            Pedido pedido = Pedido::deserializar(mensaje);
            cocinar(pedido);
            enviarPedidoAMozos(fifoCocinado, pedido);
        }
    }

    fifoACocinar.cerrar();
    fifoACocinar.eliminar();

    fifoCocinado.cerrar();
    fifoCocinado.eliminar();

    Logger::log("INFO", CHEF, getpid(), "Proceso Cocinero finalizado.");

    SignalHandler::destruir();

    return 0;
}

void ProcesoCocinero::enviarPedidoAMozos(FifoEscritura fifo, Pedido pedido) {
    std::string mensaje = pedido.serializar();
    fifo.escribir( static_cast<const void*>(mensaje.c_str()),mensaje.length() );
    Logger::log("INFO", CHEF, getpid(), "Envío a la cola cocinado.");
}

void ProcesoCocinero::cocinar(Pedido pedido) {
    int cantDePlatos = pedido.cantPlatos();
    unsigned int tiempoPorPlato = 1;// hardcodeo
    unsigned int tiempoCocinando = 0 + cantDePlatos * tiempoPorPlato;
    Logger::log("INFO", CHEF, getpid(), "Cocinando pedido..." );
    sleep( tiempoCocinando );
}

ProcesoCocinero::~ProcesoCocinero() {

}
