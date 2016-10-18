#include "../../../include/utils/fifos/FifoEscritura.h"

FifoEscritura::FifoEscritura(const std::string nombre) : Fifo(nombre) {
}

FifoEscritura::~FifoEscritura() {
}

void FifoEscritura::abrir() {
	fd = open ( nombre.c_str(),O_WRONLY );
}

//void FifoEscritura::abrir(bool bloqueante) {
//	if (bloqueante)
//		abrir();
//	else
//		fd = open ( nombre.c_str(),O_WRONLY|O_NONBLOCK );
//}

ssize_t FifoEscritura::escribir(const void* buffer,const size_t buffsize) const {
	return write ( fd,buffer,buffsize );
}
