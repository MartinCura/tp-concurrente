#include "../../../include/utils/fifos/FifoLectura.h"

FifoLectura::FifoLectura(const std::string nombre) : Fifo(nombre) {
}

FifoLectura::~FifoLectura() {
}

void FifoLectura::abrir() {
	fd = open ( nombre.c_str(),O_RDONLY );
}

ssize_t FifoLectura::leer(void* buffer,const ssize_t buffsize) const {
	ssize_t ret;
	// Si falla y el error no es por estar en no bloqueante...
	if ( (ret = read ( fd,buffer,buffsize )) < 0 ) {
			int flgs = fcntl( this->fd,F_GETFL,0 );
			if  (errno != EINTR
				 && !( flgs >= 0 && (flgs & O_NONBLOCK) && (errno == EAGAIN || errno == EWOULDBLOCK) ))
				perror( "read errno" );
	}
	return ret;
}
