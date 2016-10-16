//
// Created by martin on 27/09/16.
//

#ifndef TP_CONCURRENTE_RESTAURANTE_H
#define TP_CONCURRENTE_RESTAURANTE_H

#include <iostream>
#include <unistd.h>
#include <list>
#include "Plato.h"

#include "../procesos/ProcesoComensales.h"
#include "../procesos/ProcesoMozo.h"
#include "../procesos/ProcesoRecepcionista.h"
#include "../procesos/ProcesoCocinero.h"
#include "../../source/utils/MemoriaCompartida.h"
#include <errno.h>
#include <vector>

static const std::string ARCHIVO_SHM_CAJA = "tp_concurrente";

struct Caja {
    int ingreso = 0;
    int perdido = 0;
};

class Restaurante {
private:
    bool running;
    bool hay_luz;
    int cantRecepcionistas,
        cantMozos,
        cantMesas;
    std::list<Plato> listPlatos;

    Proceso* generadorComensales;
    std::vector<Proceso*> recepcionistas;
    std::vector<Proceso*> mozos;
    Proceso* cocinero;  // no genérico

    MemoriaCompartida<Caja> shmCaja;

    void iniciarCaja();

    void lanzarProcesos();
    void terminarProcesos();

    void inicializarRecursos();
    void eliminarRecursos();

    void reset();

    void procesarInput(std::string input);
    void procesarCorteDeLuz();
    void procesarVueltaDeLuz();
    void consultarCaja();

public:
    Restaurante();

    bool inicializado();

    void run();

    static void agregarGanancia(int cant);
    static void agregarPerdida(int cant);

    ~Restaurante();
};


#endif //TP_CONCURRENTE_RESTAURANTE_H
