// #pragma once
#ifndef __COMANDOS__
#define __COMANDOS__
/* Definições rotineiras */

#ifndef bool
#define bool char
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#define return return//é só pra ficar no maldito complete code

/* Possíveis retornos */
#ifndef ERRO
#define ERRO -1
#endif


#ifndef LOG
#define LOG 10
#endif

#ifndef WARNING
#define WARNING 15
#endif

#ifndef DEBUG
#define DEBUG 555
#endif

#ifndef EXTRA
#define EXTRA 159159
#endif

#ifndef OK
#define OK 0
#endif

#ifndef ERRO_CONEXAO_ENCERRADA_MYSQL
#define ERRO_CONEXAO_ENCERRADA_MYSQL 2006
#endif

#ifndef LOGIN_NAO_AUTORIZADO
#define LOGIN_NAO_AUTORIZADO 155
#endif

#ifndef ERRO_COMANDO_INCORRETO
#define ERRO_COMANDO_INCORRETO 156
#endif


#ifndef ERRO_DE_EXECUCAO
#define ERRO_DE_EXECUCAO 157
#endif



#define BUFFER_ADICAO_CONTEUDO 1024*5	// Inútil
#define BUFFER_CLIENTE 1024//  Quantidade de bytes que o servidor pode receber  (aceitou 1024*1024 (1MB))
#define PORTA 7420	// Porta padrão para conexão externa (a porta da interface acessível externamente)

#define PORTA_PADRAO_CONEXAO_INTERNA 13598	//Porta padrão para conexão com scripts externos executados internamente

#define PORTA_PADRAO_WEB 80	//Porta para conexão com clientes HTTP (caso necessário no futuro, implementado mas não usado)

#define QTD_CLIENTE 5 /* Quantidade de clientes máxima aceita */



/********************************************************************|
|****************************SEGURANCA DO PHP************************/
#define CHAVE_DE_SEGURANCA_PHP "MIGqAgEAAiEArPXCV6pZjBh+OKNVEM/+eIb5KgGZw24jX2mfu2GzFPcCAwEAAQIgCx8KTgTAGUgB5bZq49fb7tzxlVPJya47G9dtFcNrZ7ECEQDgJPU9398P6I0R5MCV+SeVAhEAxYqPkCR6yJ2YuCUZI903WwIRAMmp9swYo8TYYjP7MBq+9JECECeNYMVZqIP2mhd0xh3aMzUCEBGnaCJnmYNciQfqIT1vmws="
#define TAMANHO_CHAVE_DE_SEGURANCA_PHP 232
#define CHAVE_APLICACAO "APP"
#define TAMANHO_CHAVE_APLICACAO 3

/********************************************************************/

//Login temporario
#define LOGIN_USUARIO_ROOT "interface@servidor"
#define SENHA_USUARIO_ROOT "senatauri"

#define LOGIN_DE_CADASTRO "interface@servidor"
#define SENHA_DE_CADASTRO "senatauri"

#define LOGIN_DO_SITE "contratante@servidor"
#define SENHA_DO_SITE "qwertyuiopasdfghjklzxcvbnm"

#define SENHA_PADRAO_USUARIO_ANONIMO "nulo"

#define TAMANHO_CONFIRMADOR_DE_EMAIL_RUBY 21
#define CONFIRMADOR_DE_EMAIL_RUBY "scripts/confirmador.rb"

#define COMANDO_MONITOR_DO_BANCO_DE_DADOS_RUBY "ruby scripts/DatabaseMonitor/monitor.rb"


#define TAMANHO_LOGIN 64
#define TAMANHO_EMAIL 64//Mesma coisa que TAMANHO_LOGIN
#define TAMANHO_SENHA 256
#define TAMANHO_SENHA_PADRAO_USUARIO_ANONIMO 4
#define TAMANHO_SEXO_CLIENTE 3


