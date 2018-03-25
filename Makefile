#SESSÃO PRINCIPAL ******************************************************************************** ↓
OUTPUT_FILE=server

DEFINE_OUTPUT_FILE_NAME=-o

MAIN_FILE=Server.c

# `pkg-config --cflags --libs ruby-2.3`

PTHREAD_LINKER=-lpthread
#PTHREAD_LINKER=-pthread
MYSQL_LINKER=-L/usr/lib  -lmysqlclient -lz -lm -ldl -lssl -lcrypto
# RUBY_LINKER=-I/usr/include/x86_64-linux-gnu/ruby-2.3.0 -I/usr/include/ruby-2.3.0 -lruby-2.3 -lgmp -lcrypt



LINKERS=$(PTHREAD_LINKER) $(MYSQL_LINKER)

DEBUGGER=-g

COMPILE=gcc

USE_OTIMIZACAO=-O3
# USE_OTIMIZACAO=

LIKE_A_LIBRARY=-c

WARNINGS=-Wall

#STANDARD=--std=c11


all: AdaptadorDeString.o Fila.o TCPServer.o
	@echo "Compilando MAIN"
	$(COMPILE) $(WARNINGS) $(USE_OTIMIZACAO) $(STANDARD) $(MAIN_FILE) $(DEFINE_OUTPUT_FILE_NAME) $(OUTPUT_FILE) $(LINKERS) $^

# Comando pra ser usado no server GOOGLE
production: build
	@echo "Realizando ajustes finais"
	rm -f ../server
	mv server ../
	mkdir -p ../Comandos/Ferramentas
	cp Comandos/Ferramentas/ConexaoInternaComServer.rb ../Comandos/Ferramentas/ConexaoInternaComServer.rb
	cp Comandos/Ferramentas/obtemQuantidadeDeHabitantesDoIBGE.rb ../Comandos/Ferramentas/obtemQuantidadeDeHabitantesDoIBGE.rb
	@clear
	@echo "\n\nO VISU-RA Está pronto pra ser utilizado\n\n"
	@echo "Você pode executar './server', na pasta pai desta pasta"

build: all clean

debug: clean
	$(COMPILE) $(DEBUGGER) $(LIKE_A_LIBRARY) Bibliotecas/Fila/Fila.c
	$(COMPILE) $(DEBUGGER) $(LIKE_A_LIBRARY) Bibliotecas/AdaptadorDeString/AdaptadorDeString.c
	$(COMPILE) $(DEBUGGER) $(LIKE_A_LIBRARY) Bibliotecas/TCPServer/TCPServer.c
	$(COMPILE) $(DEBUGGER) $(WARNINGS) $(STANDARD) $(MAIN_FILE) $(DEFINE_OUTPUT_FILE_NAME) $(OUTPUT_FILE) $(LINKERS) AdaptadorDeString.o Fila.o TCPServer.o
	valgrind --leak-check=full --track-origins=yes ./server

run: all
	./server

clean:
	rm -f *.o

Bibliotecas/AdaptadorDeString/AdaptadorDeString.c: Bibliotecas/AdaptadorDeString/AdaptadorDeString.h

AdaptadorDeString.o: Bibliotecas/AdaptadorDeString/AdaptadorDeString.c
	@echo "Compilando AdaptadorDeString"
	$(COMPILE) $(WARNINGS) $(LIKE_A_LIBRARY) $^ $(DEFINE_OUTPUT_FILE_NAME) $@


Bibliotecas/TCPServer/TCPServer.c: Bibliotecas/TCPServer/TCPServer.h

Bibliotecas/TCPServer/TCPServer.o: Bibliotecas/TCPServer/TCPServer.c
	@echo "Compilando mini TCPServer"
	$(COMPILE) $(WARNINGS) $(LIKE_A_LIBRARY) $^ $(DEFINE_OUTPUT_FILE_NAME) $@ $(PTHREAD_LINKER)

TCPServer.o: Bibliotecas/TCPServer/TCPServer.o
	@echo "Reposicionando arquivo compilado Bibliotecas/TCPServer/TCPServer.o"
	mv $^ $@

Bibliotecas/Fila/Fila.c: Bibliotecas/Fila/Fila.h

Fila.o: Bibliotecas/Fila/Fila.c
	@echo "Compilando Fila"
	$(COMPILE) $(WARNINGS) $(LIKE_A_LIBRARY) $^ $(DEFINE_OUTPUT_FILE_NAME) $@
	# $(COMPILE) $(LIKE_A_LIBRARY) Fila/Fila.c

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

doc/ComandosMakefile.pdf: doc/src/ComandosMakefile.tex
	@echo "Gerando documentação de opções de Makefile"
	@$(CHANGE_TO_DOC_SOURCE) ; pdflatex ComandosMakefile.tex
	@$(CHANGE_TO_DOC_SOURCE) ; mv ComandosMakefile.pdf ../

doc/OrganizacaoCodigo.pdf: doc/src/OrganizacaoCodigo.tex
	@echo "Gerando documentação de organização de código"
	@$(CHANGE_TO_DOC_SOURCE) ; pdflatex OrganizacaoCodigo.tex
	@$(CHANGE_TO_DOC_SOURCE) ; mv OrganizacaoCodigo.pdf ../


doc/OperacoesBanco-FuncoesGenericas.pdf: doc/src/OperacoesBanco-FuncoesGenericas.tex
	@echo "Gerando documentação de especificação do arquivo OperacoesBanco-FuncoesGenericas.h"
	@$(CHANGE_TO_DOC_SOURCE) ; pdflatex OperacoesBanco-FuncoesGenericas.tex
	@$(CHANGE_TO_DOC_SOURCE) ; mv OperacoesBanco-FuncoesGenericas.pdf ../

doc/ArvoreDeComandos.pdf: doc/src/ArvoreDeComandos.tex
	@echo "Gerando documentação de especificação da Arvore De Comandos"
	@$(CHANGE_TO_DOC_SOURCE) ; pdflatex ArvoreDeComandos.tex
	@$(CHANGE_TO_DOC_SOURCE) ; mv ArvoreDeComandos.pdf ../


documentation: doc/ComandosMakefile.pdf doc/OrganizacaoCodigo.pdf doc/OperacoesBanco-FuncoesGenericas.pdf doc/ArvoreDeComandos.pdf
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


