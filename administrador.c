#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include "administrador.h"
#include "usuario.h"

int idLibro(){
    int x=0;
    FILE *libros;
    libros = fopen ("libros.bin", "rb");
    if (libros==NULL) return 0;

    tlibro libro;

    libro.id=0;

    while(!feof(libros))
    {
        fread(&libro,sizeof(tlibro),1,libros);
    }

    if(libro.id<0 || libro.id>100)
    {
        x = 1;
    }
    else
    {
        x = libro.id;
        x++;
    }
    fclose(libros);
    return (x);
}

void agregarLibro(){
    system("cls");
    FILE *libros;
    libros = fopen ("libros.bin", "ab");
    if (libros==NULL) return;

    int x;
    tlibro libro;
    x = idLibro(&x);
    libro.id = x;
    fflush(stdin);
    printf("Ingresa el titulo del libro: ");
    gets(libro.titulo);
    printf("\nIngresa un breve resumen del libro\n\n--");
    gets(libro.descripcion);


    printf("cantidad de libros:");
    scanf("%d",&libro.cantidad);
    libro.prestados = 0;

    fwrite(&libro,sizeof(tlibro),1,libros);
    system("pause");
    fclose(libros);
    system("cls");
    return 0;
}

void listaLibros(){
    int opcion;
    system("cls");
    FILE *libros;
    libros = fopen("libros.bin", "r+b");
    if(libros==NULL){
        printf("no hay libros agregados :(\n\n");
        system("pause");
        system("cls");
        return 0;
    }

    tlibro libro;

    fread(&libro,sizeof(tlibro),1,libros);

    while(!feof(libros)){

            if(libro.id >= 1){
                printf(" ============== #%d ============== \n",libro.id);
                printf(" \t%s\n",libro.titulo);
                printf(" ================================= \n");
                printf(" \n%s\n",libro.descripcion);
                printf(" \ncantidad:%d\tprestados:%d\n",libro.cantidad,libro.prestados);
                printf(" ================================= \n\n");
            }

        fread(&libro,sizeof(tlibro),1,libros);
    }
if(libro.id >= 1){
    do {
        printf("1 - eliminar un libro\n");
        printf("2 - editar un libro\n");
        printf("3 - retroceder\n");
        printf("escoge la opcion: ");
        scanf("%d",&opcion);
        switch (opcion){
        case 1:eliminar();
            break;
        case 2:editar();
            break;
        }
        fclose(libros);
        system("cls");
        return;
    }while(opcion != 3);
}else{
    system("cls");
    printf("No hay libros Disponibles :(\n\n");
    system("pause");
    fclose(libros);
}
    system("cls");
}

void eliminar (){
    int existe = 0;
    int cod;
    char opc;
    FILE *libros;
    libros = fopen ("libros.bin","r+b");

    tlibro libro;

    printf("escoge el codigo del libro que quieras eliminar: ");
    scanf("%d",&cod);
    fflush(stdin);

    fread(&libro,sizeof(tlibro),1,libros);

                while(!feof(libros)){
                    if(cod == libro.id){
                            existe = 1;
                        printf("\nseguro que quieres eliminar el libro: %s? [s/n]: ",libro.titulo);
                        scanf("%c",&opc);
                        if(opc == 's'){
                            memset(&libro,0x00,sizeof(tlibro));
                            fflush(stdin);
                            int pos = ftell(libros)-sizeof(tlibro);
                            fseek(libros,pos,SEEK_SET);
                            fwrite(&libro,sizeof(tlibro),1,libros);
                            system("pause");
                            break;
                        }else{
                            system("cls");
                            return 0;
                        }

                    }
                     fread(&libro,sizeof(tlibro),1,libros);
                }
                if(existe == 0){
                    printf("\nel codigo del libro no existe\n");
                    system("pause");
                }
                fclose(libros);
                system("cls");
}

void editar(){
    int existe = 0;
    int codigo;
    FILE *libros;
    libros = fopen ("libros.bin", "r+b");

    tlibro libro;

    printf("\nescoge el Id del libro:");
    scanf("%d",&codigo);
    fflush(stdin);

    fread(&libro,sizeof(tlibro),1,libros);

    while(!feof(libros)){
            if(codigo == libro.id){
                    existe = 1;
                printf("\ntitulo actual: %s\n",libro.titulo);
                printf("\nIngresa el nuevo nombre del titulo: ");
                gets(libro.titulo);
                printf("\ndescripcion actual: %s\n",libro.descripcion);
                printf("\nIngresa la nueva descripcion: ");
                gets(libro.descripcion);
                            fflush(stdin);
                            int pos = ftell(libros)-sizeof(tlibro);
                            fseek(libros,pos,SEEK_SET);
                            fwrite(&libro,sizeof(tlibro),1,libros);
                            system("pause");
                            break;
                    }
        fread(&libro,sizeof(tlibro),1,libros);
    }
    if(existe == 0){
        printf("\nel codigo del libro es incorrecto\n\n");
        system("pause");
        system("cls");
        return;
    }
    fclose(libros);
    system("cls");
}

