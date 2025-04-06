/*
Autores:
Carlos Cambero Rojas
Ismael Zahínos Márquez
Grupo 5: Miércoles 8:30
*/

#include "estacion.h"
using namespace std;

int InicializarInterfazRed(interface_t * iface, int &grupo){
pcap_if_t *avail_ifaces=NULL;
interface_t  iface2;
int err = 0;
try
{
 //Store the list of available network devices
 avail_ifaces=GetAvailAdapters();
 //Auxiliar list of available network devices
 pcap_if_t *current;
 current=avail_ifaces;
 printf("Interfaces disponibles: \n");
 
 //Print all the available network devices using the auxiliar list
 int i=0;
 while(current!=NULL){
    cout<<"["<<i<<"]"<<current->name<<endl;
    current=current->next;
    i++;
 }
 printf("Seleccione la interfaz: ");
 int a;
 cin>>a;
 //Control the user's input selecting the interface
 while(a >= i || a < 0){
    printf("Seleccione la interfaz: ");
    cin>>a;
 }
 
 //Obtain the device selected by the user
 current=avail_ifaces;
 for(int j=0;j<a;j++){
    current=current->next;
 }
 
 //Set the device name and print it's MAC address
 printf("La interfaz seleccionada es: %s\n", current->name);
 setDeviceName(iface, current->name);
 cout<<"La MAC origen es: ";
 printMAC(iface);
 

 printf("Introduzca el número de grupo\n");

 cin>>grupo;
 
 while (grupo<= 0 || grupo>10)
 {
   printf("Grupo incorrecto\n");
   cin>>grupo;
 }

 /*Open the adapter*/
 err = OpenAdapter(iface);
 cout<<"Puerto abierto correctamente"<<endl;

 

 return err;
}
catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}

}

