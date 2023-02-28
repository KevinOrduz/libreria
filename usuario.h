#ifndef USUARIO_H
#define USUARIO_H

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

#include "administrador.h"

typedef struct{
    int idp;
    char nomPersona[14];
    int idl;
    char titulo[50];
    char descripcion[100];
    char tiempoEntrega[22];
    int bloqueo;
}Ltomados;

void fecha(char *f[22]);

void continuar();

void misLibros(int idPersona);

void modificarLibro2(int idl);

void modificacion (int idPersona);

void opcionMisLibros(int idPersona);

void librosDisponibles(char nomPersona[14], int idPersona);

void modificarLibro1(int id, int cantidad);

//void cargar();

int usuario(char nomPersona[14], int idPersona);

#endif // USUARIO_H
