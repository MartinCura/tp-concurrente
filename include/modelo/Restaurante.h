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
#include "../procesos/ProcesoComensalesManager.h"
#include "ObjetosDeRestaurante.h"
#include "../handlers/Semaforo_Handler.h"
#include "../utils/json11.hpp"

class Restaurante {
private:
    Semaforo semaforoMesasListas;
    Semaforo semaforoComensales;
    Semaforo semaforoLiving;
    Semaforo semaforoCaja;

    bool setting_ok;
    bool running;
    bool hay_luz;
    unsigned cantRecepcionistas,
             cantMozos,
             cantMesas;
    std::list<Plato> listPlatos;

    ProcesoComensalesManager* comensalesManager;
    ProcesoMesasManager* mesasManager;
    Proceso* generadorComensales;
    std::vector<Proceso*> recepcionistas;
    std::vector<Proceso*> mozos;
    Proceso* cocinero;  // no genérico

    MemoriaCompartida<Caja> shmCaja;
    MemoriaCompartida<int> shm_living;

    void cargarConfiguracion();

    void iniciarCaja();
    void iniciarLiving();

    void lanzarProcesos();
    void terminarProcesos();

    void reset();

    void procesarInput(std::string input);
    void procesarCorteDeLuz();
    void procesarVueltaDeLuz();
    void consultarCaja();
    void consultarLiving();

public:
    Restaurante();

    bool inicializado();

    void run();

    static void agregarGanancia(int cant);
    static void agregarPerdida(int cant);

    ~Restaurante();
};

#endif //TP_CONCURRENTE_RESTAURANTE_H
