/*
 * ObjetosDeRestaurante.h
 *
 *  Created on: Oct 16, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_OBJETOSDERESTAURANTE_H
#define TP_CONCURRENTE_OBJETOSDERESTAURANTE_H

#include "../utils/Logger.h"
#include "../modelo/Pedido.h"
#include "../utils/Constantes.h"
#include "../excepciones/Exceptions.h"


static const std::string ARCHIVO_SHM_CAJA = "tp_concurrente";
static const std::string ARCHIVO_SHM_MESAS = "/bin/bash";

struct Caja {
    int ingreso = 0;
    int perdido = 0;
};

struct Mesa {
    int id = 0;
    bool ocupada = false;
    int cant_comensales = 0;
    int gastado = 0;

    Mesa() {}
    Mesa(int idMesa) {
        this->id = idMesa;
    }

    void reset() {
        this->ocupada = false;
        this->cant_comensales = 0;
        this->gastado = 0;
    }
};

struct MesasConPedidos {
    Pedido v[MAX_MESAS];
    bool o[MAX_MESAS];

    MesasConPedidos() {
        for (int i = 0; i < MAX_MESAS; ++i) {
            o[i] = false;
        }
    }

    Pedido get(int i) {
        if (o[i])
            return v[i];
        else throw NoHayPedidoException();
    }

    void put(int i, Pedido p) {
        v[i] = p;
        o[i] = true;
    }
};

#endif /*TP_CONCURRENTE_OBJETOSDERESTAURANTE_H*/
