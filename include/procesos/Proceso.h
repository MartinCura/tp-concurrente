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
#include <vector>
#include "../handlers/SIGINT_Handler.h"
#include "../handlers/SIGTERM_Handler.h"
#include "../handlers/SignalHandler.h"
#include "../excepciones/Exceptions.h"
#include "../utils/Logger.h"
#include "../utils/Constantes.h"
#include "../utils/fifos/FifoLectura.h"
#include "../utils/fifos/FifoEscritura.h"
#include "../handlers/Semaforo_Handler.h"

static const int BUFFSIZE = 400;

class Proceso {
protected:
    pid_t pid;
    bool stopped;
    bool _finished;

    virtual int ejecutarMiTarea() = 0;

public:
    Proceso();

    bool isStopped();

    bool finished();

    void start();

    int wait_();

    int stop_();

    int continue_();

    int interrupt_();

    /** Solo usar desde Restaurante **/
    pid_t getPID();

    virtual ~Proceso();
};

#endif //TP_CONCURRENTE_PROCESO_H
