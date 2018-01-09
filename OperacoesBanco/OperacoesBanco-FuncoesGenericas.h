//Arquivo que contem os "Scripts" mais genericos usados pelo OperaçõesBanco.h
#pragma once
#include "../Comandos/Comandos.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<mysql/mysql.h>

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0 
#endif

#ifndef bool
#define bool char
#endif

	/*
		Comandos pra executar no cloud quando for refazer a maquina kkk
		CREATE USER 'interface'@'127.0.0.1';
		CREATE DATABASE teste;
		GRANT SELECT DELETE UPDATE INSERT ON teste.* to 'interface'@'127.0.0.1';
		flush privileges;
	*/

#define DATABASE_HOST "127.0.0.1"//TESTES
// #define DATABASE_HOST "35.186.190.243"//GOOGLE CLOUD
#define DATABASE_USER "interface"
#define DATABASE_PASSWORD "essaSenhaEsoPraInteFaCeSaBeRPorQuESiM"
#define DATABASE_SCHEMA "teste"
#define DATABASE_PORT 0
#define DATABASE_DEFAULT_SOCKET NULL
#define DATABASE_DEFAULT_FLAGS 0

//MYSQL *conexao;

//Retorna TRUE se conectar ao banco com sucesso
// bool conectarBanco(MYSQL *conexao)
bool conectarBanco()
{
	if(conexao != NULL)
	{
		mysql_close(conexao);
		free(conexao);
		conexao = NULL;
	}
	conexao = (MYSQL *)malloc(sizeof(MYSQL));
	if(conexao == NULL)
	{
		printf(" Warning: Memoria insuficiente para conectar-se ao banco de dados em OperacoesBanco.h conectarBanco()\n");
		return false;
	}

	printf(" LOG: Inicializando cliente SQL em OperacoesBanco-FuncoesGenericas.h conectarBanco()\n");

	if( mysql_init(conexao) == NULL )
	{
		printf("ERRO: Não foi possível inicializar o MYSQL (OperacoesBanco.h) conectarBanco()\n");
		return false;
	}

	printf(" LOG: Cliente SQL inicializado em OperacoesBanco-FuncoesGenericas.h conectarBanco()\n");

	if( !mysql_real_connect( conexao, DATABASE_HOST, DATABASE_USER, DATABASE_PASSWORD, DATABASE_SCHEMA, DATABASE_PORT, DATABASE_DEFAULT_SOCKET, DATABASE_DEFAULT_FLAGS ))
	{
		printf("Falha ao estabelecer conexao com o banco de dados (OperacoesBanco.h) (conectarBanco())\n");
		printf("\t|ERRO: %d : %s\n", mysql_errno(conexao), mysql_error(conexao));
		if(conexao != NULL)
		{
			printf("\t|LOG: conexão diferente de NULL em OperacoesBanco-FuncoesGenericas.h conectarBanco() a16q55q8e84\n");
			mysql_close(conexao);
			mysql_thread_end();
			free(conexao);
			conexao = NULL;
			return false;
		}
		return false;
	}
	else
	{
		printf(" LOG: Conexão com banco de dados estabelecida em OperacoesBanco-FuncoesGenericas.h conectarBanco()\n");
		return true;
	}
	printf(" ERRO: Erro incompreendido em OperacoesBanco-FuncoesGenericas.h conectarBanco()\n");
	return false;
}

// Retorna true se der certo (unica opção)
bool desconectarBanco()
{
	mysql_close(conexao);
	conexao = NULL;
	return true;
}