#define TAMANHO_DO_NOME_DA_CIDADE_COM_MAIOR_NOME_DO_MUNDO 58
#define TAMANHO_ESTADO 2
#define TAMANHO_BAIRRO 58
#define TAMANHO_RUA 45
#define TAMANHO_COMPLEMENTO 160
#define TAMANHO_NUMERO 11
#define TAMANHO_CHAVE_PRIMARIA 11
#define TAMANHO_NOME 64
#define TAMANHO_SOBRENOME 64
#define TAMANHO_PLANO 2
#define TAMANHO_TELEFONE 11
#define TAMANHO_CEP 8

#define TAMANHO_COMANDO 1

#define TAMANHO_CNPJ 14
#define TAMANHO_TIPO 2

#define TAMANHO_TIPO_PRODUTO 1



#define CARACTER_SUBSTITUICAO_ESPACO "_"

#define TAMANHO_DE_INTEIRO_EM_BANCO_DE_DADOS 11

#define TAMANHO_DURACAO 2
#define TAMANHO_DATA 10 
#define TAMANHO_ID_PRODUTO 10

#define TAMANHO_DESCRICAO_PRODUTO 512
#define TAMANHO_NOME_PRODUTO 100
#define TAMANHO_CATEGORIA 2

#define TAMANHO_TITULO_FEEDBACK 100
#define TAMANHO_CONTEUDO_FEEDBACK 500






/* Constantes de Controle */
#define COMANDO_LOGIN 1
#define COMANDO_ADICIONAR 2
#define COMANDO_ATUALIZAR 3
#define COMANDO_OBTER 4
#define COMANDO_REMOVER 5
#define COMANDO_ROOT 7

#define TIPO_USUARIO "1"
#define TIPO_USUARIO_ANONIMO "2"

#define TIPO_LOGIN "7"
#define TIPO_SENHA "9"

#define TIPO_INFORMACAO_PAGINADA "##"

#define TIPO_VISUALIZACAO "2"
#define TIPO_VISUALIZACAO_ANONIMA 3
#define TIPO_VISUALIZACAO_ANONIMA_CHAR "3"
#define TIPO_VISUALIZACAO_NA_CIDADE "vC"

#define TIPO_INFORMACAO "&"

#define TIPO_QUANTIDADE "@"
#define TIPO_QUANTIDADE_DE_HABITANTES_DA_CIDADE "qC"
#define TIPO_QUANTIDADE_DE_RECLAMACOES_PRODUTO "rP"
#define TIPO_ESPECIFICO "="

#define TIPO_CONTRATANTE "$C"

#define TIPO_PRODUTO "+"
#define TIPO_NOME_PRODUTO "1."
#define TIPO_DESCRICAO_PRODUTO "Q"
#define TIPO_CATEGORIA "7z"

#define TIPO_LISTA_DESEJOS "LD"

#define TIPO_LOCALIZACAO "l0"
#define TIPO_NUMERO "n"
#define TIPO_NUMERO_NULO "nN"
#define TIPO_NOME "#"
#define TIPO_SOBRENOME "Y"
#define TIPO_SEXO "SQ"
#define TIPO_BAIRRO "b"
#define TIPO_BAIRRO_NULO "bN"
#define TIPO_CEP "L"
#define TIPO_TELEFONE ">"
#define TIPO_TELEFONE_NULO ">N"
//#define TIPO_CEP_NULO "cN"
#define TIPO_RUA "R"
#define TIPO_RUA_NULO "RN"
#define TIPO_COMPLEMENTO "M"
#define TIPO_COMPLEMENTO_NULO "MN"
#define TIPO_CIDADE "{"
#define TIPO_ESTADO "S"

#define TIPO_ID_CIDADE "$"
#define TIPO_ID_CONTRATANTE ";"
#define TIPO_ID_PRODUTO "*"
#define TIPO_ID_LOCALIZACAO "i0"

//#define TIPO_PAIS 'pa'

#define TIPO_REALIDADE_AUMENTADA "1"
#define TIPO_REALIDADE_VIRTUAL "2"
#define TIPO_REALIDADE_AUMENTADA_E_VIRTUAL "3"

//#define TIPO_VISUALIZACAO_COM_LOGIN []

#define TIPO_DATA "T"

#define TIPO_AVALIACAO "kW"
#define TIPO_FEEDBACK "tr"
#define TIPO_PONTO "Dj"

