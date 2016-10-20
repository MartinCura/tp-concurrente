/*
 * ProcesoComensales.cpp
 *
 *  Created on: Oct 14, 2016
 *      Author: emanuel
 */

#include "../../include/procesos/ProcesoComensales.h"

ProcesoComensales::ProcesoComensales(Semaforo &sem, int id_Mesa, int count) : Proceso() {
    this->id_mesa = id_Mesa;
    this->count = count;
    semaforoComer = sem;
    shmPedidos = MemoriaCompartida<struct MesasConPedidos>( ARCHIVO_SHM_MESAS,'A' );
}

int ProcesoComensales::ejecutarMiTarea() {
    Logger::log("INFO", COMN, getpid(), "Iniciando proceso de comensales...");

    SIGINT_Handler sigint_handler;
    Semaforo_Handler semaforo_handler(semaforoComer, id_mesa);
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
    SignalHandler::getInstance()->registrarHandler(SIGTERM, &semaforo_handler);

    FifoEscritura fifoNuevosPedidos( ARCHIVO_FIFO_NUEVOS_PEDIDOS );
    fifoNuevosPedidos.abrir();

    // Abro fifo de escritura para liberar una mesa
    // Se bloquea si todavía no lo abrió el ProcesoMesasManager
    FifoEscritura fifoRetirarse( ARCHIVO_FIFO_LIBERAR_MESA );
    fifoRetirarse.abrir();

    Logger::log("INFO", COMN, getpid(), "Sentándonos en la mesa " + std::to_string(id_mesa) + ".");
    // Costo temporal de sentarse
    sleep(3);

    while (!sigint_handler.getGracefulQuit()) { // está bien esto??
        Pedido pedidoAPedir = Pedido::crearRandom(getpid(), (unsigned) this->id_mesa);

        std::string mensaje = pedidoAPedir.serializar();
        Logger::log("INFO", COMN, getpid(),
                    "Esperando un mozo para pedir " + std::to_string(pedidoAPedir.cantPlatos())+ " platos"
                    + " (mesa " + std::to_string(id_mesa) +").");
        fifoNuevosPedidos.escribir( static_cast<const void*>(mensaje.c_str()),TAM_PEDIDO );

        try {
            semaforo_handler.executeNext(); /* Se bloquea hasta que le llegue aviso de que su pedido está listo */
            Pedido pedidoRecibido = recibirPedido();

            if (sigint_handler.getGracefulQuit()) {
                Logger::log("INFO", COMN, getpid(), "EHHH...se cortó la luz. Mesa " + std::to_string(id_mesa) + ". Nos vamos sin pagar...");
                break;
            }

            if (pedidoRecibido.getPid() != getpid())
                throw NoHayPedidoException();

            Logger::log("INFO", COMN, getpid(),
                        "Llegó nuestro pedido (mesa " + std::to_string(id_mesa) + "). Comiendo...");
            comer(pedidoRecibido);

        } catch (NoHayPedidoException ex) {
            Logger::log("INFO", COMN, getpid(), "Nos prometieron comida y fueron todas mentiras...");
            Logger::log("INFO", COMN, getpid(), "Quememos el lugar así esto no vuelve a pasar!!");
            Logger::log("INFO", COMN, getpid(), "Aunque, para ser honestos...");
        }

        if (Utils::generarRandom(10) % 2 == 0) {
            Logger::log("INFO", COMN, getpid(),
                        "Pagamos la cuenta y nos vamos (mesa " + std::to_string(id_mesa) + ").");
            // Escribe su id de mesa al fifo correspondiente para que PMM la libere
            std::string msj = ProcesoMesasManager::serializarIdMesa(this->id_mesa);
            fifoRetirarse.escribir(static_cast<const void *>(msj.c_str()), TAM_NUM_MESA);
            break;
        }

        Logger::log("INFO", COMN, getpid(),
                    "Seguimos con hambre, pedimos de nuevo (mesa " + std::to_string(id_mesa) + ").");
    }

    SignalHandler::destruir();

    fifoNuevosPedidos.cerrar();
    fifoRetirarse.cerrar();
    //fifoNuevosPedidos.eliminar(); Dejar comentado...porque deja procesos sueltos

    Logger::log("INFO", COMN, getpid(),
                "Comensales de la mesa " + std::to_string(id_mesa) + " retirándose del restaurante.");
    return 0;
}

Pedido ProcesoComensales::recibirPedido() {
    struct MesasConPedidos mcp = shmPedidos.leer();
    return mcp.get(this->id_mesa);
}

void ProcesoComensales::comer(Pedido pedido) {
    unsigned tiempoPorPlato = 1;
    sleep(pedido.cantPlatos() * tiempoPorPlato + 1);
}

ProcesoComensales::~ProcesoComensales() {

}
