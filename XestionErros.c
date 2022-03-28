#include <stdlib.h>
#include <stdio.h>
#include "XestionErros.h"


void erroLE(char* cadea){
    //imprimimos a cadea recibida e saímos do programa polo erro
    printf("%s", cadea);
    exit(1);
}

void erroLexico(int linea){
    //informamos do erro e imprimos a liña asociada
    printf("\nErro léxico na liña: %d\n", linea);
}