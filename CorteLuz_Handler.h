#ifndef CORTELUZ_HANDLER_H_
#define CORTELUZ_HANDLER_H_

#include <signal>
#include <assert.h>

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

    bool
};

#endif /* CORTELUZ_HANDLER_H_ */
