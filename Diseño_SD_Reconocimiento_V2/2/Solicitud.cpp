#include "SocketDatagrama.h"
#include "Solicitud.h"
#include "Mensaje.h"
#include <string.h>

unsigned int numSol = 0;

Solicitud::Solicitud() {
	socketlocal = new SocketDatagrama(0);
}

char * Solicitud::doOperation(char* ip, int puerto, int operationId, struct registro arguments) {
	
	struct mensaje msj;
	struct mensaje *msj2;

	//RESETEO DE IDENTIFICADOR DE OPERACION
	if (numSol == 32767)
		numSol = 0;
	
	//VARIABLES PARA TIMEOUT
	int cont = 0;
	int bytes = -1;

	msj.messageType = 0; //0 PETICION
	msj.requestId = numSol;
	memcpy(msj.ip, ip, 16);
	msj.puerto = puerto;
	msj.operationId = operationId;
	
	memcpy(&msj.registro,&arguments,sizeof(arguments));
	cout << "Mi voto: " << msj.registro.celular << msj.registro.CURP << msj.registro.partido << endl;
	//memcpy(msj.registro, &arguments, sizeof(arguments));
	//cout << msj.registro << endl;
	//cout << "ERROR" << endl;
	//msj.registro
	//cout << "Id operacion: " << numSol << endl;
	//cout << "ip: " << msj.ip << endl;
	//cout << "Mi voto: " << arguments << ", tam:" << sizeof(arguments) <<endl;
	//cout << "Voto: " << msj.arguments << ", tam:" << sizeof(msj.arguments) <<endl;

	PaqueteDatagrama paq((char*) &msj, sizeof(msj), ip, puerto);

	/* Utilizar recibetimeot para que reenvie la misma solicitud hasta 7 veces, si no se logra
	termina e imprime servidor no disponible */
	while (cont < 7 && bytes == -1) {

		socketlocal -> envia(&paq);
		PaqueteDatagrama paq1(sizeof(msj));
		bytes = socketlocal -> recibeTimeout(&paq1, 2, 500000);

		if(bytes > 1){
			char *response;
			response = paq1.obtieneDatos();
			//memcpy(response, msj2->arguments, sizeof(msj2->arguments));
			cout << response << endl;
			//cout << "Voto enviado -> " << resultado << endl;
			numSol++;
			return response;
		}
		else
			cont ++;

		if(cont == 7){
			cout << "Servidor no disponible, intenta mas tarde" << endl;
			exit(1);
		}
		
	}
}
