OUTPUT_FILE=server

DEFINE_OUTPUT_FILE_NAME=-o

MAIN_FILE=Server.c

MYSQL_LINKER=-L/usr/lib  -lmysqlclient -lz -lm -ldl -lssl -lcrypto

LINKERS=-lpthread $(MYSQL_LINKER)

DEBUGGER=-g

COMPILE=gcc

USE_OTIMIZACAO=-o3
#USE_OTIMIZACAO=

LIKE_A_LIBRARY=-c

WARNINGS=-Wall

#STANDARD=-std=c11

all:
	$(COMPILE) $(WARNINGS) $(USE_OTIMIZACAO) $(STANDARD) $(MAIN_FILE) $(DEFINE_OUTPUT_FILE_NAME) $(OUTPUT_FILE) $(LINKERS)

debug:
	$(COMPILE) $(DEBUGGER) $(WARNINGS) $(STANDARD) $(MAIN_FILE) $(DEFINE_OUTPUT_FILE_NAME) $(OUTPUT_FILE) $(LINKERS)
	valgrind --leak-check=full --track-origins=yes ./server

run: all
	./server

clean:
	rm *.o
