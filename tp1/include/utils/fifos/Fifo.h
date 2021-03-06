#ifndef FIFO_H_
#define FIFO_H_

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

class Fifo {
public:
	Fifo(const std::string nombre);
	virtual ~Fifo();
	virtual void abrir() = 0;
	int getfd();
	int tomarLock();
	int liberarLock();
//	void setBlocking(bool);
	void cerrar();
	void eliminar() const;

protected:
	std::string nombre;
	int fd;
	struct flock fl;
};

#endif /* FIFO_H_ */
