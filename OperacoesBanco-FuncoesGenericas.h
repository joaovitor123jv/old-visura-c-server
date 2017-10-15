//Arquivo que contem os "Scripts" mais genericos usados pelo OperaçõesBanco.h
#include "Comandos.h"
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

#define DATABASE_HOST "127.0.0.1"//TESTES
// #define DATABASE_HOST "35.186.190.243"//GOOGLE CLOUD
#define DATABASE_USER "interface"
#define DATABASE_PASSWORD "essaSenhaEsoPraInteFaCeSaBeRPorQuESiM"
#define DATABASE_SCHEMA "teste"
#define DATABASE_PORT 0
#define DATABASE_DEFAULT_SOCKET NULL
#define DATABASE_DEFAULT_FLAGS 0

MYSQL *conexao;

//Retorna TRUE se conectar ao banco com sucesso
// bool conectarBanco(MYSQL *conexao)
bool conectarBanco()//OK
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

	printf("\r Configurando servidor.............Inicializando cliente SQL");

	if( mysql_init(conexao) == NULL )
	{
		printf("ERRO: Não foi possível inicializar o MYSQL (OperacoesBanco.h) conectarBanco()\n");
		return false;
	}

	printf("\r Configurando servidor..............Inicializando cliente SQL");

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
		printf("\r Configurando servidor...............");
		return true;
	}
	//mysql_close(&conexao);
	//
	return false;
}

// Retorna TRUE, se a query retorna algo com conteúdo
bool checarSeVoltaAlgumaCoisaDaQuery(char *query)
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
	// MYSQL_ROW *linhas = NULL;
	MYSQL_FIELD *campos = NULL;

	resposta = mysql_store_result(conexao);

	if(resposta)//Se houver resposta
	{
		// printf(" \tResposta recebida do banco de dados\n");
		campos = mysql_fetch_fields(resposta);
		if(campos != NULL)
		{
			// printf(" \tExistem campos na resposta\n");
			if(mysql_num_fields(resposta) >= 1)
			{
				// printf(" \tObteve %d respostas\n", mysql_num_fields(resposta));

				if(mysql_fetch_row(resposta) != NULL)
				{
					// printf("Obteve uma ou mais respostas\n");
					mysql_free_result(resposta);
					resposta = NULL;
					// printf(" EMAIL (OperacoesBanco.h) = %s\n",email);
					// printf(" POSICAO DE MEMORIA DE EMAIL (OperacoesBanco.h) = %x\n", &email );
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

bool executaQuery(char *query)// retorna false se der algum problema, true se der certo (Não libera a query, como o checarSeVoltaAlgumaCoisaDaQuery())
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

	bool retorno = checarSeVoltaAlgumaCoisaDaQuery(query);
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
	
		if(checarSeVoltaAlgumaCoisaDaQuery(query))
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

bool checarLogin(char *email, char *senha)
{
	if(conexao == NULL)
	{
		printf("ERRO DE CONEXÃO (OperacoesBanco-FuncoesGenericas.h) (checarLogin)\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(conectarBanco())
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação em OperacoesBanco-FuncoesGenericas.h checarLogin()\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return false;
		}
	}
	if(email == NULL)
	{
		printf(" ERRO: Não há login para checar (email == NULL) em OperacoesBanco-FuncoesGenericas.h checarLogin()\n");
		return false;
	}
	if(senha == NULL)
	{
		printf(" ERRO: Senha não foi informada em OperacoesBanco.h checarLogin() asbiv3888wq2\n");
		return false;
	}
	
	char *query = NULL;
	//size_t tamanho = strlen(email) + strlen(senha) + 66 + 1;
	size_t tamanho = strlen(email) + strlen(senha) + 67;
	
	
	query = (char *)malloc(sizeof(char) * tamanho);
	if(query == NULL)
	{
		printf(" ERRO: não foi possível alocar memória para a query em OperacoesBanco-FuncoesGenericas.h checarLogin()\n");
		return false;
	}
	
	snprintf(query, sizeof(char) * tamanho, "SELECT C.idcliente from cliente C WHERE C.email=\'%s\' AND C.senha=\'%s\';", email, senha);
	
	
	if(query == NULL)
	{
		printf(" ERRO: não foi possível alocar memória para a query (2) em OperacoesBanco-FuncoesGenericas.h checarLogin()\n");
		return false;
	}
	
	bool retorno = checarSeVoltaAlgumaCoisaDaQuery(query);
	if(retorno)
	{
		printf(" LOG: Foi encontrado um ID para cliente que satisfaça as seguintes comparações: em OperacoesBanco-FuncoesGenericas.h checarLogin()\n");
		printf(" \t\tcliente.email = |%s|\n", email);
		printf(" \t\tcliente.senha = |%s|\n", senha);
	}
	query = NULL;
	return retorno;
}

bool produtoVencido(char *idProduto, char *email)//retorna TRUE se o produto estiver vencido, false se estiver dentro do prazo de validade
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
	if (email == NULL)
	{
		printf(" Warning: email == NULL em OperacoesBanco-FuncoesGenericas.h produtoVencido() chjkjtrtds54\n");
		// return true;
	}
	else
	{
		if (strcmp(email, LOGIN_DO_SITE) == 0)
		{
			return false;//PODE ACESSAR INFORMAÇÕES DE PRODUTO
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
	// char *retorno = NULL;
	
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