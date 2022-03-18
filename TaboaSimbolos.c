

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
            {"float", FLOAT},
            {"real", REAL},
            {"_", BLANK}
    };

    crear(&taboa);
    for (int i = 0; i < sizeof(palabras) / sizeof(tipoelem); ++i) {
        insertar(&taboa, palabras[i]);
        printf("%s %d, ", palabras[i].lexema, palabras[i].tipo);
    }
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
    tipoelem elemento = {cadea, IDENTIFICADOR};
    return buscarInsertar(&taboa, elemento);
}