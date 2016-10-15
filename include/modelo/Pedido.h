//
// Created by martin on 11/10/16.
//

#ifndef TP_CONCURRENTE_PEDIDO_H
#define TP_CONCURRENTE_PEDIDO_H

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

// TODO: Puedo hacer método impresionLegible que imprima solo los platos != 0 con nombre

// interfaz Serializable
class Pedido {
public:
    Pedido(int numMesa);
    void agregarPlato(int numPlato, int cant = 1);
    unsigned int cantPlatos();
    std::string serializar();
    static Pedido deserializar(std::string str_serializado);
    static int TAM_MENSAJE;

private:
    // Algún identificador para saber a qué grupo de comensales pertenece
    // TODO: Podría ser el pid_t? Ver dependiendo del IPC usado. Ser consistente con TAM_NUM_MESA
    int num_mesa;
    /* TODO
     * La idea es que cada posición del scorecard
     * indique la cantidad de cada plato posible
     * que se requiere en el pedido
     */
    int scorecard[100];//hardcodeo, tendría que ser la cantidad de platos distintos del menú
};


#endif //TP_CONCURRENTE_PEDIDO_H