// Retorna TRUE, se a query retorna algo com conteúdo
bool queryRetornaConteudo(char *query)
{
	if(query == NULL)
	{
		printf(" ERRO: Query recebida igual a NULL (OperacoesBanco.h) checarSeVoltaAlgumaCoisaDaQuery()\n");
		return false;
	}

	if(mysql_query(conexao, query))//Se ocorrer algum erro
	{
		printf(" ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) checarSeVoltaAlgumaCoisaDaQuery()\n");
		printf(" ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
		printf(" Query executada: |%s|\n", query);
		free(query);
		query = NULL;

		if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
		{
			printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h checarSeVoltaAlgumaCoisaDaQuery()\n");
			if(conectarBanco())
			{
				printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h checarSeVoltaAlgumaCoisaDaQuery()\n");
			}
			else
			{
				conexao = NULL;
				mysql_close(conexao);
				mysql_thread_end();
				free(conexao);
				printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h checarSeVoltaAlgumaCoisaDaQuery()\n");
			}
		}
		return false;
	}

	MYSQL_RES *resposta = NULL;
	MYSQL_FIELD *campos = NULL;

	resposta = mysql_store_result(conexao);

	if(resposta)//Se houver resposta
	{
		campos = mysql_fetch_fields(resposta);
		if(campos != NULL)
		{
			if(mysql_num_fields(resposta) >= 1)
			{

				if(mysql_fetch_row(resposta) != NULL)
				{
					mysql_free_result(resposta);
					resposta = NULL;
					free(query);
					query = NULL;
					return true;
				}
				else
				{
					mysql_free_result(resposta);
					resposta = NULL;
					free(query);
					query = NULL;
					return false;
				}
			}
			else
			{
				mysql_free_result(resposta);
				resposta = NULL;
				free(query);
				query = NULL;
				return false;
			}
		}	
		else
		{
			mysql_free_result(resposta);
			resposta = NULL;
			free(query);
			query = NULL;
			return false;
		}
		// return true;
	}
	else
	{
		if(resposta != NULL)
		{
			mysql_free_result(resposta);
			resposta = NULL;
		}
		free(query);
		query = NULL;
		return false;
	}
}

// retorna false se der algum problema, true se der certo (Não libera a query, como o checarSeVoltaAlgumaCoisaDaQuery())
bool executaQuery(char *query)
{
	if(query == NULL)
	{
		printf(" ERRO: Query recebida igual a NULL (OperacoesBanco.h) checarSeVoltaAlgumaCoisaDaQuery()\n");
		return false;
	}

	if(mysql_query(conexao, query))//Se ocorrer algum erro
	{
		printf(" ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) checarSeVoltaAlgumaCoisaDaQuery()\n");
		printf(" ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
		printf(" Query executada: |%s|\n", query);
		if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
		{
			printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h checarSeVoltaAlgumaCoisaDaQuery()\n");
			if(conectarBanco())
			{
				printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h checarSeVoltaAlgumaCoisaDaQuery()\n");
			}
			else
			{
				conexao = NULL;
				mysql_close(conexao);
				mysql_thread_end();
				free(conexao);
				printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h checarSeVoltaAlgumaCoisaDaQuery()\n");
			}
		}
		return false;
	}
	printf(" LOG: Query |%s| executada com sucesso em OperacoesBanco-FuncoesGenericas.h executaQuery() qkjhjdsa\n", query);
	return true;
}

char *retornaUnicoRetornoDaQuery(char *query)// Se chegou aqui, a conexao está ativa no momento
{
	if( query == NULL )
	{
		printf(" ERRO: query nula em OperacoesBanco-FuncoesGenericas.h retornaUnicoRetornoDaQuery()\n");
		return strdup("ERRO interno, tente novamente");
	}
	
	if(mysql_query(conexao, query))
	{
		printf(" ERRO: Falha ao executar query em OperacoesBanco-FuncoesGenericas.h retornaUnicoRetornoDaQuery() sakdjh\n");
		printf(" \t%s\n", mysql_error(conexao));
		return strdup("ERRO interno, tente novamente");
	}
	
	MYSQL_RES *resultado = mysql_store_result(conexao);
	
	if(resultado == NULL)// Se não houver consulta
	{
		printf(" Warning: Resultado nulo em OperacoesBanco-FuncoesGenericas.h retornaUnicoRetornoDaQuery() kqht\n");
		return strdup("ERRO interno, tente novamente");
	}
	else
	{
		
		if(mysql_num_fields(resultado) != 1)
		{
			if(mysql_num_fields(resultado) == 0)
			{
				printf(" LOG: Nada encontrado na base de dados para:\n");
				printf(" \t%s\n", query);
				printf(" \t em OperacoesBanco-FuncoesGenericas.h retornaUnicoRetornoDaQuery() 4rr455\n");
				mysql_free_result(resultado);
				free(query);
				resultado = NULL;
				query = NULL;
				return strdup("ERRO: nada encontrado");
			}
			else
			{
				printf(" Warning: Resultado inconsistente em OperacoesBanco-FuncoesGenericas.h retornaUnicoRetornoDaQuery() hekja\n");
				mysql_free_result(resultado);
				free(query);
				query = NULL;
				resultado = NULL;
				return strdup("ERRO interno, tente novamente");
			}
		}
		else
		{
			if(mysql_num_rows(resultado) > 1)
			{
				printf(" Warning: Mais de uma cidade com mesmo nome e estado em OperacoesBanco-FuncoesGenericas.h retornaUnicoRetornoDaQuery() ekjjha\n");
				printf(" \tRespostas:\n");
				MYSQL_ROW linhas;
				while((linhas = mysql_fetch_row(resultado)) != NULL)
				{
					printf(" linhas[0] -> %s\n", linhas[0]);
				}
				printf(" \n");
				char *retorno = strdup(linhas[0]);
				if (retorno == NULL)
				{
					return strdup("ERRO interno, tente novamente");
				}
				mysql_free_result(resultado);
				free(query);
				query = NULL;
				resultado = NULL;
				return retorno;//Retorna o ultimo resultado, mesmo se houver erro
			}
			else if (mysql_num_rows(resultado) == 1)
			{
				MYSQL_ROW linha;
				
				linha = mysql_fetch_row(resultado);
				
				if(linha == NULL)
				{
					printf(" ERRO: Não era pra poder retornar nulo aqui abv84eu9h89hbd\n");
					mysql_free_result(resultado);
					free(query);
					query = NULL;
					resultado = NULL;
					return strdup("ERRO interno, tente novamente");
				}
				else
				{
					char *retorno = strdup(linha[0]);
					if (retorno == NULL)
					{
						return strdup("ERRO interno, tente novamente");
					}
					mysql_free_result(resultado);
					free(query);
					query = NULL;
					resultado = NULL;
					return retorno;
				}
			}
			else
			{
				printf(" Warning: nenhum endereco localizado\n");
				mysql_free_result(resultado);
				resultado = NULL;
				free(query);
				query = NULL;
				return strdup("ERRO: nada encontrado");
			}
		}
	}
}

//Retorna TRUE se o produto existe na base de dados
// bool checarIdProduto(MYSQL *conexao, char *id)
bool checarIdProduto(char *id)//OK
{
	if(conexao == NULL)
	{
		printf("ERRO DE CONEXÃO (OperacoesBanco-FuncoesGenericas.h) (checarIdProduto())\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(conectarBanco())
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return false;
		}
	}
	if(id == NULL)
	{
		printf("ERRO: Não há login para checar (id == NULL) (OperacoesBanco-FuncoesGenericas.h) (checarIdProduto())\n");
		return false;
	}
	if(strlen(id) != 10)
	{
		printf("ERRO: Id informado contém quantidade incorreta de caracteres (OperacoesBanco-FuncoesGenericas.h) checarIdProduto())\n");
		return false;
	}

	char *query = NULL;

	query = (char *)malloc(sizeof(char) * ( strlen(id) + 69));
	if(query == NULL)
	{
		printf(" ERRO: não foi possível alocar memória para a query (OperacoesBanco-FuncoesGenericas.h) (checarIdProduto())\n");
		return false;
	}

	snprintf(query, sizeof(char) * ( strlen(id) + 69 ), "SELECT produto.idproduto from produto WHERE produto.idproduto = \'%s\';", id );

	if(query == NULL)
	{
		printf(" ERRO: não foi possível alocar memória para a query (2) (OperacoesBanco-FuncoesGenericas.h) (checarIdProduto())\n");
		return false;
	}

	bool retorno = queryRetornaConteudo(query);
	query = NULL;
	return retorno;
}

// Retorna true caso seja encontrado contratante com esse ID na base de dados
bool checarIdContratante(char *idContratante)
{
	char *query = NULL;
	int tamanho = 53 + 1 + strlen(idContratante);
	query = (char *)malloc(sizeof(char) * tamanho);
	
		if(query == NULL)
		{
			printf(" Warning: falha ao alocar memoria para query em checarIdContratante() OperacoesBanco-FuncoesGenericas.h ijhvbhsd87vbe\n");
			return false;
		}
	
		snprintf(query, tamanho, "SELECT * FROM contratante C WHERE C.idcontratante=\'%s\';", idContratante);
	
		if(query == NULL)
		{
			printf(" Warning: Falha ao formatar query para checagem de dados em checarIdContratante() OperacoesBanco-FuncoesGenericas.h dvksjbkj\n");
			return false;
		}
	
		if(queryRetornaConteudo(query))
		{
			//Se voltar algo da query
			printf(" LOG: Existe empresa com esse ID no banco de dados em checarIdContratante() OperacoesBanco-FuncoesGenericas.h\n");
			return true;
		}
		else
		{
			printf(" Warning: Não existe empresa com esse id (%s)(contratante) no banco de dados, encerrando operacao em checarIdContratante() OperacoesBanco-FuncoesGenericas.h jcvkdhkc\n", idContratante);
			return false;
		}
}

char *obterIdContratanteDoBancoPorUsuario(Usuario *usuario)
{
	if (usuario == NULL)
	{
		return NULL;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" Warning: Usuario não conectado detectado em OperacoesBanco-FuncoesGenericas.h obterIdContratante()\n");
		return NULL;
	}
	if (!usuario_PermissaoContratante(usuario))
	{
		printf(" Warning: Usuario com permissoes insuficientes detectado em OperacoesBanco-FuncoesGenericas.h obterIdContratante() sagei87sd \n");
		return NULL;
	}
	else
	{
		int tamanho = 57 + usuario_obterTamanhoLogin(usuario) + 1;
		char *query = malloc(sizeof(char) * tamanho);
		if (query == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco-FuncoesGenericas.h obterIdContratante() neoihdsaub\n");
			return NULL;
		}
		snprintf(query, tamanho, "SELECT idcontratante FROM contratante C WHERE C.email=\'%s\';", usuario_obterLogin(usuario));
		if (query == NULL)
		{
			printf(" Warning: Falha ao formatar query em OperacoesBanco-FuncoesGenericas.h obterIdContratante() ashvuieryhsd\n");
			return NULL;
		}
		char *idContratante = retornaUnicoRetornoDaQuery(query);
		if (idContratante[0] == 'E')
		{
			printf(" Warning: Nada encontrado, ou erro interno\n");
			free(idContratante);
			idContratante = NULL;
		}
		return idContratante;
	}
}


//retorna TRUE se o produto estiver vencido, false se estiver dentro do prazo de validade
bool produtoVencido(char *idProduto, Usuario *usuario)
{
	if(conexao == NULL)
	{
		printf("ERRO DE CONEXÃO (OperacoesBanco-FuncoesGenericas.h) (produtoVencido())\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(conectarBanco())
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação em OperacoesBanco-FuncoesGenericas.h produtoVencido()()\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return true;
		}
	}
	if (idProduto == NULL)
	{
		printf(" Warning: idProduto == NULL em OperacoesBanco-FuncoesGenericas.h produtoVencido() 16q54cfr\n");
		return true;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" Warning: usuario nao conectado detectado em OperacoesBanco-FuncoesGenericas.h produtoVencido() chjkjtrtds54\n");
		// return true;
	}
	else
	{
		if(usuario_PermissaoRoot(usuario))
		{
			return false;
		}
	}
	
	//int tamanho = 88+strlen(idProduto)+1;
	int tamanho = 99;//otimizado
	char *query = (char *)malloc(sizeof(char) * tamanho);
	if(query == NULL)
	{
		printf(" Warning: não foi possível alocar memoria para query em OperacoesBanco-FuncoesGenericas.h produtoVencido() qkjhjvgdf\n");
		return true;
	}
	snprintf(query, tamanho, "SELECT DATEDIFF(CURDATE(), P.datacriacao)+P.duracao FROM produto P WHERE P.idproduto=\'%s\';", idProduto);
	if(query == NULL)
	{
		printf(" Warning: não foi possível formatar query em OperacoesBanco-FuncoesGenericas.h produtoVencido() sajkdhjbr\n");
		return true;
	}

	if(!executaQuery(query))
	{
		printf(" Warning: Houveram erros ao executar a query desejada |%s| em OperacoesBanco-FuncoesGenericas.h produtoVencido() wsd456ngh\n", query);
		free(query);
		query = NULL;
		return true;
	}

	MYSQL_RES *resultado = mysql_store_result(conexao);
	
	if(resultado == NULL)
	{
		printf(" Warning: Consulta não realizada em OperacoesBanco-FuncoesGenericas.h produtoVencido() nao houve conexao\n");
		free(query);
		query = NULL;
		return true;
	}
	
	if(mysql_num_fields(resultado) == 0)
	{
		printf(" Warning: Nao houve resultados na pesquisa em OperacoesBanco-FuncoesGenericas.h produtoVencido() bnje\n");
		free(query);
		mysql_free_result(resultado);
		resultado = NULL;
		query = NULL;
		return true;
	}
	
	MYSQL_ROW linha = NULL;
	
	linha = mysql_fetch_row(resultado);
	if(linha == NULL)
	{
		printf(" Warning: Resposta Nula em OperacoesBanco-FuncoesGenericas.h produtoVencido()\n");
		free(query);
		mysql_free_result(resultado);
		resultado = NULL;
		query = NULL;
		return true;
	}
	else
	{
		if(strlen(linha[0]) > 0)
		{
			printf(" LOG: Retorno obtido com sucesso em OperacoesBanco-FuncoesGenericas.h produtoVencido() kjh3as\n"); 
			free(query);
			query = NULL;
			if(atoi(linha[0])>=0)
			{
				printf(" LOG: Produto não está vencido ainda em OperacoesBanco-FuncoesGenericas.h produtoVencido() asddf456br\n");
				mysql_free_result(resultado);
				resultado = NULL;
				return false;
			}
			else
			{
				printf(" LOG: produto está vencido, impedindo execução do código seguinte em OperacoesBanco-FuncoesGenericas.h produtoVencido() sadsa455br\n");
				mysql_free_result(resultado);
				resultado = NULL;
				return true;
			}
		}
		else
		{
			printf(" Warning: Produto nao possui visualizações anonimas cadastradas no banco de dados em OperacoesBanco-FuncoesGenericas.h produtoVencido() kejhqkfge3\n");
			free(query);
			mysql_free_result(resultado);
			resultado = NULL;
			query = NULL;
			return true;
		}
	}
}
