
#ifndef P1_TABOASIMBOLOS_H
#define P1_TABOASIMBOLOS_H

// función que inicializa a estrutura de datos e inserta as palabras reservadas
void inicioTaboa();

/* función que inserta na táboa un compoñente léxico
 * e devolve o número asociado
 * (se xa existe devolve o número asociado)
 * */
int insertarNaTaboa(char* cadea);

// función que imprime a táboa recorrendo a estrutura de datos
void imprimirTaboa();

/* función que elimina a táboa destruíndo a estrutura de datos
 * e liberando a memoria asociada
 * */
void destruirTaboa();


#endif //P1_TABOASIMBOLOS_H
