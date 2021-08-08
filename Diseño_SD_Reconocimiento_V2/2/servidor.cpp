#include "PaqueteDatagrama.h"
#include "SocketDatagrama.h"
#include "Respuesta.h"
#include "Mensaje.h"
#include "Registro.h"
#include <cstdlib>
#include <vector>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream> 
#include <iterator> 
#include <fstream>
#include <fcntl.h>

#define TAM_MAX 34
char buffer[TAM_MAX];

using namespace std;

int main(int argc, char *argv[])
{

  if(argc < 2){
		cout<<"MODO DE USO :"<<endl<<"./servidor <archivo_destino>"<<endl;
		return -1;
	}

  char acuse[25] = "VOTO REGISTRADO";
  char acuse2[25] = "VOTO YA ENVIADO";
  int p = 7671;
  Respuesta respuesta(p);
  vector<struct registro> registros_vector; 
  vector<struct registro>::iterator it = registros_vector.begin();
  int destino;

  if ((destino = open(argv[1], O_WRONLY|O_TRUNC|O_CREAT,0666)) == -1){
    perror(argv[1]);
    exit(-1);
  }

  while (1){
    struct mensaje mssgRecibido;
    struct mensaje messgEnviar;

    memcpy(&mssgRecibido, respuesta.getRequest(), sizeof(struct mensaje));

    //VOTO YA REPETIDO
    if (mssgRecibido.operationId == 2){
      cout << "ID :> " << mssgRecibido.requestId << endl;
      //2 VOTO YA ENVIADO
        cout << "Voto: " << mssgRecibido.registro.celular << mssgRecibido.registro.CURP << mssgRecibido.registro.partido << endl;
       //MENSAJE DE VOTO REGISTRADO
        memcpy(messgEnviar.arguments, acuse2, sizeof(acuse2));
        messgEnviar.messageType = 1; //1 RESPUESTA
        memcpy(messgEnviar.ip, mssgRecibido.ip, 16);
        messgEnviar.puerto = mssgRecibido.puerto;
        messgEnviar.requestId = mssgRecibido.requestId;
        respuesta.sendReply((char *)messgEnviar.arguments, messgEnviar.ip, mssgRecibido.puerto);
        cout << "-----------------------------------------" << endl;
      
    }
    else
    {
      cout << "ID :> " << mssgRecibido.requestId << endl;
      //1 VOTO 
      if (mssgRecibido.operationId == 1){
        cout << "Voto: " << mssgRecibido.registro.celular << mssgRecibido.registro.CURP << mssgRecibido.registro.partido << endl;
        //ESCRIBIR EN EL ARCHIVO
        write(destino, mssgRecibido.registro.celular, sizeof(mssgRecibido.registro.celular));
        write(destino, mssgRecibido.registro.CURP, sizeof(mssgRecibido.registro.CURP));
        write(destino, mssgRecibido.registro.partido, sizeof(mssgRecibido.registro.partido));
        //MENSAJE DE VOTO REGISTRADO
        memcpy(messgEnviar.arguments, acuse, sizeof(acuse));
        messgEnviar.messageType = 1; //1 RESPUESTA
        memcpy(messgEnviar.ip, mssgRecibido.ip, 16);
        messgEnviar.puerto = mssgRecibido.puerto;
        messgEnviar.requestId = mssgRecibido.requestId;
        respuesta.sendReply((char *)messgEnviar.arguments, messgEnviar.ip, mssgRecibido.puerto);
        cout << "-----------------------------------------" << endl;
      }
    }
  }
  return 0;
}
