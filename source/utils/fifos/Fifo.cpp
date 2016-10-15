#include <iostream>
#include "../../../include/utils/fifos/Fifo.h"

Fifo::Fifo(const std::string nombre) : nombre(nombre), fd(-1) {
	mknod ( static_cast<const char*>(nombre.c_str()),S_IFIFO|0666,0 );
}

Fifo::~Fifo() {
}

void Fifo::cerrar() {
	if ( close ( fd ) < 0 )
		std::cerr << "Close errno: " << errno << std::endl;
	fd = -1;
}

void Fifo::eliminar() const {
	if ( unlink ( nombre.c_str() ) < 0 && errno != ENOENT )
		std::cerr << "Unlink errno: " << errno << std::endl;
}
