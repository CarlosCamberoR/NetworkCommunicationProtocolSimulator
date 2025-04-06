//============================================================================
// ----------- PRACTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES -----------
// ---------------------------- CURSO 2021/22 --------------------------------
// ----------------------------- SESION1.CPP ---------------------------------
//============================================================================


/*
Autores:
Carlos Cambero Rojas
Ismael Zahínos Márquez
Grupo 5: Miércoles 8:30
*/
#include "estacion.h"

using namespace std;

int main()
{
 interface_t *iface = new interface_t();
 int err = 0,grupo = 0;
 /*Inicializa la interfaz*/
 err = InicializarInterfazRed(iface,grupo);
 
 unsigned char type[2]={(unsigned char)(0x30 + grupo),0x00};
 unsigned char mac_destino[6];


   __fpurge(stdin);  //vacia el buffer por el cin

   
   int modo = elegirModo(iface, grupo,mac_destino);

   __fpurge(stdin);

   if(modo == 1){
      funcionesMaestro(iface,mac_destino,grupo,err);
   }else{
      if(modo == 2){
         funcionesEsclavo(iface,mac_destino,grupo,err);
      }
   } 
      
     
      
/*Cierra la interfaz*/
CerrarInterfazRed(iface);

 return 0;
}
