/*
 * Logger.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: emanuel
 */

#include "../../include/utils/Logger.h"

std::ofstream Logger::logfile;

void Logger::log(std::string tipo, std::string entidad, int pid, std::string output) {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[100];
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,80,"%Y-%m-%d %I:%M:%S",timeinfo);
    std::string timestamp(buffer);
    logfile.open(LOGFILE, std::ofstream::app);
    logfile << "[" << timestamp << "]" << '\t' << tipo << '\t' << std::to_string(pid) << '\t' << entidad << '\t' << output << '\n';
    logfile.close();
}
