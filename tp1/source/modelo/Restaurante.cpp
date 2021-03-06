//
// Created by martin on 27/09/16.
//

#include <procesos/ProcesoMesasManager.h>
#include "../../include/modelo/Restaurante.h"

Restaurante::Restaurante() {
    cocinero = 0;
    comensalesManager = 0;
    mesasManager = 0;
    generadorComensales = 0;

    cantRecepcionistas = 0;//
    cantMozos = 0;//
    cantMesas = 0;//

    setting_ok = false;

    cargarConfiguracion();

    if (!setting_ok)
        return;

    semaforoMesasListas = Semaforo("/bin/grep", 0);
    semaforoComensales = Semaforo("/bin/ps", 0, cantMesas);
    semaforoLiving = Semaforo("/bin/nano", 1);
    semaforoCaja = Semaforo("/bin/echo",1);

    running = true;
    hay_luz = true;

    iniciarCaja();
    iniciarLiving();

    comensalesManager = new ProcesoComensalesManager(semaforoComensales);

    mesasManager = new ProcesoMesasManager(cantMesas, semaforoMesasListas);

    generadorComensales = new ProcesoGeneradorComensales();

    /* Creamos los procesos para los recepcionistas */
    for (unsigned i = 0; i < cantRecepcionistas; i++)
        recepcionistas.push_back(new ProcesoRecepcionista(semaforoLiving));

    /* Creamos los procesos para los mozos */
    for (unsigned i = 0; i < cantMozos; i++)
        mozos.push_back(new ProcesoMozo());

    /* Creamos el proceso para el cocinero */
    cocinero = new ProcesoCocinero();
}

void Restaurante::cargarConfiguracion() {
    std::filebuf in;
    if (!in.open("conf.json", std::ios::in)) {
        std::cout << "ERROR: No existe archivo de configuración conf.json" << std::endl;
        return;
    }

    std::istream iss(&in);

    std::istreambuf_iterator<char> eos;
    std::string s(std::istreambuf_iterator<char>(iss), eos);
    std::string err;

    json11::Json _json = json11::Json::parse(s, err);

    if (!err.empty()) {
        std::cout << "ERROR: conf.json (format)" << std::endl;
        in.close();
        return;
    }

    if (_json["recepcionistas"].is_null()) return;
		cantRecepcionistas = _json["recepcionistas"].int_value();

    if (_json["mozos"].is_null()) return;
		cantMozos = _json["mozos"].int_value();

    if (_json["mesas"].is_null()) return;
		cantMesas = _json["mesas"].int_value();

    if (cantMesas > MAX_MESAS)
        cantMesas = MAX_MESAS;

    /* TODO Menu, precios, tiempos de cocción ??? */

    setting_ok = true;
    in.close();
}

void Restaurante::lanzarProcesos() {
    mesasManager->start();

    generadorComensales->start();

    for (unsigned i = 0; i < recepcionistas.size(); i++)
        recepcionistas[i]->start();

    for (unsigned i = 0; i < mozos.size(); i++)
        mozos[i]->start();

    cocinero->start();

    semaforoMesasListas.p();
    semaforoMesasListas.eliminar();

    comensalesManager->start();
}

void Restaurante::terminarProcesos() {
    if (generadorComensales->isStopped())
        generadorComensales->continue_();
    generadorComensales->interrupt_();
    generadorComensales->wait_();

    if (cocinero->isStopped())
        cocinero->continue_();
    cocinero->interrupt_();
    cocinero->wait_();

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


    for (unsigned i = 0; i < recepcionistas.size(); i++)
        recepcionistas[i]->wait_();

    for (unsigned i = 0; i < mozos.size(); i++)
        mozos[i]->wait_();

    if (comensalesManager->isStopped())
        comensalesManager->continue_();
    comensalesManager->interrupt_();
    comensalesManager->wait_();

    if (mesasManager->isStopped())
        mesasManager->continue_();
    mesasManager->interrupt_();
    mesasManager->wait_();



    semaforoComensales.eliminar();
    semaforoLiving.eliminar();
    semaforoCaja.eliminar();
}

bool Restaurante::inicializado() {
    return setting_ok;
}

