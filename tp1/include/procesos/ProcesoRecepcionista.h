/*
 * ProcesoRecepcionista.h
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_PROCESORECEPCIONISTA_H
#define TP_CONCURRENTE_PROCESORECEPCIONISTA_H

#include "Proceso.h"
#include <stdlib.h>
#include <sstream>

class ProcesoRecepcionista : public Proceso {
private:
    Semaforo semaforoLiving;
    int ejecutarMiTarea();

public:
    ProcesoRecepcionista(Semaforo &sem);

    ~ProcesoRecepcionista();
};

#endif //TP_CONCURRENTE_PROCESORECEPCIONISTA_H
