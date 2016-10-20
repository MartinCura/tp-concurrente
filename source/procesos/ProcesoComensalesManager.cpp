/*
 * ProcesoComensalesManager.h
 *
 *  Created on: Oct 17, 2016
 *      Author: emanuel
 */

#include "../../include/procesos/ProcesoComensalesManager.h"

ProcesoComensalesManager::ProcesoComensalesManager() : Proceso() {

}

int ProcesoComensalesManager::ejecutarMiTarea() {
    Logger::log("INFO", PCM_, getpid(), "Proceso Comensales Manager iniciando...");

    FifoLectura fifoNuevosComensalesEnMesa(ARCHIVO_FIFO_NUEVOS_COMENSALES_EN_MESA);
    fifoNuevosComensalesEnMesa.abrir();

    SIGINT_Handler sigint_handler;
    SIGTERM_Handler sigterm_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
    SignalHandler::getInstance()->registrarHandler(SIGTERM, &sigterm_handler);

    kill(getpid(), SIGTERM);

    char buffer[BUFFSIZE] = "";
    while (!sigint_handler.getGracefulQuit()){
        if (sigterm_handler.isAvailible()) {
            /* Lee el id de mesa que fue ocupada y la cantidad de ocupantes)*/
            /* TODO podríamos leer la cantidad de personas por grupo. */
            ssize_t bytesLeidos = fifoNuevosComensalesEnMesa.leer( static_cast<void*>(buffer), TAM_MAX_MSJ_RECP_PCM);
            if (bytesLeidos > 0) {
                std::string msg = buffer;
                msg.resize((unsigned long) bytesLeidos);
                std::vector<std::string> param = Utils::split(msg, "-");
                if (param.size() != 2) {// param[0]:id_Mesa; param[1]:count
                    Logger::log("ERR ", PCM_, getpid(), "Lectura de datos erronea.");
                    continue;
                }
                int id_Mesa = atoi(param[0].c_str());
                int count = atoi(param[1].c_str());

                ProcesoComensales* proc = new ProcesoComensales(id_Mesa, count);
                comensales.push_back(proc);
                try {
                    proc->start();
                } catch (ProcesoTerminadoException &p) {
                    SignalHandler::destruir();
                    fifoNuevosComensalesEnMesa.cerrar();
                    /* Se manda un throw desde acá para que lo atrape Restaurante */
                    throw ProcesoTerminadoException(p.pid);
                }
            }
        } else {
            reset();
        }
    }
    SignalHandler::destruir();

    reset();

    fifoNuevosComensalesEnMesa.cerrar();
    fifoNuevosComensalesEnMesa.eliminar();
    Logger::log("INFO", PCM_, getpid(), "Proceso Comensales Manager finalizado.");
    return 0;
}

void ProcesoComensalesManager::reset() {
    for (unsigned i = 0; i < comensales.size(); i++)
        //if (!comensales[i]->finished())
            comensales[i]->interrupt_();

    for (unsigned i = 0; i < comensales.size(); i++) {
        //if (!comensales[i]->finished())
            comensales[i]->wait_();
        delete comensales[i];
    }
    comensales.clear();
}

ProcesoComensalesManager::~ProcesoComensalesManager() {
    for (unsigned i = 0; i < comensales.size(); i++)
        delete comensales[i];
}
