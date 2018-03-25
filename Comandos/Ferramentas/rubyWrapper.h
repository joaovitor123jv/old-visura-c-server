#include<stdio.h>
#include<string.h>
#include <ruby.h>
#include"../../AdaptadorDeString/AdaptadorDeString.h"
#include"../../Usuario.h"
#include "../Comandos.h"
#include "../../Bibliotecas/TCPServer/TCPServer.h"




//TODO CORRIGIR ISSO AQUI !!!
// void *chamaFuncaoRuby(const char *nomeArquivo, char *nomeFuncao, char *argumentoEnviado)
// {
// 	if (nomeArquivo == NULL)
// 	{
// 		geraLog(ERRO, "Não foi passado o nome do arquivo a ser aberto");
// 		return NULL;
// 	}
// 	if (nomeFuncao == NULL)
// 	{
// 		geraLog(ERRO, "Não foi passado o nome da função a ser executada");
// 		return NULL;
// 	}
	

// 	ruby_init();
// 	ruby_init_loadpath();
// 	int state;
// 	VALUE valorRuby;// Struct que assume valores ruby retornados (mágica da biblioteca ruby.h)

// 	rb_require(nomeArquivo);

// 	if (argumentoEnviado == NULL)
// 	{
// 		valorRuby = rb_eval_string_protect(nomeFuncao, &state);		
// 	}
// 	else
// 	{
// 		geraLog(LOG, "Gerando comando");
// 		int tamanho = strlen(nomeFuncao) + 2 + strlen(argumentoEnviado);
// 		char *comando = (char *)calloc(sizeof(char), tamanho);
// 		if (comando == NULL)
// 		{
// 			return NULL;
// 		}
// 		snprintf(comando, tamanho, "%s %s", nomeFuncao, argumentoEnviado);
// 		if (comando == NULL)
// 		{
// 			return NULL;
// 		}
// 		geraLog(LOG, "Comando ruby gerado");
// 		printf("\t Comando gerado = |%s|\n",comando);
// 		valorRuby = rb_eval_string_protect(comando, &state);
// 		free(comando);
// 		comando = NULL;
// 	}

// 	if (state)
// 	{
// 		geraLog(ERRO, "Ocorreu um erro ao executar o script especificado");
// 		return NULL;
// 	}

// 	geraLog(LOG, "Função executada em Ruby");
// 	if (RB_TYPE_P(valorRuby, T_STRING))
// 	{
// 		geraLog(LOG, "RUBY Retornou uma string");
// 	}
// 	else
// 	{
// 		geraLog(LOG, "Ruby não retornou string");
// 		return NULL;
// 	}
// 	printf("\t Retorno obtido → |%s|\n", StringValueCStr(valorRuby));

// 	// geraLog(LOG, "Chamando função RUBY");
// 	// ruby_init();
// 	// rb_require("./obtemQuantidadeDeHabitantesDoIBGE.rb");
// 	// rb_eval_string("FerramentaInterna.obtemQuantidadeDeHabitantesDaCidade");

// 	return StringValueCStr(valorRuby);
// }


void *executaArquivoRuby(void *comando)
{
	system((const char *)comando);
	pthread_exit( (void *) 0 );
	return 0;
}


/** 
 * @brief  Chama um código ruby, aceita somente um retorno, quantidade ilimitada de parâmetros de entrada
 * @note   necessário liberar retorno
 * @param  *usuario: 
 * @param  *nomeArquivo: 
 * @param  *argumentos: 
 * @retval 
 */
char *chamaFuncaoRuby(Usuario *usuario, char *nomeArquivo, char *argumentos)
{
	if (!usuarioValido(usuario, "rubyWrapper.h char *chamaFuncaoRuby(Usuario *usuario, char *nomeArquivo, char *argumentos)"))
	{
		return NULL;
	}
	if (nomeArquivo == NULL)
	{
		geraLog(ERRO, "Nome de arquivo nulo detectado");
		return NULL;
	}

	char *comando = NULL;
	if (argumentos == NULL)
	{
		argumentos = "NULL";
		geraLog(WARNING, "Nenhum argumento foi passado");
	}

	comando = concatenaStrings(6 , "ruby ./", nomeArquivo, " ", argumentos, " ", usuario_obterLogin(usuario));
	if (comando == NULL)
	{
		geraLog(ERRO, "Falha ao concatenar strings");
		return NULL;
	}

	TCPServer *servidor = newTCPServer(PORTA_PADRAO_CONEXAO_INTERNA);
	if (servidor == NULL)
	{
		free(comando);
		comando = NULL;
		return NULL;
	}


	pthread_t thread;
	if(pthread_create(&thread, NULL, executaArquivoRuby, (void *)comando) != 0)// Se der erro ao tentar criar a thread
	{
		geraLog(ERRO, "Falha ao criar thread pra executar arquivo ruby");
		return 0;
	}
	pthread_detach(thread);/* torna thread independente (destaca thread) */

	
	char *resposta = tcpServer_receiveMessage(servidor);
	freeTCPServer(servidor);
	free(comando);
	comando = NULL;
	if (resposta == NULL)
	{
		return NULL;
	}

	Tokenizer *tokenizer = new_Tokenizer(resposta, ' ');
	if (tokenizer == NULL)
	{
		free(resposta);
		resposta = NULL;
		return NULL;
	}
	char *comparacao = NULL;
	if (strcmp((comparacao = tokenizer_getNext(tokenizer)), usuario_obterLogin(usuario)) != 0)
	{
		geraLog(ERRO, "ID Não correponde");
		printf("\tRecebido = |%s|\n", comparacao);
		printf("\tEnviado = |%s|\n",usuario_obterLogin(usuario));
		delete_Tokenizer(tokenizer);
		free(resposta);
		resposta = NULL;
		return NULL;
	}

	char *retorno = strdup(tokenizer_getNext(tokenizer));

	delete_Tokenizer(tokenizer);
	free(resposta);
	resposta = NULL;
	if (retorno == NULL)
	{
		geraLog(ERRO, "Falha ao obter retorno de tokenizer");
		return NULL;
	}

	return retorno;
}