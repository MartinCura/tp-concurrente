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
        loggear("Hola, soy un Mozo y voy a atender cada 2 segundos...");
        sleep(2);

        // Recibir pedidos cocinados por cocinero
            // Si hay, entregar a comensales correctos
        // TODO: Cómo chequear cola sin bloquear? Semáforos? Locks?

        Pedido pedido;// Pedido de grupo de comensales
        enviarPedidoACocinero( fifoACocinar, pedido );
    }

    // TODO: Verificar que se llega acá
    fifoACocinar.cerrar ();
    //fifoACocinar.eliminar ();

    SignalHandler::getInstance()->destruir();

    return 0;
}

void ProcesoMozo::enviarPedidoACocinero(FifoEscritura fifo, Pedido pedido) {
    std::string mensaje = pedido.serializar();
    // El chiste es que el mensaje siempre debería tener el mismo largo, no? TODO: Chequear
    fifo.escribir(static_cast<const void*>(mensaje.c_str()),mensaje.length() );
    loggear("Envío a la cola cocinar: " + mensaje);//
}

void ProcesoMozo::loggear(std::string mensaje) {
    std::cout << "[" << getpid() << " Mozo] " << mensaje << std::endl;
}

ProcesoMozo::~ProcesoMozo() {

}
