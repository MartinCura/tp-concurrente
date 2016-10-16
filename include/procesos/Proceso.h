/*
 * Proceso.h
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_PROCESO_H
#define TP_CONCURRENTE_PROCESO_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "../handlers/SIGINT_Handler.h"
#include "../handlers/SignalHandler.h"
#include "../excepciones/Exceptions.h"

#include <iostream>

static const int BUFFSIZE = 400;//
static const std::string ARCHIVO_FIFO_COCINAR  = "/tmp/fifo_cocinar";
static const std::string ARCHIVO_FIFO_COCINADO = "/tmp/fifo_cocinado";
static const std::string ARCHIVO_FIFO_NUEVOS_PEDIDOS_COMENSALES  = "/tmp/fifo_nue_ped_come";
static const std::string ARCHIVO_FIFO_NUEVOS_PEDIDOS_COBROS_COMENSALES  = "/tmp/fifo_nue_ped_cobros_com";


class Proceso {
protected:
    pid_t pid;
    bool stopped;

    virtual int ejecutarMiTarea() = 0;
    void loggear(std::string mensaje);

public:
    Proceso();

    bool isStopped();

    void start();

    int wait_();

    int stop_();

    int continue_();

    int interrupt_();

    int kill_();

    // TODO: Cuidado, puede que este método solo funciona para el proceso Restaurante? Usar getpid().
    // Tranqui...la idea es que lo use sólo Restaurante.
    pid_t getPID();

    ~Proceso();
};

#endif //TP_CONCURRENTE_PROCESO_H
