/*
 * ProcesoComensales.cpp
 *
 *  Created on: Oct 14, 2016
 *      Author: emanuel
 */

#include "../../include/procesos/ProcesoComensales.h"

ProcesoComensales::ProcesoComensales(int id_Mesa, int count) : Proceso() {
    this->id_mesa = id_Mesa;
    this->count = count;
}

int ProcesoComensales::ejecutarMiTarea() {
    Logger::log("INFO", COMN, getpid(), "Iniciando proceso de comensales...");

    SIGINT_Handler sigint_handler;
    SIGTERM_Handler sigterm_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
    SignalHandler::getInstance()->registrarHandler(SIGTERM, &sigterm_handler);

    FifoEscritura fifoNuevosPedidos(ARCHIVO_FIFO_NUEVOS_PEDIDOS);
    fifoNuevosPedidos.abrir();

    while (!sigint_handler.getGracefulQuit()) { // está bien esto??
        Pedido pedido = Pedido::crearRandom(getpid(), this->id_mesa);

        std::string mensaje = pedido.serializar();
        fifoNuevosPedidos.escribir( static_cast<const void*>(mensaje.c_str()),TAM_PEDIDO );
        Logger::log("INFO", COMN, getpid(), "Esperando que un mozo tome nuestro pedido.");

        //sigterm_handler.executeNext(); /* Se bloquea hasta que le llegue su pedido */

        Logger::log("INFO", COMN, getpid(), "Llegó nuestro pedido. Comiendo...");
        comer(pedido);

        if (Utils::generarRandom(2) % 2 == 0) {
            Logger::log("DEBG", COMN, getpid(), "Mesa " + std::to_string(id_mesa) + " sigue con hambre, pedimos de nuevo.");

        } else {
            std::ostringstream oss;
            oss << "ME QUIERO IR mesa: " << this->id_mesa << " getpid: " << getpid() << std::endl;//
            Logger::log("DEBG", COMN, getpid(), oss.str());
            // Abro fifo de escritura para liberar una mesa
            // Se bloquea si todavía no lo abrió el ProcesoMesasManager
            FifoEscritura fifoRetirarse( ARCHIVO_FIFO_LIBERAR_MESA );
            fifoRetirarse.abrir();

            // Escribe su id de mesa al fifo correspondiente para que PMM la libere
            std::string msj = ProcesoMesasManager::serializarIdMesa(this->id_mesa);
            fifoRetirarse.escribir( static_cast<const void*>(msj.c_str()),TAM_NUM_MESA );
            fifoRetirarse.cerrar();
            break;
        }
    }
    SignalHandler::destruir();
    sigterm_handler.eliminarSemaforo();
    fifoNuevosPedidos.cerrar();
    //fifoNuevosPedidos.eliminar(); Dejar comentado...porque deja procesos sueltos
    Logger::log("INFO", COMN, getpid(), "Comensales retirándose del restaurante.");
    return 0;
}

void ProcesoComensales::comer(Pedido pedido) {
    unsigned tiempoPorPlato = 1;
    sleep(pedido.cantPlatos() * tiempoPorPlato);
}

ProcesoComensales::~ProcesoComensales() {

}
