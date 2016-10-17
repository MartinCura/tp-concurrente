//
// Created by martin on 27/09/16.
//

#ifndef TP_CONCURRENTE_RESTAURANTE_H
#define TP_CONCURRENTE_RESTAURANTE_H

#include <list>
#include "Plato.h"

#include "../procesos/ProcesoGeneradorComensales.h"
#include "../procesos/ProcesoMozo.h"
#include "../procesos/ProcesoRecepcionista.h"
#include "../procesos/ProcesoCocinero.h"
#include "../procesos/ProcesoMesasManager.h"
#include "ObjetosDeRestaurante.h"

class Restaurante {
private:
    bool running;
    bool hay_luz;
    int cantRecepcionistas,
        cantMozos,
        cantMesas;
    std::list<Plato> listPlatos;

    ProcesoMesasManager* mesasManager;
    Proceso* generadorComensales;
    std::vector<Proceso*> recepcionistas;
    std::vector<Proceso*> mozos;
    Proceso* cocinero;  // no genérico

    std::vector<Semaforo> semaforos;

    MemoriaCompartida<Caja> shmCaja;
//    std::vector<MemoriaCompartida<Mesa>> mesas;

    void iniciarCaja();
//    void inicializarMesas();

    void lanzarProcesos();
    void terminarProcesos();

    void inicializarRecursos();
    void eliminarRecursos();

    void reset();

    void procesarInput(std::string input);
    void procesarCorteDeLuz();
    void procesarVueltaDeLuz();
    void consultarCaja();

public:
    Restaurante();

    bool inicializado();

    void run();

    static void agregarGanancia(int cant);
    static void agregarPerdida(int cant);

    ~Restaurante();
};

#endif //TP_CONCURRENTE_RESTAURANTE_H
