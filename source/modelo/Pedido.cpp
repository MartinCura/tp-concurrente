//
// Created by martin on 11/10/16.
//

#include "../../include/modelo/Pedido.h"


Pedido::Pedido() {
    for (int i = 0; i < 100; ++i) {     //TODO: hardcodeo
        scorecard[i] = 0;
    }
}

// Asumo máximo de cierto plato, en un pedido, abajo de 100
std::string Pedido::serializar() {
    std::ostringstream oss;

    oss << num_mesa << ";";
    for (int i = 0; i < 100; ++i) {  //TODO: hardcodeo
        int cant = this->scorecard[i];
        // Normalizar números en el serial a 2 dígitos
        if (cant < 10)
            oss << 0;
        oss << cant << ",";
    }
    oss << '\n';
    return oss.str();
}

Pedido Pedido::deserializar(std::string str_serializado) {
    std::stringstream ss(str_serializado);
    Pedido pedido;

    ss >> pedido.num_mesa;
    ss.ignore();
    for (int i = 0; i < 100; ++i) { //TODO: hardcodeo
        ss >> pedido.scorecard[i];
        ss.ignore();
    }
    return pedido;
}

int Pedido::cantPlatos() {
    int sum = 0;
    for (int i = 0; i < 100; ++i) { //TODO: hardcodeo
        sum += scorecard[i];
    }
    return sum;
}
