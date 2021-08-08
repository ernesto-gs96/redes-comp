#include "SocketMulticast.h" 
#include "SocketDatagrama.h"
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

SocketMulticast::SocketMulticast(int puerto) {

    historial[0].requestId = -1;
    s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int reuse = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) == -1) {
        printf("Error al llamar a la función setsockopt\n");
        exit(0);
    }
    bzero((char *)&direccionLocal, sizeof(direccionLocal));
    direccionLocal.sin_family = AF_INET;
    direccionLocal.sin_addr.s_addr = INADDR_ANY;
    direccionLocal.sin_port = htons(puerto);
    bind(s, (struct sockaddr *)&direccionLocal, sizeof(direccionLocal));
}

SocketMulticast::~SocketMulticast() {
    close(s);
}

void SocketMulticast::unirseGrupo(char* ip) {
    multicast.imr_multiaddr.s_addr = inet_addr(ip);
    multicast.imr_interface.s_addr = htonl(INADDR_ANY);
    setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) &multicast, sizeof(multicast));
}

void SocketMulticast::salirseGrupo(char *ip) {
    if ( multicast.imr_multiaddr.s_addr == inet_addr(ip) ) {
        setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void *) &multicast, sizeof(multicast));       
    }
}

int SocketMulticast::envia(PaqueteDatagrama &paqueteDatagrama, unsigned char ttl) {
    
    int n = setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &ttl, sizeof(ttl));

    if ( n < 0 ) {
         printf("Ha ocurrido un error al enviar el paquete \n");
    } else {
        int client = sizeof(direccionForanea);
        bzero((char *)&direccionForanea, sizeof(direccionForanea));
        direccionForanea.sin_family = AF_INET;
        direccionForanea.sin_addr.s_addr = inet_addr(paqueteDatagrama.obtieneDireccion());
        direccionForanea.sin_port = htons(paqueteDatagrama.obtienePuerto());
        sendto(s, paqueteDatagrama.obtieneDatos(), paqueteDatagrama.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, (socklen_t)client);
    }
    
}

int SocketMulticast::enviaConfiable(PaqueteDatagrama & paqueteDatagrama, unsigned char ttl, int num_receptores) {
    
    int contador = 0, id[2],n,nn = 1;

    n = setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &ttl, sizeof(ttl));
    if (n == -1){
        cout << "ERROR EN SETSCOCKOPT METODO ENVIACONFIABLE" << endl;
        cout << strerror (errno) << endl;
    }

    int client = sizeof(direccionForanea);
    bzero((char *)&direccionForanea, sizeof(direccionForanea));
    direccionForanea.sin_family = AF_INET;
    direccionForanea.sin_addr.s_addr = inet_addr(paqueteDatagrama.obtieneDireccion());
    direccionForanea.sin_port = htons(paqueteDatagrama.obtienePuerto());
    
    n = sendto(s, paqueteDatagrama.obtieneDatos(), paqueteDatagrama.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, (socklen_t)client);
    if (n == -1){
        cout << "ERROR EN SENDTO METODO ENVIACONFIABLE" << endl;
        cout << strerror (errno) << endl;
    }

    SocketDatagrama socketUnicast(7200);
    PaqueteDatagrama confirmacion(2*sizeof(int));

    for(int i = 0; i < num_receptores; i++ ){
        
        n = socketUnicast.recibeTimeout(confirmacion,2,500000);
        while (n < 0)
        {
            //cout << strerror (errno) << endl;
            //cout <<"\n"<< endl;
            if (nn == 7)
                break;
            cout << "Enviando respuesta nuevamente..." << endl;
            n = sendto(s, paqueteDatagrama.obtieneDatos(), paqueteDatagrama.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, (socklen_t)client);
            if (n == -1){
                cout << "ERROR EN SENDTO METODO ENVIACONFIABLE" << endl;
                cout << strerror (errno) << endl;
            }           
                
            n = socketUnicast.recibeTimeout(confirmacion,2,500000);
            nn++;
        }

        if (n <= 0)
        {
            cout << "ERROR EN RECIBETIMEOUT METODO ENVIACONFIABLE" << endl;
            cout << strerror (errno) << endl;
            break;
        }
        else{
            memcpy(&id,confirmacion.obtieneDatos(),2*sizeof(int));
            contador++;
        }
    }

    socketUnicast.~SocketDatagrama();

    if (num_receptores == contador) 
        return 1;
    else {
        cout << "FALTARON RESPUESTAS" << endl; 
        return -1;
    }

}

int SocketMulticast::recibe(PaqueteDatagrama &paqueteDatagrama) {
    int client = sizeof(direccionForanea);
    int n = recvfrom(s, paqueteDatagrama.obtieneDatos(), paqueteDatagrama.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, (socklen_t*)&client);
    paqueteDatagrama.inicializaIp(inet_ntoa(direccionForanea.sin_addr));
    paqueteDatagrama.inicializaPuerto(ntohs(direccionForanea.sin_port));
    
    return n;
}

int SocketMulticast::recibeConfiable(PaqueteDatagrama &paqueteDatagrama) {
    
    int datos[2];
    int aux;
    int client = sizeof(direccionForanea);
    
    int n = recvfrom(s, paqueteDatagrama.obtieneDatos(), paqueteDatagrama.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, (socklen_t*)&client);
    if (n == -1){
        cout << "Error en recvfrom del metodo recibeConfiable" << endl;
    }
    
    memcpy(&datos,paqueteDatagrama.obtieneDatos(),2 * sizeof(int));
    if (indicador == 99)
        indicador = 0;
    
    //cout << datos[1] << endl;
    for (int i = 0; i < indicador; i++){
        //cout << historial[i].requestId << " =?= "<< datos[1] << endl;
        if (datos[1] == historial[i].requestId){
            return -2;
        }
    }
    

    //cout << "Guardando mensaje en historial" << endl;
    memcpy(historial[indicador].arguments, paqueteDatagrama.obtieneDatos(), 2*sizeof(int)); //Ultimo mensaje enviado
	memcpy(historial[indicador].ip, paqueteDatagrama.obtieneDireccion(), 16);
	historial[indicador].puerto = paqueteDatagrama.obtienePuerto();
	historial[indicador++].requestId = datos[1];

    paqueteDatagrama.inicializaIp(inet_ntoa(direccionForanea.sin_addr));
    paqueteDatagrama.inicializaPuerto(ntohs(direccionForanea.sin_port));
    cout << paqueteDatagrama.obtieneDireccion() << endl;
    
    SocketDatagrama socketUnicast(7200);  
    PaqueteDatagrama confirmacion((char*)paqueteDatagrama.obtieneDatos(), paqueteDatagrama.obtieneLongitud(),paqueteDatagrama.obtieneDireccion(),7200);
    
    
    n = socketUnicast.envia(confirmacion);
    if (n == -1){
        cout << "ERROR ENVIA recibeConfiable" << endl;
    }
    
    socketUnicast.~SocketDatagrama();

    memcpy(&aux,paqueteDatagrama.obtieneDatos(),2*sizeof(int));

    return aux;
}
