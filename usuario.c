#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "usuario.h"
#include "administrador.h"

int flagPrestar;

void fecha(char *f[22])
{
    time_t t = time(NULL);
    struct tm tiempoLocal = *localtime(&t);
    char fechaHora[22];
    int bytesEscritos = 0;
    char *formato = "%d/%m/%Y %H:%M:%S";
    if(flagPrestar == 1){
        tiempoLocal.tm_min += 1; // Increase the minute by 1
        mktime(&tiempoLocal); // Update the time
         bytesEscritos = strftime(fechaHora, sizeof fechaHora, formato, &tiempoLocal);

    }else {
        bytesEscritos = strftime(fechaHora, sizeof fechaHora, formato, &tiempoLocal);
    }

//  if (bytesEscritos != 0) {
//    printf("Fecha y hora: %s", fechaHora);
//  } else {
//    printf("Error formateando fecha");
//  }
    strcpy(f, fechaHora);

}

void continuar()
{
    printf("Presione una tecla para continuar\n\n");
    getch();
}

void misLibros(int idPersona){
    system("cls");
    Ltomados tomados;
    FILE *librosTomados;
    librosTomados = fopen ("librosTomados.bin","rb");
    if(librosTomados==NULL){
        system("cls");
        printf("\nNo tienes libros prestados :(\n");
        system("pause");
        system("cls");
        return 0;
    }
    if(librosTomados!=NULL){
        printf(" ================MIS LIBROS================\n");

    while(fread(&tomados, sizeof(Ltomados), 1, librosTomados) == 1){
        if(idPersona == tomados.idp){
                printf(" codigo del libro:%d\n\n\t%s\n\n--%s\n\nTiempo de entrega: %s\n",tomados.idl, tomados.titulo,tomados.descripcion,tomados.tiempoEntrega);
                if(tomados.bloqueo == 1){
                        printf("\ntu tiempo de entrega de este libro expiro, debes entregarlo\n");
                        printf("===========================================================\n");
                }else{
                    printf(" ==========================================\n\n");
                }
        }
    }
    fclose(librosTomados);
    opcionMisLibros(idPersona);
    }else{
        system("cls");
        printf("no tienes ningun libro :(\n\n");
        system("pause");
        system("cls");
    }

    system("cls");
    return 0;

}

int buscadorBloqueo(int idPersona){

    FILE *cuentas;
    cuentas = fopen ("cuentas.bin","rb");

    sesiones sesion;

    fread(&sesion,sizeof(sesiones),1,cuentas);

    while(!feof(cuentas)){
        if (idPersona == sesion.id){
            break;
        }
        fread(&sesion,sizeof(sesiones),1,cuentas);
    }

    fclose(cuentas);
    return sesion.bloqueo;

}

int desbloquearUsuario(idPersona){

    FILE *cuentas;
    cuentas = fopen ("cuentas.bin","r+b");

    sesiones sesion;

    fread(&sesion,sizeof(sesiones),1,cuentas);

    while(!feof(cuentas)){
        if (idPersona == sesion.id){
            sesion.bloqueo = 0;
            int pos = ftell(cuentas)-sizeof(sesiones);
            fseek(cuentas,pos,SEEK_SET);
            fwrite(&sesion,sizeof(sesiones),1,cuentas);
            break;
        }
        fread(&sesion,sizeof(sesiones),1,cuentas);
    }

    fclose(cuentas);
    return sesion.bloqueo;

}

void modificarLibro2(int idl){

    FILE *libros;
    libros = fopen ("libros.bin","r+b");

    tlibro libro;

    fread(&libro,sizeof(tlibro),1,libros);

    while(!feof(libros)){
            if(idl == libro.id){
            libro.cantidad = libro.cantidad + 1;
            libro.prestados = libro.prestados - 1;
            int pos = ftell(libros)-sizeof(tlibro);
            fseek(libros,pos,SEEK_SET);
            fwrite(&libro,sizeof(tlibro),1,libros);
            break;
            }
            fread(&libro,sizeof(tlibro),1,libros);
    }
    fclose(libros);
}

void modificacion (int idPersona){

    FILE *librosTomados;
    librosTomados = fopen ("librosTomados.bin","r+b");
    if(librosTomados==NULL)return;

    Ltomados tomados;

    int codigo;
    char opc;
    int existe = 0;


    printf("\n\nEscoge el codigo de libro que vas a entregar:");
                scanf("%d",&codigo);
                fflush(stdin);
                fread(&tomados,sizeof(Ltomados),1,librosTomados);

                while(!feof(librosTomados)){
                    if(idPersona == tomados.idp && codigo == tomados.idl){
                        printf("\nseguro que quieres entregar el libro: %s? [s/n]",tomados.titulo);
                        scanf("%c",&opc);
                        if(opc == 's'){
                            desbloquearUsuario(tomados.idp);
                            existe = 1;
                            modificarLibro2(tomados.idl);
                            memset(&tomados,0x00,sizeof(Ltomados));
                            fflush(stdin);
                            int pos = ftell(librosTomados)-sizeof(Ltomados);
                            fseek(librosTomados,pos,SEEK_SET);
                            fwrite(&tomados,sizeof(Ltomados),1,librosTomados);
                            break;
                        }else{
                            system("cls");
                            return 0;
                        }

                    }
                     fread(&tomados,sizeof(Ltomados),1,librosTomados);
                }
                if(existe==0){
                    printf("\nel codigo de libro que digitaste no se encuentra en tu lista...\n");
                    system("cls");
                }
                fclose(librosTomados);
                system("cls");
}

