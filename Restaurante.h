//
// Created by martin on 27/09/16.
//

#ifndef TP_CONCURRENTE_RESTAURANTE_H
#define TP_CONCURRENTE_RESTAURANTE_H

#include <iostream>
#include <unistd.h>
#include <list>
#include "Plato.h"

#include "ProcesoMozo.h"
#include "ProcesoRecepcionista.h"
#include "ProcesoCocinero.h"
#include <errno.h>
#include <vector>

class Restaurante {
private:
    int cantRecepcionistas,
        cantMozos,
        cantMesas;
    std::list<Plato> listPlatos;

    std::vector<Proceso*> recepcionistas;
    std::vector<Proceso*> mozos;
    Proceso* cocinero;

    void inicializarRecursos();
    void eliminarRescursos();

public:
    Restaurante();

    bool inicializado();

    void run();

    void reset();

    ~Restaurante();
};


#endif //TP_CONCURRENTE_RESTAURANTE_H
