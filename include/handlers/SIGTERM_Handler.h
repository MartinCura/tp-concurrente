/*
 * ProcesoComensales.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: emanuel
 */

#ifndef SIGTERM_HANDLER_H
#define SIGTERM_HANDLER_H

#include <signal.h>
#include <assert.h>
#include <iostream>

#include "EventHandler.h"
#include "../utils/Semaforo.h"

static const std::string FILENAME = "/bin/ps";

class SIGTERM_Handler : public EventHandler {

	private:
		Semaforo semaforo;

	public:

		SIGTERM_Handler () {
            semaforo = Semaforo(FILENAME, 0);
		}

		~SIGTERM_Handler () {
		}

		virtual int handleSignal ( int signum ) {
			assert ( signum == SIGTERM );
			std::cout << "Libero semáforo" << std::endl;//
			semaforo.v();
			return 0;
		}

		void eliminarSemaforo() {
			semaforo.eliminar();
		}

		void executeNext() {
            semaforo.p();
			std::cout << "Soy libre!!!" << std::endl;//
        }
};

#endif /* SIGTERM_HANDLER_H */
