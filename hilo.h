#ifndef USUARIO_H
#define USUARIO_H

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

void *cargar_persona(void *arg);

void validarFecha(int dia,int mes,int ano,int horas,int minutos,int segundos,int idPersona);

void bloquearUsuario();

void bloquearLibro();

void cargar();

pthread_t thread;

pthread_create(&thread,NULL,cargar_persona,NULL);

pthread_join(thread,NULL);

#endif // USUARIO_H
