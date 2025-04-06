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
#include "recibir.h"

using namespace std;//REVISAR

#ifndef _ENVIAR_H
#define _ENVIAR_H

/*
*Send a caracter from a source to a destination
* @ *adapter: pointer to the interface struct
* @ datos: data sent 
* @ mac_origen[6]: source 
* @ mac_destino[6]: destination
* @ type[2]: type of the message 
* Return 0 when there is no error
*/
int EnviarCaracter(interface_t *adapter, unsigned char datos, unsigned char mac_origen[6], unsigned char mac_destino[6], unsigned char type[2]);

/*
* Send a text to a destination
* @ *adapter: pointer to the interface struct
* @ mac_destino[6]: destination station mac 
* @ grupo: number of the group to which it belongs 
* Return 0 when there is no error
*/
int enviarTexto(interface_t *adapter, unsigned char mac_destino[6], int grupo);

/*
* Launch function 1 
* @ *adapter: pointer to the interface struct
* @ mac_destino[6]: destination station mac 
* @ grupo: number of the group to which it belongs 
* Return 0 when there is no error
*/
int F1(interface_t *adapter,unsigned char mac_destino[6],int grupo);

/*
* Start of F3 function master
* @ *adapter: pointer to the interface struct
* @ mac_destino[6]: destination station mac 
* @ grupo: number of the group to which it belongs
* @ ofs: ofstream to write in a file
* Return 0 when there is no error
*/
int F3M(interface_t *adapter, unsigned char mac_destino[6], int grupo,ofstream& ofs);

/*
* F3 function eslave
* @ *adapter: pointer to the interface struct
* @ mac_destino[6]: destination station mac 
* @ grupo: number of the group to which it belongs
* @ ofs: ofstream to write in a file
* Return 0 when there is no error
*/
int F3E(interface_t *adapter, unsigned char mac_destino[6], int grupo,ofstream& f);

/*
* Realizes selection in the master
* @ *adapter: pointer to the interface struct
* @ mac_destino[6]: destination station mac 
* @ grupo: number of the group to which it belongs
* @ ofs: ofstream to write in a file
* Return 0 when there is no error
*/
int seleccionMaestro(interface_t *adapter, unsigned char mac_destino[6], int grupo,ofstream& ofs);

/*
* Realiza sondeo en el maestro
* @ *adapter: pointer to the interface struct
* @ mac_destino[6]: destination station mac 
* @ grupo: number of the group to which it belongs
* @ ofs: ofstream to write in a file
* Return 0 when there is no error
*/
int sondeoMaestro(interface_t *adapter, unsigned char mac_destino[6], int grupo,ofstream& ofs);

/*
* Envía el fichero en tramas usando los protocolos
* @ *adapter: pointer to the interface struct
* @ mac_destino[6]: destination station mac 
* @ grupo: number of the group to which it belongs
* @ tipoP: tipo de protocolo (R o T)
* @ ofs: ofstream to write in a file
* Return 0 when there is no error
*/
int EnviarFicheroProtocolo(interface_t *adapter, unsigned char mac_destino[6], int grupo, unsigned char tipoP,ofstream& ofs);

/*
* Detecta la pulsación de la tecla F4 y aumenta el contador de errores.
* @&er: Cantidad de errores
*/
void F4(int &er);

#endif