/*
 * Logger.h
 *
 *  Created on: Oct 15, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_LOGGER_H
#define TP_CONCURRENTE_LOGGER_H

#include <fstream>
#include <time.h>

#include "Utils.h"
#include "Semaforo.h"

static const std::string FILENAME = "Logger.h";
static const std::string LOGFILE = "restorrente.log";

class Logger {
private:
    static std::ofstream logfile;
    static Logger* logger;
    static Semaforo* semaforo;
    Logger();

public:
    static Logger* getInstance();

    void log(std::string tipo, std::string entidad, int pid, std::string output);

    static void destruir();
};

#endif /* TP_CONCURRENTE_LOGGER_H_ */
