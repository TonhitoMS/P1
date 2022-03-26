#include <stdio.h>
#include <stdlib.h>
#include "AnalizadorSintactico.h"
#include "AnalizadorLexico.h"


void analise(){
    CompLex *lex;

    lex = (CompLex*) malloc(sizeof(CompLex));
    lex->lex = NULL;

    while(sigLex(lex)){
        printf("\n<%d, ""%s"">\n", lex->tipo, lex->lex);
    }
    //sigLex(lex);
    //printf("<%d, ""%s"">\n", lex->tipo, lex->lex);

    printf("\nChegou ao fin do ficheiro\n");

    if(lex->lex){
        free(lex->lex);
    }

    free(lex);
}