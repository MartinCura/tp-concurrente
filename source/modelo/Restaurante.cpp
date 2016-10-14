//
// Created by martin on 27/09/16.
//

#include "../../include/modelo/Restaurante.h"

Restaurante::Restaurante() {
    running = true;
    hay_luz = true;
    cantRecepcionistas = 1;
    cantMozos = 2;
    recepcionistas.clear();
    mozos.clear();

    generadorComensales = new ProcesoComensales();

    /* Creamos los procesos para los recepcionistas */
    for (unsigned i = 0; i < cantRecepcionistas; i++)
        recepcionistas.push_back(new ProcesoRecepcionista());

    /* Creamos los procesos para los mozos */
    for (unsigned i = 0; i < cantMozos; i++)
        mozos.push_back(new ProcesoMozo());

    /* Creamos el proceso para el cocinero */
    cocinero = new ProcesoCocinero();
}

void Restaurante::inicializarRecursos() {
    /* TODO capaz está al pedo (si vamos a usar semáforos, debería servir) */
}

void Restaurante::eliminarRecursos() {
    /* TODO capaz está al pedo (si vamos a usar semáforos, debería servir) */
}

void Restaurante::lanzarProcesos() {
    generadorComensales->start();

    for (unsigned i = 0; i < recepcionistas.size(); i++)
        recepcionistas[i]->start();

    for (unsigned i = 0; i < mozos.size(); i++)
        mozos[i]->start();

    cocinero->start();
}

void Restaurante::terminarProcesos() {
    if (generadorComensales->isStopped())
        generadorComensales->continue_();
    generadorComensales->interrupt_();
    generadorComensales->wait_();

    for (unsigned i = 0; i < recepcionistas.size(); i++) {
        Proceso* proc = recepcionistas[i];
        if (proc->isStopped())
            proc->continue_();
        proc->interrupt_();
        proc->wait_();  // TODO podría no hacerlo acá y hacerlo en un sólo for???
    }

    for (unsigned i = 0; i < mozos.size(); i++) {
        Proceso* proc = mozos[i];
        if (proc->isStopped())
            proc->continue_();
        proc->interrupt_();
        proc->wait_();  // TODO podría no hacerlo acá???
    }

    if (cocinero->isStopped())
        cocinero->continue_();
    cocinero->interrupt_();
    cocinero->wait_();
}

bool Restaurante::inicializado() {
    return true;    // útil TODO ver si se carga bien el archivo de configuración
}

void Restaurante::run() {
    try {
        std::cout << "["<< getpid() <<" Rest]...iniciando Restorrente..." << std::endl;
        std::cout << "["<< getpid() <<" Rest] Esperando a que arranquen el Recepcionista y el Mozo..." << std::endl;

        lanzarProcesos();

        std::string input;

        /* Manejamos los cortes y vueltas de luz y cierre del restaurante desde std::cin */
        while (running) {
            std::cin >> input;
            procesarInput(input);
        }

        sleep(1);

        terminarProcesos();

        std::cout << "["<< getpid() <<" Rest] ... VAMO A IRNO ..." << std::endl;
        std::cout << "["<< getpid() <<" Rest] ...cerrando Restorrente..." << std::endl; // Nombre hardcodeado ¬¬

    } catch (ProcesoTerminadoException &p) {
        std::cout << "["<< p.pid <<"] Terminado." << std::endl;
    }
}

void Restaurante::procesarInput(std::string input) {
    if (input == QUIT)
        running = false;
    else if (input == CORTAR_LUZ)
        procesarCorteDeLuz();
    else if (input == DEVOLVER_LUZ)
        procesarVueltaDeLuz();
    else if (input == CONSULTAR_CAJA)
        consultarCaja();
}

void Restaurante::procesarCorteDeLuz() {
    if (hay_luz) {
        /* TODO hay que "vaciar" todo (reiniciar) y parar los procesos (SIGSTOP???) hasta que vuelva la luz */

        /* Detenemos los procesos */
        generadorComensales->stop_();

        for (unsigned i = 0; i < recepcionistas.size(); i++)
            recepcionistas[i]->stop_();

        for (unsigned i = 0; i < mozos.size(); i++)
            mozos[i]->stop_();

        cocinero->stop_();

        hay_luz = false;
    }
}

void Restaurante::procesarVueltaDeLuz() {
    if (!hay_luz) {
        /* TODO hay que reanudar los procesos pero en 0 (fifos vacíos y otras yerbas, etc) (SIGCONT???) */

        /* Reanudamos los procesos */
        generadorComensales->continue_();
        
        for (unsigned i = 0; i < recepcionistas.size(); i++)
            recepcionistas[i]->continue_();

        for (unsigned i = 0; i < mozos.size(); i++)
            mozos[i]->continue_();

        cocinero->continue_();

        hay_luz = true;
    }
}

void Restaurante::consultarCaja() {
    std::cout << "...consultando caja..." << std::endl;
}

void Restaurante::reset() {
    // Registrar comida entregada pero no pagada como pérdida
    // Cocinero, mozos, recepcionista olvidan lo que estaban haciendo (se resetean)
}

Restaurante::~Restaurante() {
    if (generadorComensales != 0)
        delete generadorComensales;

    for (unsigned i = 0; i < recepcionistas.size(); i++)
        delete recepcionistas[i];

    for (unsigned i = 0; i < mozos.size(); i++)
        delete mozos[i];

    if (cocinero != 0)
        delete cocinero;
}
