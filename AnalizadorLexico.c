#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "AnalizadorLexico.h"
#include "SistemaDeEntrada.h"
#include "TaboaSimbolos.h"
#include "XestionErros.h"
#include "Definicions.h"

// tamaño inicial do lexema e aumento que se realiza
#define MEMORIA 16


// variable global para almacenar a liña de cara a mostrar un erro
int linea = 1;

// variable que almacena a memoria ocupada polo compoñente léxico co que se estea traballando
int memoria;


// función que inicializa a estrutura do compoñente léxico que se empregará
void iniciarLex(CompLex *lex){
    // almacenamos a memoria que ocupa o lexema
    memoria = MEMORIA;
    // liberamos a memoria que houbera asociada
    if(lex->lex){
        free(lex->lex);
        lex->lex = NULL;
    }

    // reservamos a memoria para a cadea e enchemos os campos '\0'
    lex->lex = (char*) malloc(memoria * sizeof(char));
    memset(lex->lex, 0, memoria);
    //tipo a 0 para detectar posibles erros
    lex->tipo = 0;
}

// función para introducir un novo caracter no array
void novoCar(CompLex *lex, char c){
    // engadimos o novo caracter ao array
    strncat(lex->lex, &c, 1);
    // se se chega ao límite auméntase o tamaño do array que almacena o lexema
    if(strlen(lex->lex)+1 == memoria){
        memoria += MEMORIA;
        lex->lex = (char*) realloc(lex->lex, memoria * sizeof(char));
    }
}

// autómata que recoñece os comentarios (cando se recoñece primeiro '/')
int comentarios(CompLex *lex) {
    //variable que almacena o caracter devolto polo sistema de entrada
    char car;

    /* solicitamos ao sistema de entrada o seguinte caracter.
     * Non almacenamos os caracteres xa que non se tratan de ningún lexema,
     * menos no caso de que sexa o caracter '/'
     * */
    car = seguinteCaracter();
    //se é '/' trátase dun comentario de liña
    if(car == '/'){
        car = seguinteCaracter();
        while(car != '\n' && car != EOF){
            car = seguinteCaracter();
        }
        retrocederCaracter();
    }
    // se é '*' trátase dun comentario de bloque
    else if(car == '*'){
        // recoñecemos todos os caracteres ata que atopamos "*/"
        while(1){
            car = seguinteCaracter();
            if(car == '\n') linea ++;
            else if(car == '*'){
                car = seguinteCaracter();
                // se o caracter é '/', acaba o comentario de bloque, e aceptamos o comentario
                if(car == '/') { // probar: se non é / retroceder caracter e que siga o while
                    aceptarLexema();
                    break;
                }
                // noutro caso volvemos ao comezo do bucle
                else{
                    retrocederCaracter();
                }
            }
            //se recoñecemos a fin do ficheiro sen que pechara o comentario de bloque, emitimos un erro
            else if(car == EOF){
                retrocederCaracter();
                aceptarLexema();
                erroLexico(linea);
                break;
            }
        }
    }
    // se despois do primeiro '/' hai outra cousa, trátase do caracter '/'
    else{
        lex->tipo = lex->lex[strlen(lex->lex)-1];
        retrocederCaracter();
        aceptarLexema();
        return 1;
    }
    return 0;
}

// autómata que recoñece strings, cando xa se recoñeceu '"'
int strings(CompLex *lex){
    char car;
    // variable que indica se se encontrou o lexema
    int encontrado = 0;
    int erro = 0;

    //solicitamos o seguinte caracter e almacenámolo
    car = seguinteCaracter();
    novoCar(lex, car);
    // mentres non encontremos '"' sen un caracter de escape '/' antes, gardamos os caracteres que aparecen
    while(car != '"' || (lex->lex[strlen(lex->lex)-2] == '\\' && lex->lex[strlen(lex->lex)-3] != '\\')){
        car = seguinteCaracter();
        novoCar(lex, car);
        /* se encontramos a fin do ficheiro antes de atopar o peche da cadea de caracteres,
         * poñemos a variable local erro a 1, para despois emitir o erro
         * */
        if(car == EOF){
            retrocederCaracter();
            aceptarLexema();
            erroLexico(linea);
            erro = 1;
            break;
        }
    }

    // se a variable erro non está activa, aceptamos a cadea
    if(!erro){
        lex->lex[strlen(lex->lex)] = '\0';
        encontrado = 1;
        lex->tipo = STRING;
    }

    return encontrado;
}

/* autómata que recoñece un identificador ou unha palabar reservada,
 * unha vez recoñecido o primeiro caracter (alfabético ou '_')
 * */
