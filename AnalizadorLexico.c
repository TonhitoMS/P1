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
    memset(lex->lex, 0, memoria);
    lex->tipo = 0;
}

void novoCar(CompLex *lex, char c){
    strncat(lex->lex, &c, 1);
    if(strlen(lex->lex)+1 == memoria){
        memoria += MEMORIA;
        lex->lex = (char*) realloc(lex->lex, memoria * sizeof(char));
    }
}

int comentarios(CompLex *lex) {
    char car;

    car = seguinteCaracter();
    if(car == '/'){
        car = seguinteCaracter();
        while(car != '\n' && car != EOF){
            car = seguinteCaracter();
        }
        retrocederCaracter();
    }
    else if(car == '*'){
        while(1){
            car = seguinteCaracter();
            //printf("%c", car);
            if(car == '\n') linea ++;
            else if(car == '*'){
                car = seguinteCaracter();
                //printf("%c", car);
                if(car == '/') { // probar: se non é / retroceder caracter e que siga o while
                    aceptarLexema();
                    break;
                }
                else{
                    retrocederCaracter();
                }
                /*
                else if (car == '\n') linea++;
                else if(car == EOF){
                    retrocederCaracter();
                    aceptarLexema();
                    break;
                }*/
            }
            else if(car == EOF){
                retrocederCaracter();
                aceptarLexema();
                break;
            }
        }
    }
    else{
        lex->tipo = lex->lex[strlen(lex->lex)-1];
        retrocederCaracter();
        aceptarLexema();
        return 1;
    }
    return 0;
}

int strings(CompLex *lex){
    char car;
    int encontrado = 0;
    car = seguinteCaracter();
    novoCar(lex, car);
    while(car != '"' || (lex->lex[strlen(lex->lex)-2] == '\\' && lex->lex[strlen(lex->lex)-3] != '\\')){
        car = seguinteCaracter();
        novoCar(lex, car);
    }
    lex->lex[strlen(lex->lex)] = '\0';
    encontrado = 1;
    lex->tipo = STRING;

    return encontrado;
}

int idOUreservada(CompLex *lex){
    char car;
    int encontrado = 0;

    car = seguinteCaracter();
    novoCar(lex, car);

    if(isalpha(car) || isdigit(car) || car == '_'){
        while (isalpha(car) || isdigit(car) || car == '_'){
            car = seguinteCaracter();
            //printf("%c\n", car);
            novoCar(lex, car);
        }
        lex->lex[strlen(lex->lex)-1] = '\0';
        retrocederCaracter();
        aceptarLexema();
        encontrado++;
        lex->tipo = insertarNaTaboa(lex->lex);
    }
    else if (isalpha(lex->lex[strlen(lex->lex)-2])){
        lex->lex[strlen(lex->lex)-1] = '\0';
        retrocederCaracter();
        aceptarLexema();
        encontrado++;
        lex->tipo = insertarNaTaboa(lex->lex);
    }
    else if(lex->lex[strlen(lex->lex)-2] == '_'){
        lex->lex[strlen(lex->lex)-1] = '\0';
        retrocederCaracter();
        aceptarLexema();
        encontrado++;
        lex->tipo = insertarNaTaboa(lex->lex);
    }

    return encontrado;
}

int asignacion(CompLex *lex){
    char car;
    int encontrado = 0;

    car = seguinteCaracter();
    novoCar(lex, car);
    if(car == '='){
        encontrado ++;
        lex->tipo = OP_ASIG;
        aceptarLexema();
    }
    else{
        lex->lex[strlen(lex->lex)-1] = '\0';
        lex->tipo = lex->lex[strlen(lex->lex)-1];
        encontrado ++;
        retrocederCaracter();
        aceptarLexema();
    }
    return encontrado;
}

int sumaAsignacion(CompLex *lex){
    char car;
    int encontrado = 1;

    car = seguinteCaracter();
    novoCar(lex, car);
    if(car == '='){
        encontrado ++;
        lex->tipo = OP_SUMA_ASIG;
        aceptarLexema();
    }
    else{
        lex->lex[strlen(lex->lex)-1] = '\0';
        lex->tipo = lex->lex[strlen(lex->lex)-1];
        encontrado ++;
        retrocederCaracter();
        aceptarLexema();
    }
    return encontrado;
}

