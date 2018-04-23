#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "Comandos.h"
#include "../Usuario.h"
#include "../OperacoesBanco/OperacoesBanco.h"

#ifndef CODIGO_COMANDO_REMOVER
#define CODIGO_COMANDO_REMOVER


/*
	@author João Vitor Antoniassi Segantin
	@github https://github.com/joaovitor123jv
*/


bool comandoRemover(Usuario *usuario)/* APP 5 algumaCoisa */
{
	if( usuario == NULL )
	{
		printf(" Warning: usuario nulo detectado em Comando-Remover.h comandoRemover()\n");
		return false;
	}
	if (usuario_PermissaoAnonimo(usuario))
	{
		printf(" Warning: Usuario anonimo não pode excluir sua conta em Comando-Remover.h comandoRemover()\n");
		return false;
	}
	printf(" LOG: Solicitando remocao de algo em Comando-Remover.h comandoRemover()\n");
	char *token = usuario_getNextToken(usuario);
	if( token == NULL )
	{
		printf(" Warning: Comando insuficiente em Comando-Remover.h comandoRemover()\n");
		return false;
	}
	return false;
}





bool deletarProduto(Usuario *usuario, char *idProduto)
{
	if (!conexaoAtiva())
	{
		geraLog(ERRO, "Conexao inativa com o banco de dados detectada");
		return false;
	}

	if (!checarIdProduto(idProduto))
	{
		geraLog(WARNING, "Produto não existe na base de dados");
		return false;
	}
	geraLog(LOG, "Produto existe na base de dados");

	// int tamanho = strlen(idProduto);
	// int tamanho = 10 + 64 + 1;
	int tamanho = 74 + 10;
	char *query = (char *)calloc(sizeof(char), tamanho);
	if (query == NULL)
	{
		geraLog(WARNING, "Falha ao alocar memoria para query");
		return false;
	}
	snprintf(query, tamanho, "DELETE FROM visualizacaoDeUsuario WHERE produto_idproduto LIKE BINARY \'%s\';", idProduto);//Elimina erros de case-sensitive
	if (query == NULL)
	{
		geraLog(WARNING, "Falha ao formatar query aoighr");
		return false;
	}
	if (!executaQuery(query))
	{
		geraLog(WARNING, "Falha ao executar query");
		return false;
	}


	tamanho = 10 + 59 + 1;
	query = (char *)realloc(query, tamanho);
	if (query == NULL)
	{
		geraLog(WARNING, "Falha ao realocar memoria para query weuiotb");
		return false;
	}
	snprintf(query, tamanho, "DELETE FROM feedBackCliente WHERE idproduto LIKE BINARY \'%s\';", idProduto);
	if (query == NULL)
	{
		geraLog(WARNING, "Falha ao formatar query dakjhbt");
		return false;
	}
	if (!executaQuery(query))
	{
		geraLog(WARNING, "Falha ao executar query");
		return false;
	}

	tamanho = 10 + 59 + 1;
	query = (char *)realloc(query, tamanho);
	if (query == NULL)
	{
		geraLog(WARNING, "Falha ao realocar memoria para query kerjym5");
		return false;
	}
	snprintf(query, tamanho, "DELETE FROM produto WHERE idproduto LIKE BINARY \'%s\' LIMIT 1;", idProduto);
	if (query == NULL)
	{
		geraLog(WARNING, "Falha ao formatar query tn654");
		return false;
	}
	if (!executaQuery(query))
	{
		geraLog(WARNING, "Falha ao executar query");
		free(query);
		query = NULL;
		return false;
	}
	else
	{
		geraLog(LOG, "Produto deletado do banco de dados com sucesso");
		free(query);
		query = NULL;
		return true;
	}
}

#endif //CODIGO_COMANDO_REMOVER