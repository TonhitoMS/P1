#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "AnalizadorLexico.h"
#include "SistemaDeEntrada.h"
#include "TaboaSimbolos.h"
#include "XestionErros.h"
#include "Definicions.h"

#define MEMORIA 16



int linea = 1;

int memoria;


void iniciarLex(CompLex *lex){
    memoria = MEMORIA;
    if(lex->lex){
        free(lex->lex);
        lex->lex = NULL;
    }

    lex->lex = (char*) malloc(memoria * sizeof(char));
    lex->tipo = 0;
}

void novoCar(CompLex *lex, char c){
    strncat(lex->lex, &c, 1);
    if(strlen(lex->lex)+1 == memoria){
        memoria += MEMORIA;
        lex->lex = (char*) realloc(lex->lex, memoria * sizeof(char));
    }

}

int sigLex(CompLex *lex){
    char car;
    int encontrado = 0;
    int fin;

    while(!encontrado){
        iniciarLex(lex);
        car = seguinteCaracter();
        novoCar(lex, car);
        fin = 1;

        if(isalpha(car) || car == '_'){
            car = seguinteCaracter();
            novoCar(lex, car);

            if(isalpha(car) || isdigit(car) || car == '_'){
                while (isalpha(car) || isdigit(car) || car == '_'){
                    car = seguinteCaracter();
                    printf("%c\n", car);
                    novoCar(lex, car);
                }
                lex->lex[strlen(lex->lex)-1] = '\0';
                retrocederCaracter();
                aceptarLexema();
                encontrado++;
                fin = 1;
                lex->tipo = insertarNaTaboa(lex->lex);
            }
        }
    }

    return fin;
}









