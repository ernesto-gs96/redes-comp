#ifndef __Mensaje__
#define __Mensaje__
#include "Registro.h"
//Definicion de identificadores para operaciones permitidas

// Definicion del formato de mensaje
struct mensaje
{
    int puerto;
    char ip[16];
    int messageType;        //0 = Solicitud, 1 = Respuesta
    unsigned int requestId; //Identificador del mensaje
    int operationId;        //Identificador de la operación
    int arguments[1];      //cambiado a 1
    struct registro registro;
};

#endif
