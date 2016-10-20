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

#include "EventHandler.h"
//#include "../utils/Semaforo.h"
//#include <iostream>

//static const std::string FILENAME = "/bin/ps";

class SIGTERM_Handler : public EventHandler {

	private:
		bool availible;
//		Semaforo semaforo;
	public:

		SIGTERM_Handler () {
			availible = false;
			//semaforo = Semaforo(FILENAME, 0);
		}

		~SIGTERM_Handler () {
		}

		virtual int handleSignal ( int signum ) {
			assert ( signum == SIGTERM );
			//std::cout << "me llegó la señal" << std::endl;
			//semaforo.v();
			if (availible)
				availible = false;
			else
				availible = true;
			return 0;
		}

		void eliminarSemaforo() {
			//semaforo.eliminar();
		}

		bool isAvailible() { //executeNext() {
			return availible;
			//semaforo.p();
        }
};

#endif /* SIGTERM_HANDLER_H */
