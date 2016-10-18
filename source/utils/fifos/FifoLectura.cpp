#include "../../../include/utils/fifos/FifoLectura.h"

FifoLectura::FifoLectura(const std::string nombre) : Fifo(nombre) {
}

FifoLectura::~FifoLectura() {
}

void FifoLectura::abrir() {
	fd = open ( nombre.c_str(),O_RDONLY );
}

void FifoLectura::abrir(bool bloqueante) {
	if (bloqueante)
		abrir();
	else
		fd = open ( nombre.c_str(),O_RDONLY|O_NONBLOCK );
}

int FifoLectura::tomarLock() {
	this->fl.l_type = F_RDLCK;
	return fcntl ( this->fd,F_SETLKW,&(this->fl) );
}

ssize_t FifoLectura::leer(void* buffer,const size_t buffsize) const {
	ssize_t ret = -1;
    if (fd < 0)
        perror( "fifo no abierto" );
	else if ( (ret = read ( fd,buffer,buffsize )) < 0 ) {
        // Si falla y el error no es por estar en no bloqueante...
        int flgs = fcntl( this->fd,F_GETFL,0 );
        if  (errno != EINTR
             && !( flgs >= 0 && (flgs & O_NONBLOCK) && (errno == EAGAIN || errno == EWOULDBLOCK) ))
            perror( "read errno" );
    }
	return ret;
}
