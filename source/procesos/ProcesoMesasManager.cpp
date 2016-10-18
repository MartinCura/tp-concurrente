//
// Created by martin on 16/10/16.
//

#include <utils/MemoriaCompartida.h>
#include <modelo/ObjetosDeRestaurante.h>
#include <modelo/Restaurante.h>

ProcesoMesasManager::ProcesoMesasManager(int cantMesas) : Proceso() {
    this->cantMesas = cantMesas;
    for (unsigned i = 0; i < cantMesas; ++i) {
        struct Mesa m(i);
        vMesas.push_back(m);
    }
}

void ProcesoMesasManager::lanzarMesasDisponiblesIniciales(FifoEscritura fifo) {
    for (unsigned i = 0; i < cantMesas; i++) {
        std::string strId = serializarIdMesa(i);
        fifo.escribir(static_cast<const void*>( strId.c_str()),TAM_NUM_MESA );
    }
    Logger::log("INFO", PMM_, getpid(), "Todas las mesas fueron inicializadas...");
}

int ProcesoMesasManager::ejecutarMiTarea() {
    Logger::log("INFO", PMM_, getpid(), "Iniciando Proceso Mesas Manager...");
    FifoEscritura fifoMesasLibres( ARCHIVO_FIFO_MESAS_LIBRES );
    fifoMesasLibres.abrir();

    // Abro fifo de lectura para liberar una mesa de forma no bloqueante
    FifoLectura fifoRetiradas( ARCHIVO_FIFO_LIBERAR_MESA );
    fifoRetiradas.abrir(false);

    lanzarMesasDisponiblesIniciales(fifoMesasLibres);

    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    char buffer[TAM_NUM_MESA+1];
    //ssize_t bytesLeidos = 0;

    while (!sigint_handler.getGracefulQuit()) {

        // Si encuentra una mesa que liberar, manda a caja lo gastado en ella y la libera
        if ((/*bytesLeidos = */fifoRetiradas.leer( static_cast<void*>(buffer),TAM_NUM_MESA )) > 0 ) {
            std::ostringstream oss;//
            oss << "PMM leí en fifoRetiradas: `" << buffer << "`" << std::endl;//
            Logger::log("DEBG", PMM_, getpid(), oss.str());//

            std::stringstream ss(buffer);
            int numMesa = -1;
            ss >> numMesa;
            if (numMesa >= 0) {
                Restaurante::agregarGanancia(vMesas[numMesa].gastado);
                vMesas[numMesa].gastado = 0;
                vMesas[numMesa].ocupada = false;
                std::string msj = serializarIdMesa(numMesa);
                fifoMesasLibres.escribir(static_cast<const void*>(msj.c_str()),TAM_NUM_MESA );
            }
        }

    }
    SignalHandler::destruir();
    Logger::log("INFO", PMM_, getpid(), "Proceso Mesas Manager finalizado.");
    fifoMesasLibres.cerrar();
    fifoMesasLibres.eliminar();
    return 0;
}

void ProcesoMesasManager::reset() {
    for (unsigned i = 0; i < this->cantMesas; ++i) {
        if (vMesas[i].gastado > 0)
            Restaurante::agregarPerdida(vMesas[i].gastado);
        vMesas[i].reset();
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

ProcesoMesasManager::~ProcesoMesasManager() {
}
