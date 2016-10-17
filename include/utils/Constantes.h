/*
 * Constantes.h
 *
 *  Created on: Oct 14, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_CONSTANTES_H
#define TP_CONCURRENTE_CONSTANTES_H

/* Ya sé, los nombres son malísimos */
#define QUIT            "quit"
#define CORTAR_LUZ      "corte"
#define DEVOLVER_LUZ    "luz"
#define CONSULTAR_CAJA  "caja"
#define HELP            "help"

#define MOZO    "MOZO"
#define CHEF    "CHEF"
#define RECP    "RECP"
#define REST    "REST"
#define GCOM    "GCOM"
#define PMM_    "PMM_"
#define COMN    "COMN"

static const int MAX_COMENSALES_EN_GRUPO = 5;

static const std::string ARCHIVO_FIFO_NUEVOS_PEDIDOS  = "/tmp/fifo_nuevos_pedidos";

static const std::string ARCHIVO_FIFO_COCINAR  = "/tmp/fifo_cocinar";
static const std::string ARCHIVO_FIFO_COCINADO = "/tmp/fifo_cocinado";

static const std::string ARCHIVO_FIFO_MESAS_LIBERADAS = "/tmp/fifo_mesas_liberadas";
static const std::string ARCHIVO_FIFO_MESAS_LIBRES = "/tmp/fifo_mesas_libres";

static const std::string ARCHIVO_FIFO_LLEGADA_COM  = "/tmp/fifo_llegada_com";
static const std::string ARCHIVO_FIFO_LIVING_COM  = "/tmp/fifo_living_com";

static const std::string FILENAME_SEM_COM_RECP = "../procesos/ProcesoComensales.cpp";
static const std::string FILENAME_SEM_RECP_COM = "../procesos/ProcesoRecepcionista.cpp";

static const int SEMAFORO_COM_RECP = 1;
static const int SEMAFORO_RECP_COM = 2;

#endif //TP_CONCURRENTE_CONSTANTES_H
