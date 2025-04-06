/*
Autores:
Carlos Cambero Rojas
Ismael Zahínos Márquez
Grupo 5: Miércoles 8:30
*/

#include "enviar.h"
using namespace std;

int EnviarCaracter(interface_t *adapter, unsigned char datos, unsigned char mac_origen[6], unsigned char mac_destino[6], unsigned char type[2]){
    unsigned char *caracter = new unsigned char;
    
    try
    {
       *caracter = datos;
        unsigned char* trama;
        trama=BuildFrame(mac_origen,mac_destino,type,caracter);
        SendFrame(adapter,trama,1);
        
        delete caracter;
        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}

int F1(interface_t *adapter,unsigned char mac_destino[6],int grupo){
    char car;
    unsigned char type[2]={(unsigned char)(0x30 + grupo),0x00};
    int err = 0;
    while (car != 27 && err == 0){
        /*RecibirCaracter*/
        char valor = 0;
        valor=recibirTramaTexto(adapter,grupo);
        /*Detecta la tecla, la almacena y la muestra*/
        if(kbhit()){
            car=getch();
            /*Enviar caracter*/
            if(car!=0 && car != 27){
                err = EnviarCaracter(adapter,car,adapter->MACaddr,mac_destino,type);
            }
            else{
                if (car == 27 ){
                    if(kbhit()){
                        car = getch();
                        car = getch();
                    }
                } 
            }
        }
   }
   return err;
}

int enviarTexto(interface_t *adapter, unsigned char mac_destino[6], int grupo){
 ifstream f;
 f.open("envio.txt");
 unsigned char texto1[255];
 char texto2[255];
 unsigned char* trama;
 unsigned char type[2] = {(unsigned char)(0x30 + grupo),0x00};
 if(f.is_open()){
     while (!f.eof()){
        f.read(texto2,254);
        if(f.gcount() > 0){
            for (int i = 0; i < 254; i++){
                texto1[i] = texto2[i];
            }
            texto1[f.gcount()]= '\0';
            trama=BuildFrame(adapter->MACaddr,mac_destino,type,texto1);
            SendFrame(adapter,trama,f.gcount());
        }
     }
     
 }else{
     cout<<"Error al abrir archivo"<<endl;
     return 1;
 }
 return 0;
}

int F3M(interface_t *adapter, unsigned char mac_destino[6], int grupo,ofstream& ofs){
    unsigned char datos[3];
    unsigned char NT = '0';
    unsigned char type[2] = {(unsigned char)(0x30 + grupo),0x00};
    unsigned char* trama;
    int tipo;
    cout<< endl;
    cout<< "Modo paro y espera" << endl;
    cout << "Elige el tipo de operacion" << endl;
    cout << "[1]- Seleccion" << endl;
    cout << "[2]- Sondeo" <<endl;
    cin >> tipo;
    while (tipo != 1 && tipo != 2){
        cout << "Valor incorrecto" << endl;
        cin >> tipo;
    }
    if (tipo == 1){
        seleccionMaestro(adapter, mac_destino, grupo,ofs);
    }
    else{
        if(tipo==2){
            sondeoMaestro(adapter, mac_destino, grupo, ofs);
        }
    }

    return 0;
}

int seleccionMaestro(interface_t *adapter, unsigned char mac_destino[6], int grupo,ofstream& ofs){
    unsigned char datos[3];
    unsigned char type[2] = {(unsigned char)(0x30 + grupo),0x00};
    unsigned char* trama;
    unsigned char protoc='0';

    datos[0]='R';
    datos[1]=5;
    datos[2]='0';
    trama=BuildFrame(adapter->MACaddr,mac_destino,type,datos);
    SendFrame(adapter,trama,3);
    mostrarMensaje(datos,'E');
    while (recibirTramaTextoProc(adapter,mac_destino,grupo,ofs,protoc) != 6);
    cout<<endl;
    
    EnviarFicheroProtocolo(adapter, mac_destino, grupo, 'R', ofs);
    
    datos[0]='R';
    datos[1]=4;
    datos[2]='0';
    trama=BuildFrame(adapter->MACaddr,mac_destino,type,datos);
    SendFrame(adapter,trama,3);
    
    cout<<endl;
    mostrarMensaje(datos,'E');
    
    while (recibirTramaTextoProc(adapter,mac_destino,grupo,ofs,protoc) != 6);

    return 0;
}



int EnviarFicheroProtocolo(interface_t *adapter, unsigned char mac_destino[6], int grupo, unsigned char tipoP,ofstream& ofs){
    ifstream f;
    f.open("EProtoc.txt");
    bool v = false;
    int recibido = 0;
    unsigned char protoc='0';
    unsigned char datos[260];
    unsigned char BCE;
    unsigned char NT = '0';
    char texto2[255];
    unsigned char* trama;
    unsigned char type[2] = {(unsigned char)(0x30 + grupo),0x00};
    int er = 0;
    unsigned char valCar;
    
    if(f.is_open()){
        while (!f.eof()){
            F4(er);
            f.read(texto2,254);
            if(f.gcount() > 0){
                datos[0] = tipoP;
                datos[1] = 2;
                datos[2] = NT;
                datos[3] = f.gcount();
                BCE=texto2[0];
                for (int i = 0; i < f.gcount(); i++){
                    datos[i + 4] = texto2[i];
                    if(i!=0){
                        BCE = datos[i+4] ^ BCE;
                    }
                }
                if(BCE<=0 || BCE>=255){
                    BCE=1;
                }

                datos[f.gcount() + 4]= BCE;
                    if (er != 0)
                    {
                        cout<<"INTRODUCIENDO ERROR"<<endl;
                        valCar = datos[4];
                        datos[4] = (unsigned char)135;
                        er--;
                    }

                   F4(er);

                trama=BuildFrame(adapter->MACaddr,mac_destino,type,datos);
                SendFrame(adapter,trama,f.gcount() + 5);

                cout << "E " << tipoP << " STX " << NT << ' ' << (int)BCE << endl;
                v = false;
                while (!v){
                    recibido = recibirTramaTextoProc(adapter,mac_destino,grupo, ofs,protoc);
                    if(recibido == 6){
                        v = true;
                    }else{
                        if(recibido == 21){
                            datos[4] = valCar;
                            trama=BuildFrame(adapter->MACaddr,mac_destino,type,datos);
                            SendFrame(adapter,trama,f.gcount() + 5);
                            cout << "E " << tipoP << " STX " << NT << ' ' << (int)BCE << endl;
                        }
                    }
                }
                F4(er);
                if(NT == '0'){
                    NT = '1';
                }
                else{
                    NT = '0';
                }
            }
        }
        f.close();
    }
    else{
        cout<<"Error al abrir archivo"<<endl;
        return 1;
    }

    return 0;
}



int F3E(interface_t *adapter, unsigned char mac_destino[6], int grupo,ofstream& ofs){
    cout<<endl;
    cout<< "Modo paro y espera" << endl;
    cout << "Esperando al maestro" << endl;
    unsigned char protoc='0';
    unsigned char NT = '0';
    bool b = false;
    int respuesta = 0;
    unsigned char datos[3];
    unsigned char type[2] = {(unsigned char)(0x30 + grupo),0x00};
    unsigned char* trama;


    while (recibirTramaTextoProc(adapter,mac_destino,grupo,ofs, protoc) != 5);
    cout<<endl;
    if(protoc=='R'){
        while (recibirTramaTextoProc(adapter,mac_destino,grupo,ofs, protoc) != 4);
    }
    else{
        EnviarFicheroProtocolo(adapter, mac_destino, grupo, 'T', ofs);
        datos[0]='T';
        datos[1]=4;
        datos[2]= NT;
        trama=BuildFrame(adapter->MACaddr,mac_destino,type,datos);
        SendFrame(adapter,trama,3);
        
        cout<<endl;
        mostrarMensaje(datos,'E');

        while (!b){
            respuesta = recibirTramaTextoProc(adapter,mac_destino,grupo,ofs,protoc);
        if(respuesta == 21){
                datos[0]='T';
                datos[1]=4;
                
                if(NT == '0'){
                    NT='1';
                }
                else{
                    NT='0';
                }
                datos[2]=NT;

                trama=BuildFrame(adapter->MACaddr,mac_destino,type,datos);
                SendFrame(adapter,trama,3);
                
                cout<<endl;
                mostrarMensaje(datos,'E');
        }else{
            if(respuesta == 6){
                b = true;
            }
        }
        }
    }

    return 0;
    
}


int sondeoMaestro(interface_t *adapter, unsigned char mac_destino[6], int grupo,ofstream& ofs){
    unsigned char datos[3];
    unsigned char type[2] = {(unsigned char)(0x30 + grupo),0x00};
    unsigned char* trama;
    unsigned char NT='0';

    datos[0]='T';
    datos[1]=5;
    datos[2]=NT;
    trama=BuildFrame(adapter->MACaddr,mac_destino,type,datos);
    SendFrame(adapter,trama,3);
    mostrarMensaje(datos,'E');
    unsigned char protoc='0';
    while (recibirTramaTextoProc(adapter,mac_destino,grupo,ofs,protoc) != 4);


    int op=0;
    cout<<"¿Acepta el cierre de la comunicación?"<<endl;
    cout<<"[1] - si"<<endl;
    cout<<"[2] - no"<<endl;
    cin>>op;

    while(op!=1 && op!=2){
        cout<<"Opción errónea"<<endl;
        cin>>op;
    }

    while(op!=1){
        datos[0] = 'T';
        datos[1] = 21;
        datos[2] = NT;
        trama=BuildFrame(adapter->MACaddr,mac_destino,type,datos);
        SendFrame(adapter,trama,4);
        mostrarMensaje(datos,'E');

        unsigned char protoc='0';
        while (recibirTramaTextoProc(adapter,mac_destino,grupo,ofs,protoc) != 4);
        cout<<"¿Acepta el cierre de la comunicación?"<<endl;
        cout<<"[1] - si"<<endl;
        cout<<"[2] - no"<<endl;
        cin>>op;

        while(op!=1 && op!=2){
            cout<<"Opción errónea"<<endl;
            cin>>op;
        }

        if(NT == '0'){
            NT = '1';
        }
        else{
            NT = '0';
        }
    }

    if(op==1){
        datos[0] = 'T';
        datos[1] = 6;
        datos[2] = NT;
        trama=BuildFrame(adapter->MACaddr,mac_destino,type,datos);
        SendFrame(adapter,trama,4);
        mostrarMensaje(datos,'E');
    }

    return 0;
}

void F4(int &er){
    char tl;
    if(kbhit()){
         tl = getch();
        if(tl == 27){
            if(kbhit()){
                tl = getch();
                if(tl == 'O'){
                    tl = getch();
                    if(tl == 'S'){
                        er++;
                    }
                    }
                }
            }
}
}



