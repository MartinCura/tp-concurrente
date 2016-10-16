//
// Created by martin on 11/10/16.
//

#ifndef TP_CONCURRENTE_PEDIDO_H
#define TP_CONCURRENTE_PEDIDO_H

#include <iostream>
#include <sstream>
#include <string>

// interfaz Serializable
class Pedido {
public:
    Pedido();
    //void agregarPlato(int);
    int cantPlatos();
    int num_mesa;
    std::string serializar();
    static Pedido deserializar(std::string str_serializado);

private:
    // Algún identificador para saber a qué grupo de comensales pertenece
    // TODO: Podría ser el pid_t? Ver dependiendo del IPC usado.

    /* TODO
     * La idea es que cada posición del scorecard
     * indique la cantidad de cada plato posible
     * que se requiere en el pedido
     */
    int scorecard[100];//hardcodeo, tendría que ser la cantidad de platos distintos del menú
};


#endif //TP_CONCURRENTE_PEDIDO_H
