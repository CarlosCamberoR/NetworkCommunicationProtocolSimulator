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
#include "enviar.h"

#ifndef _ESTACION_H
#define _ESTACION_H

/*
*Open the adapter and initialize the interface
* @ iface: interface struct 
* Return 0 when there is no error
*/
int InicializarInterfazRed(interface_t *iface, int &grupo);

/*
*Open the adapter and close the interface
* @ iface: interface struct 
* Return 0 when there is no error
*/
int CerrarInterfazRed(interface_t *iface);

//Funciones basicas para gestionar tarjetas de red

/*
*Print the MAC address from iface
* @ iface: interface struct 
* Return 0 when there is no error
*/
int printMAC(interface_t *adapter);

/*
* Makes you choose the mode of the terminal
* @ *adapter: pointer to the interface struct
* @ grupo: number of the group to which it belongs 
* @ result: destination station mac 
* Return the Mac of the destination station(result) and 0 when there is no error 
*/
int elegirModo(interface_t *adapter, int grupo, unsigned char result[6]);

/*
* Initialize slave mode 
* @ *adapter: pointer to the interface struct
* @ grupo: number of the group to which it belongs 
* @ mac_origen: destination station mac 
* Return the Mac of the destination station(mac_origen) and 0 when there is no error 
*/
int inicializarEsclavo(interface_t *adapter, int grupo,unsigned char mac_origen[6]);

/*
* Initialize master mode 
* @ *adapter: pointer to the interface struct
* @ grupo: number of the group to which it belongs 
* @ mac_origen: destination station mac 
* Return the Mac of the destination station(mac_origen) and 0 when there is no error 
*/
int inicializarMaestro(interface_t *adapter, int grupo,unsigned char mac_origen[6]);

/*
* Launches the master mode menu 
* @ *adapter: pointer to the interface struct  
* @ @ mac_destino: destination station mac  
* @ grupo: number of the group to which it belongs
* @ err: error number  
* Return 0 when there is no error 
*/
int funcionesMaestro(interface_t *adapter,unsigned char mac_destino[6],int grupo,int err);

/*
* Launches the slave mode menu 
* @ *adapter: pointer to the interface struct  
* @ @ mac_destino: destination station mac  
* @ grupo: number of the group to which it belongs
* @ err: error number  
* Return 0 when there is no error  
*/
int funcionesEsclavo(interface_t *adapter,unsigned char mac_destino[6],int grupo,int err);


#endif