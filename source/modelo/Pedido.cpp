//
// Created by martin on 11/10/16.
//

#include <utils/Constantes.h>
#include <utils/Utils.h>
#include "../../include/modelo/Pedido.h"


Pedido::Pedido(int numMesa, pid_t unPid) {
    if (numMesa < 0 || numMesa >= pow(10, TAM_NUM_MESA))
        throw std::invalid_argument( "argumento inválido" );

    for (int i = 0; i < CANT_PLATOS_DISTINTOS; ++i) {
        scorecard[i] = 0;
    }
    this->num_mesa = numMesa;
    this->pid = unPid;
}

void Pedido::agregarPlato(int numPlato, int cant) {
    if (numPlato < 0 || numPlato > CANT_PLATOS_DISTINTOS || cant < 0)
        throw std::invalid_argument( "argumento inválido" );

    int maxCantPlatos = (int) pow(10, TAM_CANT_PLATO) - 1;
    this->scorecard[numPlato] += cant;
    if (this->scorecard[numPlato] > maxCantPlatos)
        this->scorecard[numPlato] = maxCantPlatos;
}

pid_t Pedido::getPid() {
    return this->pid;
}

int Pedido::getNumMesa() {
    return this->num_mesa;
}

unsigned int Pedido::cantPlatos() {
    unsigned int sum = 0;
    for (int i = 0; i < CANT_PLATOS_DISTINTOS; ++i) {
        sum += scorecard[i];
    }
    return sum;
}


// Formato: `pid;numMesa;cantPlato0,cantPlato1,cantPlato2,...,cantPlatoMax,\n`
std::string Pedido::serializar() {
    std::ostringstream oss;

    // Los for normalizan número a la cantidad de dígitos correcta para el serial, el primer 1 salva caso borde = 0
    pid_t pid = this->pid;
    for (int z = pid; z < pow(10, TAM_PID - 1); z *= 10) {
        oss << 0;
    }
    oss << pid << ";";
    for (int z = num_mesa > 0 ? num_mesa : 1 ; z < pow(10, TAM_NUM_MESA - 1); z *= 10) {
        oss << 0;
    }
    oss << num_mesa << ";";
    for (int i = 0; i < CANT_PLATOS_DISTINTOS; ++i) {
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
    int pid, numMesa;

    ss >> pid;
    ss.ignore();

    ss >> numMesa;
    ss.ignore();

    Pedido pedido(numMesa, pid);

    for (int i = 0; i < CANT_PLATOS_DISTINTOS; ++i) {
        ss >> pedido.scorecard[i];
        ss.ignore();
    }
    return pedido;
}


// Crea un Pediddo rándom para un comensal, con entre 0 y 4 platos rándom por comensal, pero nunca vacío
Pedido Pedido::crearRandom(pid_t pidComensal, unsigned numMesa, unsigned cantComensales) {
    Pedido pedido(numMesa, pidComensal);
    for (unsigned comensal = 0; comensal < cantComensales; ++comensal) {
        unsigned hambreComensal = (unsigned) Utils::generarRandom(5) - 1;
        for (unsigned plato = 0; plato < hambreComensal; ++plato) {
            pedido.agregarPlato( Utils::generarRandom(CANT_PLATOS_DISTINTOS) );
        }
    }
    if (pedido.cantPlatos() == 0)
        pedido.agregarPlato( Utils::generarRandom(CANT_PLATOS_DISTINTOS) );
    return pedido;
}