int frecha(CompLex *lex){
    char car;
    int encontrado = 1;

    car = seguinteCaracter();
    novoCar(lex, car);
    if(car == '-'){
        encontrado ++;
        lex->tipo = OP_FRECHA;
        aceptarLexema();
    }
    else{
        lex->lex[strlen(lex->lex)-1] = '\0';
        lex->tipo = lex->lex[strlen(lex->lex)-1];
        encontrado ++;
        retrocederCaracter();
        aceptarLexema();
    }

    return encontrado;
}

int hex(CompLex *lex){
    char car;
    int encontrado = 0;

    car = seguinteCaracter();
    novoCar(lex, car);
    while(isxdigit(car)){
        car = seguinteCaracter();
        novoCar(lex, car);
    }
    retrocederCaracter();
    lex->tipo = ENTEIRO;
    lex->lex[strlen(lex->lex)-1] = '\0';
    encontrado ++;
    aceptarLexema();

    return encontrado;
}

int numE(CompLex *lex){
    char car;
    int encontrado = 0;

    car = seguinteCaracter();
    novoCar(lex, car);
    if(car == '+' || car == '-'){
        car = seguinteCaracter();
        novoCar(lex, car);
    }
    while(isdigit(car)){
        car = seguinteCaracter();
        novoCar(lex, car);
    }
    if(car == 'i'){
        lex->tipo = IMAXINARIO;
        encontrado ++;
        aceptarLexema();
    }
    else{
        retrocederCaracter();
        lex->lex[strlen(lex->lex)-1] = '\0';
        lex->tipo = REAL;
        encontrado ++;
        aceptarLexema();
    }

    return encontrado;
}

int numConPunto(CompLex *lex){
    char car;
    int encontrado = 0;

    car = seguinteCaracter();
    novoCar(lex, car);
    while(isdigit(car)){
        car = seguinteCaracter();
        novoCar(lex, car);
    }
    if(car == 'e' || car == 'E'){
        encontrado = numE(lex);
    }
    else if(!isdigit(lex->lex[strlen(lex->lex) - 2])){
        lex->lex[strlen(lex->lex)-1] = '\0';
        lex->tipo = lex->lex[strlen(lex->lex)-1];
        encontrado ++;
        retrocederCaracter();
        aceptarLexema();
    }
    else{
        retrocederCaracter();
        lex->lex[strlen(lex->lex)-1] = '\0';
        lex->tipo = REAL;
        encontrado ++;
        aceptarLexema();
    }

    return encontrado;
}

