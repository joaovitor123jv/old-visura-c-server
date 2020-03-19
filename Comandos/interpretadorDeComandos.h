#include <stdio.h>
#include <string.h>
#include "Comandos.h"


#include "../Bibliotecas/AdaptadorDeString/AdaptadorDeString.h"

#include <mysql/mysql.h>
MYSQL *conexao;

#include "../Bibliotecas/Usuario.h"
#include "../OperacoesBanco/OperacoesBanco.h"
#include "Comando-Login.h"
#include "Comando-Adicao.h"
#include "Comando-Obter.h"
#include "Comando-Root.h"
#include "Comando-Remover.h"
#include "Comando-Atualizar.h"


// int comandoAtualizar(char *comando);


char* interpretaComando(char *comando, bool *autorizado, int *resultado, Usuario *usuario)
{
	char *nomeAplicacao = NULL;
	int tipoComando;
	char *login = NULL;
	// const char *localizacao = "interpretadorDeComandos.h interpretaComando()";

	

	if(autorizado == NULL)
	{
		printf("ERRO INDEFINIDO (interpretadorDeComandos.h) (interpretaComando())\n");
		geraLog(ERRO, "Interno jajajhehbioa");
		*resultado = ERRO;
		return NULL;
	}
	
	if(comando == NULL)
	{
		geraLog(ERRO, "Interno jbgqaoit");
		*resultado = ERRO;
		return NULL;
	}

	geraLog(LOG, "Gerando TOKENIZER AQUI !!!");
	usuario_setTokenizer(usuario, comando);

	/* Separa a primeira palavra */

	nomeAplicacao = usuario_getNextToken(usuario);

	if( nomeAplicacao == NULL )
	{
		// printf(" Warning: Comando nulo detectado em interpretadorDeComandos.h interpretaComando()\n");
		geraLog(WARNING, "Comando nulo detectado");
		*resultado = ERRO;
		return NULL;
	}

	if( !stringTamanhoIgual(nomeAplicacao, TAMANHO_CHAVE_APLICACAO) )
	{
		geraLog(WARNING, "Tamanho de chave incorreto detectado");
		*resultado = ERRO;
		return NULL;
	}

	if(strcmp(nomeAplicacao, CHAVE_APLICACAO) != 0)/* CHAVE DE Aplicacao */
	{
		geraLog(WARNING, "CHAVE DE APLICACAO INCORRETA (APP)");
		*resultado = ERRO;
		return NULL;
	}


	if( !(*autorizado) )
	{
		geraLog(LOG, "Cliente ainda não autorizando, iniciando processo de login");

		login = comandoLogin(usuario);

		if( login  != NULL )
		{
			*autorizado = true;
			*resultado = OK;
			return login;
		}
		else
		{
			*autorizado = false;
			*resultado = LOGIN_NAO_AUTORIZADO;
			return NULL;
		}
		geraLog(ERRO, "Falha fatal askjgvbri");
		exit(1);
		*resultado = ERRO;
	}
	else
	{
		// printf(" LOG: Cliente Já logado, continuando em interpretadorDeComandos.h interpretaComando()\n");
		geraLog(LOG, "Cliente já locado, continuando interpretacao");
	}

	if(usuario_obterLogin(usuario) == NULL)
	{
		geraLog(ERRO, "Falha fatal orh89sd875");
		exit(1);
		*resultado = ERRO;
		return NULL;
	}
	else
	{
		geraLog(LOG, "Usuario conectado e autenticado com sucesso");
		usuario_mostrarDados(usuario);
	}


	nomeAplicacao = usuario_getNextToken(usuario);

	if( stringMaior(nomeAplicacao, TAMANHO_COMANDO) )
	{
		// printf(" ERRO: Aplicação contem informação excessiva (interpretadorDeComandos.h) interpretaComando()\n");
		geraLog(WARNING, "Tamanho de chave de aplicacao incompatível (APP) não identificado");
		*resultado = ERRO;
		return NULL;
	}

	tipoComando = nomeAplicacao[0] - '0';/* converte letra em inteiro ('0' == 0) */

	if(tipoComando>9 || tipoComando < 0)/* Define " Alcance dos comandos " (de 0 a 9) */
	{
		geraLog(WARNING, "TIPO DE COMANDO NÂO GERENCIADO: ");
		printf("\t\tTipo de comando = %d\n", tipoComando);
		*resultado = ERRO_COMANDO_INCORRETO;
		return NULL;
	}


	switch(tipoComando)
	{
		case COMANDO_ADICIONAR:
			// printf(" LOG: Requisitando \"COMANDO_ADICIONAR\" em interpretadorDeComandos.h interpretaComando()\n");
			geraLog(LOG, "Requisitando comandoAdicionar()");
			int retorno = comandoAdicionar(usuario);
			if( retorno == RETORNO_OK )
			{
				// printf(" LOG: comando adicionar deu certo interpretadorDeComandos.h (interpretaComando())\n");
				geraLog(LOG, "Sucesso ao executar o comando de adição");
				*resultado = OK;
				return NULL;
			}
			else
			{
				// printf(" LOG: comando adicionar NÃO deu certo interpretadorDeComandos.h (interpretaComando())\n");
				geraLog(LOG, "Comando adicionar não deu certo");
				*resultado = RETORNO_ADICAO_RECUSADA;
				return NULL;
			}
			break;

		case COMANDO_ATUALIZAR:/* COMANDO_ATUALIZAR = 3 */
			// printf(" LOG: Requisitando \"COMANDO_ATUALIZAR\" em interpretadorDeComandos.h interpretaComando()\n");
			geraLog(LOG, "Requisitando comandoAtualizar()");
			*resultado = REQUISITANDO_ATUALIZACAO;
			return NULL;
			break;

		case COMANDO_OBTER:
			// printf(" LOG: Requisitando \"COMANDO-OBTER\" em interpretadorDeComandos.h interpretaComando()\n");
			geraLog(LOG, "requisitando comandoObter()");
			*resultado = REQUISITANDO_OBTENCAO;
			return NULL;
			break;

		case COMANDO_REMOVER:
			// printf(" LOG: Requisitando \"COMANDO_REMOVER\" em interpretadorDeComandos.h interpretaComando()\n");
			geraLog(LOG, "Requisitando comando remover");
			if (comandoRemover(usuario))
			{
				*resultado = REQUISITANDO_REMOCAO;	
			}
			else
			{
				*resultado = OK;
			}
			return NULL;
			break;

		case ERRO:
			// printf("Erro na execução do comando\n");
			geraLog(ERRO, "Erro na execucao do comando q089g721b");
			*resultado = ERRO_DE_EXECUCAO;
			return NULL;
			break;

		case COMANDO_ROOT: // 7
			// printf(" Warning: COMANDO_ROOT REQUISITADO em interpretadorDeComandos.h interpretaComando()\n");
			geraLog(WARNING, "****** REQUISITANDO COMANDO ROOT *******");
			*resultado = REQUISITANDO_ROOT; // 17
			return NULL;
			break;

		default:
			// printf(" Requisição não manipulada, (interpretadorDeComandos.h)\n");
			geraLog(ERRO, "Requisição não manipulada");
			*resultado = ERRO;
			return NULL;
			break;
	}

	// printf(" Exceção não manipulada (interpretadorDeComandos.h)\n");
	geraLog(ERRO, "Exceção não manipulada");
	*resultado = ERRO;
	return NULL;
}

