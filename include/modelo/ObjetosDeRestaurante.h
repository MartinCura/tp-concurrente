/*
 * ObjetosDeRestaurante.h
 *
 *  Created on: Oct 16, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_OBJETOSDERESTAURANTE_H
#define TP_CONCURRENTE_OBJETOSDERESTAURANTE_H

static const std::string ARCHIVO_SHM_CAJA = "tp_concurrente";
static const std::string ARCHIVO_SHM_MESA = "/bin/bash";

struct Caja {
    int ingreso = 0;
    int perdido = 0;
};

struct Mesa {
    int id = 0;
    bool ocupada = false;
    int cant_comensales = 0;
};

#endif /*TP_CONCURRENTE_OBJETOSDERESTAURANTE_H*/
