#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "TaboaSimbolos.h"
#include "SistemaDeEntrada.h"
#include "AnalizadorSintactico.h"


int main() {

    //inicializamos a táboa de símbolos e o sistema de entrada
    inicioTaboa();

    iniciarSDE("concurrentSum.go");

    //comezamos a análise do ficheiro indicado anteriormente
    analise();

    //imprimimos a táboa de símbolos ao finalizar a análise
    imprimirTaboa();

    //pechamos o sistema de entrada (pechando o ficheiro asociado)
    pecharSDE();

    //eliminamos a táboa de símbolos (liberando a memoria asociada)
    destruirTaboa();

    return 0;
}
