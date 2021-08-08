#ifndef __Respuesta__
#define __Respuesta__

#include "SocketDatagrama.h"
#include "Mensaje.h"
#include "Historial.h"

using namespace std;

class Respuesta
{

  public:
    Respuesta(int pl);
    struct mensaje *getRequest(void);
    void sendReply(char *respuesta, char *ipCliente, int puertoCliente);

  private:
    SocketDatagrama *socketlocal;
    struct mensaje *contenido;
    unsigned int numSol;
    struct historial historial[100];
    int indicador = 0;
};

#endif
