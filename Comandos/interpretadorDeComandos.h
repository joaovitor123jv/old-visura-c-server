#include <stdio.h>
#include <string.h>
#include "Comandos.h"


#include "../AdaptadorDeString.h"
#include "../Usuario.h"
#include "../OperacoesBanco/OperacoesBanco.h"
#include "Comando-Login.h"
#include "Comando-Adicao.h"
#include "Comando-Obter.h"
#include "Comando-Root.h"
#include "Comando-Remover.h"


/* Variável de CONTROLE */
/***********************************IMPORTANTE********************************************/
/* **********************bool interpretando = false;**************************************/
/*****************************************************************************************/

int comandoAtualizar(char *comando);


char* interpretaComando(char *comando, bool *autorizado, int *resultado, char* email, bool *usuarioAnonimo, Usuario *usuario)
{
	interpretando = true;
	char *nomeAplicacao = NULL;
	int tipoComando;
	char *login = NULL;

	

	if(autorizado == NULL)
	{
		interpretando = false;
		printf("ERRO INDEFINIDO (interpretadorDeComandos.h) (interpretaComando())\n");
		*resultado = ERRO;
		return NULL;
	}
	
	if(comando == NULL)
	{
		interpretando = false;
		*resultado = ERRO;
		return NULL;
	}

	nomeAplicacao = strtok(comando, " ");/* Separa a primeira palavra */
	if( nomeAplicacao == NULL )
	{
		printf(" Warning: Comando nulo detectado em interpretadorDeComandos.h interpretaComando()\n");
		interpretando = false;
		*resultado = ERRO;
		return NULL;
	}

//	if( ! (stringTamanhoIgual(nomeAplicacao, TAMANHO_CHAVE_APLICACAO) ))
	if( !stringTamanhoIgual(nomeAplicacao, TAMANHO_CHAVE_APLICACAO) )
	{
		printf(" Warning: Tamanho de chave de aplicacao incompativel em interpretadorDeComandos.h interpretaComando()\n");
		interpretando = false;
		*resultado = ERRO;
		return NULL;
	}

	if(strcmp(nomeAplicacao, CHAVE_APLICACAO) != 0)/* CHAVE DE Aplicacao */
	{
		interpretando = false;
		printf(" NOME DA APLICACAO INCORRETO, INTERROMPENDO CONEXAO (interpretadorDeComandos.h)\n");
		*resultado = ERRO;
		return NULL;
	}


	if( !(*autorizado) )
	{
		printf(" LOG: Cliente ainda não autorizado, INTERROMPENDO PROCESSO PARA SOLICITAR LOGIN\n");
		//printf(" DEBUG: COMANDO (depois de fazer o primeiro strtok) = %s\n", comando);



		//printf(" ** DEBUG: Endereço de usuario antes da criacao em interpretadorDeComandos.h interpretaComando(): %ld\n", &usuario);
		login = comandoLogin(usuarioAnonimo, usuario);
		//printf(" ** DEBUG: Endereço de usuario depois da criacao em interpretadorDeComandos.h interpretaComando(): %ld\n", &usuario);
		//printf(" ***********usuario->login = %s em interpretadorDeComandos.h interpretaComando()\n", usuario_obterLogin(usuario));





		//printf(" DEBUG: Retornou do Comando-Login (interpretadorDeComandos.h) interpretaComando()\n");

		

		if( login  != NULL )
		{
			interpretando = false;
			*autorizado = true;
			*resultado = OK;
			return login;
		}
		else
		{
			interpretando = false;
			*autorizado = false;
			*resultado = ERRO;
			return NULL;
		}
		printf("FALHA FATAL (interpretadorDeComandos.h)\n");
		exit(1);
		*resultado = ERRO;
	}
	else
	{
		printf(" LOG: Cliente Já logado, continuando em interpretadorDeComandos.h interpretaComando()\n");
	}

	if(email == NULL)
	{
		printf(" ERRO: FALHA FATAL (email == NULL) em interpretadorDeComandos.h interpretaComando()\n");
		exit(1);
		interpretando = false;
		*resultado = ERRO;
		return NULL;
	}
	else
	{
		printf(" LOG: EMAIL NÂO NULO |%s| !!! em interpretadorDeComandos.h interpretaComando()\n", email);
	}


	nomeAplicacao = strtok(NULL, " ");

//	if(strlen(nomeAplicacao) > TAMANHO_COMANDO)/* checa se há mais de uma letra na segunda "palavra" */
	if( stringMaior(nomeAplicacao, TAMANHO_COMANDO) )
	{
		interpretando = false;
		printf(" ERRO: Aplicação contem informação excessiva (interpretadorDeComandos.h) interpretaComando()\n");
		*resultado = ERRO;
		return NULL;
	}

	tipoComando = nomeAplicacao[0] - '0';/* converte letra em inteiro ('0' == 0) */

	if(tipoComando>9 || tipoComando < 0)/* Define " Alcance dos comandos " (de 0 a 9) */
	{
		interpretando = false;
		printf(" Warning: TIPO DE COMANDO NÂO GERENCIADO: %d (interpretadorDeComandos.h) interpretaComando()\n", tipoComando);
		// return ERRO;
		*resultado = ERRO;
		return NULL;
	}


	switch(tipoComando)
	{
		case COMANDO_ADICIONAR:
			printf(" LOG: Requisitando \"COMANDO_ADICIONAR\" em interpretadorDeComandos.h interpretaComando()\n");
			if(comandoAdicionar(usuario))
			{
				printf(" LOG: comando adicionar deu certo interpretadorDeComandos.h (interpretaComando())\n");
				interpretando = false;
				*resultado = OK;
				return NULL;
			}
			else
			{
				printf(" LOG: comando adicionar NÃO deu certo interpretadorDeComandos.h (interpretaComando())\n");
				interpretando = false;
				*resultado = REQUISITANDO_ADICAO;
				return NULL;
			}
			break;

		case COMANDO_ATUALIZAR:/* COMANDO_ATUALIZAR = 3 */
			printf(" LOG: Requisitando \"COMANDO_ATUALIZAR\" em interpretadorDeComandos.h interpretaComando()\n");
			interpretando = false;

			*resultado = REQUISITANDO_ATUALIZACAO;
			return NULL;
			break;

		case COMANDO_OBTER:
			printf(" LOG: Requisitando \"COMANDO-OBTER\" em interpretadorDeComandos.h interpretaComando()\n");
			*resultado = REQUISITANDO_OBTENCAO;
			return NULL;
			break;

		case COMANDO_REMOVER:
			printf(" LOG: Requisitando \"COMANDO_REMOVER\" em interpretadorDeComandos.h interpretaComando()\n");
			if (comandoRemover(usuario))
			{
				interpretando = false;
				*resultado = REQUISITANDO_REMOCAO;	
			}
			else
			{
				interpretando = false;
				*resultado = OK;
			}
			return NULL;
			break;

		case ERRO:
			printf("Erro na execução do comando\n");
			interpretando = false;
			*resultado = ERRO;
			return NULL;
			break;

		case COMANDO_ROOT: // 7
			printf(" Warning: COMANDO_ROOT REQUISITADO em interpretadorDeComandos.h interpretaComando()\n");
			*resultado = REQUISITANDO_ROOT; // 17
			return NULL;
			break;

		default:
			printf(" Requisição não manipulada, (interpretadorDeComandos.h)\n");
			interpretando = false;
			*resultado = ERRO;
			return NULL;
			break;
	}

	interpretando = false;
	printf(" Exceção não manipulada (interpretadorDeComandos.h)\n");
	*resultado = ERRO;
	return NULL;
}

