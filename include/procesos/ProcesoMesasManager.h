//
// Created by martin on 16/10/16.
//

#ifndef TP_CONCURRENTE_PROCESOMESASMANAGER_H
#define TP_CONCURRENTE_PROCESOMESASMANAGER_H

#include "Proceso.h"

class ProcesoMesasManager : public Proceso {
private:
    int ejecutarMiTarea();

public:
    ProcesoMesasManager();
    void reset();
    ~ProcesoMesasManager();
};


#endif //TP_CONCURRENTE_PROCESOMESASMANAGER_H
