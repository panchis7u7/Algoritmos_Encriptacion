//-----------------------------------------------------------------------
//  Algoritmo de Cesar. Carlos Sebastian Madrigal Rodriguez.
//
//  Para Compilacion: 
//  make    (si es que se tiene make instalado)
//
//  Sin make:
//  gcc -o cesar cesar.c
//
//  Ejemplos de ejecucion: 
//  ./cesar [numero de rotaciones] --crypt [Texto a encriptar]
//  ./cesar [numero de rotaciones] --decrypt [Texto a desencriptar] 
// 
//-----------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INICIO_ASCII_MINUSCULAS 97
#define INICIO_ASCII_MAYUSCULAS 65
#define LONGITUD_ALFABETO 26
#define MOD(i, n) (i % n + n) % n

//./cesar 5 --crypt [texto a cifrar]
//./cesar 5 --decrypt [texto a cifrar]

char* encriptar(int desplazamientos, char* mensaje);
char* desencriptar(int desplazamientos, char* mensajeEncriptado);

int main(int argc, char** argv){
    if(argc < 4){
        fprintf(stderr, "Error en los argumentos!\n");
        return -1;
    }

    int desplazamientos = (int)((*argv[1])-'0');
    printf("El numero de desplazamientos es %d.\n", desplazamientos);
    char* mensaje = argv[3];

    if(strcmp(argv[2], "--crypt") == 0){
        printf("El mensaje a encriptar es: %s.\n", mensaje);
        mensaje = encriptar(desplazamientos, mensaje);
        printf("El mensaje encriptado es: %s.\n", mensaje);
    } else if(strcmp(argv[2], "--decrypt") == 0){
        printf("El mensaje a desencriptar es: '%s'.\n", mensaje);
        mensaje = desencriptar(desplazamientos, mensaje);
        printf("El mensaje desencriptado es: '%s'.\n", mensaje);
    } else {
        fprintf(stderr, "El argumento '%s', no esta definido!\n", argv[2]);
        exit(-1);
    }

    return 0;
}

char* encriptar(int desplazamientos, char* mensaje){
    size_t i = 0;
    for(i = 0; i < strlen(mensaje); ++i){
        if(!isupper(mensaje[i]) && isalpha(mensaje[i]))
            mensaje[i] = (char)(((((int)mensaje[i]) - INICIO_ASCII_MINUSCULAS +
            desplazamientos) % LONGITUD_ALFABETO) + 'a');
        else if(isupper(mensaje[i]) && isalpha(mensaje[i]))
            mensaje[i] = (char)(((((int)mensaje[i]) - INICIO_ASCII_MAYUSCULAS +
            desplazamientos) % LONGITUD_ALFABETO) + 'A');
        else mensaje[i] = mensaje[i];
    }
    return mensaje;
}

char* desencriptar(int desplazamientos, char* mensaje){
    size_t i = 0;
    for(i = 0; i < strlen(mensaje); ++i){
        if(!isupper(mensaje[i]) && isalpha(mensaje[i]))
            mensaje[i] = (char)(MOD(((int)mensaje[i]) - INICIO_ASCII_MINUSCULAS -
            desplazamientos, LONGITUD_ALFABETO)+'a');
        else if(isupper(mensaje[i]) && isalpha(mensaje[i]))
            mensaje[i] = (char)(MOD(((int)mensaje[i]) - INICIO_ASCII_MAYUSCULAS -
            desplazamientos, LONGITUD_ALFABETO)+'A');
        else mensaje[i] = mensaje[i];
    }
    return mensaje;
}

//-----------------------------------------------------------------------