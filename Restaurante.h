//
// Created by martin on 27/09/16.
//

#ifndef TP_CONCURRENTE_RESTAURANTE_H
#define TP_CONCURRENTE_RESTAURANTE_H


#include <list>
#include "Plato.h"

class Restaurante {
private:
    int cantDeRecepcionistas,
        cantMozos,
        cantMesas;
    std::list<Plato> mapPlatos; // Plato, precio. Considerar hacer la clase Plato, en cuyo caso cambiar por una lista.
};


#endif //TP_CONCURRENTE_RESTAURANTE_H
