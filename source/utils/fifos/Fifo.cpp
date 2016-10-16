#include <iostream>
#include "../../../include/utils/fifos/Fifo.h"

Fifo::Fifo(const std::string nombre) : nombre(nombre), fd(-1) {
	mknod ( static_cast<const char*>(nombre.c_str()),S_IFIFO|0666,0 );
}

Fifo::~Fifo() {
}

int Fifo::getfd() {
    return this->fd;
}

void Fifo::setBlocking(bool bloqueante) {
	int opts = fcntl( this->fd,F_GETFL,0 );
	if (opts < 0) {
		perror( "fcntl(F_GETFL)" );
		return;
	}
	if (bloqueante)
		opts &= ~O_NONBLOCK;
	else
		opts &= O_NONBLOCK; // opts |= O_NONBLOCK; tira error

	if (fcntl( this->fd,opts ) < 0)
		perror( "fnctl(F_SETFL)" );
}

void Fifo::cerrar() {
	if ( close ( fd ) < 0 )
		perror( "Close errno: " );
	fd = -1;
}

void Fifo::eliminar() const {
	if ( unlink ( nombre.c_str() ) < 0 && errno != ENOENT )
		perror( "Unlink errno" );
}
