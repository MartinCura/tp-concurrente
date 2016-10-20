#include "../../include/utils/Semaforo.h"
Semaforo::Semaforo(){}

Semaforo :: Semaforo ( const std::string& nombre, const int valorInicial, int cantSems )
												: valorInicial(valorInicial), cantSems(cantSems) {
	//if (cantSems > SEMMSL)
	key_t clave = ftok ( nombre.c_str(),'a' );
	this->id = semget ( clave,cantSems,0666|IPC_CREAT );

	this->inicializar ();
}

Semaforo::~Semaforo() {
}

int Semaforo :: inicializar () const {

	union semnum {
		int val;
		struct semid_ds* buf;
		ushort* array;
		struct seminfo *__buf;
	};

	semnum init;
	init.val = this->valorInicial;
	int resultado = -1;

	for (unsigned short s = 0; s < this->cantSems; ++s) {
		resultado = semctl ( this->id,s,SETVAL,init );
	}
	return resultado;
}

int Semaforo :: p (unsigned short semnum) const {

	struct sembuf operacion;

	operacion.sem_num = semnum;	// número de semaforo
	operacion.sem_op  = -1;		// restar 1 al semaforo
	operacion.sem_flg = SEM_UNDO;

	int resultado = semop ( this->id,&operacion,1 );
	return resultado;
}

int Semaforo :: v (unsigned short semnum) const {

	struct sembuf operacion;

	operacion.sem_num = semnum;	// número de semaforo
	operacion.sem_op  = 1;		// sumar 1 al semaforo
	operacion.sem_flg = SEM_UNDO;

	int resultado = semop ( this->id,&operacion,1 );
	return resultado;
}

void Semaforo :: eliminar () const {
	for (unsigned short s = 0; s < this->cantSems; ++s) {
		semctl ( this->id,s,IPC_RMID );
	}
}
