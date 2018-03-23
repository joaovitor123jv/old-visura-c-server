#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Comandos.h"
#include "../OperacoesBanco/OperacoesBanco.h"
#include "../AdaptadorDeString/AdaptadorDeString.h"

#include <ruby.h>

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
		return atualizarQuantidadeDeHabitantesDaCidade(usuario);
	}
	else
	{
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
	char *nomeDaCidade = strdup(token);
	if (nomeDaCidade == NULL)
	{
		geraLog(ERRO, "Falha ao duplicar nome da cidade, abortando");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	token = usuario_getNextToken(usuario);
	if (token == NULL)
	{
		geraLog(WARNING, "Comando insuficiente detectado kjdwgrt");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	if(!stringTamanhoIgual(token, TAMANHO_ESTADO))
	{
		geraLog(WARNING, "Tamanho do nome do estado está incorreto");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	char *nomeDoEstado = strdup(token);
	if (nomeDoEstado == NULL)
	{
		geraLog(ERRO, "Falha ao duplicar nome de estado");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}



	// TODO Colocar ruby aqui




	//					Tamanho da string de execucao do script						+ tamanho do nome da cidade + tamanho do nome do estado + quantidade de espacos + \0
	int tamanho = strlen(SCRIPT_DE_ATUALIZACAO_DE_QUANTIDADE_DE_HABITANTES_DE_CIDADE) + strlen(nomeDaCidade) + TAMANHO_ESTADO + 2 + 1;
	char *comando = (char *)calloc(sizeof(char), tamanho);
	if (comando == NULL)
	{
		geraLog(ERRO, "Falha ao alocar memoria para comando");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	snprintf(comando, tamanho, "%s %s %s", SCRIPT_DE_ATUALIZACAO_DE_QUANTIDADE_DE_HABITANTES_DE_CIDADE, nomeDaCidade, nomeDoEstado);
	if (comando == NULL)
	{
		geraLog(ERRO, "Falha ao formatar comando");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	int retorno = system(comando);
	if (retorno != 0)
	{
		geraLog(ERRO, "Falha ao atualizar dados");
		return RETORNO_FALHA_AO_ATUALIZAR_STR_DINAMICA;
	}
	else
	{
		geraLog(LOG, "Conteudo atualizado com sucesso");
		return RETORNO_OK_STR_DINAMICA;
	}







}