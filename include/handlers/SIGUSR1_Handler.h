/*
 * ProcesoComensales.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: emanuel
 */

#ifndef SIGUSR1_HANDLER_H
#define SIGUSR1_HANDLER_H

#include <signal.h>
#include <assert.h>

#include "EventHandler.h"
#include "../utils/Semaforo.h"

class SIGUSR1_Handler : public EventHandler {

private:
	ProcesoMesasManager* pmm;

public:

	SIGUSR1_Handler (ProcesoMesasManager* unPmm) {
		this->pmm = unPmm;
	}

	~SIGUSR1_Handler () {
	}

	virtual int handleSignal ( int signum ) {
		printf("HandleSignal SIGUSR1\n");//
		assert ( signum == SIGUSR1 );
		this->pmm->reset();
		return 0;
	}
};

#endif /* SIGUSR1_HANDLER_H */
