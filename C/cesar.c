#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INICIO_ASCII_MINUSCULAS 97
#define INICIO_ASCII_MAYUSCULAS 65
#define LONGITUD_ALFABETO 26

int main(int argc, char** argv){
    if(argc < 3){
        fprintf(stderr, "Error en los argumentos!\n");
        return -1;
    }

    int desplazamientos = (int)((*argv[1])-'0');
    printf("El numero de desplazamientos es %d.\n", desplazamientos);

    char* mensaje = argv[2];
    printf("El mensaje a encriptar es: %s.\n", mensaje);

    int i = 0;
    for(i = 0; i < strlen(mensaje); ++i){
        /*Metodo de branchless programming.
        mensaje[i] = (char)(((((((int)mensaje[i]) - INICIO_ASCII_MINUSCULAS +
            desplazamientos) % LONGITUD_ALFABETO) + 'a')*(!isupper(mensaje[i]))) +
            ((((((int)mensaje[i]) - INICIO_ASCII_MAYUSCULAS +
            desplazamientos) % LONGITUD_ALFABETO) + 'A')*(isupper(mensaje[i]))));*/
        
        if(!isupper(mensaje[i]))
            mensaje[i] = (char)(((((int)mensaje[i]) - INICIO_ASCII_MINUSCULAS +
            desplazamientos) %LONGITUD_ALFABETO) + 'a');
        else
            mensaje[i] = (char)(((((int)mensaje[i]) - INICIO_ASCII_MAYUSCULAS +
            desplazamientos) %LONGITUD_ALFABETO) + 'A');
    }

    printf("El mensaje encriptado es: %s.\n", mensaje);

    return 0;
}