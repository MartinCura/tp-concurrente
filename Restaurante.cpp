//
// Created by martin on 27/09/16.
//

#include "Restaurante.h"

Restaurante::Restaurante() {
    cantRecepcionistas = 1;
    cantMozos = 2;
    recepcionistas.clear();
    mozos.clear();
    cocinero = 0;
}

void Restaurante::inicializarRecursos() {

}

void Restaurante::eliminarRescursos() {

}

bool Restaurante::inicializado() {
    return true;
}

void Restaurante::run() {
    std::vector<Proceso*> basurero;
    try {
        std::cout << "["<< getpid() <<" Rest]...iniciando Restorrente..." << std::endl;
        std::cout << "["<< getpid() <<" Rest] Esperando a que arranquen el Recepcionista y el Mozo..." << std::endl;

        /* Creamos los procesos para los recepcionistas */
        for (unsigned i = 0; i < cantRecepcionistas; i++) {
            Proceso* proc_recep = new ProcesoRecepcionista();
            recepcionistas.push_back(proc_recep);
            basurero.push_back(proc_recep);
            proc_recep->start();
        }

        /* Creamos los procesos para los mozos */
        for (unsigned i = 0; i < cantMozos; i++) {
            Proceso* proc_mozo = new ProcesoMozo();
            mozos.push_back(proc_mozo);
            basurero.push_back(proc_mozo);
            proc_mozo->start();
        }

        /* Creamos el proceso para el cocinero */
        cocinero = new ProcesoCocinero();
        basurero.push_back(cocinero);
        cocinero->start();

        sleep(5);
        /* Mandamos señales de interrupción (SIGINT) a los recepcionistas y
          esperamos a que terminen.
        */
        for (unsigned i = 0; i < cantRecepcionistas; i++) {
                recepcionistas[i]->sigint_();
                recepcionistas[i]->wait_();
        }

        sleep(1);
        /* Mandamos señales de interrupción (SIGINT) a los mozos y esperamos
           a que terminen.
        */
        for (unsigned i = 0; i < cantMozos; i++) {
                mozos[i]->sigint_();
                mozos[i]->wait_();
        }

        sleep(2);
        /* Mandamos una señal de interrupción (SIGINT) al proceso del cocinero */
        cocinero->sigint_();
        /* Esperamos a que termine el proceso del cocinero */
        cocinero->wait_();

        for (unsigned i = 0; i < basurero.size(); i++)
            delete basurero[i];

        std::cout << "["<< getpid() <<" Rest] ... VAMO A IRNO ..." << std::endl;
        std::cout << "["<< getpid() <<" Rest] ...cerrando Restorrente..." << std::endl;
    } catch (ProcesoTerminadoException &p) {
        std::cout << "["<< p.pid <<"] Terminado." << std::endl;
        for (unsigned i = 0; i < basurero.size(); i++)
            delete basurero[i];
    }
}

void Restaurante::reset() {

}

Restaurante::~Restaurante() {
}
