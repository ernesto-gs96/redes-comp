#include "SocketDatagrama.h"
#include "Respuesta.h"
#include "Registro.h"
#include<iostream>
#include<string.h>
using namespace std;

PaqueteDatagrama dp(sizeof(mensaje));

Respuesta::Respuesta(int puerto) {
	socketlocal = new SocketDatagrama(puerto);
	indicador = 0;
	historial[0].requestId = -1;
}

struct mensaje* Respuesta::getRequest() {

	//RESETEO DE IDENTIFICADOR DE OPERACION
	if (indicador == 99)
        indicador = 0;

    // Creamos un datagrama de 'recibo'
	socketlocal->recibe(&dp);

    //Pasamos los datos    
	contenido = (struct mensaje*) dp.obtieneDatos();
	contenido->puerto = dp.obtienePuerto();
	memcpy(contenido->ip,dp.obtieneDireccion(),16);
	//unsigned int reqId = 0;
	//memcpy(&reqId, &contenido->requestId, sizeof(contenido->requestId));

	for (int i = 0; i < indicador; i++){
        if (contenido->requestId == historial[i].requestId){
			cout << "Paquete repetido..." << endl;
			contenido->operationId = 2;
            return contenido;
        }
    }
	
	memcpy( historial[indicador].arguments, dp.obtieneDatos(), dp.obtieneLongitud());
	memcpy(historial[indicador].ip, dp.obtieneDireccion(), 16);
	historial[indicador].puerto = dp.obtienePuerto();
	historial[indicador++].requestId = contenido->requestId;

	return contenido;
	
}

void Respuesta::sendReply(char * respuesta, char * ipCliente, int puertoCliente) {
	struct mensaje *m1;
	m1 = (struct mensaje *) respuesta;
	cout << "Ip Cliente >: " << ipCliente << endl;
	PaqueteDatagrama datagramaEnvio((char*) m1, sizeof(m1), ipCliente, puertoCliente);
	socketlocal->envia(&datagramaEnvio);
}
