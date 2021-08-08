#include "SocketMulticast.h"
#include "SocketDatagrama.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define TAM_MAX_DATA 650
using namespace std;
    
int main(int argc, char const *argv[]){
    //srand (time(NULL));

    if(argc < 6){
        cout<<"MODO DE USO >: ./emisor <ip> <puerto> <ttl> <depositos> <receptores>"<<endl;
        return -1;
    }

    char* ip = (char*) argv[1];
    int puerto = atoi(argv[2]);
    int t = atoi(argv[3]);
    int numd = atoi(argv[4]); //Depoitos
    int numr = atoi(argv[5]); //Num de receptores
    unsigned char ttl = (unsigned char) t;

    int nbd = 0;
    int resp,deposito;
    int aux[2];
    int veces = 7,n;

    SocketMulticast socketMulticast(puerto);

    for(int i = 0; i < numd; i++ ){
        deposito = rand() %  9 + 1;
        cout << deposito << endl;
        aux[0] = deposito;
        aux[1] = i;// id
        PaqueteDatagrama paqueteDatagrama_Envio((char*)aux,2*sizeof(int),ip,puerto);
        int verificacion = socketMulticast.enviaConfiable(paqueteDatagrama_Envio,ttl,numr);
        cout << "Estamos en: " << i << endl;
        if ( verificacion == -1 ) {
            cout << "Ha ocurrido un errror" << endl;
            break;
        }
                  
    }

    return 0;
}