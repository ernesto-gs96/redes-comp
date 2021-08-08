#include "SocketDatagrama.h"
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>


using namespace std;

SocketDatagrama::SocketDatagrama(int puerto){
    int _aux;
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == -1)
    {
        cout << "ERROR EN SOCKET EN EL CONSTRUCTOR SOCKETDATAGRAMA" << endl;
        cout << strerror (errno) << endl;
    }
    bzero((char *)&direccionLocal, sizeof(direccionLocal));
    direccionLocal.sin_family = AF_INET;
    direccionLocal.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY inet_addr(argv[1])
    direccionLocal.sin_port = htons(puerto);
    _aux = bind(s, (struct sockaddr *)&direccionLocal, sizeof(direccionLocal));
    if (_aux == -1)
    {
        cout << "ERROR EN bind EN EL CONSTRUCTOR SOCKETDATAGRAMA" << endl;
        cout << strerror (errno) << endl;
    }
    
}

SocketDatagrama::~SocketDatagrama(){
    close(s);
}

int SocketDatagrama::envia(PaqueteDatagrama &paqueteDatagrama){
    int client = sizeof(direccionForanea);
    bzero((char *)&direccionForanea, sizeof(direccionForanea));
    direccionForanea.sin_family = AF_INET;
    direccionForanea.sin_addr.s_addr = inet_addr(paqueteDatagrama.obtieneDireccion());
    direccionForanea.sin_port = htons(paqueteDatagrama.obtienePuerto());
    int res=sendto(s, paqueteDatagrama.obtieneDatos(), paqueteDatagrama.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, (socklen_t)client);
    if(res == -1){
        cout<<"No se envio"<<endl;
        return -1;
    }else{
        cout<<"Se envio"<<endl;
        return 1;
    }
}

int SocketDatagrama::recibe(PaqueteDatagrama &paqueteDatagrama){
    int client = sizeof(direccionForanea);
    int n = recvfrom(s, paqueteDatagrama.obtieneDatos(), paqueteDatagrama.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, (socklen_t*)&client);
    paqueteDatagrama.inicializaIp(inet_ntoa(direccionForanea.sin_addr));
    paqueteDatagrama.inicializaPuerto(ntohs(direccionForanea.sin_port));
    
    if(n<0){
        cout<<"No se recibio"<<endl;
    }else{
        cout<<"Se recibio"<<endl;
    }
    return n;
}

int SocketDatagrama::recibeTimeout(PaqueteDatagrama & p, time_t segundos, suseconds_t microsegundos) {
    
    int n;
    int client = sizeof(direccionForanea);
    timeval.tv_sec = segundos;
    timeval.tv_usec = microsegundos;
    
    n = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeval, sizeof(timeval));
    if (n == -1)
    {
        cout << "ERROR EN SETSCOCKOPT METODO RECIBETIMEOUT" << endl;
        cout << strerror (errno) << endl;
    }

    n = recvfrom(s,p.obtieneDatos(), p.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, (socklen_t*)&client);
    if(n < 0){
		if(errno == EWOULDBLOCK)
		{
			fprintf(stderr, "Tiempo para recepción transcurrido\n");
			return -1;
		}
		else
		{
			fprintf(stderr, "Error en recvfrom\n");
            return 0;
		}
			
	}

    p.inicializaIp(inet_ntoa(direccionForanea.sin_addr));
    p.inicializaPuerto(ntohs(direccionForanea.sin_port));
    //cout << p.obtieneDireccion() << endl;

    return 1;
    
    /*int *conf = (int*)p.obtieneDatos();
    //cout << "conf:" << *conf << endl;
    if (conf > 0)
        return 1;
    else
        return -2;*/
    
}

struct sockaddr_in SocketDatagrama::getDirForanea() {
    return this->direccionForanea;
}