int numSenPuntoInicio(CompLex *lex, char car){
    int encontrado = 0;

    while(isdigit(car)){
        car = seguinteCaracter();
        novoCar(lex, car);
    }
    if((car == 'x' || car == 'X') && strlen(lex->lex) == 2 && lex->lex[0] == '0'){
        encontrado = hex(lex);
    }
    else if(car == '.'){
        encontrado = numConPunto(lex);
    }
    else if(car == 'E' || car == 'e'){
        encontrado = numE(lex);
    }
    else if(car == 'i'){
        lex->tipo = IMAXINARIO;
        encontrado ++;
        aceptarLexema();
    }
    else{
        retrocederCaracter();
        lex->lex[strlen(lex->lex)-1] = '\0';
        lex->tipo = ENTEIRO;
        encontrado ++;
        aceptarLexema();
    }
    return encontrado;
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
            encontrado = idOUreservada(lex);
        }
        else if(car == '"'){
            encontrado = strings(lex);
        }
        else if(car == EOF){
            return 0;
        }
        else if(car == '/'){
            encontrado = comentarios(lex);
        }
        else if(car == ';' || car == ',' || car == '=' || car == '-' || car == '*' || car == '(' || car == ')' || car == '[' || car == ']' || car == '{' || car == '}'){
            aceptarLexema();
            lex->tipo = car;
            encontrado++;
        }
        //--------------NÚMEROS---------
        else if(isdigit(car)){//decimais
            encontrado = numSenPuntoInicio(lex, car);
            /*while(isdigit(car)){
                car = seguinteCaracter();
                novoCar(lex, car);
            }
            if((car == 'x' || car == 'X') && strlen(lex->lex) == 2 && lex->lex[0] == '0'){
                encontrado = hex(lex);
                *//*car = seguinteCaracter();
                novoCar(lex, car);
                while(isxdigit(car)){
                    car = seguinteCaracter();
                    novoCar(lex, car);
                }
                retrocederCaracter();
                lex->tipo = ENTEIRO;
                lex->lex[strlen(lex->lex)-1] = '\0';
                encontrado ++;
                aceptarLexema();*//*
            }
            else if(car == '.'){
                car = seguinteCaracter();
                novoCar(lex, car);
                while(isdigit(car)){
                    car = seguinteCaracter();
                    novoCar(lex, car);
                }
                if(car == 'e' || car == 'E'){
                    encontrado = numE(lex);
                    *//*car = seguinteCaracter();
                    novoCar(lex, car);
                    if(car == '+' || car == '-'){
                        car = seguinteCaracter();
                        novoCar(lex, car);
                    }
                    while(isdigit(car)){
                        car = seguinteCaracter();
                        novoCar(lex, car);
                    }
                    if(car == 'i'){
                        lex->tipo = IMAXINARIO;
                        encontrado ++;
                        aceptarLexema();
                    }
                    else{
                        retrocederCaracter();
                        lex->lex[strlen(lex->lex)-1] = '\0';
                        lex->tipo = REAL;
                        encontrado ++;
                        aceptarLexema();
                    }*//*
                }
                else{
                    retrocederCaracter();
                    lex->lex[strlen(lex->lex)-1] = '\0';
                    lex->tipo = REAL;
                    encontrado ++;
                    aceptarLexema();
                }
            }
            else if(car == 'E' || car == 'e'){
                encontrado = numE(lex);
                *//*car = seguinteCaracter();//repitese co anterior
                novoCar(lex, car);
                if(car == '+' || car == '-'){
                    car = seguinteCaracter();
                    novoCar(lex, car);
                }
                while(isdigit(car)){
                    car = seguinteCaracter();
                    novoCar(lex, car);
                }
                if(car == 'i'){
                    lex->tipo = IMAXINARIO;
                    encontrado ++;
                    aceptarLexema();
                }
                else{
                    retrocederCaracter();
                    lex->lex[strlen(lex->lex)-1] = '\0';
                    lex->tipo = REAL;
                    encontrado ++;
                    aceptarLexema();//ata aquí
                }*//*
            }
            else if(car == 'i'){
                lex->tipo = IMAXINARIO;
                encontrado ++;
                aceptarLexema();
            }
            else{
                retrocederCaracter();
                lex->lex[strlen(lex->lex)-1] = '\0';
                lex->tipo = ENTEIRO;
                encontrado ++;
                aceptarLexema();
            }*/
        }
        else if(car == '.'){//repítese co outro else de punto
            encontrado = numConPunto(lex);
            /*printf("\nEncontrado: %d\n", encontrado);
            car = seguinteCaracter();
            novoCar(lex, car);
            while(isdigit(car)){
                car = seguinteCaracter();
                novoCar(lex, car);
            }
            if(car == 'e' || car == 'E'){
                encontrado = numE(lex);
                car = seguinteCaracter();
                novoCar(lex, car);
                if(car == '+' || car == '-'){
                    car = seguinteCaracter();
                    novoCar(lex, car);
                }
                while(isdigit(car)){
                    car = seguinteCaracter();
                    novoCar(lex, car);
                }
                if(car == 'i'){
                    lex->tipo = IMAXINARIO;
                    encontrado ++;
                    aceptarLexema();
                }
                else{
                    retrocederCaracter();
                    lex->lex[strlen(lex->lex)-1] = '\0';
                    lex->tipo = REAL;
                    encontrado ++;
                    aceptarLexema();
                }
            }*/
            /*else{
                lex->lex[strlen(lex->lex)-1] = '\0';
                lex->tipo = lex->lex[strlen(lex->lex)-1];
                encontrado ++;
                retrocederCaracter();
                aceptarLexema();
            }*/
        }

        else if(car == ':'){
            encontrado = asignacion(lex);
        }
        else if(car == '+'){
            encontrado = sumaAsignacion(lex);
        }
        else if(car == '<'){
            encontrado = frecha(lex);
        }

    }

    return fin;
}









