CC= gcc -Wall
HEADER_FILES_DIR = .
INCLUDES = -I $(HEADER_FILES_DIR)
MAIN= anaLex
SRCS = main.c AnalizadorLexico.c AnalizadorSintactico.c abb.c SistemaDeEntrada.c TaboaSimbolos.c XestionErros.c
DEPS = $(HEADER_FILES_DIR)/AnalizadorLexico.h AnalizadorSintactico.h abb.h Definicions.h SistemaDeEntrada.h TaboaSimbolos.h XestionErros.h
OBJS = $(SRCS:.c=.o)
$(MAIN): $(OBJS)
	$(CC) -o $(MAIN) $(OBJS)
	rm -f *.o *~
%.o: %.c $(DEPS)
	$(CC) -c $< $(INCLUDES)
cleanall: clean
	rm -f $(MAIN)
clean:
	rm -f *.o *~
