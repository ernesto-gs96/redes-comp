#ifndef __SOCKETDATAGRAMA_H__
#define __SOCKETDATAGRAMA_H__

#include "PaqueteDatagrama.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <cstring>
#include <errno.h>
#include <iostream>
using namespace std;

class SocketDatagrama{
	public:
		SocketDatagrama();
		SocketDatagrama(int);
		SocketDatagrama(int, char *);
		~SocketDatagrama();
		//Recibe un paquete tipo datagrama proveniente de este socket
		int recibe(PaqueteDatagrama *p);
		//Envía un paquete tipo datagrama desde este socket
		int envia(PaqueteDatagrama *p);
		//
		int recibeTimeout(PaqueteDatagrama *p, time_t segundos, suseconds_t microsegundos);
	
	private:
		struct sockaddr_in direccionLocal;
		struct sockaddr_in direccionForanea;
		int _s; //ID socket
		struct timeval timeout;
};

#endif