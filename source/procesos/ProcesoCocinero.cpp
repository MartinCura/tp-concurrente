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

    if (BUFFSIZE < Pedido::TAM_MENSAJE + 1) {
        perror("BUFFSIZE muy chico");
        exit(1);
    }
    char buffer[BUFFSIZE];

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

    while (!sigint_handler.getGracefulQuit()) {
        sleep(3);

        // Bloquea si todavía no hay más pedidos para cocinar
        ssize_t bytesLeidos = fifoACocinar.leer( static_cast<void*>(buffer),Pedido::TAM_MENSAJE );

        std::string mensaje = buffer;
        mensaje.resize ( (unsigned long) bytesLeidos );
        //Logger::log("INFO", CHEF, getpid(), "Recibí pedido: " + mensaje);
        Pedido pedidoACocinar = Pedido::deserializar(mensaje);

        cocinar(pedidoACocinar);

    }

    fifoACocinar.cerrar();
    fifoACocinar.eliminar();

    fifoCocinado.cerrar();
    fifoCocinado.eliminar();

    Logger::log("INFO", CHEF, getpid(), "Proceso Cocinero finalizado.");

    // Aca si no existía, se crea una nueva y se la elimina... TODO
    SignalHandler::destruir();

    return 0;
}

void ProcesoCocinero::enviarPedidoAMozos(FifoEscritura fifo, Pedido pedido) {
    std::string mensaje = pedido.serializar();
    fifo.escribir( static_cast<const void*>(mensaje.c_str()),mensaje.length() );
    Logger::log("INFO", CHEF, getpid(), "Envío a la cola cocinado: " + mensaje);//
}

// TODO
void ProcesoCocinero::cocinar(Pedido pedido) {
    int cantDePlatos = pedido.cantPlatos();
    unsigned int tiempoPorPlato = 1;//TODO: hardcodeo
    unsigned int tiempoCocinando = 0 + cantDePlatos * tiempoPorPlato;
    Logger::log("INFO", CHEF, getpid(), "Cocinando pedido..." );
    sleep( tiempoCocinando );
}

ProcesoCocinero::~ProcesoCocinero() {

}
