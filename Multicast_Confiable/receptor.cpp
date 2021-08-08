#include "SocketMulticast.h"
#include "SocketDatagrama.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define TAM_MAX_DATA 650
using namespace std;

int main(int argc, char const *argv[])
{
    if(argc < 3){
        cout<<"MODO DE USO >: ./receptor <ip> <puerto>"<<endl;
        return -1;
    }

    int nbd = 0;
    int datos[2];
    char* ip = (char *) argv[1];
    int puerto = atoi(argv[2]);
    int n;
    

    SocketMulticast socketMulticast(puerto);
    PaqueteDatagrama paqueteDatagrama_Recepcion(2*sizeof(int));
    socketMulticast.unirseGrupo(ip);     

    while ( 1 ) {
        //cout << "Esperando respuesta..." << endl;
        n = socketMulticast.recibeConfiable(paqueteDatagrama_Recepcion);
        //cout << "n:" << n << endl;
        if (n == -2)
        {
            //Mensaje repetido no hacer la operacion nuevamente
        }
        else{
            memcpy(&datos,paqueteDatagrama_Recepcion.obtieneDatos(),2*sizeof(int));
            nbd = datos[0] + nbd;
            cout<< "NBD: "<<nbd << endl;
        }
    }

    socketMulticast.salirseGrupo(ip);
    return 0;
}