#pragma once
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

/* Possíveis retornos */
#ifndef ERRO
#define ERRO -1
#endif

#ifndef OK
#define OK 0
#endif

/* Variável de CONTROLE */
/***********************************IMPORTANTE********************************************/
/*************************/bool interpretando = false;/***********************************/
/*****************************************************************************************/


/********************************************************************|
|****************************SEGURANCA DO PHP************************/
#define CHAVE_DE_SEGURANCA_PHP "MIGqAgEAAiEArPXCV6pZjBh+OKNVEM/+eIb5KgGZw24jX2mfu2GzFPcCAwEAAQIgCx8KTgTAGUgB5bZq49fb7tzxlVPJya47G9dtFcNrZ7ECEQDgJPU9398P6I0R5MCV+SeVAhEAxYqPkCR6yJ2YuCUZI903WwIRAMmp9swYo8TYYjP7MBq+9JECECeNYMVZqIP2mhd0xh3aMzUCEBGnaCJnmYNciQfqIT1vmws="
#define TAMANHO_CHAVE_DE_SEGURANCA_PHP 232
#define CHAVE_APLICACAO "APP"

/********************************************************************/
#define LOGIN_DE_CADASTRO "interface@servidor"
#define SENHA_DE_CADASTRO "senatauri"

#define LOGIN_DO_SITE "contratante@servidor"
#define SENHA_DO_SITE "qwertyuiopasdfghjklzxcvbnm"


#define TAMANHO_LOGIN 64
#define TAMANHO_EMAIL 64//Mesma coisa que TAMANHO_LOGIN
#define TAMANHO_SENHA 256
#define TAMANHO_SENHA_PADRAO_USUARIO_ANONIMO 4
#define TAMANHO_SEXO_CLIENTE 3


#define TAMANHO_DO_NOME_DA_CIDADE_COM_MAIOR_NOME_DO_MUNDO 58
#define TAMANHO_ESTADO 2
#define TAMANHO_BAIRRO 58
#define TAMANHO_COMPLEMENTO 160
#define TAMANHO_NUMERO 11
#define TAMANHO_CHAVE_PRIMARIA 11
#define TAMANHO_NOME 64
#define TAMANHO_SOBRENOME 64
#define TAMANHO_PLANO 2
#define TAMANHO_TELEFONE 11

#define TAMANHO_COMANDO 1

#define TAMANHO_CNPJ 14
#define TAMANHO_TIPO 2



#define CARACTER_SUBSTITUICAO_ESPACO "_"

#define TAMANHO_DE_INTEIRO_EM_BANCO_DE_DADOS 11

#define TAMANHO_DURACAO 2
#define TAMANHO_DATA 10 
#define TAMANHO_ID_PRODUTO 10

#define TAMANHO_DESCRICAO_PRODUTO 140
#define TAMANHO_NOME_PRODUTO 100


/* RETORNOS */

#define REQUISITANDO_LOGIN 1
#define REQUISITANDO_ATUALIZACAO 2
#define REQUISITANDO_ESPECIFICO 3
#define REQUISITANDO_ADICAO 4
#define REQUISITANDO_OBTENCAO 6

#define OPERACAO_EXECUTADA_COM_SUCESSO 5



/* Constantes de Controle */
#define COMANDO_LOGIN 1
#define COMANDO_ADICIONAR 2
#define COMANDO_ATUALIZAR 3
#define COMANDO_OBTER 4

#define TIPO_USUARIO "1"
#define TIPO_USUARIO_ANONIMO "2"

#define TIPO_LOGIN "7"
#define TIPO_SENHA "9"

#define TIPO_VISUALIZACAO "2"
#define TIPO_VISUALIZACAO_ANONIMA 3
#define TIPO_VISUALIZACAO_ANONIMA_CHAR "3"

#define TIPO_INFORMACAO "&"

#define TIPO_QUANTIDADE "@"
#define TIPO_ESPECIFICO "="

#define TIPO_CONTRATANTE "$C"

#define TIPO_PRODUTO "+"
#define TIPO_NOME_PRODUTO "1."
#define TIPO_DESCRICAO_PRODUTO "Q"


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

//#define TIPO_PAIS 'pa'

#define TIPO_REALIDADE_VIRTUAL "V"
#define TIPO_REALIDADE_AUMENTADA "A"
#define TIPO_REALIDADE_AUMENTADA_E_VIRTUAL "J"

//#define TIPO_VISUALIZACAO_COM_LOGIN []

#define TIPO_DATA "T"

#define TIPO_AVALIACAO "kW"


#define SENHA_PADRAO_USUARIO_ANONIMO "nulo"



#define BUFFER_ADICAO_CONTEUDO 1024*5

#define TOP_10_NOVOS "J"


// #define ARQUIVO_DE_INDICES "indice"
// #define FORMATACAO_DE_INDICE "%d %s"//Número de ID (Server), NUMERO DO IDENTIFICADOR (nome do arquivo)



/* Retornos entre funções */
#define RETORNO_OK 1
#define RETORNO_ERRO_DE_PARAMETRO 0
#define RETORNO_ERRO -1
#define RETORNO_ERRO_CHAR "ERRO\0"
#define RETORNO_ERRO_DE_CONEXAO -2
#define RETORNO_ERRO_FALTA_DE_MEMORIA -3
#define RETORNO_NULO -4
#define RETORNO_NOT_FOUND "NOT_FOUND\0"

/* Fim retornos entre funções */


