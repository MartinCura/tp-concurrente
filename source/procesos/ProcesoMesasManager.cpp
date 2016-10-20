//
// Created by martin on 16/10/16.
//

#include <utils/MemoriaCompartida.h>
#include <modelo/ObjetosDeRestaurante.h>
#include <modelo/Restaurante.h>
#include <handlers/SIGUSR1_Handler.h>

ProcesoMesasManager::ProcesoMesasManager(unsigned cantMesas, Semaforo &sem) : Proceso() {
    this->cantMesas = cantMesas;
    for (unsigned i = 0; i < cantMesas; ++i) {
        struct Mesa m(i);
        vMesas.push_back(m);
    }
    semaforoMesasListas = sem;
}

void ProcesoMesasManager::lanzarMesasDisponiblesIniciales(FifoEscritura fifo) {
    for (unsigned i = 0; i < cantMesas; i++) {
        std::string strId = serializarIdMesa(i);
        fifo.escribir(static_cast<const void*>( strId.c_str()),TAM_NUM_MESA );
    }
    Logger::log("INFO", PMM_, getpid(), "Todas las mesas fueron inicializadas...");
    semaforoMesasListas.v();
}

int ProcesoMesasManager::ejecutarMiTarea() {
    Logger::log("INFO", PMM_, getpid(), "Iniciando Proceso Mesas Manager...");
    FifoEscritura fifoMesasLibres( ARCHIVO_FIFO_MESAS_LIBRES );
    fifoMesasLibres.abrir();

    // Abro fifo de lectura para liberar una mesa de forma no bloqueante
    FifoLectura fifoRetiradas( ARCHIVO_FIFO_LIBERAR_MESA );
    fifoRetiradas.abrir(false);

    // Abro fifo de lectura para tomar actualizaciones a los saldos de las mesas.
    FifoLectura fifoSaldosMesa( ARCHIVO_FIFO_SALDOS_MESA );
    fifoSaldosMesa.abrir(false);

    lanzarMesasDisponiblesIniciales(fifoMesasLibres);

    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    SIGUSR1_Handler sigusr1_handler(this);
    SignalHandler::getInstance()->registrarHandler(SIGUSR1, &sigusr1_handler);

    MemoriaCompartida<struct MesasConPedidos> shmMesas;
    try {
        shmMesas = MemoriaCompartida<struct MesasConPedidos>( ARCHIVO_SHM_MESAS,'A' );

    } catch ( std::string& mensaje ) {
        std::cerr << mensaje << std::endl;
    }

    char buffer[TAM_NUM_MESA+1] = "";
    //ssize_t bytesLeidos = 0;

    while (!sigint_handler.getGracefulQuit()) {

        // Si encuentra una mesa que liberar, manda a caja lo gastado en ella y la libera
        if ((/*bytesLeidos = */fifoRetiradas.leer( static_cast<void*>(buffer),TAM_NUM_MESA )) > 0 ) {
            std::stringstream ss(buffer);
            int numMesa = -1;
            ss >> numMesa;
            if (numMesa >= 0) {
                Logger::log("INFO", PMM_, getpid(),
                            "Libero la mesa " + std::to_string(numMesa)
                            + " (gastado " + std::to_string(vMesas[numMesa].gastado) + " $).");

                Restaurante::agregarGanancia(vMesas[numMesa].gastado);
                vMesas[numMesa].ocupada = false;
                vMesas[numMesa].gastado = 0;

                std::string msj = serializarIdMesa(numMesa);
                fifoMesasLibres.escribir( static_cast<const void*>(msj.c_str()),TAM_NUM_MESA );
            }
        }
        // Proceso costos de los pedidos.
        registrarPedidoEnUnaMesa(fifoSaldosMesa);
    }
    SignalHandler::destruir();

    fifoMesasLibres.cerrar();
    fifoMesasLibres.eliminar();

    fifoRetiradas.cerrar();
    fifoRetiradas.eliminar();

    // TODO: wait a algo?
    shmMesas.liberar();

    Logger::log("INFO", PMM_, getpid(), "Proceso Mesas Manager finalizado.");
    return 0;
}

/*
 * Método que toma los pedidos entregados de la FIFO y sumariza sus costos en la mesa correspondiente.
 */
void ProcesoMesasManager::registrarPedidoEnUnaMesa(FifoLectura fifoSaldosMesa) {
    try {
        char buffer[TAM_PEDIDO+1] = "";

        ssize_t bytesLeidos = fifoSaldosMesa.leer( static_cast<void*>(buffer),TAM_PEDIDO );
        if (bytesLeidos > 0) {
            std::string mensaje = buffer;
            mensaje.resize((unsigned long) bytesLeidos);
            Pedido pedido = Pedido::deserializar(mensaje);

            // Por ahora todos los platos valen lo mismo.
            unsigned costoPedido = pedido.cantPlatos() * COSTO_PLATO;
            vMesas[pedido.getNumMesa()].gastado += costoPedido;

            Logger::log("INFO", PMM_, getpid(),
                        "Registro pedido en mesa: " + std::to_string(pedido.getNumMesa())
                        + " como " + std::to_string(costoPedido) + " $.");
        }
    } catch (std::invalid_argument ex) {
        Logger::log("ERR", PMM_, getpid(), "Pasé un argumento inválido a creación de pedido");
    }
}

void ProcesoMesasManager::reset() {
    Logger::log("INFO", PMM_, getpid(), "Todas las mesas se liberan.");
    unsigned sumaPerdido = 0;
    for (unsigned i = 0; i < this->cantMesas; ++i) {
        if (vMesas[i].gastado > 0)
            sumaPerdido += vMesas[i].gastado;
        vMesas[i].reset();
    }
    if (sumaPerdido > 0) {
        Logger::log("INFO", PMM_, getpid(), "Se registra una pérdida de " + std::to_string(sumaPerdido) + ".");
        Restaurante::agregarPerdida(sumaPerdido);
    } else {
        Logger::log("INFO", PMM_, getpid(), "No se registró pérdida.");
    }
}

std::string ProcesoMesasManager::serializarIdMesa(int id_mesa) {
    std::ostringstream oss;
    // Normaliza número a la cantidad de dígitos correcta para el serial, el primer 1 salva caso borde
    for (int z = id_mesa > 0 ? id_mesa : 1 ; z < pow(10, TAM_NUM_MESA - 1); z *= 10) {
        oss << 0;
    }
    oss << id_mesa;
    return oss.str();
}

int ProcesoMesasManager::stop__() {
     return Proceso::stop_();
}

void ProcesoMesasManager::vaciar() {
    kill(pid, SIGUSR1);
}

ProcesoMesasManager::~ProcesoMesasManager() {

}
