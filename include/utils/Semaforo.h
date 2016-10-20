#ifndef SEMAFORO_H_
#define SEMAFORO_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <string>

class Semaforo {

private:
	int id;
	int valorInicial;
	int cantSems;

	int inicializar () const;

public:
	Semaforo();
	Semaforo ( const std::string& nombre,const int valorInicial,int cantSems = 1 );
	~Semaforo();

	int p ( unsigned short semnum = 0 ) const; // decrementa
	int v ( unsigned short semnum = 0 ) const; // incrementa
	void eliminar () const;
};

#endif /* SEMAFORO_H_ */
