#include "TaboaSimbolos.h"
#include "abb.h"
#include <stdio.h>
#include "Definicions.h"


// declaramos a estrutura de datos como variable global
abb taboa;

void inicioTaboa(){
    // array de estruturas que conteñen as palabras reservadas e o número asociado
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

    // inicializamos a estrutura de datos
    crear(&taboa);

    // recorremos o array de palabras reservadas insertándoas na estrutura de datos
    for (int i = 0; i < sizeof(palabras) / sizeof(tipoelem); ++i) {
        insertar(&taboa, palabras[i]);
        printf("%s %d, ", palabras[i].lexema, palabras[i].tipo);
    }

    // mostramos a táboa por pantalla
    imprimirTaboa(taboa);
}

void imprimirTaboa(){
    // chamamos á función que imprime a estrutura de datos
    printf("\n\n----------Táboa de símbolos--------\n");
    imprimir(taboa);
    printf("-----------------------------------\n");

}

void destruirTaboa(){
    // destruímos a táboa eliminando a estrutura de datos (libera a memoria asociada)
    destruir(&taboa);
}

int insertarNaTaboa(char* cadea){
    /* introducimos a cadea na estrutura tipoelem que recibirá e función da estrutura de datos.
     * O número asociado é 0 por elección, xa que todos os lexemas novos que se introducen na
     * táboa de símbolos son identificadores
     * */
    tipoelem elemento = {cadea, 0};
    // chamamos á función da estrutura de datos e devolvemos o número asociado ao lexema
    return buscarInsertar(&taboa, elemento);
}