int CerrarInterfazRed(interface_t *iface){
    try
    {
        
        return CloseAdapter(iface);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    
}

int printMAC(interface_t *adapter){
    int err = GetMACAdapter(adapter);
    if(err == 0){
        for(int i = 0;i<6;i++){
            printf("%02hhx:",adapter->MACaddr[i]);
        }
        cout<< endl;
    }

 return err;
}

int elegirModo(interface_t *adapter, int grupo, unsigned char result[6]){
    cout<<"Elige el modo del terminal:"<<endl;
    cout<<"[1] Maestro"<<endl;
    cout<<"[2] Esclavo"<<endl;
    int a;
    cin>>a;

    while(a!=1 && a!=2){
    cout<<"[1] Maestro"<<endl;
    cout<<"[2] Esclavo"<<endl;
        cin>>a;
    }

    if(a==1){
        inicializarMaestro(adapter, grupo, result);
        
    }
    else{
        if(a==2){
            inicializarEsclavo(adapter, grupo, result);
        }
    }
    return a;
}

int inicializarEsclavo(interface_t *adapter, int grupo,unsigned char mac_origen[6]){
    char type1=0x30+grupo;
    apacket_t *headerRecibido = new apacket_t();
    unsigned char type[2];
    unsigned char mac[6];
    bool recibido=false;

    cout<<"Esperando a que se una la estación maestra."<<endl;

    while(!recibido){
         *headerRecibido=ReceiveFrame(adapter);
        if(headerRecibido->packet != 0){
        if((headerRecibido->packet[12]==type1) && (headerRecibido->packet[13]==1)){
            for(int i=0;i<6;i++){
                mac_origen[i]=headerRecibido->packet[6+i];
            }
            type[0]=type1;
            type[1]=2;
            recibido=true;
        }
      }
    }
    

    unsigned char *headerEnviar=BuildHeader(adapter->MACaddr,mac_origen,type);
    SendFrame(adapter,headerEnviar,0);

    delete headerRecibido;
    headerRecibido = nullptr;
   
    cout<<"Estación encontrada. La MAC destino es: ";
    for(int i = 0;i<6;i++){
            printf("%02hhx:",mac_origen[i]);
        }
        cout<< endl;
    return 0;
}

int inicializarMaestro(interface_t *adapter, int grupo, unsigned char mac_origen[6]){
    unsigned char type1=0x30+grupo;
    apacket_t *headerRecibido = new apacket_t();
    unsigned char type[2];
    type[0]=type1;
    type[1]=1;
    unsigned char broadcast[6]={0xff,0xff,0xff,0xff,0xff,0xff};
    unsigned char *header=BuildHeader(adapter->MACaddr,broadcast,type);
    SendFrame(adapter,header,0);
    bool recibido=false;

    cout<<"Esperando a que se una la estación esclava."<<endl;

    while(!recibido){
       *headerRecibido=ReceiveFrame(adapter);
        if(headerRecibido->packet != 0){
        if((headerRecibido->packet[12]==type1) && (headerRecibido->packet[13]==2)){
            for(int i=0;i<6;i++){
                mac_origen[i]=headerRecibido->packet[6 + i];
            }
            recibido=true;
        }
        }
    }

    delete headerRecibido;
    headerRecibido = nullptr;

     cout<<"Estación encontrada. La MAC destino es: ";
    for(int i = 0;i<6;i++){
            printf("%02hhx:",mac_origen[i]);
        }
        cout<< endl;

    return 0;
}

int funcionesMaestro(interface_t *adapter,unsigned char mac_destino[6],int grupo,int err){
    char tecla;
    cout<< "Seleccionar funcion:" << endl;
    cout<< "F1 - Envío de caracteres interactivo" << endl;
    cout<< "F2 - Envío de un fichero"<< endl;
    cout<< "F3 - Modo paro y espera"<< endl;
    cout<< "F4 - Introduccion errores durante el protocolo"<< endl;
    cout<< "esc - Salir"<< endl;

    ofstream ofs;
    while (tecla != 27 && err ==0)
    {
        if(kbhit()){
            tecla = getch();
            if(tecla == 27){
                if(kbhit()){
                    tecla = getch();
                    if(tecla == 'O'){
                        tecla = getch();
                        switch (tecla)
                        {
                        case 'P'://funcion 1
                            F1(adapter,mac_destino,grupo);
                            break;
                        case 'Q'://funcion 2
                            enviarTexto(adapter,mac_destino,grupo);
                            break;
                        case 'R'://funcion 3
                            ofs.open("RProtoc.txt");
                            F3M(adapter,mac_destino,grupo, ofs);
                            ofs.close();
                            break;
                        default:
                            break;
                        }                 
                        cout<< "Seleccionar funcion:" << endl;
                        cout<< "F1 - Envío de caracteres interactivo" << endl;
                        cout<< "F2 - Envío de un fichero"<< endl;
                        cout<< "F3 - Modo paro y espera"<< endl;
                        cout<< "F4 - Introduccion errores durante el protocolo"<< endl;
                        cout<< "esc - Salir"<< endl;
                    }else{
                        if(tecla == 0){
                            tecla = 27;
                        }
                    }
                }
            }
        }
    }
    
    return 0;
}

int funcionesEsclavo(interface_t *adapter,unsigned char mac_destino[6],int grupo,int err){
    char tecla;
    cout<< "Seleccionar funcion:" << endl;
    cout<< "F1 - Envío de caracteres interactivo" << endl;
    cout<< "F3 - Modo paro y espera"<< endl;
    cout<< "F4 - Introduccion errores durante el protocolo"<< endl;
    cout<< "esc - Salir"<< endl;
    ofstream ofs;
    while (tecla != 27 && err ==0)
    {
        recibirTramaTexto(adapter,grupo);
        if(kbhit()){
            tecla = getch();
            if(tecla == 27){
                if(kbhit()){
                    tecla = getch();
                    if(tecla == 'O'){
                        tecla = getch();
                        switch (tecla)
                        {
                        case 'P'://funcion 1
                            F1(adapter,mac_destino,grupo);
                            break;
                        case 'R'://funcion 3
                            ofs.open("RProtoc.txt",_S_trunc);
                            F3E(adapter,mac_destino,grupo, ofs);
                            ofs.close();
                            break;
                        default:
                            break;
                        }
                        cout<< "Seleccionar funcion:" << endl;
                        cout<< "F1 - Envío de caracteres interactivo" << endl;
                        cout<< "F3 - Modo paro y espera"<< endl;
                        cout<< "F4 - Introduccion errores durante el protocolo"<< endl;
                        cout<< "esc - Salir"<< endl;
                    }else{
                        if(tecla == 0){
                            tecla = 27;
                        }
                    }
                }
            }
        }
    }
    
    return 0;
}