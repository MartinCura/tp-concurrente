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

class Proceso {
protected:
    pid_t pid;

    virtual int ejecutarMiTarea() = 0;
    void loggear(std::string mensaje);

public:
    Proceso();

    void start();

    int wait_();

    int stop_();

    int sigint_();

    int kill_();

    // TODO: Cuidado, puede que este método solo funciona para el proceso Restaurante? Usar getpid().
    pid_t getPID();

    ~Proceso();
};

#endif //TP_CONCURRENTE_PROCESO_H
