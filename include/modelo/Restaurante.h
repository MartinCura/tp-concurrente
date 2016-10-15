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
#include <errno.h>
#include <vector>

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

    ~Restaurante();
};


#endif //TP_CONCURRENTE_RESTAURANTE_H
