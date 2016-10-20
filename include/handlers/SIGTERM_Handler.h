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
//#include "../utils/Semaforo.h"

class SIGTERM_Handler : public EventHandler {

	private:
		bool availible;
//		Semaforo semaforo;
//		int id_semaforo;

	public:

		SIGTERM_Handler (bool value) {//Semaforo &sem, int id_sem) {
			availible = value;
//            semaforo = sem;
//			id_semaforo = id_sem;
		}

		~SIGTERM_Handler () {
		}

		virtual int handleSignal ( int signum ) {
			assert ( signum == SIGTERM );
			//semaforo.v(id_semaforo);
			if (availible)
				availible = false;
			else
				availible = true;
			return 0;
		}

		bool isAvailible() {
			return availible;
		}
//		void executeNext() {
//            semaforo.p(id_semaforo);
//        }
};

#endif /* SIGTERM_HANDLER_H */
