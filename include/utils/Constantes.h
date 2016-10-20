/*
 * Constantes.h
 *
 *  Created on: Oct 14, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_CONSTANTES_H
#define TP_CONCURRENTE_CONSTANTES_H

#include <string>
#include <cmath>

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
#define PCM_    "PCM_"
#define TERM    "TERM"

static const int MAX_COMENSALES_EN_GRUPO = 5;

static const int TAM_MAX_MSJ_RECP_PCM = 5;

static const std::string ARCHIVO_FIFO_NUEVOS_PEDIDOS = "/tmp/fifo_nuevos_pedidos";

static const std::string ARCHIVO_FIFO_NUEVOS_COMENSALES_EN_MESA = "/tmp/fifo_nuevos_comensales_en_mesa";

static const std::string ARCHIVO_FIFO_COCINAR  = "/tmp/fifo_cocinar";
static const std::string ARCHIVO_FIFO_COCINADO = "/tmp/fifo_cocinado";

static const std::string ARCHIVO_FIFO_MESAS_LIBRES = "/tmp/fifo_mesas_libres";
static const std::string ARCHIVO_FIFO_LIBERAR_MESA = "/tmp/fifo_liberar_mesa";
static const std::string ARCHIVO_FIFO_SALDOS_MESA = "/tmp/fifo_saldos_mesa";


static const std::string ARCHIVO_FIFO_LLEGADA_COM = "/tmp/fifo_llegada_com";
static const std::string ARCHIVO_FIFO_LIVING_COM  = "/tmp/fifo_living_com";

static const std::string FILENAME_SEM_COM_RECP = "../procesos/ProcesoComensales.cpp";
static const std::string FILENAME_SEM_RECP_COM = "../procesos/ProcesoRecepcionista.cpp";

static const int SEMAFORO_COM_RECP = 1;
static const int SEMAFORO_RECP_COM = 2;


/*
 * No estoy seguro cómo se hace el envío para que sepan cuánto sacar, por lo que hago mensajes
 * de tamaño prefijado, decidiendo la cantidad de dígitos del número de mesa y de la cantidad de platos
 * Número de mesa está entre 000 y 999 mientras que se puede pedir entre 00 y 99 de un plato en un Pedido.
 * El PID en Ubuntu tiene un máximo de 32768, por lo que permito 5 dígitos ~ mc
 * TODO: Ajustar este mensaje para la entrega.
*/
static const int CANT_PLATOS_DISTINTOS = 100;  // Platos distintos disponibles
static const int TAM_NUM_MESA = 3;   // Mesas disponibles = 10^TAM_NUM_MESA
static const int TAM_CANT_PLATO = 2; // Orden máxima de un cierto plato por Pedido = 10^TAM_CANT_PLATO
static const int TAM_PID = 5;        // pid máximo = 32768, innecesario chequearlo

static const int MAX_MESAS = pow(10, TAM_NUM_MESA);    // Mesas disponibles: 0 a (CANT_MESAS-1)

// Esto es lo más feo que programé en mucho tiempo jaja
static const size_t TAM_PEDIDO = (TAM_PID + 1) + (TAM_NUM_MESA + 1) + (TAM_CANT_PLATO + 1) * 100 + 1;
                               //     pid      ;    núm_mesa     ;      cant, ... ,cant,          \n


#endif //TP_CONCURRENTE_CONSTANTES_H
