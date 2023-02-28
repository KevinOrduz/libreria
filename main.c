#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "usuario.h"
#include "administrador.h"
#include "hilo.h"

cargar_persona(void *arg);

void iniciarSesion()
{
    system("cls");
    FILE *cuentas;
    cuentas = fopen ("cuentas.bin", "a+b");
    if(cuentas==NULL)return;

    int x;
    char usuariom[50];
    char contrasenam[50];
    int intentos = 0;
    char opcion;
    int existe;

    sesiones sesion;


    do
    {
        existe = 0;
        fflush(stdin);
        printf("usuario: ");
        gets(usuariom);
        printf("contraseña: ");
        gets(contrasenam);

        fseek(cuentas,0,SEEK_SET);

        if (strcmp (usuariom, "admin") == 0 && strcmp (contrasenam, "admin") == 0)
            {
                administrador();
                existe = 1;
            }

        while (fread(&sesion, sizeof(sesiones), 1, cuentas) == 1) {
            if (strcmp(usuariom, sesion.usuario) == 0 && strcmp(contrasenam, sesion.contrasena) == 0) {
                usuario(sesion.usuario, sesion.id);
                existe = 1;
            break;
            }
        }

        if (existe == 0)
        {
            printf("\nel usuario o la contraseña son incorrectas...\n\n");
            intentos++;
            if (intentos >= 3)
            {
                fflush(stdin);
                system("pause");
                system("cls");
                printf("\nte invitamos a que te registres!!!\n");
                printf("\nquieres registrarte???[s/n]\n");
                scanf("%c",&opcion);
                system("cls");
            }

            if(opcion == 's' || opcion == 'S')
            {
                registrarse();
                return 0;
            }
        }

    }
    while(existe == 0);
    fclose(cuentas);
    system("cls");
    return 0;
}

int idusuario(){
    int x=0;
    FILE *cuentas;
    cuentas = fopen ("cuentas.bin", "rb");
    if (cuentas==NULL) return 0;

    sesiones sesion;

    sesion.id=0;

    while(!feof(cuentas))
    {
        fread(&sesion,sizeof(sesiones),1,cuentas);
    }

    if(sesion.id<0 || sesion.id>100)
    {
        x = 1;
    }
    else
    {
        x = sesion.id;
        x++;
    }
    fclose(cuentas);
    return (x);
}

int validarExiste(char usuario[14]){
    int existe = 0;
    FILE *cuentas;
    cuentas = fopen("cuentas.bin", "rb");

    sesiones sesion;

    fread(&sesion,sizeof(sesiones),1,cuentas);

    while(!feof(cuentas)){
        if(strcmp(usuario,sesion.usuario)==0){
            printf("\neste usuario ya existe....\n");
            existe = 1;
        }
        fread(&sesion,sizeof(sesiones),1,cuentas);
    }
    fclose(cuentas);

    return existe;
}

void registrarse()
{
    int x;
    system("cls");
    FILE *cuentas;
    cuentas = fopen("cuentas.bin", "ab");
    if(cuentas==NULL)return;

    sesiones sesion;

    x = idusuario(&x);
    sesion.id = x;
    char usuario[14];
    char contrasena[11];
    printf(" ========================= \n");
    printf("         REGISTRO \n");
    printf(" ========================= \n\n");
    fflush(stdin);
    printf("Ingresa un nombre de usuario: ");
    gets(usuario);
    printf("Ingresa una contraseña: ");
    gets(contrasena);
    sesion.bloqueo = 0;
    fflush(stdin);

    int existe = validarExiste(usuario);

    if(existe==0){
    strcpy(sesion.usuario,usuario);
    strcpy(sesion.contrasena,contrasena);
    fwrite(&sesion,sizeof(sesiones),1,cuentas);
    printf("\n\nse ha creado correctamente la cuenta\n\n");
    }
    fclose(cuentas);
    system("pause");
    system("cls");
    return 0;
}

int main()
{
    int opcion;

    pthread_t thread;

    pthread_create(&thread,NULL,cargar_persona,NULL);

    do
    {
        printf("1 - Iniciar Sesion\n");
        printf("2 - Registrarse\n");
        printf("Ingresa la opcion:");
        scanf("%d",&opcion);
        switch(opcion)
        {
        case 1:iniciarSesion();
            break;
        case 2:registrarse();
            break;
        }
    }
    while(opcion!=3);

    system("pause");
    return 0;
}
