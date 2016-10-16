//
// Created by martin on 11/10/16.
//


#include "../../include/modelo/Pedido.h"


/*
 * No estoy seguro cómo se hace el envío para que sepan cuánto sacar, por lo que hago mensajes
 * de tamaño prefijado, decidiendo la cantidad de dígitos del número de mesa y de la cantidad de platos
 * Número de mesa está entre 000 y 999 mientras que se puede pedir entre 00 y 99 de un plato en un Pedido.
 * TODO: Ajustar este mensaje para la entrega.
 */
int CANT_PLATOS = 100;  // Platos distintos disponibles
int TAM_NUM_MESA = 3;   // Mesas disponibles = 10^TAM_NUM_MESA
int TAM_CANT_PLATO = 2; // Orden máxima de un cierto plato por Pedido = 10^TAM_CANT_PLATO

// Esto es lo más feo que programé en mucho tiempo jaja
int Pedido::TAM_MENSAJE = (TAM_NUM_MESA + 1) + (TAM_CANT_PLATO + 1) * 100 + 1;
                        //  núm_mesa      ;      cant, ... ,cant,          \n


Pedido::Pedido(int numMesa) {
    if (numMesa < 0 || numMesa >= pow(10, TAM_NUM_MESA))
        throw std::invalid_argument( "argumento inválido" );

    for (int i = 0; i < CANT_PLATOS; ++i) {
        scorecard[i] = 0;
    }
    this->num_mesa = numMesa;
}

void Pedido::agregarPlato(int numPlato, int cant) {
    if (numPlato < 0 || numPlato > CANT_PLATOS || cant < 0)
        throw std::invalid_argument( "argumento inválido" );

    int maxCantPlatos = (int) pow(10, TAM_CANT_PLATO) - 1;
    this->scorecard[numPlato] += cant;
    if (this->scorecard[numPlato] > maxCantPlatos)
        this->scorecard[numPlato] = maxCantPlatos;
}

int Pedido::getNumMesa() {
    return this->num_mesa;
}

unsigned int Pedido::cantPlatos() {
    unsigned int sum = 0;
    for (int i = 0; i < CANT_PLATOS; ++i) {
        sum += scorecard[i];
    }
    return sum;
}


std::string Pedido::serializar() {
    std::ostringstream oss;

    // Normaliza número a la cantidad de dígitos correcta para el serial, el primer 1 salva caso borde
    for (int z = num_mesa > 0 ? num_mesa : 1 ; z < pow(10, TAM_NUM_MESA - 1); z *= 10) {
        oss << 0;
    }
    oss << num_mesa << ";";
    for (int i = 0; i < CANT_PLATOS; ++i) {
        int cant = this->scorecard[i];
        for (int z = cant > 0 ? cant : 1; z < pow(10, TAM_CANT_PLATO - 1); z *= 10) {
            oss << 0;
        }
        oss << cant << ",";
    }
    oss << '\n';
    return oss.str();
}

Pedido Pedido::deserializar(std::string str_serializado) {
    std::stringstream ss(str_serializado);
    int numMesa;

    ss >> numMesa;
    Pedido pedido(numMesa);

    ss.ignore();

    for (int i = 0; i < CANT_PLATOS; ++i) {
        ss >> pedido.scorecard[i];
        ss.ignore();
    }
    return pedido;
}
