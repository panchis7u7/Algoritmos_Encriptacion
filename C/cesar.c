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
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

//Calculo de modulo positivo.
#define MOD(i, n) (i % n + n) % n

char* encriptar(int desplazamientos, char* mensaje);
char* desencriptar(int desplazamientos, char* mensajeEncriptado);

int main(int argc, char** argv){
    if(argc < 4){
        fprintf(stderr, "%sError en los argumentos!\n", KRED);
        return -1;
    }

    printf("%s--------------------------------------------------------\n", KCYN);
    printf("\t\t%sCifrado de Cesar!\n\n", KYEL);

    int desplazamientos = (int)((*argv[1])-'0');
    printf("%sEl numero de desplazamientos es %d.\n\n", KCYN, desplazamientos);
    char* mensaje = argv[3];

    if(strcmp(argv[2], "--crypt") == 0){
        printf("%sEl mensaje a encriptar es: %s%s\n", KWHT, KYEL, mensaje);
        mensaje = encriptar(desplazamientos, mensaje);
        printf("%sEl mensaje encriptado es: %s%s\n\n", KWHT, KYEL, mensaje);
    } else if(strcmp(argv[2], "--decrypt") == 0){
        printf("%sEl mensaje a desencriptar es: %s%s\n", KWHT, KYEL, mensaje);
        mensaje = desencriptar(desplazamientos, mensaje);
        printf("%sEl mensaje desencriptado es: %s%s\n\n", KWHT, KYEL, mensaje);
    } else {
        fprintf(stderr, "%sEl argumento '%s', no esta definido!\n\n", KRED, argv[2]);
        exit(-1);
    }

    printf("%s--------------------------------------------------------\n", KCYN);

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