int idOUreservada(CompLex *lex){
    char car;
    int encontrado = 0;

    // solicitamos o seguinte caracter e almacenámolo
    car = seguinteCaracter();
    novoCar(lex, car);

    /* se é un díxito, un caracter alfabético ou '_', recoñecemos todos os caracteres
     * deste tipo que veñan a continuación
     * */
    if(isalpha(car) || isdigit(car) || car == '_'){
        while (isalpha(car) || isdigit(car) || car == '_'){
            car = seguinteCaracter();
            novoCar(lex, car);
        }
        /* cando encontramos un caracter distinto aceptamos o lexema eliminado o último caracter lido,
         * xa que non é parte do identificador ou a palabra reservada. Retrocedemos no sistema de entrada
         * para volver a procesar este caracter. Tamén poñemos a 1 a variable encontrado.
         * */
        lex->lex[strlen(lex->lex)-1] = '\0';
        retrocederCaracter();
        aceptarLexema();
        encontrado++;
        lex->tipo = insertarNaTaboa(lex->lex);
    }
    //se o caracter lido non é alfabético, díxito ou '_', o identificador é un único caracter
    else if (isalpha(lex->lex[strlen(lex->lex)-2])){
        //aceptamos o id eliminando o último caracter lido como no caso anterior
        lex->lex[strlen(lex->lex)-1] = '\0';
        retrocederCaracter();
        aceptarLexema();
        encontrado++;
        lex->tipo = insertarNaTaboa(lex->lex);
    }
    //se ese único caracter se tratade '_', mostrámolo como tal
    else if(lex->lex[strlen(lex->lex)-2] == '_'){
        //aceptamos o lexema
        lex->lex[strlen(lex->lex)-1] = '\0';
        retrocederCaracter();
        aceptarLexema();
        encontrado++;
        lex->tipo = insertarNaTaboa(lex->lex);
    }

    // devolvemos 1 se se encontrou un lexema
    return encontrado;
}

//función que recoñece o símbolo ":=", cando ':' foi encontrado
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

//función que recoñece "+=" cando '+' foi encontrado
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

//función que recoñece "<-" cando < foi encontrado
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

//autómata que recoñece número hexadecimais da forma 0x1234ABC cando foi recoñecido "0x"
int hex(CompLex *lex){
    //mesmas variables que nos casos anteriores
    char car;
    int encontrado = 0;

    //solicitamos o seguinte caracter ao sistema de entrada e almacenámolo
    car = seguinteCaracter();
    novoCar(lex, car);
    //mentres sexan caracteres hexadecimais aceptamos
    while(isxdigit(car)){
        car = seguinteCaracter();
        novoCar(lex, car);
    }
    /* cando son caracteres doutro tipo recoñecemos o número,
     * eliminando como en casos anteriores o último caracter e
     * retrocedendo o sistema de entrada
     * */
    retrocederCaracter();
    lex->tipo = ENTEIRO;
    lex->lex[strlen(lex->lex)-1] = '\0';
    encontrado ++;
    aceptarLexema();

    return encontrado;
}

// autómata que recoñece números con expoñente (tanto con '.' como sen el)
int numE(CompLex *lex){
    char car;
    int encontrado = 0;

    // pedimos o caracter e almacenámolo
    car = seguinteCaracter();
    novoCar(lex, car);
    //se é un simbolo '+' ou '-' continuamos
    if(car == '+' || car == '-'){
        car = seguinteCaracter();
        novoCar(lex, car);
    }
    // mentres haxa díxitos aceptamos
    while(isdigit(car)){
        car = seguinteCaracter();
        novoCar(lex, car);
    }
    // se aparece 'i', trátase dun número imaxinario
    if(car == 'i'){
        lex->tipo = IMAXINARIO;
        encontrado ++;
        aceptarLexema();
    }
    /* noutro caso, aceptamos o número, de novo retrocedendo
     * no sistema de entrada e substituíndo o último caracter
     * */
    else{
        retrocederCaracter();
        lex->lex[strlen(lex->lex)-1] = '\0';
        lex->tipo = REAL;
        encontrado ++;
        aceptarLexema();
    }

    return encontrado;
}


/* autómata que recoñece números que conteñen '.':
 * No medio do número (111.11)
 * ou no comezo (.111).
 * Tamén recoñece o propio punto
 * */