void Restaurante::run() {
    try {
        Logger::log("INFO", REST, getpid(), "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        Logger::log("INFO", REST, getpid(), "Abriendo Restorrente...");

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

        Logger::log("INFO", REST, getpid(), "Estado final de la caja");
        consultarCaja();
        Logger::log("INFO", REST, getpid(), "Cerrando Restorrente.");
        Logger::log("INFO", REST, getpid(), "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    } catch (ProcesoTerminadoException &p) {
        Logger::log("INFO", TERM, p.pid, "El proceso [" + std::to_string(p.pid) + "] terminó correctamente.");
    }
}

void Restaurante::procesarInput(std::string input) {
    // normalizar input a minúsculas
    if (input == QUIT || input == "exit")
        running = false;
    else if (input == CORTAR_LUZ)
        procesarCorteDeLuz();
    else if (input == DEVOLVER_LUZ)
        procesarVueltaDeLuz();
    else if (input == CONSULTAR_CAJA)
        consultarCaja();
    else if (input == LIVING)
        consultarLiving();
    else if (input == HELP) {
        std::cout << "Comandos:\n";
        std::cout << "\tquit\t\tFinaliza Restorrente\n";
        std::cout << "\tcorte\t\tGenera un corte de suministro de energía\n";
        std::cout << "\tluz\t\tRetorna el suministro de energía\n";
        std::cout << "\tcaja\t\tConsulta el estado de la caja\n";
        std::cout << "\tliving\t\tConsulta la cantidad de personas esperando en el living\n";
    } else std::cout << "Comando no encontrado. Ingrese \"help\" para obtener ayuda.\n";
}

void Restaurante::procesarCorteDeLuz() {

    if (hay_luz) {
        /* TODO hay que "vaciar" todo (reiniciar) y parar los procesos (SIGSTOP???) hasta que vuelva la luz */

        /* Detenemos los procesos */
        kill(generadorComensales->getPID(), SIGTERM);
        generadorComensales->stop_();

        //comensalesManager->reset(); // TODO: Cuidado en qué proceso/pid se corre esto, es el del restaurante
        kill(comensalesManager->getPID(), SIGTERM);
        //comensalesManager->stop_();

        for (unsigned i = 0; i < recepcionistas.size(); i++)
            recepcionistas[i]->stop_();

        for (unsigned i = 0; i < mozos.size(); i++)
            mozos[i]->stop_();

        cocinero->stop_();

        mesasManager->vaciar();

        shm_living.escribir(0);

//            mesasManager->stop__();// Pruebo no frenarlo total es innecesario
        Logger::log("INFO", REST, getpid(), "~~~~~~~~~~¡SE GENERÓ UN CORTE DE LUZ!~~~~~~~~~~");
        hay_luz = false;
    }
}

void Restaurante::procesarVueltaDeLuz() {

    if (!hay_luz) {
        Logger::log("INFO", REST, getpid(), "~~~~~~~~~~¡SE REANUDÓ EL SUMINISTRO DE ENERGÍA!~~~~~~~~~~");
        /* TODO hay que reanudar los procesos pero en 0 (fifos vacíos y otras yerbas, etc) (SIGCONT???) */

        /* Reanudamos los procesos */
        kill(generadorComensales->getPID(), SIGTERM);
        generadorComensales->continue_();

        comensalesManager->continue_();

        for (unsigned i = 0; i < recepcionistas.size(); i++)
            recepcionistas[i]->continue_();

        for (unsigned i = 0; i < mozos.size(); i++)
            mozos[i]->continue_();

        cocinero->continue_();

        //mesasManager->continue_(); No lo continuo porque no lo frené
        hay_luz = true;
    }
}

void Restaurante::iniciarCaja() {
    try {
        shmCaja = MemoriaCompartida<Caja>( ARCHIVO_SHM_CAJA,'A' );

        struct Caja laCaja = Caja();
        shmCaja.escribir( laCaja );

    } catch ( std::string& mensaje ) {
        std::cerr << mensaje << std::endl;
    }
}

void Restaurante::iniciarLiving() {
    try {
        shm_living = MemoriaCompartida<int>( ARCHIVO_SHM_LIVING,'A' );
        shm_living.escribir( 0 );

    } catch ( std::string& mensaje ) {
        std::cerr << mensaje << std::endl;
    }
}

void Restaurante::consultarCaja() {
    semaforoCaja.p();
    struct Caja laCaja = shmCaja.leer();
    semaforoCaja.v();
    std::ostringstream oss;
    oss << std::endl << " ---- Consulta de caja ---- "    << std::endl
                     << " - Ingresado\t Perdido  - "       << std::endl
                     << " - " + std::to_string(laCaja.ingreso) + "\t\t " + std::to_string(laCaja.perdido) + "\t  -"
                                                        << std::endl
                     << " -------------------------- ";
    std::string output = oss.str();

    Logger::log("INFO", REST, getpid(), "Revisando la Caja..." + output);
    std::cout << output << std::endl << std::endl;
}

void Restaurante::consultarLiving() {
    semaforoLiving.p();
    int cantidad = shm_living.leer();
    semaforoLiving.v();
    std::ostringstream oss;
    oss << std::endl << " ----- Consulta de Living ----- "    << std::endl
                     << " -- Personas esperando : " + std::to_string(cantidad) + " --"<< std::endl
                     << " ------------------------------ ";
    std::string output = oss.str();

    Logger::log("INFO", REST, getpid(), "Revisando el Living..." + output);
    std::cout << output << std::endl << std::endl;
}

void Restaurante::agregarGanancia(int cant) {
    if (cant < 0)
        throw std::invalid_argument( "cant no puede ser negativa" );

    try {
        MemoriaCompartida<Caja> shmUnaCaja( ARCHIVO_SHM_CAJA,'A' );
        // TODO Agregar lock? ~~~
        struct Caja laCaja = shmUnaCaja.leer();
        laCaja.ingreso += cant;
        shmUnaCaja.escribir(laCaja);

    } catch ( std::string& mensaje ) {
        std::cerr << mensaje << std::endl;
    }
}

void Restaurante::agregarPerdida(int cant) {
    if (cant < 0)
        throw std::invalid_argument( "cant no puede ser negativa" );

    try {
        MemoriaCompartida<Caja> shmUnaCaja( ARCHIVO_SHM_CAJA,'A' );
        // TODO Agregar lock? ~~~
        struct Caja laCaja = shmUnaCaja.leer();
        laCaja.perdido += cant;
        shmUnaCaja.escribir(laCaja);

    } catch ( std::string& mensaje ) {
        std::cerr << mensaje << std::endl;
    }
}


//void Restaurante::reset() {
//    // Registrar comida entregada pero no pagada como pérdida
//    // Cocinero, mozos, recepcionista olvidan lo que estaban haciendo (se resetean)
//}

Restaurante::~Restaurante() {
    if (comensalesManager != 0)
        delete comensalesManager;

    if (mesasManager != 0)
        delete mesasManager;

    if (generadorComensales != 0)
        delete generadorComensales;

    for (unsigned i = 0; i < recepcionistas.size(); i++)
        delete recepcionistas[i];

    for (unsigned i = 0; i < mozos.size(); i++)
        delete mozos[i];

    if (cocinero != 0)
        delete cocinero;
}
