/*
 * ProcesoComensalesManager.h
 *
 *  Created on: Oct 17, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_PROCESOCOMENSALESMANAGER_H
#define TP_CONCURRENTE_PROCESOCOMENSALESMANAGER_H

#include "ProcesoComensales.h"

class ProcesoComensalesManager : public Proceso {
private:
    Semaforo semaforoComensales;
    std::vector<ProcesoComensales*> comensales;
    int ejecutarMiTarea();

public:
    ProcesoComensalesManager(Semaforo &sem);

    void reset();

    ~ProcesoComensalesManager();
};

#endif //TP_CONCURRENTE_PROCESOCOMENSALESMANAGER_H
