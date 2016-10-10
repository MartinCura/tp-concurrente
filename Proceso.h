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
#include "SIGINT_Handler.h"
#include "SignalHandler.h"
#include "Exceptions.h"

#include <iostream>

class Proceso {
protected:
    pid_t pid;

    virtual int ejecutarMiTarea() = 0;

public:
    Proceso();

    void start();

    int wait_();

    int stop_();

    int sigint_();

    int kill_();

    pid_t getPID();

    ~Proceso();
};

#endif //TP_CONCURRENTE_PROCESO_H
