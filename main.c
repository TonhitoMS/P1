#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "TaboaSimbolos.h"
#include "SistemaDeEntrada.h"
#include "AnalizadorSintactico.h"

#define MEMORIA 8

typedef struct{
    char* lex;
    int tipo;
}CompLex;



int memoria;

int recursiva(int resul){

    if(resul < 6){
        resul ++;
        resul = recursiva(resul);
    }

    return resul;

}

void iniciarLex1(CompLex *lex){
    memoria = MEMORIA;
    if(lex->lex){
        free(lex->lex);
        lex->lex = NULL;
    }

    lex->lex = (char*) malloc(memoria * sizeof(char));
    lex->tipo = 0;
}

void novoCar1(CompLex *lex, char c){
    strncat(lex->lex, &c, 1);
    if(strlen(lex->lex) == memoria){
        memoria += MEMORIA;
        lex->lex = (char*) realloc(lex->lex, memoria * sizeof(char));
    }

}

int main() {

    inicioTaboa();

/*    int resul = recursiva(0);

    printf("\n%d\n", resul);*/

    iniciarSDE("concurrentSum.go");

    analise();

    imprimirTaboa();

    pecharSDE();

    destruirTaboa();

    if(isalpha(' ')){
        printf("\nCaracter\n");
    }

    /*char car = seguinteCaracter();

    printf("\n%c\n", car);

    retrocederCaracter();


    for (int i = 0; i < 721; ++i) {

        car = seguinteCaracter();
        //retrocederCaracter();
        printf("%c", car);

        //car = seguinteCaracter();
        //printf("%c", car);

    }

    char* proba = (char*) malloc(8 * sizeof(char));
    char fin = proba[7];
    proba = "ola que tal";

    if(proba){
        //printf("\n%d\n", strlen(proba));
        printf("\n%c\n", proba[11]);
        //printf("\n%d\n", sizeof(proba) / sizeof(char));
    }

    retrocederCaracter();
    destruirTaboa();


    pecharSDE();

    CompLex cp;

    iniciarLex1(&cp);

    for (int i = 0; i < MEMORIA * 2; ++i) {
        novoCar1(&cp, 'c');
    }

    printf("\n%s\n", cp.lex);*/

    return 0;
}
