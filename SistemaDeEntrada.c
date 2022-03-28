#include "SistemaDeEntrada.h"
#include "XestionErros.h"
#define N 128
//macro para calcular o tamaño dun array
#define TARRAY(x)  (sizeof(x) / sizeof((x)[0]))

//estrutura que contén os dous buffers, e os punteiros inicio e dianteiro
typedef struct{
    char buffer1[N+1];
    char buffer2[N+1];
    // punteiro que indica onde comeza o compoñente que se está recoñecendo
    int inicio;
    // punteiro que inidica o caracter que se está lendo
    int dianteiro;
}dobleBuffer;

//declaramos o doble buffer como variable global
dobleBuffer dB;
// tamén o arquivo
FILE *arquivo;
// variable que indica en que buffer se está traballando
int buffer;
// variable que indica se se cambiou de buffer ao retroceder un caracter
int retroceso = 0;
// variable que almacena a posición do arquivo que estamos analizando
long posArq = 0;
// variable que almacena os bytes lidos, para situar a fin da cadea cando se len os últimos bytes do arquivo
int bytes = N;

void iniciarSDE(char *arq){
    //inicializamos as variables de control do sistema de entrada
    dB.inicio = 0;
    dB.dianteiro = 0;
    buffer = 0;

    //abrimos os arquivo comprobando os posibles erros
    if((arquivo = fopen(arq, "r")) == NULL){
        erroLE("Erro abrindo o arquivo ");
    }
    fseek(arquivo, posArq, SEEK_CUR);
    fread(dB.buffer1, sizeof (char), TARRAY(dB.buffer1) - 1, arquivo);
    dB.buffer1[TARRAY(dB.buffer1) - 1] = EOF;

    if(ferror(arquivo)){
        erroLE("Erro no inicio do buffer A");
    }
    else{
        //alamcenamos a posición do punteiro de lectura do arquivo
        fflush(arquivo);
        posArq = ftell(arquivo);
    }
}

void encherBuffer(){
    //dependendo do buffer que está activo, enchemos un ou outro
    if(!buffer){
        //situamos o punteiro de lectura na posición que tiñamos almacenada
        fseek(arquivo, posArq, SEEK_SET);
        /* obtemos os bytes necesarios para encher o buffer, almacenando este número na variable bytes
         * para cando se chega á fin do ficheiro e non se obteñen os suficientes bytes para encher
         * o buffer
         * */
        bytes = fread(dB.buffer2, sizeof (char), TARRAY(dB.buffer2) - 1, arquivo);
        //segundo os bytes lidos, situamos a fin do ficheiro, para o caso anteriormente exposto
        dB.buffer2[bytes] = EOF;

        //comprobamos se se produciu algún erro
        if(ferror(arquivo)){
            erroLE("Erro no enchido do buffer 2");
        }
        else{
            /* forzamos a saído dos datos do buffer temporal, para posteriormente almacenar a posición
             * do arquivo
             * */
            fflush(arquivo);
            posArq = ftell(arquivo);
            //situamos o punteiro dianteiro na primeira posición do buffer
            dB.dianteiro = 0;
        }

    }
    else if(buffer){ // situación análoga para o outro buffer
        fseek(arquivo, posArq, SEEK_SET);
        bytes = fread(dB.buffer1, sizeof (char), TARRAY(dB.buffer1) - 1, arquivo);
        dB.buffer1[bytes] = EOF;

        if(ferror(arquivo)){
            erroLE("Erro no inicio do buffer 1");
        }
        else{
            fflush(arquivo);
            posArq = ftell(arquivo);
            dB.dianteiro = 0;
        }

    }

}

// cambiamos a variable que indica o buffer que está activo
void cambiarBActivo(){
    buffer = !buffer;
}

char seguinteCaracter(){
    char car = '\0';

    //dependendo do buffer que estea activo, traballamos nun buffer ou outro
    if(!buffer){
        /* se o caracter que lemos é EOF, comprobamos se é a fin do ficheiro ou
         * o centinela
         * */
        if((car = dB.buffer1[dB.dianteiro]) == EOF){
            if(!feof(arquivo)){
                /* se é o centinela enchemos o buffer se non se retrocedeu anteriormente
                 * chamando novamente á función, o que devolverá finalmente o primeiro caracter
                 * do seguinte buffer
                 * */
                if(!retroceso) encherBuffer();
                else retroceso = 0;
                cambiarBActivo();
                car = seguinteCaracter();
            }
            // noutro caso avanzamos o punteiro dianteiro
            else dB.dianteiro ++;
        }
        else dB.dianteiro++;
    }
    // situación análoga se está activo o outro buffer
    else if(buffer){
        if((car = dB.buffer2[dB.dianteiro]) == EOF){
            if(!feof(arquivo)){
                if(!retroceso) encherBuffer();
                else retroceso = 0;
                cambiarBActivo();
                car = seguinteCaracter();
            }
            else dB.dianteiro++;
        }
        else dB.dianteiro++;
    }

    //devolvemos o caracter lido (se é EOF acabará a análise)
    return car;
}

void retrocederCaracter(){
    /* se o buffer está na primeira posición poñemos a 1 a variable que indica
     * se se retrocedeu ao buffer anterior, e igualamos dianteiro á última posición
     * (a anterior ao EOF centinela)
     * */
    if(dB.dianteiro == 0){
        cambiarBActivo();
        dB.dianteiro = N - 1;
        retroceso = 1;
    }
    //noutro caso retrocedemos o punteiro dianteiro do buffer
    else dB.dianteiro --;


}

// se se acepta o lexema, igualamos os punterios
void aceptarLexema(){
    dB.inicio = dB.dianteiro;
}


// pechamos o ficheiro cando rematamos a análise
void pecharSDE(){
    fclose(arquivo);
}
