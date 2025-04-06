/*
Autores:
Carlos Cambero Rojas
Ismael Zahínos Márquez
Grupo 5: Miércoles 8:30
*/
#include <stdio.h>
#include <stdio_ext.h>
#include <iostream>
#include <fstream>
#include "linkLayer.h"

//using namespace std; //COMPROBAR SI ES CORRECTO

#ifndef _RECIBIR_H
#define _RECIBIR_H

/*
*Send a caracter from a source to a destination
* @ *adapter: pointer to the interface struct
* Return a char when the message is not empty and 0 otherwise
*/
char RecibirCaracter(interface_t *adapter);

/*
*Returns a message type depending on the variables 
* @ grupo: number of the group to which it belongs 
* @ funcion: number of the function to perform 
* @ type1: group code 
* @ type2: function code 
* Return the group code(type1) and the function code 
*/
void elegirTipo(int grupo, int funcion, unsigned char &type1, unsigned char &type2);

/*
* Receive a header 
* @ *adapter: pointer to the interface struct
* @ *mac_origen: destination station mac
* Return the Mac of the destination station(mac_origen) and 0 when there is no error
*/
int RecibirHeader(interface_t *adapter, unsigned char *mac_origen[14]);

/*
*Receive a text message 
* @ *adapter: pointer to the interface struct
* @ grupo: number of the group to which it belongs 
* Return 0 when there is no error
*/
int recibirTramaTexto (interface_t *adapter, int grupo);

/*
*Recibe las tramas de protocolos de paro y espera.
* @ *adapter: pointer to the interface struct
* @ mac_destino: destination station mac 
* @ grupo: number of the group to which it belongs 
* @ f: ofstream to write in RProtoc.txt
* @ protoc: is the type of the protocol received
* Return 0 when there is no error
*/
int recibirTramaTextoProc (interface_t *adapter,unsigned char mac_destino[6], int grupo, std::ofstream& f,unsigned char& protoc);

/*
*Calculates the BCE of the packet
* @ *paquete: packet that needs a BCE
* Return BCE
*/
unsigned char calcularBCE(apacket_t *paquete);

/*
* Muestra los mensajes del protocolo
* @ datos[]: vector con los datos que se van a mostrar
* @ direccion: tipo de mensaje (E o R)
*/
void mostrarMensaje(unsigned char datos[], char direccion);


#endif