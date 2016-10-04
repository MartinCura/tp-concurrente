#ifndef CORTELUZ_HANDLER_H_
#define CORTELUZ_HANDLER_H_

#include <signal.h>
#include <assert.h>
#include "EventHandler.h"

class CorteLuz_Handler : public EventHandler {
private:
    bool corteGenerado;

public:
    CorteLuz_Handler() : corteGenerado(false) {
    }

    ~CorteLuz_Handler() {
    }

    virtual int handleSignal(int signum) {
        assert(signum == SIGSTOP);
        corteGenerado = true;
        return 0;
    }

    bool fueGenerado() {
        return corteGenerado;
    }
};

#endif /* CORTELUZ_HANDLER_H_ */
