//
// Created by martin on 27/09/16.
//

#include "Restaurante.h"

Restaurante::Restaurante() {

}

bool Restaurante::inicializar() {
    return true;
}

void Restaurante::run() {
    try {
        std::cout << "["<< getpid() <<" Rest]...iniciando Restorrente..." << std::endl;
        std::cout << "["<< getpid() <<" Rest] Esperando a que arranquen el Recepcionista y el Mozo..." << std::endl;
        Proceso* proc_recep = new ProcesoRecepcionista();
        Proceso* proc_mozo = new ProcesoMozo();

        /* Arrancan los procesos */
        proc_recep->start();
        proc_mozo->start();

        sleep(5);
        /* Mandamos una señal de interrupción (SIGINT) el proceso del recepcionista */
        proc_recep->sigint_();

        sleep(1);
        /* Mandamos una señal de interrupción (SIGINT) el proceso del mozo */
        proc_mozo->sigint_();

        /* Esperamos a que termine el proceso del recepcionista */
        proc_recep->wait_();
        /* Esperamos a que termine el proceso del mozo */
        proc_mozo->wait_();

        delete proc_recep;
        delete proc_mozo;
        std::cout << "["<< getpid() <<" Rest] ... VAMO A IRNO ..." << std::endl;
        std::cout << "["<< getpid() <<" Rest] ...cerrando Restorrente..." << std::endl;
    } catch (ProcesoTerminadoException &p) {
        std::cout << "["<< p.pid <<"] Terminado." << std::endl;
    }
}

void Restaurante::reset() {

}

Restaurante::~Restaurante() {
}
