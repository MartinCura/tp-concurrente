/*
 * Proceso.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#include "Proceso.h"

Proceso::Proceso() {
    pid = 0;
}

pid_t Proceso::getPID() {
    return pid;
}

int Proceso::wait_() {
    return waitpid(pid, 0, 0);
}

int Proceso::stop_() {
    return kill(pid, SIGSTOP);
}
int Proceso::sigint_() {
    return kill(pid, SIGINT);
}

int Proceso::kill_() {
    return kill(pid, SIGKILL);
}

Proceso::~Proceso() {
}
