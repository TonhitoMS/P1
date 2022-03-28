#include <stdio.h>
#include <stdlib.h>
#include "AnalizadorSintactico.h"
#include "AnalizadorLexico.h"


void analise(){
    // declaramos a structura que almacena o compoñente léxico e reservamos memoria para ela
    CompLex *lex;
    lex = (CompLex*) malloc(sizeof(CompLex));
    lex->lex = NULL;

    // comezamos o bucle que chamará ao analizador sintáctico (remata cando a función devolve 1)
    while(sigLex(lex)){
        printf("\n<%d, ""%s"">\n", lex->tipo, lex->lex);
    }


    //liberamos a memoria asociada á string da estructura, se esta non se liberou
    if(lex->lex){
        free(lex->lex);
    }

    //liberamos a memoria asociada á estructura
    free(lex);
}