#include <stdio.h>
#include <stdlib.h>
#include "TaboaSimbolos.h"

int recursiva(int resul){

    if(resul < 6){
        resul ++;
        resul = recursiva(resul);
    }

    return resul;

}

int main() {

    inicioTaboa();
    destruirTaboa();

    int resul = recursiva(0);

    printf("\n%d\n", resul);

    return 0;
}
