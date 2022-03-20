#include "SistemaDeEntrada.h"
#define N 128
#define TARRAY(x)  (sizeof(x) / sizeof((x)[0]))

typedef struct{
    char buffer1[N+1];
    char buffer2[N+1];
    int inicio;
    int dianteiro;
}dobleBuffer;


dobleBuffer dB;
FILE *arquivo;
int buffer;
int retroceso = 0;
long posArq = 0;

void iniciarSDE(char *arq){
    dB.inicio = 0;
    dB.dianteiro = 0;
    buffer = 0;

    if((arquivo = fopen(arq, "r")) == NULL){
        printf("Erro abrindo o arquivo %s\n", arq);
    }
    fseek(arquivo, posArq, SEEK_CUR);
    fread(dB.buffer1, sizeof (char), TARRAY(dB.buffer1) - 1, arquivo);
    dB.buffer1[TARRAY(dB.buffer1) - 1] = EOF;

    if(ferror(arquivo)){
        printf("Erro no inicio do buffer A");
    }
    else{
        fflush(arquivo);
        posArq = ftell(arquivo);
    }
}

void encherBuffer(){
    if(!buffer){
        //printf("\nEnchemos buffer2\n");
        //printf("\n%ld\n", posArq);
        fseek(arquivo, posArq, SEEK_SET);
        fread(dB.buffer2, sizeof (char), TARRAY(dB.buffer2) - 1, arquivo);
        dB.buffer2[TARRAY(dB.buffer2) - 1] = EOF;

        if(ferror(arquivo)){
            printf("Erro no inicio do buffer A");
        }
        else{
            fflush(arquivo);
            //printf("\n%d\n", posArq);
            posArq = ftell(arquivo);
            //printf("\n%ld\n", posArq);
            dB.dianteiro = 0;
        }

    }
    else if(buffer){
        fseek(arquivo, posArq, SEEK_SET);
        fread(dB.buffer1, sizeof (char), TARRAY(dB.buffer1) - 1, arquivo);
        dB.buffer1[TARRAY(dB.buffer1) - 1] = EOF;

        if(ferror(arquivo)){
            printf("Erro no inicio do buffer A");
        }
        else{
            fflush(arquivo);
            posArq = ftell(arquivo);
            dB.dianteiro = 0;
        }

    }

}

void cambiarBActivo(){
    /*if(!buffer) buffer = 1;
    else buffer = 0;*/
    buffer = !buffer;

}

char seguinteCaracter(){
    char car = '\0';

    if(!buffer){
        if((car = dB.buffer1[dB.dianteiro]) == EOF){
            //printf("\nEncontramos EOF buffer 1\n");
            if(!feof(arquivo)){
                //printf("\nNon é fin de ficheiro\n");
                if(!retroceso) encherBuffer();
                else retroceso = 0;
                cambiarBActivo();
                car = seguinteCaracter();
            }

        }
        else dB.dianteiro++;
    }
    else if(buffer){
        if((car = dB.buffer2[dB.dianteiro]) == EOF){
            //printf("\nEncontramos EOF\n");
            if(!feof(arquivo)){
                if(!retroceso) encherBuffer();
                else retroceso = 0;
                cambiarBActivo();
                car = seguinteCaracter();
            }

        }
        else dB.dianteiro++;
    }

    return car;
}

void retrocederCaracter(){
    if(!buffer){
        if(dB.dianteiro == 0){
            cambiarBActivo();
            dB.dianteiro = N - 1; // última posición do buffer anterior
            retroceso = 1;
        }
        else dB.dianteiro --;
    }

    else if(buffer){
        if(dB.dianteiro == 0){
            cambiarBActivo();
            dB.dianteiro = N - 1;
            retroceso = 1;
        }
        else dB.dianteiro --;
    }

}

void aceptarLexema(){
    dB.inicio = dB.dianteiro;
}



void pecharSDE(){
    fclose(arquivo);
}
