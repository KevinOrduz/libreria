#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "usuario.h"
#include "administrador.h"
#include "hilo.h"

void *cargar_persona(void *arg){
    while (1) { // Bucle infinito para que la función se ejecute constantemente
        cargar();
        sleep(5); // Esperamos 5 segundos antes de la siguiente ejecución
    }
}

 void validarFecha(int dia,int mes,int ano,int horas,int minutos,int segundos,int idPersona){
//    printf("\ndia: %d mes: %d año: %d hora: %d minutos: %d segundos: %d",dia,mes,ano,horas,minutos,segundos);
    struct tm fecha_entrega;
		fecha_entrega.tm_year =ano - 1900; /* año de entrega - 1900 */
		fecha_entrega.tm_mon =mes - 1; /* mes de entrega - 1 */
		fecha_entrega.tm_mday =dia; /* día de entrega */
		fecha_entrega.tm_hour = horas; // establece la hora en 5:00 PM
		fecha_entrega.tm_min = minutos; // establece los minutos en 0
		fecha_entrega.tm_sec = segundos; // establece los segundos en 0

		time_t tiempo_entrega = mktime(&fecha_entrega);

		time_t hora_actual;
		time(&hora_actual); // obtiene la hora actual

		time_t tiempo_limite = tiempo_entrega;/* aquí se debe calcular el valor Unix timestamp correspondiente a las 5:00 PM del día de entrega */
		if (hora_actual > tiempo_limite) {
		  bloquearUsuario(idPersona);
		  bloquearLibro(idPersona);
		}
}

void bloquearUsuario(int idPersona){
    FILE *cuentas;
    cuentas = fopen("cuentas.bin", "r+b");
    if(cuentas==NULL)return;

    sesiones sesion;

    fread(&sesion,sizeof(sesiones),1,cuentas);
    while(!feof(cuentas)){
        if (sesion.id == idPersona){
            sesion.bloqueo = 1;
            int pos = ftell(cuentas)-sizeof(sesiones);
            fseek(cuentas,pos,SEEK_SET);
            fwrite(&sesion,sizeof(sesiones),1,cuentas);
            break;
        }
        fread(&sesion,sizeof(sesiones),1,cuentas);
    }

    fclose(cuentas);
}
void bloquearLibro(int idPersona){
    FILE *librosTomados;
    librosTomados = fopen("librosTomados.bin", "r+b");
    if(librosTomados==NULL)return;

    Ltomados tomados;

    fread(&tomados,sizeof(Ltomados),1,librosTomados);
    while(!feof(librosTomados)){
        if (tomados.idp == idPersona){
            tomados.bloqueo = 1;
            int pos = ftell(librosTomados)-sizeof(Ltomados);
            fseek(librosTomados,pos,SEEK_SET);
            fwrite(&tomados,sizeof(Ltomados),1,librosTomados);
            break;
        }
        fread(&tomados,sizeof(Ltomados),1,librosTomados);
    }

    fclose(librosTomados);
}

void fechaInteger(char tiempoEntrega[22], int idPersona){
    char *token;
    int dia, mes, ano, horas, minutos, segundos;
    token = strtok(tiempoEntrega, "/");
    dia = atoi(token);
    token = strtok(NULL, "/");
    mes = atoi(token);
    token = strtok(NULL, "/ ");
    ano = atoi(token);
    token = strtok(NULL, ":");
    horas = atoi(token);
    token = strtok(NULL, ":");
    minutos = atoi(token);
    token = strtok(NULL, ":");
    segundos = atoi(token);
    validarFecha(dia,mes,ano,horas,minutos,segundos,idPersona);
}

 void cargar (){
    FILE *librosTomados;
    librosTomados = fopen ("librosTomados.bin","rb");

    Ltomados tomados;

    while (fread(&tomados, sizeof(Ltomados), 1, librosTomados) == 1) {
        fechaInteger(tomados.tiempoEntrega, tomados.idp);
    }

    fclose(librosTomados);
 }
