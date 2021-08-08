#include "SocketDatagrama.h"



SocketDatagrama::SocketDatagrama(int port)
{
	bzero((char *)&direccionLocal, sizeof(direccionLocal));
	_s = socket(AF_INET, SOCK_DGRAM, 0);
	direccionLocal.sin_family = AF_INET;
	direccionLocal.sin_addr.s_addr = INADDR_ANY;
	direccionLocal.sin_port = htons(port);
	bind(_s, (struct sockaddr *)&direccionLocal,sizeof(direccionLocal));
}

SocketDatagrama::SocketDatagrama(int port, char * address)
{
	bzero((char *)&direccionLocal, sizeof(direccionLocal));
	_s = socket(AF_INET, SOCK_DGRAM, 0);
	direccionLocal.sin_family = AF_INET;
	direccionLocal.sin_addr.s_addr = inet_addr(address);
	direccionLocal.sin_port = htons(port);
	bind(_s, (struct sockaddr *)&direccionLocal,sizeof(direccionLocal));
}

SocketDatagrama::SocketDatagrama()
{
	bzero((char *)&direccionLocal, sizeof(direccionLocal));
	_s = socket(AF_INET, SOCK_DGRAM, 0);
	direccionLocal.sin_family = AF_INET;
	direccionLocal.sin_addr.s_addr = INADDR_ANY;
	direccionLocal.sin_port = htons(0);
	bind(_s, (struct sockaddr *)&direccionLocal,sizeof(direccionLocal));
}

SocketDatagrama::~SocketDatagrama()
{
	shutdown(_s, 2);
}

int SocketDatagrama::recibe(PaqueteDatagrama *p)
{
	unsigned int addr_len = sizeof(direccionForanea);
	int respuesta = recvfrom(_s, p->obtieneDatos(), p->obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, &addr_len);
	p->inicializaIp(inet_ntoa(direccionForanea.sin_addr));
	p->inicializaPuerto(ntohs(direccionForanea.sin_port));
	return respuesta;
}

int SocketDatagrama::recibeTimeout(PaqueteDatagrama *p, time_t segundos, suseconds_t microsegundos)
{
	unsigned int addr_len = sizeof(direccionForanea);
	timeout.tv_sec = segundos; //2
	timeout.tv_usec = microsegundos;//500000

	setsockopt(_s, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
	int respuesta = recvfrom(_s, p->obtieneDatos(), p->obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, &addr_len);
	//cout << "Respuesta: " << respuesta << endl;
	if(respuesta < 0){
		if(errno == EWOULDBLOCK){
			fprintf(stderr, "Tiempo para recepción transcurrido\n");
			return -1;
		}		
		else{
			fprintf(stderr, "Error en recvfrom\n");
		}
			
	}
	p->inicializaIp(inet_ntoa(direccionForanea.sin_addr));
	p->inicializaPuerto(ntohs(direccionForanea.sin_port));

	return respuesta;

}


int SocketDatagrama::envia(PaqueteDatagrama *p)
{
	direccionForanea.sin_family = AF_INET;
	direccionForanea.sin_addr.s_addr = inet_addr(p->obtieneDireccion());
	direccionForanea.sin_port = htons(p->obtienePuerto());
	sendto(_s, p->obtieneDatos(), p->obtieneLongitud(), 0, (struct sockaddr *) &direccionForanea, sizeof(direccionForanea));
}