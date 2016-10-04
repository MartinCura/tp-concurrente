#include "Restaurante.h"

int main() {
    Restaurante restaurante;
    if (!restaurante.inicializar())
        return -1;
    restaurante.run();
    return 0;
}
