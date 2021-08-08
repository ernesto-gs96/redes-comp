#ifndef __Solicitud__
#define __Solicitud__

#include "SocketDatagrama.h"
#include "Registro.h"
#include <iostream>

using namespace std;

class Solicitud {
    
public:
    Solicitud();
    char * doOperation(char *IP, int puerto, int operationId, struct registro arguments);
    
private:
    SocketDatagrama *socketlocal;
};

#endif