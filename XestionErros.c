#include <stdlib.h>
#include <stdio.h>
#include "XestionErros.h"


void erroLE(char* cadea){
    printf("%s", cadea);
    exit(0);
}

void erroLexico(int linea){
    printf("Erro léxico la liña: %d", linea);
}