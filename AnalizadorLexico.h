

#ifndef P1_ANALIZADORLEXICO_H
#define P1_ANALIZADORLEXICO_H

typedef struct{
    // array de caracteres que almacena o lexema
    char* lex;
    // tipo de compoñente léxico segundo Definicions.h
    int tipo;
}CompLex;

// función do analizador léxico que devolve o seguinte lexema e un enteiro cando se chega ao final
int sigLex(CompLex *lex);

#endif //P1_ANALIZADORLEXICO_H
