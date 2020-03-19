#include<stdio.h>
#include<string.h>
// #include <ruby.h>
#include"../../Bibliotecas/AdaptadorDeString/AdaptadorDeString.h"
#include "../../Bibliotecas/Usuario.h"
#include "../Comandos.h"
#include "../../Bibliotecas/TCPServer/TCPServer.h"



void *executaArquivoRuby(void *comando)
{
	int resultado = system((const char *)comando);
	if (resultado == 0)
	{
		geraLog(LOG, "Script Ruby executado com sucesso");
	}
	else
	{
		geraLog(WARNING, "Ocorreu algum problema ao executar o script ruby");
	}
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