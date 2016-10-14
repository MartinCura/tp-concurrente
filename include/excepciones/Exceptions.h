/*
 * Exceptions.h
 *
 *  Created on: Oct 9, 2016
 *      Author: emanuel
 */

#ifndef TP_CONCURRENTE_EXCEPTIONS_H_
#define TP_CONCURRENTE_EXCEPTIONS_H_

#include <exception>
#include <unistd.h>

class ProcesoTerminadoException : public std::exception {
public:
    pid_t pid;
	ProcesoTerminadoException(pid_t id) : pid(id){}
};




#endif /* TP_CONCURRENTE_EXCEPTIONS_H_ */
