/*
Autores:
Carlos Cambero Rojas
Ismael Zahínos Márquez
Grupo 5: Miércoles 8:30
*/

#include "recibir.h"
using namespace std;

char RecibirCaracter(interface_t *adapter){
    apacket_t trama;
    const unsigned char* datosTrama;
    unsigned char dato;

    trama=ReceiveFrame(adapter);
    datosTrama=trama.packet;
    
    if(datosTrama!=nullptr){
        dato=datosTrama[14];
        return dato;
    }
    return 0;
}


void elegirTipo(int grupo, int funcion,  unsigned char & type1, unsigned char & type2){
    type1=0x30+grupo;
    if(funcion==1){
        type2=0x1;
    }
    else{
        if(funcion==2){
            type2=0x2;
        }
        else{
            type2=0x0;
        }
    }
}

int RecibirHeader(interface_t *adapter,unsigned char *mac_origen[14]){
    apacket_t *headerRecibido = new apacket_t();
    const unsigned char* paquete;
    *headerRecibido=ReceiveFrame(adapter);
    paquete=headerRecibido->packet;
    for(int i=0;i<14;i++){
        *mac_origen[i]=paquete[i];
    }
    delete headerRecibido;
    headerRecibido = nullptr;
    return 0;
}

int recibirTramaTexto (interface_t *adapter, int grupo){
    apacket_t *tramaRecibida = new apacket_t();
    const unsigned char* paquete;
    unsigned char type1=0x30+grupo;
    int cont = 0;
    *tramaRecibida=ReceiveFrame(adapter);
    if(tramaRecibida->packet != 0){
        if((tramaRecibida->packet[12]==type1) && (tramaRecibida->packet[13]==0)){
            paquete=tramaRecibida->packet;
            for(int i = 14; i < tramaRecibida->header.len ;i++){
                cout<<(char)paquete[i];
            }
            cout<<endl<<"Se han recibido "<< tramaRecibida->header.len - 14 <<" caracteres"<< endl;
        }
    }
    return 0;
}



int recibirTramaTextoProc (interface_t *adapter,unsigned char mac_destino[6], int grupo, ofstream& f,unsigned char& protoc){
    apacket_t *tramaRecibida = new apacket_t();
    
    unsigned char datos[3];
    unsigned char datosRec[4];
    unsigned char* trama;
    char texto2[255];
    unsigned char BCE;
    const unsigned char* paquete;
    unsigned char type1=(unsigned char)(0x30 + grupo);
    unsigned char type[2] = {(unsigned char)(0x30 + grupo),0x00};
    int cont = 0;
    *tramaRecibida=ReceiveFrame(adapter);
    if(tramaRecibida->packet != 0){
        if((tramaRecibida->packet[12]==type1) && (tramaRecibida->packet[13]==0)){
            protoc=tramaRecibida->packet[14];//Tipo de protocolo
            datosRec[0] = tramaRecibida->packet[14];
            datosRec[1] = tramaRecibida->packet[15];
            datosRec[2] = tramaRecibida->packet[16];
            switch (datosRec[1])
            {
            case 5:
                cout<<endl;
                mostrarMensaje(datosRec,'R');
                datos[0] = tramaRecibida->packet[14];
                datos[1] = 6;
                datos[2] = tramaRecibida->packet[16];
                trama=BuildFrame(adapter->MACaddr,mac_destino,type,datos);
                SendFrame(adapter,trama,4);
                mostrarMensaje(datos,'E');
                break;
            case 4:
                mostrarMensaje(datosRec,'R');
                
                if(tramaRecibida->packet[14]=='R'){
                    datos[0] = tramaRecibida->packet[14];
                    datos[1] = 6;
                    datos[2] = tramaRecibida->packet[16];
                    trama=BuildFrame(adapter->MACaddr,mac_destino,type,datos);
                    SendFrame(adapter,trama,4);
                    mostrarMensaje(datos,'E');
                }
                
                break;
            case 6:
                mostrarMensaje(datosRec,'R');
                break;
            case 21:
                mostrarMensaje(datosRec,'R');
                break;
            case 2:
                for (int i = 0; i < tramaRecibida->packet[17]; i++){
                    texto2[i] = (char)tramaRecibida->packet[i + 18];
                }
                BCE=calcularBCE(tramaRecibida);
                datosRec[3] = tramaRecibida->packet[(int)tramaRecibida->packet[17]+18];
                    datosRec[4] = BCE;
                    mostrarMensaje(datosRec,'R');

               if((int)BCE == (int)tramaRecibida->packet[(int)tramaRecibida->packet[17]+18]){
                    f.write(texto2,tramaRecibida->packet[17]);
                    datos[0] = tramaRecibida->packet[14];
                    datos[1] = 6;
                    datos[2] = tramaRecibida->packet[16];
                    trama=BuildFrame(adapter->MACaddr,mac_destino,type,datos);
                    SendFrame(adapter,trama,4);
                    mostrarMensaje(datos,'E');
               }else{
                    datos[0] = tramaRecibida->packet[14];
                    datos[1] = 21;
                    datos[2] = tramaRecibida->packet[16];
                    trama=BuildFrame(adapter->MACaddr,mac_destino,type,datos);
                    SendFrame(adapter,trama,4);
                    mostrarMensaje(datos,'E');
               }
                break;
            default:
            cout<<"error"<<endl;
                break;
            }
            unsigned char protocolo=tramaRecibida->packet[15];

        delete tramaRecibida;
        tramaRecibida=NULL;

        return protocolo;
            
        }
        
    }
    delete tramaRecibida;
    tramaRecibida=NULL;

    return 0;
}


unsigned char calcularBCE(apacket_t *paquete){
    unsigned char BCE=paquete->packet[18];
    for (int i = 1; i < paquete->packet[17]; i++){
        BCE = paquete->packet[i + 18] ^ BCE;
    }
    if(BCE<=0 || BCE>=255){
        BCE=1;
    }
    return BCE;
}

void mostrarMensaje(unsigned char datos[], char direccion){
    switch (datos[1])
                {
                case 4:
                    cout<< direccion << " " <<datos[0] << " EOT " <<datos[2]<<endl;
                    break;
                case 5:
                    cout<< direccion << " " <<datos[0] << " ENQ " <<datos[2]<<endl;
                    break;
                case 6:
                    cout<< direccion <<" " <<datos[0] << " ACK " <<datos[2]<<endl;
                    break;
                case 21:
                    cout<< direccion <<" " <<datos[0] << " NACK " <<datos[2]<<endl;
                    break;
                case 2:
                    if(direccion == 'R'){
                        cout<< direccion <<" " <<datos[0] << " STX " <<datos[2]<<" " << (int)datos[3] <<" " << (int)datos[4] <<endl;
                    }else{
                    cout<< direccion <<" " <<datos[0] << " STX " <<datos[2]<< " " << (int)datos[3] <<endl;
                    }
                    break;
                
                default:
                cout<< "Error al mostrar el mensaje"<<endl;
                    break;
                }
}