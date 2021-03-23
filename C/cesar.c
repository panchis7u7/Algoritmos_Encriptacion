#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    for(i = 0; i < strlen(mensaje); ++i)
        mensaje[i] += desplazamientos;

    printf("El mensaje encriptado es: %s.\n", mensaje);

    return 0;
}