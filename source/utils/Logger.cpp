/*
 * Logger.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: emanuel
 */

#include "../../include/utils/Logger.h"

Logger* Logger::logger = 0;
Semaforo* Logger::semaforo = 0;
std::ofstream Logger::logfile;

Logger::Logger() {}

Logger* Logger::getInstance() {
    if (logger == 0) {
        logger = new Logger();
        semaforo = new Semaforo(FILENAME, 1);
    }
    return logger;
}

void Logger::log(std::string tipo, std::string entidad, int pid, std::string output) {
    semaforo->p();
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[100];
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,80,"%Y-%m-%d %I:%M:%S",timeinfo);
    std::string timestamp(buffer);
    std::string pid_s = int_to_string(pid);
    logfile.open(LOGFILE, std::ofstream::app);
    logfile << "[" << timestamp << "]" << '\t' << tipo << '\t' << pid_s << '\t' << entidad << '\t' << output << '\n';
    logfile.close();
    semaforo->v();
}

void Logger::destruir() {
    if (logger != 0) {
        delete logger;
        logger = 0;
    }
    if (semaforo != 0) {
        semaforo->eliminar();
        delete semaforo;
        semaforo = 0;
    }
}
