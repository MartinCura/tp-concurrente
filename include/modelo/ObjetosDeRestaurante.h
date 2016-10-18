/*
 * ObjetosDeRestaurante.h
 *
 *  Created on: Oct 16, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_OBJETOSDERESTAURANTE_H
#define TP_CONCURRENTE_OBJETOSDERESTAURANTE_H

static const std::string ARCHIVO_SHM_CAJA = "tp_concurrente";

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

#endif /*TP_CONCURRENTE_OBJETOSDERESTAURANTE_H*/
