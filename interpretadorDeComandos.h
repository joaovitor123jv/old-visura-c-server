#include <stdio.h>
#include <string.h>
#include "Comandos.h"


bool usuarioRoot(char *email);



#include "OperacoesBanco.h"
#include "Comando-Login.h"
#include "Comando-Adicao.h"
#include "Comando-Obter.h"



/* Variável de CONTROLE */
/***********************************IMPORTANTE********************************************/
/* **********************bool interpretando = false;**************************************/
/*****************************************************************************************/

// bool comandoLogin(char *comando);
int comandoAtualizar(char *comando);





// int interpretaComando(char *comando, bool *autorizado, char *email)
char* interpretaComando(char *comando, bool *autorizado, int *resultado, char* email, bool *usuarioAnonimo)
// int interpretaComando(char *comando, bool *autorizado)
{
	// printf("Posicao de EMAIL recebida (interpretando) 1 =  %x\n", email);
	interpretando = true;
	char *nomeAplicacao = NULL;
	int tipoComando;
	char *login = NULL;

//	char *comandoADividir = NULL;

	// printf("EMAIL APONTA PARA (6) : %x\n", email);
	

	if(autorizado == NULL)
	{
		interpretando = false;
		printf("ERRO INDEFINIDO (interpretadorDeComandos.h) (interpretaComando())\n");
		// return ERRO;
		*resultado = ERRO;
		return NULL;
	}
	
	if(comando == NULL)
	{
		interpretando = false;
		// return ERRO;
		*resultado = ERRO;
		return NULL;
	}

//	comandoADividir = malloc(sizeof(char) * BUFFER_CLIENTE);
//	if(comandoADividir == NULL)
//	{
//		printf(" Warning: Falha ao alocar comando para ser dividido em interpretadorDeComandos.h interpretaComando()\n");
//		interpretando = false;
//		*resultado = ERRO;
//		return NULL;
//	}

	nomeAplicacao = strtok(comando, " ");/* Separa a primeira palavra */
//	strcpy(comandoADividir, comando);
//	if(comandoADividir == NULL)
//	{
//		printf(" Warning: Falha ao copiar comando para dividir em interpretadorDeComandos.h interpretaComando()\n");
//	}
//	nomeAplicacao = strtok(comandoADividir, " ");/* Separa a primeira palavra */
//
//	free(comandoADividir);
//	comandoADividir = NULL;

	if(strcmp(nomeAplicacao, CHAVE_APLICACAO) != 0)/* CHAVE DE Aplicacao */
	{
		interpretando = false;
		printf(" NOME DA APLICACAO INCORRETO, INTERROMPENDO CONEXAO (interpretadorDeComandos.h)\n");
		// return ERRO;
		*resultado = ERRO;
		return NULL;
	}
	// printf(" SEGUINDO COM A INTERPRETAÇÃO (interpretadorDeComandos.h)\n");


	if( !(*autorizado) )
	{
		printf(" Cliente ainda não autorizado, INTERROMPENDO PROCESSO PARA SOLICITAR LOGIN\n");
		printf(" COMANDO (depois de fazer o primeiro strtok) = %s\n", comando);
		login = comandoLogin(comando, usuarioAnonimo);
		printf(" Retornou do Comando-Login (interpretadorDeComandos.h) interpretaComando()\n");
		if( login  != NULL )
		{
			interpretando = false;
			*autorizado = true;
			// return OK;
			*resultado = OK;
			return login;
		}
		else
		{
			interpretando = false;
			*autorizado = false;
			// return REQUISITANDO_LOGIN;
			*resultado = ERRO;
			return NULL;
		}
		printf("FALHA FATAL (interpretadorDeComandos.h)\n");
		exit(1);
		*resultado = ERRO;
		//return ERRO;
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
		// return ERRO;
		*resultado = ERRO;
		return NULL;
	}
	else
	{
		printf(" LOG: EMAIL NÂO NULO |%s| !!! em interpretadorDeComandos.h interpretaComando()\n", email);
	}


	nomeAplicacao = strtok(NULL, " ");

	if(strlen(nomeAplicacao) > TAMANHO_COMANDO)/* checa se há mais de uma letra na segunda "palavra" */
	{
		interpretando = false;
		printf(" ERRO: Aplicação contem informação excessiva (interpretadorDeComandos.h) interpretaComando()\n");
		// return ERRO;
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
			printf(" Adição requisitada (interpretadorDeComandos.h) interpretaComando()\n");
			if(comandoAdicionar(email, *usuarioAnonimo))
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
			// return REQUISITANDO_ADICAO;
				*resultado = REQUISITANDO_ADICAO;
				return NULL;
			}
			break;

		case COMANDO_ATUALIZAR:/* COMANDO_ATUALIZAR = 3 */
			printf(" Atualização requisitada (interpretadorDeComandos.h)\n");
			printf(" NÂO IMPLEMENTADO (interpretadorDeComandos.h)\n");
			interpretando = false;
			// return REQUISITANDO_ATUALIZACAO;

			*resultado = REQUISITANDO_ATUALIZACAO;
			return NULL;
			break;

		case COMANDO_OBTER:
			printf(" Obtencao de algo requisitado\n");
			printf(" Não preparado\n");
//			if(comandoObter())
//			{
//				*resultado = REQUISITANDO_OBTENCAO;
//			}
//			else
//			{
//				*resultado = ERRO;
//			}
			*resultado = REQUISITANDO_OBTENCAO;
			//interpretando = false;
			return NULL;
			break;

		case ERRO:
			printf("Erro na execução do comando\n");
			interpretando = false;
			*resultado = ERRO;
			return NULL;
			break;

		default:
			printf(" Requisição não manipulada, (interpretadorDeComandos.h)\n");
			interpretando = false;
			// return ERRO;
			*resultado = ERRO;
			return NULL;
			break;
	}

	interpretando = false;
	printf(" Exceção não manipulada (interpretadorDeComandos.h)\n");
	// return ERRO;
	*resultado = ERRO;
	return NULL;
}



bool usuarioRoot(char *email)
{
	if(email == NULL)
	{
		return false;
	}
	if(strcmp(email, LOGIN_DE_CADASTRO) == 0)
	{
		return true;
	}
	if(strcmp(email, LOGIN_DO_SITE) == 0)
	{
		return true;
	}
	return false;
}