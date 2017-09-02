OUTPUT_FILE=server

DEFINE_OUTPUT_FILE_NAME=-o

MAIN_FILE=Server.c

MYSQL_LINKER=-L/usr/lib  -lmysqlclient -lz -lm -ldl -lssl -lcrypto

FILA_OBJ=Fila.o

LINKERS=-lpthread $(MYSQL_LINKER)

DEBUGGER=-g

COMPILE=gcc

USE_OTIMIZACAO=-o3
#USE_OTIMIZACAO=

LIKE_A_LIBRARY=-c

WARNINGS=-Wall

#STANDARD=-std=c11


all: Fila_Objeto
	@echo "Compilando MAIN"
	$(COMPILE) $(WARNINGS) $(USE_OTIMIZACAO) $(STANDARD) $(MAIN_FILE) $(DEFINE_OUTPUT_FILE_NAME) $(OUTPUT_FILE) $(LINKERS) $(FILA_OBJ)
	rm *.o

debug: Fila.o
	$(COMPILE) $(DEBUGGER) $(WARNINGS) $(STANDARD) $(MAIN_FILE) $(DEFINE_OUTPUT_FILE_NAME) $(OUTPUT_FILE) $(LINKERS) $(FILA_OBJ)
	valgrind --leak-check=full --track-origins=yes ./server

run: all
	./server

clean:
	rm *.o

Fila_Objeto: 
	@echo "Compilando Fila"
	$(COMPILE) $(LIKE_A_LIBRARY) Fila/Fila.c