#define TIPO_INFORMACOES_PRODUTO "iP"
#define TIPO_INFORMACOES_MINHAS "iM"

#define TIPO_ID_DE_PRODUTO_POR_EMPRESA "33"
#define TIPO_ID_DE_EMPRESA_POR_PRODUTO "EP"

















#define TOP_10_NOVOS "J"
#define TOP_10_MELHORES "9&"


// #define ARQUIVO_DE_INDICES "indice"
// #define FORMATACAO_DE_INDICE "%d %s"//Número de ID (Server), NUMERO DO IDENTIFICADOR (nome do arquivo)



/* RETORNOS */

#define REQUISITANDO_LOGIN 998
#define REQUISITANDO_ATUALIZACAO 999
#define REQUISITANDO_ESPECIFICO 1000
#define REQUISITANDO_ADICAO 1001
#define REQUISITANDO_OBTENCAO 1002
#define REQUISITANDO_REMOCAO 1003
#define REQUISITANDO_ROOT 1004

#define OPERACAO_EXECUTADA_COM_SUCESSO 1005

/* Retornos entre funções */
#define RETORNO_OK 1
#define RETORNO_OK_STR_DINAMICA strdup("OK");

#define RETORNO_ERRO_INTERNO 2
#define RETORNO_ERRO_DE_PARAMETRO 8
#define RETORNO_ERRO 0
#define RETORNO_ERRO_DE_CONEXAO -2
#define RETORNO_ERRO_FALTA_DE_MEMORIA -3
#define RETORNO_NULO -4
#define RETORNO_NAO_AUTORIZADO -10

#define RETORNO_ADICAO_RECUSADA 3
#define RETORNO_COMANDO_INCORRETO 4

#define RETORNO_COMANDO_INCORRETO_STR "ERRO: Comando incorreto detectado"

#define RETORNO_ERRO_CHAR "ERRO\0"
#define RETORNO_NOT_FOUND "NOT_FOUND\0"
#define RETORNO_ERRO_INTERNO_STR_DINAMICA strdup("ERRO: interno, tente novamente");
#define RETORNO_ERRO_INTERNO_STR "ERRO: interno, tente novamente"
#define RETORNO_ERRO_INTERNO_BANCO_STR_DINAMICA strdup("ERRO: interno(banco de dados), tente novamente");
#define RETORNO_ERRO_NOT_FOUND_STR_DINAMICA strdup("ERRO: nada encontrado");
#define RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA strdup("ERRO: Comando incorreto/insuficiente detectado");
#define RETORNO_ERRO_COMANDO_NAO_CONSTRUIDO_STR_DINAMICA strdup("ERRO: Comando em construcao");
#define RETORNO_ERRO_NAO_AUTORIZADO_STR_DINAMICA strdup("ERRO: Operacao nao autorizada");
#define RETORNO_ERRO_NAO_AUTORIZADO_STR "ERRO: Operacao nao autorizada"
#define RETORNO_FALHA_AO_ATUALIZAR_STR_DINAMICA strdup("ERRO: Falha ao atualizar dados");

/* Fim retornos entre funções */

// #define SCRIPT_DE_ATUALIZACAO_DE_QUANTIDADE_DE_HABITANTES_DE_CIDADE "Comandos/Ferramentas/obtemQuantidadeDeHabitantesDoIBGE.rb"

#define SCRIPT_DE_ATUALIZACAO_DE_QUANTIDADE_DE_HABITANTES_DE_CIDADE "Comandos/Ferramentas/obtemQuantidadeDeHabitantesDoIBGE.rb"






/* Coisas do Banco de Dados */


#define DATABASE_HOST "127.0.0.1"//TESTES
// #define DATABASE_HOST "35.186.190.243"//GOOGLE CLOUD
#define DATABASE_USER "interface"
#define DATABASE_PASSWORD "essaSenhaEsoPraInteFaCeSaBeRPorQuESiM"
#define DATABASE_SCHEMA "teste"
#define DATABASE_PORT 0
#define DATABASE_DEFAULT_SOCKET NULL
#define DATABASE_DEFAULT_FLAGS 0








#endif //__COMANDOS__