void librosPrestados(){
    int existe = 0;
    system("cls");
    FILE *libros;
    libros = fopen ("libros.bin", "rb");
    if (libros==NULL){
            system("cls");
            printf("no hay libros prestados :(\n\n\n");
            system("pause");
            system("cls");
        return 0;
        }

    tlibro libro;

    fread(&libro,sizeof(tlibro),1,libros);

    while(!feof(libros)){
        if(libro.prestados > 1){
            existe = 1;
            printf("=======titulo: %s=======\n",libro.titulo);
            mostrar1(libro.id);

        }
        fread(&libro,sizeof(tlibro),1,libros);
    }

    if(existe == 0){
        printf("\nno hay libros prestados :(\n\n");
        system("pause");
    }

    fclose(libros);
    system("pause");
    system("cls");
}

void mostrar1(int idl){

    FILE *librosTomados;
    librosTomados = fopen("librosTomados.bin","rb");

    Ltomados tomados;

    fread(&tomados,sizeof(Ltomados),1,librosTomados);

    while(!feof(librosTomados)){

        if(idl == tomados.idl){
            printf("usuario: %s\n",tomados.nomPersona);
        }

        fread(&tomados,sizeof(Ltomados),1,librosTomados);
    }
    printf("\n\n");
    fclose(librosTomados);

}

void librosPrestadosUsuarios(){
    system("cls");
    int existe = 0;
    FILE *librosTomados;
    librosTomados = fopen ("librosTomados.bin","rb");
    if(librosTomados==NULL){
        system("cls");
        printf("no se han prestado libros :(\n");
        system("pause");
        system("cls");
        return;
    }

    FILE *cuentas;
    cuentas = fopen("cuentas.bin","rb");
    if(cuentas==NULL)return;

    Ltomados tomados;
    sesiones sesion;

    fread(&sesion,sizeof(sesiones),1,cuentas);
    fread(&tomados,sizeof(Ltomados),1,librosTomados);

    while(!feof(cuentas)){

            while(!feof(librosTomados)){
                existe = 1;
                if(sesion.id == tomados.idp){
                printf(" ======usuario: %s======\n",sesion.usuario);
                mostrar2(sesion.id);
                break;
            }
            fread(&tomados,sizeof(Ltomados),1,librosTomados);
        }
        fread(&sesion,sizeof(sesiones),1,cuentas);
        fread(&tomados,sizeof(Ltomados),1,librosTomados);
        }

        if(existe == 0){
        printf("\nno hay libros prestados :(\n\n");
        system("pause");
    }

    fclose(cuentas);
    system("pause");
    system("cls");

}
void mostrar2(int id){

    FILE *librosTomados;
    librosTomados = fopen ("librosTomados.bin","rb");
    if(librosTomados==NULL)return;

    Ltomados tomados;

    fread(&tomados,sizeof(Ltomados),1,librosTomados);
    while(!feof(librosTomados)){
        if(tomados.idp == id){
            printf("titulo: %s\n",tomados.titulo);
        }
        fread(&tomados,sizeof(Ltomados),1,librosTomados);
    }
    printf("\n\n");
    fclose(librosTomados);
}

void usuariosBloqueados(){
    system("cls");
    int cont=0;
    FILE *cuentas;
    cuentas = fopen("cuentas.bin","rb");
    if(cuentas==NULL){
        system("cls");
        printf("no hay usuarios :(");
        system("pause");
        system("cls");
        return;
    }

    sesiones sesion;

    fread(&sesion,sizeof(sesiones),1,cuentas);

    while(!feof(cuentas)){
            if(sesion.bloqueo == 1){
                printf("%s\n",sesion.usuario);
                cont++;
            }

        fread(&sesion,sizeof(sesiones),1,cuentas);
    }
    printf("\n\n");

    if(cont == 0){
        printf("no hay usuarios bloqueados\n\n");
    }

    fclose(cuentas);
    system("pause");
    system("cls");
    return 0;
}

void administrador(){
    system("cls");
    int opcion;
    do {
        printf("1 - agregar libros\n");
        printf("2 - lista de libros\n");
        printf("3 - libros prestados\n");
        printf("4 - libros prestados por usuario\n");
        printf("5 - usuarios retrasados en entregas\n");
        printf("6 - Cerrar Sesion\n");
        printf("Ingresa la opcion:");
        scanf("%d",&opcion);
    switch(opcion){
    case 1:agregarLibro();
        break;
    case 2:listaLibros();
        break;
    case 3:librosPrestados();
        break;
    case 4:librosPrestadosUsuarios();
        break;
    case 5:usuariosBloqueados();
        break;
    }
    }while(opcion != 6);
    system("cls");
    return 0;
}
