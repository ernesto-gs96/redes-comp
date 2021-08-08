//
//  main.c
//  Cliente fragmentado
//
//  Created by Ricardo Aandres Jimenez Rodriguez on 26/11/19.
//  Copyright © 2019 Ricardo Aandres Jimenez Rodriguez. All rights reserved.
//

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define PORT1 8060
#define PORT2 8070
#define PORT3 8080
   
int main(int argc, char const *argv[])
{
    int sock = 0,sock1=0,sock2=0;
    long long int valread=0;
    struct sockaddr_in serv_addr;
    char datos[1024] = {0};
    srand(time(NULL));
    //Primer servidor
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT1);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    //Segundo servidor
    if ((sock1 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
     {
         printf("\n Socket creation error \n");
         return -1;
     }
    
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_port = htons(PORT2);
        
     // Convert IPv4 and IPv6 addresses from text to binary form
     if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
     {
         printf("\nInvalid address/ Address not supported \n");
         return -1;
     }
    
     if (connect(sock1, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
     {
         printf("\nConnection Failed \n");
         return -1;
     }

    //Tercer servidor
    if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
     {
         printf("\n Socket creation error \n");
         return -1;
     }
    
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_port = htons(PORT3);
        
     // Convert IPv4 and IPv6 addresses from text to binary form
     if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
     {
         printf("\nInvalid address/ Address not supported \n");
         return -1;
     }
    
     if (connect(sock2, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
     {
         printf("\nConnection Failed \n");
         return -1;
     }
    
    while(1)
    {
        int opciones=0;
        printf("1)Subir archivo\n2)Recuperar archivo\n3)Salir\n");
        scanf("%d",&opciones);
        
        send(sock , &opciones , sizeof(int), 0 );
        send(sock1 , &opciones ,sizeof(int), 0 );
        send(sock2 , &opciones ,sizeof(int), 0 );
        if(opciones<=0||opciones>=4)
        {
            continue;
        }
        if(opciones==1)
        {
                   FILE *archivo=NULL;
                   int entero_a_enviar=0, aleatorio=0,listo=0;
                   FILE *fich,*recuperacion;
                   long int salir_archivo=0;
                   long long int tamano=0,pedazos=0,checar=0,faltantes=0,final=0,tamano_1=0,tamano_2=0,tamano_3=0,cont=0,veces=0;
                   char cadena_enviar[1024]={0};
                   char *cad_env1,*cad_env2,*cad_env3;
                   char primera_opcion[]={"\n123"};
                   char segunda_opcion[]={"\n213"};
                   char tercera_opcion[]={"\n321"};
                   char nombre_archivo[1024]={0};
                   char variable_intermedia[1024]={0};
                   char *nombre_archivo_recup;
                   char recup_dir[2048]={"/Users/Richi/Documents/Documents/Documentos/Redes/Recuperaciones/"};
                   char direcion[1024]={0};
                       printf("Ingrese la ruta del archivo a subir\n");
                       scanf("%s",direcion);
                       
                       printf("Ingrese el nombre del archivo a subir\n");
                       scanf("%s",nombre_archivo);
                       fich=fopen(direcion, "rb");
                       fseek(fich, 0, SEEK_END);
                       tamano=ftell(fich);
                       pedazos=tamano/3;
                       checar=tamano-(pedazos*3);
                       faltantes=pedazos+checar;
                       tamano_1=pedazos;
                       tamano_2=pedazos;
                       tamano_3=faltantes;
                       veces=(pedazos/1024);
                       rewind(fich);
                       aleatorio=1+rand()%(4-1);
                       if(aleatorio==1)
                       {
                           //enviamos primero al del puerto 8060
                           send(sock , nombre_archivo , 1024, 0 );
                           recv(sock, &listo,sizeof(int),0);
                           
                           while(cont<veces){
                               fread(cadena_enviar, 1, 1024, fich);
                               send(sock, &entero_a_enviar, sizeof(int), 0);
                               recv(sock, &listo,sizeof(int),0);
                               send(sock, cadena_enviar , 1024, 0 );
                               recv(sock, &listo,sizeof(int),0);
                               //fread(cadena_enviar, 1, 1024, fich);
                               cont++;
                           }
                           entero_a_enviar=2;
                           send(sock, &entero_a_enviar, sizeof(int), 0);
                           entero_a_enviar=0;
                           cont=0;
                           //enviamos al del puerto 8070
                           send(sock1, nombre_archivo , 1024, 0 );
                           recv(sock1, &listo,sizeof(int),0);
                           
                           while(cont<veces){
                               fread(cadena_enviar, 1, 1024, fich);
                               send(sock1, &entero_a_enviar, sizeof(int), 0);
                               recv(sock1, &listo,sizeof(int),0);
                               send(sock1, cadena_enviar , 1024, 0 );
                               recv(sock1, &listo,sizeof(int),0);
                               cont++;
                           }
                           entero_a_enviar=2;
                           send(sock1, &entero_a_enviar, sizeof(int), 0);
                           entero_a_enviar=0;
                           
                           //enviamos al del puerto 8080
                           send(sock2, nombre_archivo , 1024, 0 );
                           recv(sock2, &listo,sizeof(int),0);

                           while(1){
                               char cadena_enviar1[1024]={0};
                               salir_archivo=fread(cadena_enviar1, 1, 1024, fich);
                               if(salir_archivo!=0)
                               {
                                    send(sock2, &entero_a_enviar, sizeof(int), 0);
                                    recv(sock2, &listo,sizeof(int),0);
                                    send(sock2, cadena_enviar1 , 1024, 0 );
                                    recv(sock2, &listo,sizeof(int),0);
                               }
                               else{
                                   break;
                               }
                           }
                           entero_a_enviar=2;
                           send(sock2, &entero_a_enviar, sizeof(int), 0);
                           fclose(fich);
    
                           //Creacion archivos para recuperar
                           strcpy(variable_intermedia, nombre_archivo);
                           nombre_archivo_recup=strtok(nombre_archivo,".");
                           strcat(nombre_archivo_recup, ".txt");
                           strcat(recup_dir, nombre_archivo_recup);
                           recuperacion=fopen(recup_dir, "w");
                           fputs(variable_intermedia, recuperacion);
                           fputs(primera_opcion, recuperacion);
                           fclose(recuperacion);
                       }
                       else if(aleatorio==2)
                       {
                           
                           //enviamos primero al del puerto 8070
                                send(sock1 , nombre_archivo , 1024, 0 );
                                recv(sock1, &listo,sizeof(int),0);
                                       
                                       while(cont<veces){
                                           fread(cadena_enviar, 1, 1024, fich);
                                           send(sock1, &entero_a_enviar, sizeof(int), 0);
                                           recv(sock1, &listo,sizeof(int),0);
                                           send(sock1, cadena_enviar , 1024, 0 );
                                           recv(sock1, &listo,sizeof(int),0);
                                           //fread(cadena_enviar, 1, 1024, fich);
                                           cont++;
                                       }
                                       entero_a_enviar=2;
                                       send(sock1, &entero_a_enviar, sizeof(int), 0);
                                       entero_a_enviar=0;
                                       cont=0;
                                       //enviamos al del puerto 8060
                                       send(sock, nombre_archivo , 1024, 0 );
                                       recv(sock, &listo,sizeof(int),0);
                                       
                                       while(cont<veces){
                                           fread(cadena_enviar, 1, 1024, fich);
                                           send(sock, &entero_a_enviar, sizeof(int), 0);
                                           recv(sock, &listo,sizeof(int),0);
                                           send(sock, cadena_enviar , 1024, 0 );
                                           recv(sock, &listo,sizeof(int),0);
                                           cont++;
                                       }
                                       entero_a_enviar=2;
                                       send(sock, &entero_a_enviar, sizeof(int), 0);
                                       entero_a_enviar=0;
                                       
                                       //enviamos al del puerto 8080
                                       send(sock2, nombre_archivo , 1024, 0 );
                                       recv(sock2, &listo,sizeof(int),0);

                                       while(1){
                                           char cadena_enviar1[1024]={0};
                                           salir_archivo=fread(cadena_enviar1, 1, 1024, fich);
                                           if(salir_archivo!=0)
                                           {
                                                send(sock2, &entero_a_enviar, sizeof(int), 0);
                                                recv(sock2, &listo,sizeof(int),0);
                                                send(sock2, cadena_enviar1 , 1024, 0 );
                                                recv(sock2, &listo,sizeof(int),0);
                                           }
                                           else{
                                               break;
                                           }
                                       }
                                       entero_a_enviar=2;
                                       send(sock2, &entero_a_enviar, sizeof(int), 0);
                                       fclose(fich);
                
                                       //Creacion archivos para recuperar
                                       strcpy(variable_intermedia, nombre_archivo);
                                       nombre_archivo_recup=strtok(nombre_archivo,".");
                                       strcat(nombre_archivo_recup, ".txt");
                                       strcat(recup_dir, nombre_archivo_recup);
                                       recuperacion=fopen(recup_dir, "w");
                                       fputs(variable_intermedia, recuperacion);
                                       fputs(segunda_opcion, recuperacion);
                                       fclose(recuperacion);
                       }
                       else if(aleatorio==3)
                       {
                           //enviamos primero al del puerto 8080
                                                  send(sock2 , nombre_archivo , 1024, 0 );
                                                  recv(sock2, &listo,sizeof(int),0);
                                                  
                                                  while(cont<veces){
                                                      fread(cadena_enviar, 1, 1024, fich);
                                                      send(sock2, &entero_a_enviar, sizeof(int), 0);
                                                      recv(sock2, &listo,sizeof(int),0);
                                                      send(sock2, cadena_enviar , 1024, 0 );
                                                      recv(sock2, &listo,sizeof(int),0);
                                                      //fread(cadena_enviar, 1, 1024, fich);
                                                      cont++;
                                                  }
                                                  entero_a_enviar=2;
                                                  send(sock2, &entero_a_enviar, sizeof(int), 0);
                                                  entero_a_enviar=0;
                                                  cont=0;
                                                  //enviamos al del puerto 8070
                                                  send(sock1, nombre_archivo , 1024, 0 );
                                                  recv(sock1, &listo,sizeof(int),0);
                                                  
                                                  while(cont<veces){
                                                      fread(cadena_enviar, 1, 1024, fich);
                                                      send(sock1, &entero_a_enviar, sizeof(int), 0);
                                                      recv(sock1, &listo,sizeof(int),0);
                                                      send(sock1, cadena_enviar , 1024, 0 );
                                                      recv(sock1, &listo,sizeof(int),0);
                                                      cont++;
                                                  }
                                                  entero_a_enviar=2;
                                                  send(sock1, &entero_a_enviar, sizeof(int), 0);
                                                  entero_a_enviar=0;
                                                  
                                                  //enviamos al del puerto 8060
                                                  send(sock, nombre_archivo , 1024, 0 );
                                                  recv(sock, &listo,sizeof(int),0);

                                                  while(1){
                                                      char cadena_enviar1[1024]={0};
                                                      salir_archivo=fread(cadena_enviar1, 1, 1024, fich);
                                                      if(salir_archivo!=0)
                                                      {
                                                           send(sock, &entero_a_enviar, sizeof(int), 0);
                                                           recv(sock, &listo,sizeof(int),0);
                                                           send(sock, cadena_enviar1 , 1024, 0 );
                                                           recv(sock, &listo,sizeof(int),0);
                                                      }
                                                      else{
                                                          break;
                                                      }
                                                  }
                                                  entero_a_enviar=2;
                                                  send(sock, &entero_a_enviar, sizeof(int), 0);
                                                  fclose(fich);
                           
                                                  //Creacion archivos para recuperar
                                                  strcpy(variable_intermedia, nombre_archivo);
                                                  nombre_archivo_recup=strtok(nombre_archivo,".");
                                                  strcat(nombre_archivo_recup, ".txt");
                                                  strcat(recup_dir, nombre_archivo_recup);
                                                  recuperacion=fopen(recup_dir, "w");
                                                  fputs(variable_intermedia, recuperacion);
                                                  fputs(tercera_opcion, recuperacion);
                                                  fclose(recuperacion);
                       }
               }
        else if(opciones==2)
        {
            int listo=0;
            FILE *fich,*recuperacion;
            long long int tamano_1=0,tamano_2=0,tamano_3=0;
            char *cad_reciv1,*cad_reciv2,*cad_reciv3;
            char *nombresito;
            char primera_opcion[]={"123"};
            char segunda_opcion[]={"213"};
            char tercera_opcion[]={"321"};
            char nombre_archivo[1024]={0};
            char nombre_archivo_enviar[1024]={0};
            char *nombre_archivo_recup;
            char Nombre_archivo_final[100]={0};
            char Tipo_de_recuperacion[4]={0};
            char recup_dir[2048]={"/Users/Richi/Documents/Documents/Documentos/Redes/Recuperaciones/"};
            char Archivos_recuperados[2048]={"/Users/Richi/Documents/Documents/Documentos/Redes/Servidores_fragmentados/Archivos_recuperados/"};
            printf("Ingrese el nombre del archivo a recuperar\n");
            scanf("%s",nombre_archivo);

            strcpy(nombre_archivo_enviar, nombre_archivo);
            nombre_archivo_recup=strtok(nombre_archivo, ".");
            strcat(nombre_archivo_recup, ".txt");
            strcat(recup_dir, nombre_archivo_recup);
            recuperacion=fopen(recup_dir, "r");
            fgets(Nombre_archivo_final, 100, recuperacion);
            fgets(Tipo_de_recuperacion, 4, recuperacion);
            nombresito=strtok(Nombre_archivo_final, "\n");
            //printf("%s\n",Tipo_de_recuperacion);
            if(strcmp(Tipo_de_recuperacion, primera_opcion)==0)
            {
                int entero_reciv=0;
                strcat(Archivos_recuperados, nombresito);
                fich=fopen(Archivos_recuperados, "wb+");
                send(sock, nombre_archivo_enviar , 1024, 0 );
                recv(sock, &listo,sizeof(int),0);
                while(1)
                {
                    char cad_contenido_archivo[1024]={0};
                    recv( sock ,&entero_reciv,sizeof(int),0);
                    if(entero_reciv==2)
                    {
                        break;
                    }
                    else{
                        send(sock, &listo , sizeof(int) , 0 );
                        recv(sock,cad_contenido_archivo, 1024,0);
                        send(sock, &listo , sizeof(int) , 0 );
                        fwrite(cad_contenido_archivo, 1, 1024, fich);
                    }
                }
                entero_reciv=0;
                
                send(sock1, nombre_archivo_enviar , 1024, 0 );
                recv(sock1, &listo,sizeof(int),0);
                while(1)
                {
                    char cad_contenido_archivo[1024]={0};
                    recv( sock1 ,&entero_reciv,sizeof(int),0);
                    if(entero_reciv==2)
                    {
                        break;
                    }
                    else{
                        send(sock1, &listo , sizeof(int) , 0 );
                        recv(sock1,cad_contenido_archivo, 1024,0);
                        send(sock1, &listo , sizeof(int) , 0 );
                        fwrite(cad_contenido_archivo, 1, 1024, fich);
                    }
                }
                entero_reciv=0;
                
                send(sock2, nombre_archivo_enviar , 1024, 0 );
                recv(sock2, &listo,sizeof(int),0);
                while(1)
                {
                    char cad_contenido_archivo[1024]={0};
                    recv( sock2 ,&entero_reciv,sizeof(int),0);
                    if(entero_reciv==2)
                    {
                        break;
                    }
                    else{
                        send(sock2, &listo , sizeof(int) , 0 );
                        recv(sock2,cad_contenido_archivo, 1024,0);
                        send(sock2, &listo , sizeof(int) , 0 );
                        fwrite(cad_contenido_archivo, 1, 1024, fich);
                    }
                }
                entero_reciv=0;
                fclose(fich);
            }
            else if(strcmp(Tipo_de_recuperacion, segunda_opcion)==0)
            {
                int entero_reciv=0;
                strcat(Archivos_recuperados, nombresito);
                fich=fopen(Archivos_recuperados, "wb+");
                send(sock1, nombre_archivo_enviar , 1024, 0 );
                recv(sock1, &listo,sizeof(int),0);
                while(1)
                {
                    char cad_contenido_archivo[1024]={0};
                    recv( sock1 ,&entero_reciv,sizeof(int),0);
                    if(entero_reciv==2)
                    {
                        break;
                    }
                    else{
                        send(sock1, &listo , sizeof(int) , 0 );
                        recv(sock1,cad_contenido_archivo, 1024,0);
                        send(sock1, &listo , sizeof(int) , 0 );
                        fwrite(cad_contenido_archivo, 1, 1024, fich);
                    }
                }
                entero_reciv=0;
                
                send(sock, nombre_archivo_enviar , 1024, 0 );
                recv(sock, &listo,sizeof(int),0);
                while(1)
                {
                    char cad_contenido_archivo[1024]={0};
                    recv( sock ,&entero_reciv,sizeof(int),0);
                    if(entero_reciv==2)
                    {
                        break;
                    }
                    else{
                        send(sock, &listo , sizeof(int) , 0 );
                        recv(sock,cad_contenido_archivo, 1024,0);
                        send(sock, &listo , sizeof(int) , 0 );
                        fwrite(cad_contenido_archivo, 1, 1024, fich);
                    }
                }
                entero_reciv=0;
                
                send(sock2, nombre_archivo_enviar , 1024, 0 );
                recv(sock2, &listo,sizeof(int),0);
                while(1)
                {
                    char cad_contenido_archivo[1024]={0};
                    recv( sock2 ,&entero_reciv,sizeof(int),0);
                    if(entero_reciv==2)
                    {
                        break;
                    }
                    else{
                        send(sock2, &listo , sizeof(int) , 0 );
                        recv(sock2,cad_contenido_archivo, 1024,0);
                        send(sock2, &listo , sizeof(int) , 0 );
                        fwrite(cad_contenido_archivo, 1, 1024, fich);
                    }
                }
                entero_reciv=0;
                fclose(fich);
                
            }
            else if(strcmp(Tipo_de_recuperacion, tercera_opcion)==0)
            {
                int entero_reciv=0;
                strcat(Archivos_recuperados, nombresito);
                fich=fopen(Archivos_recuperados, "wb+");
                send(sock2, nombre_archivo_enviar , 1024, 0 );
                recv(sock2, &listo,sizeof(int),0);
                while(1)
                {
                    char cad_contenido_archivo[1024]={0};
                    recv( sock2 ,&entero_reciv,sizeof(int),0);
                    if(entero_reciv==2)
                    {
                        break;
                    }
                    else{
                        send(sock2, &listo , sizeof(int) , 0 );
                        recv(sock2,cad_contenido_archivo, 1024,0);
                        send(sock2, &listo , sizeof(int) , 0 );
                        fwrite(cad_contenido_archivo, 1, 1024, fich);
                    }
                }
                entero_reciv=0;
                
                send(sock1, nombre_archivo_enviar , 1024, 0 );
                recv(sock1, &listo,sizeof(int),0);
                while(1)
                {
                    char cad_contenido_archivo[1024]={0};
                    recv( sock1 ,&entero_reciv,sizeof(int),0);
                    if(entero_reciv==2)
                    {
                        break;
                    }
                    else{
                        send(sock1, &listo , sizeof(int) , 0 );
                        recv(sock1,cad_contenido_archivo, 1024,0);
                        send(sock1, &listo , sizeof(int) , 0 );
                        fwrite(cad_contenido_archivo, 1, 1024, fich);
                    }
                }
                entero_reciv=0;
                
                send(sock, nombre_archivo_enviar , 1024, 0 );
                recv(sock, &listo,sizeof(int),0);
                while(1)
                {
                    char cad_contenido_archivo[1024]={0};
                    recv( sock ,&entero_reciv,sizeof(int),0);
                    if(entero_reciv==2)
                    {
                        break;
                    }
                    else{
                        send(sock, &listo , sizeof(int) , 0 );
                        recv(sock,cad_contenido_archivo, 1024,0);
                        send(sock, &listo , sizeof(int) , 0 );
                        fwrite(cad_contenido_archivo, 1, 1024, fich);
                    }
                }
                entero_reciv=0;
                fclose(fich);
            }
        }
        else if(opciones==3)
        {
            break;
        }
    }
    close(sock);
    close(sock1);
    close(sock2);
    return 0;
}
