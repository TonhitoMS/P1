#include <string.h>
#include <stdlib.h>
#include "TaboaSimbolos.h"
#include "abb.h"
#include <stdio.h>
#include "Definicions.h"

abb taboa;

void inicioTaboa(){
    tipoelem palabras[] = {
            {"package", PACKAGE},
            {"import", IMPORT},
            {"func", FUNC},
            {"chan", CHAN},
            {"var", VAR},
            {"for", FOR},
            {"range", RANGE},
            {"go", GO},
            {"float32", FLOAT},
            {"real", REAL},
            {"_", BLANK},
    };

    crear(&taboa);
    for (int i = 0; i < sizeof(palabras) / sizeof(tipoelem); ++i) {
        insertar(&taboa, palabras[i]);
        printf("%s %d, ", palabras[i].lexema, palabras[i].tipo);
    }
    //printf("\nPackage: %d\n",buscarInsertar(&taboa, palabras[0]));
    imprimirTaboa(taboa);
}

void imprimirTaboa(){
    printf("\n\n----------Táboa de símbolos--------\n");
    imprimir(taboa);
    printf("-----------------------------------\n");

}

void destruirTaboa(){
    destruir(&taboa);
}

int insertarNaTaboa(char* cadea){
    tipoelem elemento = {cadea, 0};
    /*elemento.lexema = (char*) malloc(strlen(cadea) * sizeof(char));
    elemento.lexema[0] = '\0';
    strncat(elemento.lexema, cadea, strlen(cadea));*/
    //printf("\nCadea: %s\n", elemento.lexema);
    return buscarInsertar(&taboa, elemento);
}