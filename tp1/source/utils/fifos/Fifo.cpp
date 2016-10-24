#include <iostream>
#include "../../../include/utils/fifos/Fifo.h"

Fifo::Fifo(const std::string nombre) : nombre(nombre), fd(-1) {
	mknod ( nombre.c_str(),S_IFIFO|0666,0 );

	this->fl.l_type = F_WRLCK;
	this->fl.l_whence = SEEK_SET;
	this->fl.l_start = 0;
	this->fl.l_len = 0;
}

Fifo::~Fifo() {
}

int Fifo::getfd() {
    return this->fd;
}

int Fifo::tomarLock() {
	this->fl.l_type = F_WRLCK;
	return fcntl ( this->fd,F_SETLKW,&(this->fl) );
}

int Fifo::liberarLock() {
	this->fl.l_type = F_UNLCK;
	return fcntl ( this->fd,F_SETLK,&(this->fl) );
}

//void Fifo::setBlocking(bool bloqueante) {
//	int opts = fcntl( this->fd,F_GETFL,0 );
//	if (opts < 0) {
//		perror( "fcntl(F_GETFL)" );
//		return;
//	}
//	if (bloqueante)
//		opts &= ~O_NONBLOCK;
//	else
//		opts &= O_NONBLOCK; // opts |= O_NONBLOCK; tira error
//
//	if (fcntl( this->fd,opts ) < 0)
//		perror( "fnctl(F_SETFL)" );
//}

void Fifo::cerrar() {
	if ( close ( fd ) < 0 )
		perror( "Close errno: " );
	fd = -1;
}

void Fifo::eliminar() const {
	if ( unlink ( nombre.c_str() ) < 0 && errno != ENOENT )
		perror( "Unlink errno" );
}
