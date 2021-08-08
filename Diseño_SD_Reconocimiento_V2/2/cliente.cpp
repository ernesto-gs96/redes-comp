#include "Solicitud.h"
#include "PaqueteDatagrama.h"
#include "SocketDatagrama.h"
#include "Registro.h"
#include <cstdlib>
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

int main(int argc, char *argv[]){

	if(argc < 3){
		cout<<"MODO DE USO :"<<endl<<"./cliente <ip> <archivo_origen> <numero_de_votos>"<<endl;
		return -1;
	}

	char* response;
	char* ip = argv[1];
	int puerto = 7671, n = atoi(argv[3]);
	Solicitud cli;
	struct registro reg;
	int origen, nbytes;

	if((origen = open(argv[2],O_RDONLY)) == -1){
        perror(argv[1]);
        exit(-1);
    }

	for(int i = 0; i < n; i++){

		//ENCONTRAR UTLIDAD A nbytes
		read(origen,reg.celular,sizeof(reg.celular));
		read(origen,reg.CURP,sizeof(reg.CURP));
		read(origen,reg.partido,sizeof(reg.partido));
		//cout << reg.celular << reg.CURP << reg.partido << endl;
		response = cli.doOperation(ip,puerto,1,reg);
		cout << response << endl;
		cout <<"------------------------------------------------------------" << endl;
	}

	
	return 0;
}
