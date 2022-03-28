

#ifndef P1_SISTEMADEENTRADA_H
#define P1_SISTEMADEENTRADA_H

#include <stdio.h>

// función que inicializa o sistema de entrada (apertura do arquivo, inicialización das variables)
void iniciarSDE(char *arq);

// función que pecha o sistema de entrada (peche do arquivo)
void pecharSDE();

// función que devolve o seguinte caracter do buffer
char seguinteCaracter();

//función que retrocede o punteiro do caracter que se está lendo
void retrocederCaracter();

//función que iguala os punteiros inicio e dianteiro, para cando se recoñece o sistema
void aceptarLexema();



#endif //P1_SISTEMADEENTRADA_H
