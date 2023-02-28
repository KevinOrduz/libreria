#ifndef ADMINISTRADOR_H
#define ADMINISTRADOR_H

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    int id;
    char titulo[50];
    char descripcion[500];
    int cantidad;
    int prestados;
}tlibro;

typedef struct
{
    int id;
    int bloqueo;
    char usuario[14];
    char contrasena[11];

} sesiones;

int idLibro();

void agregarLibro();

void listaLibros();

void eliminar();

void librosPrestados();

void mostrar1(int idl);

void librosPrestadosUsuarios();

void mostrar2();

void administrador();

#endif // USUARIO_H
