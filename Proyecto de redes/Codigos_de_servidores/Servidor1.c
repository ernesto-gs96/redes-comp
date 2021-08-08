//
//  main.c
//  Cliente_drive
//
//  Created by Ricardo Aandres Jimenez Rodriguez on 10/8/19.
//  Copyright © 2019 Ricardo Aandres Jimenez Rodriguez. All rights reserved.
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8060
int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
      long int valread=0;
      struct sockaddr_in address;
      int opt = 1;
      int addrlen = sizeof(address);
     int opciones=0,listo=0;
     
       
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,&opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
         
        char direccion[2048]={"/Users/Richi/Documents/Documents/Documentos/Redes/Servidores_fragmentados/Servidor1/"};
        long long int tamano=0;
        int entero_reciv=0;
        FILE *archivo=NULL;
        char nombre_del_archivo[1024]={0};
        
        recv(new_socket ,&opciones, sizeof(int),0);
        if(opciones<=0||opciones>=4)
        {
            continue;
        }
        if(opciones==1)
        {
            recv( new_socket ,nombre_del_archivo, 1024,0);
            strcat(direccion, nombre_del_archivo);
            archivo=fopen(direccion, "wb+");
            send(new_socket , &listo , sizeof(int) , 0 );
            while(1)
            {
                char cad_contenido_archivo[1024]={0};
                recv( new_socket ,&entero_reciv,sizeof(int),0);
                if(entero_reciv==2)
                {
                    break;
                }
                else{
                    send(new_socket , &listo , sizeof(int) , 0 );
                    recv( new_socket ,cad_contenido_archivo, 1024,0);
                    send(new_socket , &listo , sizeof(int) , 0 );
                    fwrite(cad_contenido_archivo, 1, 1024, archivo);
                }
            }
            fclose(archivo);
        }
        else if(opciones==2)
        {
            long int salir_archivo;
            int entero_a_enviar=0;
            int con=0;
            char cad_contenido_archivo[1024]={0};
            recv( new_socket ,nombre_del_archivo, 1024,0);
            send(new_socket , &listo , sizeof(int) , 0 );
            strcat(direccion, nombre_del_archivo);
            archivo=fopen(direccion, "rb");
            
            while(1){
                char cad_contenido_archivo1[1024]={0};
                salir_archivo=fread(cad_contenido_archivo1, 1, 1024, archivo);
                if(salir_archivo!=0)
                {
                    send(new_socket, &entero_a_enviar, sizeof(int), 0);
                    recv(new_socket, &listo,sizeof(int),0);
                    send(new_socket, cad_contenido_archivo1, 1024, 0 );
                    recv(new_socket, &listo,sizeof(int),0);
                }
                else{
                    break;
                }
            }
            entero_a_enviar=2;
            send(new_socket, &entero_a_enviar, sizeof(int), 0);
            fclose(archivo);
        }
        else if(opciones==3)
        {
            break;
        }
    }
    close(new_socket);
    return 0;
}
