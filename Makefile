#SESSÃO PRINCIPAL ******************************************************************************** ↓
OUTPUT_FILE=server

DEFINE_OUTPUT_FILE_NAME=-o

MAIN_FILE=Server.c

MYSQL_LINKER=-L/usr/lib  -lmysqlclient -lz -lm -ldl -lssl -lcrypto

FILA_OBJ=Fila.o

PTHREAD_LINKER=-lpthread
#PTHREAD_LINKER=-pthread

LINKERS=$(PTHREAD_LINKER) $(MYSQL_LINKER)

DEBUGGER=-g

COMPILE=gcc

#USE_OTIMIZACAO=-o3
USE_OTIMIZACAO=

LIKE_A_LIBRARY=-c

WARNINGS=-Wall

#STANDARD=--std=c11


all: AdaptadorDeString.o $(FILA_OBJ)
	@echo "Compilando MAIN"
	$(COMPILE) $(WARNINGS) $(USE_OTIMIZACAO) $(STANDARD) $(MAIN_FILE) $(DEFINE_OUTPUT_FILE_NAME) $(OUTPUT_FILE) $(LINKERS) $^

build: all
	rm *.o

debug: 
	$(COMPILE) $(DEBUGGER) $(LIKE_A_LIBRARY) Fila/Fila.c
	$(COMPILE) $(DEBUGGER) $(LIKE_A_LIBRARY) AdaptadorDeString/AdaptadorDeString.c
	$(COMPILE) $(DEBUGGER) $(WARNINGS) $(STANDARD) $(MAIN_FILE) $(DEFINE_OUTPUT_FILE_NAME) $(OUTPUT_FILE) $(LINKERS) AdaptadorDeString.o $(FILA_OBJ)
	valgrind --leak-check=full --track-origins=yes ./server

run: all
	./server

clean:
	rm -f *.o

AdaptadorDeString/AdaptadorDeString.c: AdaptadorDeString/AdaptadorDeString.h

AdaptadorDeString.o: AdaptadorDeString/AdaptadorDeString.c
	@echo "Compilando AdaptadorDeString"
	$(COMPILE) $(LIKE_A_LIBRARY) $^ $(DEFINE_OUTPUT_FILE_NAME) $@

$(FILA_OBJ): 
	@echo "Compilando Fila"
	$(COMPILE) $(LIKE_A_LIBRARY) Fila/Fila.c

#SESSÃO PRINCIPAL ******************************************************************************** ↑
#SESSÃO DE DOCUMENTACAO ************************************************************************** ↓
DOC_SOURCES=doc/src
CHANGE_TO_DOC_SOURCE=cd $(DOC_SOURCES)

#Limpa todos os arquivos .pdf gerados por "make documentation"
forget:
	@echo "Atenção ! removendo documentação"
	@echo "Removendo doc/ComandosMakefile.pdf"
	@rm -f doc/ComandosMakefile.pdf
	@echo "Removendo doc/OrganizacaoCodigo.pdf"
	@rm -f doc/OrganizacaoCodigo.pdf

doc/ComandosMakefile.pdf:
	@echo "Gerando documentação de opções de Makefile"
	@$(CHANGE_TO_DOC_SOURCE) ; pdflatex ComandosMakefile.tex
	@$(CHANGE_TO_DOC_SOURCE) ; mv ComandosMakefile.pdf ../

doc/OrganizacaoCodigo.pdf:
	@echo "Gerando documentação de organização de código"
	@$(CHANGE_TO_DOC_SOURCE) ; pdflatex OrganizacaoCodigo.tex
	@$(CHANGE_TO_DOC_SOURCE) ; mv OrganizacaoCodigo.pdf ../

documentation: doc/ComandosMakefile.pdf doc/OrganizacaoCodigo.pdf
	@echo "Gerando meta-documentação (índice)"
	@echo "Toda a documentação necessária foi gerada com sucesso !"
	@echo "Limpando arquivos residuais da compilação da documentação"
	@$(CHANGE_TO_DOC_SOURCE) ; rm -f ComandosMakefile.log
	@$(CHANGE_TO_DOC_SOURCE) ; rm -f texput.log
	@$(CHANGE_TO_DOC_SOURCE) ; rm -f ComandosMakefile.aux
	@echo "Todos os arquivos residuais da compilação foram removidos"
	@echo "Compilação finalizada"






full: forget documentation build
	@echo "Compilação geral concluida"


