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

static const std::string LOGFILE = "restorrente.log";

class Logger {
private:
    static std::ofstream logfile;

public:
    static void log(std::string tipo, std::string entidad, int pid, std::string output);
};

#endif /* TP_CONCURRENTE_LOGGER_H_ */