void opcionMisLibros(int idPersona){
    int opcion = 0;
    int codigo;
    char opc;
    int existe = 0;
            printf("1 - entregar un libro\n");
            printf("2 - retroceder\n");
            printf("Ingresa la opcion:");
            scanf("%d",&opcion);
            fflush(stdin);
            if(opcion == 1){
                    modificacion(idPersona);
            }else if(opcion == 2){
                return 0;
            }
        return 0;
}

void librosDisponibles(char nomPersona[14], int idPersona){
    system("cls");
    int codigo;
    int existe=0;
    char fechaH[22];

    FILE *librosTomados;
    librosTomados = fopen ("librosTomados.bin", "ab");
    if(librosTomados==NULL){
        system("cls");
        printf("no hay libros disponibles :(\n\n\n");
        system("pause");
        system("cls");
        return;
    }

    FILE *libros;
    libros = fopen ("libros.bin", "rb");
    if(libros==NULL){
        system("cls");
        printf("no hay libros disponibles :(\n\n\n");
        system("pause");
        system("cls");
        return;
    }

    tlibro libro;

    int bloqueo = buscadorBloqueo(idPersona);

    if(bloqueo == 0){

    flagPrestar = 1;
    fecha(&fechaH);
    Ltomados tomados;

    fread(&libro,sizeof(tlibro),1,libros);

   while(!feof(libros)){

       if(libro.cantidad > 0){
            printf(" ========== codigo de libro: #%d ========== \n",libro.id);
            printf("             %s\n\n",libro.titulo);
            printf(" %s\n\n",libro.descripcion);
            printf(" ================================= \n");
       }
        fread(&libro,sizeof(tlibro),1,libros);
   }

    printf("\n\nselecciona el codigo del libro que vas a sacar:");
    scanf("%d",&codigo);

    fseek(libros,0,SEEK_SET);
    fread(&libro,sizeof(tlibro),1,libros);


   while(!feof(libros)){
        if(codigo == libro.id && libro.cantidad > 0){
            tomados.idp = idPersona;
            strcpy(tomados.nomPersona, nomPersona);
            tomados.idl = libro.id;
            strcpy(tomados.titulo,libro.titulo);
            strcpy(tomados.descripcion,libro.descripcion);
            strcpy(tomados.tiempoEntrega, fechaH);
            tomados.bloqueo = 0;
            fwrite(&tomados,sizeof(Ltomados),1,librosTomados);
            modificarLibro1(libro.id ,libro.cantidad);
            system("cls");
            printf("\n\ntu libro %s fue sacado exitosamente.\n\nverifica tu libro en la opcion: ->Mis libros<-\n\n",libro.titulo);
            existe = 1;
            break;
        }
        fread(&libro,sizeof(tlibro),1,libros);
   }
   if(existe==0){
        printf("\ncodigo del libro no disponible o no existe...\n\n");
   }

    }else{
        printf("tu cuenta esta bloqueado por no entregar un libro\n");
    }


    fclose(libros);
    fclose(librosTomados);
    system("pause");
    system("cls");
    return 0;

}

void modificarLibro1(int id, int cantidad){

    FILE *libros;
    libros = fopen ("libros.bin","r+b");

    tlibro libro;

    fread(&libro,sizeof(tlibro),1,libros);

    while(!feof(libros)){
            if(id == libro.id){
            libro.cantidad = libro.cantidad - 1;
            libro.prestados = libro.prestados + 1;
            int pos = ftell(libros)-sizeof(tlibro);
            fseek(libros,pos,SEEK_SET);
            fwrite(&libro,sizeof(tlibro),1,libros);
            break;
            }
            fread(&libro,sizeof(tlibro),1,libros);
    }
    fclose(libros);
}

int usuario(char nomPersona[14], int idPersona){
    system("cls");
    int opcion;

    do{

        printf("1 - Mis libros\n");
        printf("2 - libros disponibles\n");
        printf("3 - cerrar sesion\n");
        printf("Ingresa la opcion:");
        scanf("%d", &opcion);
        switch(opcion){
            case 1:misLibros(idPersona);
                break;
            case 2:librosDisponibles(nomPersona, idPersona);
                break;
        }
    }while(opcion != 3);

    return 0;
}
