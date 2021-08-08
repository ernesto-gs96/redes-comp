#include "PaqueteDatagrama.h"
#include "SocketDatagrama.h"
#include <cstdlib>
#include <iostream>
#include <string.h>

using namespace std;

int main(int argc, char* argv[]){
  //Se crea un socket para recibir informacipn en el puerto 7001
  SocketDatagrama socket = SocketDatagrama(7200);
	int parametros[2];

	while(1){
    //Se crea un datagrama de 'recibo'
		PaqueteDatagrama datagrama = PaqueteDatagrama(2 * sizeof(int));
		socket.recibe(&datagrama); //Se recibe el datagrama

    //Obtenemos los datos de datagrama y los copiamos
		memcpy(parametros, datagrama.obtieneDatos(), 2 * sizeof(int));

        int num1 = parametros[0];
        int num2 = parametros[1];
        cout<<"Numero 1 recibido: "<<num1<<" Numero 2 recibido:"<<num2<<endl;
        int res=num1+num2;
        //Del dataframa recibido obtenemos, datos, puerto y direccion IP
        cout << " Dirección " << datagrama.obtieneDireccion() <<" "<< "Puerto: " << datagrama.obtienePuerto() << endl;
        //Creamos un datagrama de 'envio' con la respuesta, el tamaño de la respuesta, dir IP y puerto de quien envia
        PaqueteDatagrama databack =  PaqueteDatagrama((char*) &res, sizeof(bool), datagrama.obtieneDireccion(), datagrama.obtienePuerto());
	      socket.envia(&databack); //Enviamos el paquete de envio
    }
    return 0;
}