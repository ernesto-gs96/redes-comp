#ifndef __Historial__
#define __Historial__
#define TAM_MAX_DATA1 4000
//Definicion de identificadores para operaciones permitidas

// Definicion del formato de mensaje
struct historial
{
    //Identificador del usuario
    int puerto;
    char ip[16];
    //Identificador de la solicitud
    unsigned int requestId; //Identificador del mensaje
    //ultimo mensaje enviado
    int arguments[TAM_MAX_DATA1];
};

#endif