int numConPunto(CompLex *lex){
    char car;
    int encontrado = 0;

    // novo caracter do sistema de entrada
    car = seguinteCaracter();
    novoCar(lex, car);

    /* se os caracteres que están antes e despois do '.'
     * non son números, entón tratase do propio caracter '.'
     * */
    if(!isdigit(lex->lex[strlen(lex->lex) - 3]) && !isdigit(car)){
        lex->lex[strlen(lex->lex)-1] = '\0';
        lex->tipo = lex->lex[strlen(lex->lex)-1];
        encontrado ++;
        retrocederCaracter();
        aceptarLexema();
    }
    else{
        // mentres sexan díxitos aceptamos
        while (isdigit(car)) {
            car = seguinteCaracter();
            novoCar(lex, car);
        }
        // se detectamos unha 'E' ou 'e' é un número con expoñente
        if (car == 'e' || car == 'E') {
            encontrado = numE(lex);
        }
        /* noutro caso aceptamos o número.
         * Coma sempre, retrocedemos o sistema de entrada e
         * eliminamos o último caracter recoñecido
         * */
        else {
            retrocederCaracter();
            lex->lex[strlen(lex->lex) - 1] = '\0';
            lex->tipo = REAL;
            encontrado++;
            aceptarLexema();
        }
    }

    return encontrado;
}

/* autómata que recoñece números sen punto no inicio.
 * Recibe o caracter recoñecido anterior á función xa que
 * se necesita para desenvolver o autómata
 * */
int numSenPuntoInicio(CompLex *lex, char car){
    int encontrado = 0;

    // mentres haxa díxitos aceptamos
    while(isdigit(car)){
        car = seguinteCaracter();
        novoCar(lex, car);
    }
    /* se recoñecemos 'x' ou 'X' e a cadea que levamos procesado é
     * "0x", trátase dun número hexadecimal
     * */
    if((car == 'x' || car == 'X') && strlen(lex->lex) == 2 && lex->lex[0] == '0'){
        encontrado = hex(lex);
    }
    /* se encontramos un punto (estamos na situación 1234.),
     * trátase dun número que contén '.'
     * */
    else if(car == '.'){
        encontrado = numConPunto(lex);
    }
    // se detectamos 'E' ou 'e', atopamos un número con expoñente
    else if(car == 'E' || car == 'e'){
        encontrado = numE(lex);
    }
    /* no caso de atopar unha 'i', trátase dun número imaxinario sen '.'
     * da forma 11234i
     * */
    else if(car == 'i'){
        lex->tipo = IMAXINARIO;
        encontrado ++;
        aceptarLexema();
    }
    /* noutro caso aceptamos o lexema, xa que acabou o número.
     * Coma nos casos anteriores retrocedemos o sistema de entrada,
     * e substituímos o último caracter lido.
     * */
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
    // variable para almacenar o caracter actual
    char car;
    // variable para o recoñecemento dun lexema
    int encontrado = 0;
    // variable para que continúe a consumición do analizador sintáctico
    int fin;

    // mentres non se encontre lexema seguimos recoñecendo caracteres
    while(!encontrado){
        // inicializamos o lexema, solicitamos o primeiro caracter e gardámolo
        iniciarLex(lex);
        car = seguinteCaracter();
        novoCar(lex, car);
        fin = 1;

        /* se empeza por un caracter alfabético ou '_', chamamos ao autómata
         * que recoñece identificadores e palabras reservadas
         * */
        if(isalpha(car) || car == '_'){
            encontrado = idOUreservada(lex);
        }
        // se comeza por '"', trátase dunha string
        else if(car == '"'){
            encontrado = strings(lex);
        }
        // se recoñecemos o caracter de fin de ficheiro acaba a análise
        else if(car == EOF){
            return 0;
        }
        // se comeza por '/' poden ser comentarios ou o propio caracter
        else if(car == '/'){
            encontrado = comentarios(lex);
        }
        // signos de puntuación
        else if(car == ';' || car == ',' || car == '=' || car == '-' || car == '*' || car == '(' || car == ')' || car == '[' || car == ']' || car == '{' || car == '}'){
            aceptarLexema();
            lex->tipo = car;
            encontrado++;
        }
        //--------------NÚMEROS---------
        /* se empeza por un número, chamamos ao autómata que recoñece números
         * que non empezan por punto, e que fará as chamadas sucesivas aos
         * distintos autómatas
         * */
        else if(isdigit(car)){
            encontrado = numSenPuntoInicio(lex, car);
        }
        /* se empeza por '.' pode ser un número ou o propio '.'
         * Chamamos ao autómata que recoñece este tipo de lexemas
         * */
        else if(car == '.'){
            encontrado = numConPunto(lex);
        }
        // se empeza por ':' pode ser ':=' ou o propio caracter
        else if(car == ':'){
            encontrado = asignacion(lex);
        }
        // se empeza por +:' pode ser '+=' ou o propio caracter
        else if(car == '+'){
            encontrado = sumaAsignacion(lex);
        }
        // se empeza por '<' pode ser '<-' ou o propio caracter
        else if(car == '<'){
            encontrado = frecha(lex);
        }
        /* se recoñecemos un salto de liña actualizamos a variable,
         * para o momento de emitir un posible erro
         * */
        else if(car == '\n') linea ++;

    }

    // devolvemos o valor 0 ou 1 dependendo de se xa se chegou ao final do ficheiro
    return fin;
}









