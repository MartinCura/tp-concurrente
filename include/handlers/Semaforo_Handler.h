/*
 * Semaforo_Handler.cpp
 *
 *  Created on: Oct 20, 2016
 *      Author: emanuel
 */

#ifndef SEMAFORO_HANDLER_H
#define SEMAFORO_HANDLER_H

#include <signal.h>
#include <assert.h>

#include "EventHandler.h"
#include "../utils/Semaforo.h"

class Semaforo_Handler : public EventHandler {

	private:
		Semaforo semaforo;

	public:

		Semaforo_Handler (Semaforo &sem) {
			semaforo = sem;
		}

		~Semaforo_Handler () {
		}

		virtual int handleSignal ( int signum ) {
			assert ( signum == SIGTERM );
			semaforo.v();
			return 0;
		}

		void executeNext() {
			semaforo.p();
        }
};

#endif /* SEMAFORO_HANDLER_H */
