//
// Created by martin on 11/10/16.
//

#ifndef TP_CONCURRENTE_PEDIDO_H
#define TP_CONCURRENTE_PEDIDO_H

#include "../utils/Constantes.h"
#include <stdexcept>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>


class Pedido {
public:
    Pedido(int numMesa = -1, pid_t unPid = -1);

    pid_t getPid();
    int getNumMesa();

    void agregarPlato(int numPlato, int cant = 1);
    unsigned int cantPlatos();

    std::string serializar();
    static Pedido deserializar(std::string str_serializado);

    static Pedido crearRandom(pid_t pidComensal, unsigned numMesa, unsigned cantComensales = 3);

private:
    pid_t pid;
    int num_mesa;

    /*
     * La idea es que cada posición del scorecard
     * indique la cantidad de cada plato posible
     * que se requiere en el pedido
     */
    int scorecard[CANT_PLATOS_DISTINTOS];
};


#endif //TP_CONCURRENTE_PEDIDO_H
