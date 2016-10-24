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
		int id_semaforo;

	public:

		Semaforo_Handler (Semaforo &sem, int id_sem) {
			semaforo = sem;
			id_semaforo = id_sem;
		}

		~Semaforo_Handler () {
		}

		virtual int handleSignal ( int signum ) {
			assert ( signum == SIGTERM );
			semaforo.v(id_semaforo);
			return 0;
		}

		void executeNext() {
			semaforo.p(id_semaforo);
        }
};

#endif /* SEMAFORO_HANDLER_H */
