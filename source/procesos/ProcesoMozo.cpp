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

    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    loggear("Hola, soy un Mozo y estoy atendiendo");
    while (!sigint_handler.getGracefulQuit()){
        loggear("Mozo disponible.");

        //Miro si tengo nuevos pedidos solicitados por los comensales
        tomarPedidosComensales();
        loggear("Termine de procesar pedidos de comensales.");

        //Miro si hay comensales pidiendo la cuenta.
        cobrarAComensales();
        loggear("Termine de cobrar a comensales.");

        //Miro si el cocinero tiene pedidos listos para entregar.
        entregarPedidosAComensales();

        sleep(2);
    }
    SignalHandler::getInstance()->destruir();
    return 0;
}

/**
 * Metodo que revisa la fifo de pedidos nuevos de comensales.
 *
 */
void ProcesoMozo::tomarPedidosComensales() {
    char buffer[BUFFSIZE];

    //Abro FIFO de Lectura para tomar los pedidos nuevos de comensales.
    FifoLectura fifoNuevosPedidos(ARCHIVO_FIFO_NUEVOS_PEDIDOS_COMENSALES);
    fifoNuevosPedidos.abrir();

    // TODO: Ver si el BUFFSIZE es correcto.
    Pedido pedido;
    ssize_t bytesLeidos = fifoNuevosPedidos.leer(static_cast<void *>(buffer), BUFFSIZE);

    while (bytesLeidos>0) {
        loggear("Hay pedidos por procesar!!");

        std::string mensaje = buffer;
        mensaje.resize(bytesLeidos);
        loggear("Recibí pedido: " + mensaje);
        pedido = Pedido::deserializar(mensaje);

        enviarPedidoACocinero(pedido);

        bytesLeidos = fifoNuevosPedidos.leer(static_cast<void *>(buffer), BUFFSIZE);
    }

    fifoNuevosPedidos.cerrar();
}


/**
 * Metodo que revisa la fifo de pedidos que ya estan listos para entregar al comensal.
 *
 */
void ProcesoMozo::entregarPedidosAComensales() {
    char buffer[BUFFSIZE];

    // Abro fifo de escritura para pedidos ya cocinados y para entregar
    // Se bloquea hasta que aparezca el cocinero
    FifoLectura fifoPedidosParaEntregar ( ARCHIVO_FIFO_COCINADO );
    fifoPedidosParaEntregar.abrir();

    Pedido pedido;
    ssize_t bytesLeidos = fifoPedidosParaEntregar.leer(static_cast<void *>(buffer), BUFFSIZE);

    while (bytesLeidos>0) {
        loggear("Tomando Pedidos ya cocinados para entregar a comensal.");

        std::string mensaje = buffer;
        mensaje.resize(bytesLeidos);
        loggear("Pedido " + mensaje);
        pedido = Pedido::deserializar(mensaje);

        //TODO: Ver como hacer para entregar pedidos.
        //TODO: Una vez entregado el pedido al comensal se computa a la cuenta de este.

        bytesLeidos = fifoPedidosParaEntregar.leer(static_cast<void *>(buffer), BUFFSIZE);
    }

    fifoPedidosParaEntregar.cerrar();
}

/**
 * Metodo que revisa la fifo de Comensales que pidieron la cuenta
 *
 */
void ProcesoMozo::cobrarAComensales() {
    char buffer[BUFFSIZE];

    //Abro FIFO de lectura para tomar los comensales que pidieron la cuenta.
    FifoLectura fifoPedidosCobrosComensales ( ARCHIVO_FIFO_NUEVOS_PEDIDOS_COBROS_COMENSALES );
    fifoPedidosCobrosComensales.abrir();

    Pedido pedido;
    ssize_t bytesLeidos = fifoPedidosCobrosComensales.leer(static_cast<void *>(buffer), BUFFSIZE);

    while (bytesLeidos>0) {
        loggear("Leyendo las mesas de comensales que pidieron la cuenta");

        std::string mensaje = buffer;
        mensaje.resize(bytesLeidos);
        pedido = Pedido::deserializar(mensaje);
        loggear("Mesa " + pedido.num_mesa);


        //Restaurante::agregarGanancia(100);
        //TODO: Sumar el gasto total de la mesa y depositar ese monto en CAJA.

        bytesLeidos = fifoPedidosCobrosComensales.leer(static_cast<void *>(buffer), BUFFSIZE);
    }

    fifoPedidosCobrosComensales.cerrar();
}

/**
 * Proceso que agrega a la FIFO del cocinero un nuevo pedido.
 * @param pedido
 */
void ProcesoMozo::enviarPedidoACocinero(Pedido pedido) {

    // Abro fifo de escritura para pedidos al cocinero
    // Se bloquea hasta que aparezca el cocinero
    FifoEscritura fifoACocinar ( ARCHIVO_FIFO_COCINAR );
    fifoACocinar.abrir();

    std::string mensaje = pedido.serializar();
    fifoACocinar.escribir(static_cast<const void*>(mensaje.c_str()),mensaje.length() );
    loggear("Envío pedido a cocinero" + mensaje);//
}

void ProcesoMozo::loggear(std::string mensaje) {
    std::cout << "[" << getpid() << " Mozo] " << mensaje << std::endl;
}



ProcesoMozo::~ProcesoMozo() {

}
