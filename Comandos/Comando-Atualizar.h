#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Comandos.h"
#include "../OperacoesBanco/OperacoesBanco.h"
#include "../AdaptadorDeString/AdaptadorDeString.h"

#include "Ferramentas/rubyWrapper.h"

char *comandoAtualizar(Usuario *usuario);//Retorna mensagem direto ao usuario (OK, caso dê certo)

char *atualizarQuantidadeDeHabitantesDaCidade(Usuario *usuario);//Atualiza a quantidade de habitantes da cidade, no estado informado (APP 3 qC nomeDaCidade nomeDoEstado)


/** 
 * @brief  Wrapper, redirecionador para as funções corretas
 * @note   também checa alguns dados, como validação de usuário
 * @param  *usuario: usuário logado
 * @retval retorna direto ao usuario, uma string dinâmica
 */
char *comandoAtualizar(Usuario *usuario)/* APP 3 */
{
	static const char *localizacao = "Comando-Atualizar.h comandoAtualizar(Usuario *usuario)";
	if (!usuarioValido(usuario, localizacao))
	{
		geraLog(ERRO, "Usuario invalido detectado");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	
	geraLog(LOG, "**************Comando - Atualizar ***************");
	// usuario_mostrarDados(usuario);

	char *token = usuario_getNextToken(usuario);
	if(token == NULL)
	{
		geraLog(ERRO, "Comando insuficiente detectado, token nulo");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}

	if (stringMaior(token, TAMANHO_TIPO))
	{
		geraLog(WARNING, "Token com tamanho exagerado detectado");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	if (strcmp(TIPO_QUANTIDADE_DE_HABITANTES_DA_CIDADE, token) == 0)
	{
		geraLog(LOG, "Solicitando atualização de quantidade de habitantes na cidade");
		return atualizarQuantidadeDeHabitantesDaCidade(usuario);
	}
	else
	{
		geraLog(ERRO, "Comando não compreendido");
		return RETORNO_ERRO_COMANDO_NAO_CONSTRUIDO_STR_DINAMICA;
	}
}

/** 
 * @brief  Formula um comando pra ser executado por um script em ruby que atualiza as informações da cidade indicada
 * @note   utiliza um script ruby
 * @param  *usuario: 
 * @retval retorna direto ao usuario, uma string dinamica
 */
char *atualizarQuantidadeDeHabitantesDaCidade(Usuario *usuario)// APP 3 qC nomeCidade nomeEstado
{
	static const char *localizacao = "Comando-Atualizar.h atualizarQuantidadeDeHabitantesDaCidade(Usuario *usuario)";
	if (!usuarioValido(usuario, localizacao))
	{
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	geraLog(LOG, "Obtendo token");
	char *token = usuario_getNextToken(usuario);
	if (token == NULL)
	{
		geraLog(WARNING, "Comando insuficiente detectado askjfghrjk");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	if(stringMaior(token, TAMANHO_DO_NOME_DA_CIDADE_COM_MAIOR_NOME_DO_MUNDO))
	{
		geraLog(WARNING, "Tamanho do nome da cidade excedeu os limites");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	geraLog(LOG, "Duplicando token");
	char *nomeDaCidade = strdup(token);
	if (nomeDaCidade == NULL)
	{
		geraLog(ERRO, "Falha ao duplicar nome da cidade, abortando");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	geraLog(LOG, "Obtendo proximo token");
	token = usuario_getNextToken(usuario);
	if (token == NULL)
	{
		geraLog(WARNING, "Comando insuficiente detectado kjdwgrt");
		free(nomeDaCidade);
		nomeDaCidade = NULL;
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	if(!stringTamanhoIgual(token, TAMANHO_ESTADO))
	{
		geraLog(WARNING, "Tamanho do nome do estado está incorreto");
		free(nomeDaCidade);
		nomeDaCidade = NULL;
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	geraLog(LOG, "Duplicando token");
	char *nomeDoEstado = strdup(token);
	if (nomeDoEstado == NULL)
	{
		geraLog(ERRO, "Falha ao duplicar nome de estado");
		free(nomeDaCidade);
		nomeDaCidade = NULL;
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	if (!cidadeExisteNoBanco(nomeDaCidade, nomeDoEstado))
	{
		geraLog(WARNING, "Cidade não existe no banco de dados");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}


	geraLog(DEBUG, "Gerando argumento");
	char *argumento = concatenaStrings(3, nomeDaCidade, " ", nomeDoEstado);
	if (argumento == NULL)
	{
		free(nomeDaCidade);
		free(nomeDoEstado);
		nomeDaCidade = NULL;
		nomeDoEstado = NULL;
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	geraLog(DEBUG, "Chamando função ruby");
	// char *retorno = chamaFuncaoRuby("./obtemQuantidadeDeHabitantesDoIBGE.rb", "FerramentaInterna.new", argumento);
	char *retorno = chamaFuncaoRuby(usuario, SCRIPT_DE_ATUALIZACAO_DE_QUANTIDADE_DE_HABITANTES_DE_CIDADE, argumento);
	geraLog(LOG, "Liberando argumento");
	free(argumento);
	argumento = NULL;

	if (retorno == NULL)
	{
		geraLog(ERRO, "Ocorreu algum erro no wrapper, e não foi possível obter o retorno");
		free(nomeDoEstado);
		free(nomeDaCidade);
		nomeDaCidade = NULL;
		nomeDoEstado = NULL;
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	geraLog(DEBUG, "Retorno do ruby: ");
	printf("\t\tRetorno = |%s|\n", retorno);

	if (strcmp(retorno, "ERRO") == 0)
	{
		geraLog(WARNING, "Ocorreu um erro ao tentar atualizar a quantidade de habitantes da cidade");
		return RETORNO_ERRO_NOT_FOUND_STR_DINAMICA;
	}

	if(addNumeroDeHabitantesACidadeAoBanco(usuario, nomeDaCidade, nomeDoEstado, retorno))
	{
		free(nomeDaCidade);
		free(nomeDoEstado);
		free(retorno);
		nomeDaCidade = NULL;
		nomeDoEstado = NULL;
		retorno = NULL;
		return RETORNO_OK_STR_DINAMICA;
	}

	free(nomeDaCidade);
	free(nomeDoEstado);
	free(retorno);
	nomeDaCidade = NULL;
	nomeDoEstado = NULL;
	retorno = NULL;
	return RETORNO_ERRO_INTERNO_STR_DINAMICA;

}

/** 
 * @brief  Formula um comando pra ser executado por um script em ruby que atualiza as informações da cidade indicada
 * @note   Uso interno, somente, não libera nada
 * @param  *usuario: 
 * @retval 
 */
bool interno_atualizarQuantidadeDeHabitantesDaCidade(Usuario *usuario, char *nomeDaCidade, char *nomeDoEstado)
{
	static const char *localizacao = "Comando-Atualizar.h atualizarQuantidadeDeHabitantesDaCidade(Usuario *usuario)";
	if (!usuarioValido(usuario, localizacao))
	{
		return false;
	}
	if (nomeDaCidade == NULL)
	{
		geraLog(ERRO, "nomeDaCidade nulo detectado");
		return false;
	}
	if (nomeDoEstado == NULL)
	{
		geraLog(ERRO, "nomeDoEstado nulo detectado");
		return false;
	}
	
	char *argumento = concatenaStrings(3, nomeDaCidade, " ", nomeDoEstado);
	if (argumento == NULL)
	{
		return false;
	}
	
	char *retorno = chamaFuncaoRuby(usuario, SCRIPT_DE_ATUALIZACAO_DE_QUANTIDADE_DE_HABITANTES_DE_CIDADE, argumento);
	free(argumento);
	argumento = NULL;

	if (retorno == NULL)
	{
		geraLog(ERRO, "Ocorreu algum erro no wrapper, e não foi possível obter o retorno");
		return false;
	}
	geraLog(DEBUG, "Retorno do ruby: ");
	printf("\t\tRetorno = |%s|\n", retorno);

	if (strcmp(retorno, "ERRO") == 0)
	{
		geraLog(WARNING, "Ocorreu um erro ao tentar atualizar a quantidade de habitantes da cidade");
		return false;
	}

	return addNumeroDeHabitantesACidadeAoBanco(usuario, strdup(nomeDaCidade), strdup(nomeDoEstado), retorno);// Função já libera o char *retorno
}