#include "Restaurante.h"

int main() {
    Restaurante restaurante;
    if (!restaurante.inicializado())
        return -1;
    restaurante.run();
    return 0;
}
