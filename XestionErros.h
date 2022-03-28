
#ifndef P1_XESTIONERROS_H
#define P1_XESTIONERROS_H

// función para mostrar erros de entrada/saída (apertura de ficheiros, doble buffer)
void erroLE(char* cadea);

// función para mostrar erros léxicos durante a análise (xunto coa liña onde se produce)
void erroLexico(int linea);

#endif //P1_XESTIONERROS_H
