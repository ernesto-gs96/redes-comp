#include "PaqueteDatagrama.h"
#include "SocketDatagrama.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{

	if(argc < 4){
		cout<<"MODO DE USO :"<<endl<<"./cliente <ip> <numero1> <numero2>"<<endl;
		return -1;

	}
	
	//Creamos una conexion el puerto 7000
	SocketDatagrama socket = SocketDatagrama(7000);
	char* ip = argv[1];
	int numeros[2];
	numeros[0] = (int)*argv[2];
	numeros[1] = (int)*argv[3];
	//Creamos un paquete datagram de 'envio'
	PaqueteDatagrama datagrama = PaqueteDatagrama((char *) numeros, 2 * sizeof(int),ip, 7001);
	socket.envia(&datagrama); //Se manda el datagrama al servidor

	//Se crea un paquete datagram de 'recibo'
	PaqueteDatagrama databack =  PaqueteDatagrama(sizeof(int));
	socket.recibe(&databack); //Se recibe el datagrama de recibo

	int * ans = (int*)databack.obtieneDatos(); //Obtenemso lo datos del datagrama de recibo
	cout<< " Dirección " << datagrama.obtieneDireccion() << " puerto: " << datagrama.obtienePuerto() << endl;
	cout << "Respuesta="<<(*ans) << endl;
	return 0;
}