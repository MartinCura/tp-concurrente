//
// Created by martin on 27/09/16.
//

#include "../../include/modelo/Restaurante.h"

Restaurante::Restaurante() {
    running = true;
    hay_luz = true;
    cantRecepcionistas = 2;
    cantMozos = 2;

    generadorComensales = new ProcesoComensales();

    /* Creamos los procesos para los recepcionistas */
    for (unsigned i = 0; i < cantRecepcionistas; i++)
        recepcionistas.push_back(new ProcesoRecepcionista());

    /* Creamos los procesos para los mozos */
    for (unsigned i = 0; i < cantMozos; i++)
        mozos.push_back(new ProcesoMozo());

    /* Creamos el proceso para el cocinero */
    cocinero = new ProcesoCocinero();

    inicializarRecursos();
}

void Restaurante::inicializarRecursos() {
    /* TODO capaz está al pedo (si vamos a usar semáforos, debería servir) */
    Semaforo sem1(FILENAME_SEM_COM_RECP, 0);
    //SemaphoreHandler::getInstance()->agregarSemaforo(SEMAFORO_COM_RECP, sem);
    semaforos.push_back(sem1);

    Semaforo sem2(FILENAME_SEM_RECP_COM, cantRecepcionistas);
    //SemaphoreHandler::getInstance()->agregarSemaforo(SEMAFORO_RECP_COM, sem);
    semaforos.push_back(sem2);

    for (unsigned i = 0; i < recepcionistas.size(); i++) {
        recepcionistas[i]->addSemaphore(SEMAFORO_COM_RECP, sem1);
        recepcionistas[i]->addSemaphore(SEMAFORO_RECP_COM, sem2);
    }
    generadorComensales->addSemaphore(SEMAFORO_COM_RECP, sem1);
    generadorComensales->addSemaphore(SEMAFORO_RECP_COM, sem2);
}

void Restaurante::eliminarRecursos() {
    /* TODO capaz está al pedo (si vamos a usar semáforos, debería servir) */
    for (unsigned i = 0; i < semaforos.size(); i++)
        semaforos[i].eliminar();

    //SemaphoreHandler::destruir();
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
        //proc->wait_();  // TODO podría no hacerlo acá y hacerlo en un sólo for???
    }

    for (unsigned i = 0; i < mozos.size(); i++) {
        Proceso* proc = mozos[i];
        if (proc->isStopped())
            proc->continue_();
        proc->interrupt_();
        //proc->wait_();  // TODO podría no hacerlo acá???
    }

    if (cocinero->isStopped())
        cocinero->continue_();
    cocinero->interrupt_();

    for (unsigned i = 0; i < recepcionistas.size(); i++)
        recepcionistas[i]->wait_();

    for (unsigned i = 0; i < mozos.size(); i++)
        mozos[i]->wait_();

    cocinero->wait_();
}

bool Restaurante::inicializado() {
    return true;    // útil TODO ver si se carga bien el archivo de configuración
}

void Restaurante::run() {
    try {
        Logger::log("INFO", REST, getpid(), "Iniciando Restorrente...");

        lanzarProcesos();

        /* Manejamos los cortes y vueltas de luz y cierre del restaurante desde std::cin */
        std::string input = "";
        while (running) {
            std::cout << "Ingrese un comando >> ";
            std::getline(std::cin, input);
            if (input != "")
                procesarInput(input);
        }

        terminarProcesos();

        eliminarRecursos();

        Logger::log("INFO", REST, getpid(), "Cerrando Restorrente...");
        //Logger::destruir();

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
    else if (input == HELP) {
        std::cout << "Comandos:\n";
        std::cout << "\tquit\t\tFinaliza Restorrente\n";
        std::cout << "\tcorte\t\tGenera un corte de suministro de energía\n";
        std::cout << "\tluz\t\tRetorna el suministro de energía\n";
        std::cout << "\tcaja\t\tConsulta el estado de la caja\n";
    } else std::cout << "Comando no encontrado. Ingrese \"help\" para obtener ayuda.\n";
}

void Restaurante::procesarCorteDeLuz() {
    if (hay_luz) {
        Logger::log("INFO", REST, getpid(), "Se generó un corte de luz");
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
        Logger::log("INFO", REST, getpid(), "Se reanudó el suministro de energía");
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
    Logger::log("INFO", RECP, getpid(), "Consulta de caja");
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

    semaforos.clear();
//    for (unsigned i = 0; i < semaforos.size(); i++)
//        delete semaforos[i];
}
