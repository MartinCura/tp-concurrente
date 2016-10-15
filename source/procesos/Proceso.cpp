/*
 * Proceso.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#include "../../include/procesos/Proceso.h"

Proceso::Proceso() {
    pid = 0;
    stopped = true;
}

bool Proceso::isStopped() {
    return stopped;
}

void Proceso::start() {
    stopped = false;
    pid_t id = fork();
    if (id == 0) {
        ejecutarMiTarea();
        throw ProcesoTerminadoException(getpid());
    } else {
        pid = id;
    }
}

pid_t Proceso::getPID() {
    return pid;
}

int Proceso::wait_() {
    return waitpid(pid, 0, 0);
}

int Proceso::stop_() {
    stopped = true;
    return kill(pid, SIGSTOP);
}

int Proceso::continue_() {
    stopped = false;
    return kill(pid, SIGCONT);
}

int Proceso::interrupt_() {
    return kill(pid, SIGINT);
}

Proceso::~Proceso() {
}
