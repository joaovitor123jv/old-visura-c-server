//Arquivo que contem os "Scripts" mais genericos usados pelo OperaçõesBanco.h
#pragma once
#include "../Comandos/Comandos.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<mysql/mysql.h>

#include "../Bibliotecas/AdaptadorDeString/AdaptadorDeString.h"
#include "../Bibliotecas/Usuario.h"

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


#ifndef CONEXAO_COM_BANCO_DE_DADOS_DEFINIDA
	#define CONEXAO_COM_BANCO_DE_DADOS_DEFINIDA
	MYSQL *conexao;
#endif


/** 
 * @brief  Conecta-se ao banco e armazena os resultados em MYSQL *conexao.
 * @note   Não é seguro em multithread
 * @retval true, se conectou, false caso contrario.
 */
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

/** 
 * @brief  Encerra conexao com o banco de dados
 * @note   Trabalhando com variavel global MYSQL *conexao
 * @retval true, sempre
 */
bool desconectarBanco()
{
	mysql_close(conexao);
	conexao = NULL;
	mysql_library_end();
	return true;
}


/** 
 * @brief  Checa se a conexão com o banco de dados está ativa, e tenta uma re-conexao
 * @note   Só checa se a conexao é nula...
 * @retval true, caso a conexao esteja ativa
 */
bool conexaoAtiva()
{
	if(conexao == NULL)
	{
		printf("ERRO DE CONEXÃO (OperacoesBanco-FuncoesGenericas.h) (checarConexao())\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(conectarBanco())
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação em OperacoesBanco-FuncoesGenericas.h checarConexao()()\n");
			return true;
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return false;
		}
	}
	return true;
}


/** 
 * @brief  Executa a query no banco de dados, e, se a query retornar algo, então retorna true
 * @note   Libera a query
 * @param  *query: Query a ser executada no banco de dados
 * @retval true, se retornar conteudo, false caso contrario
 */
bool queryRetornaConteudo(Usuario *usuario, char *query)
{
	if(query == NULL)
	{
		printf(" ERRO: Query recebida igual a NULL (OperacoesBanco.h) checarSeVoltaAlgumaCoisaDaQuery()\n");
		return false;
	}

	if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
	{
		printf(" ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) checarSeVoltaAlgumaCoisaDaQuery()\n");
		printf(" ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
		printf(" Query executada: |%s|\n", query);
		free(query);
		query = NULL;

		if(mysql_errno(usuario->conexao) == ERRO_CONEXAO_ENCERRADA_MYSQL)// SERVER MYSQL SUMIU
		{
			printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h checarSeVoltaAlgumaCoisaDaQuery()\n");
			if(conectarBanco())
			{
				printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h checarSeVoltaAlgumaCoisaDaQuery()\n");
			}
			else
			{
				usuario->conexao = NULL;
				mysql_close(usuario->conexao);
				mysql_thread_end();
				free(usuario->conexao);
				printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h checarSeVoltaAlgumaCoisaDaQuery()\n");
			}
		}
		return false;
	}

	MYSQL_RES *resposta = NULL;
	MYSQL_FIELD *campos = NULL;

	resposta = mysql_store_result(usuario->conexao);

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
/** 
 * @brief  Executa a query, e retorna false, caso seja recusada pelo banco de dados
 * @note   Não libera a query
 * @param  *query: Query a ser executada no banco de dados
 * @retval true, se query for executada, false caso contrario
 */
bool executaQuery(Usuario *usuario, char *query)
{
	if(query == NULL)
	{
		printf(" ERRO: Query recebida igual a NULL (OperacoesBanco.h) checarSeVoltaAlgumaCoisaDaQuery()\n");
		return false;
	}

	if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
	{
		printf(" ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) checarSeVoltaAlgumaCoisaDaQuery()\n");
		printf(" ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
		printf(" Query executada: |%s|\n", query);
		if(mysql_errno(usuario->conexao) == ERRO_CONEXAO_ENCERRADA_MYSQL)// SERVER MYSQL SUMIU
		{
			printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h checarSeVoltaAlgumaCoisaDaQuery()\n");
			if(conectarBanco())
			{
				printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h checarSeVoltaAlgumaCoisaDaQuery()\n");
			}
			else
			{
				usuario->conexao = NULL;
				mysql_close(usuario->conexao);
				mysql_thread_end();
				free(usuario->conexao);
				printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h checarSeVoltaAlgumaCoisaDaQuery()\n");
			}
		}
		return false;
	}
	printf(" LOG: Query |%s| executada com sucesso em OperacoesBanco-FuncoesGenericas.h executaQuery() qkjhjdsa\n", query);
	return true;
}

/** 
 * @brief  Obtem o primeiro retorno de uma query, somente. Retorna NULL caso falhe
 * @note   Libera query, Uso interno
 * @param  *query: Query a ser executada no banco de dados
 * @retval NULL, caso de erro. Resultado da query caso contrario
 */
char *obterRetornoUnicoDaQuery(Usuario *usuario, char *query)
{
	if (!usuario_conexaoAtiva(usuario))
	{
		printf(" ERRO: Conexao inativa detectada em OperacoesBanco-FuncoesGenericas.h obterRetornoUnicoDaQuery()\n");
		return NULL;
	}
	if( query == NULL )
	{
		printf(" ERRO: query nula em obterRetornoUnicoDaQuery() OperacoesBanco-FuncoesGenericas.h\n");
		return NULL;
	}

	printf(" LOG: Executando query: |%s| em OperacoesBanco-FuncoesGenericas.h obterRetornoUnicoDaQuery()\n", query);
	bool tentarDenovo = false;
	int tentativas = 0;
	
	do
	{
		if(mysql_query(usuario->conexao, query))
		{
			if( mysql_errno(usuario->conexao) == ERRO_CONEXAO_ENCERRADA_MYSQL)
			{
				printf(" ERRO: Conexão com o banco de dados encerrada detectada em OperacoesBanco-FuncoesGenericas.h obterRetornoUnicoDaQuery() artb9vcn\n");
				usuario->conexao = NULL;
				conectarBanco();
				if( tentativas > 2 )
				{
					tentarDenovo = false;
				}
				else
				{
					tentarDenovo = true;
				}
				tentativas = tentativas + 1;
			}
			printf(" ERRO: Falha ao executar query em obterRetornoUnicoDaQuery() OperacoesBanco-FuncoesGenericas.h sakdjh\n");
			printf(" ERRO MYSQL nº%d = \t|%s|\n", mysql_errno(usuario->conexao) ,mysql_error(usuario->conexao));
			return NULL;
		}
	}while(tentarDenovo);
	
	MYSQL_RES *resultado = mysql_store_result(usuario->conexao);
	
	if(resultado == NULL)// Se não houver consulta
	{
		printf(" Warning: Resultado nulo em obterRetornoUnicoDaQuery() OperacoesBanco-FuncoesGenericas.h kqht\n");
		return NULL;
	}
	else
	{
		geraLog(LOG, "Analisando resultados obtidos");
		if(mysql_num_fields(resultado) != 1)
		{
			if(mysql_num_fields(resultado) == 0)
			{
				printf(" LOG: Nada encontrado na base de dados para:\n");
				printf(" \t%s\n", query);
				printf(" \t em obterRetornoUnicoDaQuery() OperacoesBanco-FuncoesGenericas.h 4rr455\n");
				mysql_free_result(resultado);
				free(query);
				resultado = NULL;
				query = NULL;
				return NULL;
			}
			else
			{
				printf(" Warning: Resultado inconsistente em obterRetornoUnicoDaQuery() OperacoesBanco-FuncoesGenericas.h hekja\n");
				mysql_free_result(resultado);
				free(query);
				query = NULL;
				resultado = NULL;
				return NULL;
			}
		}
		else
		{
			if(mysql_num_rows(resultado) > 1)
			{
				printf(" Warning: Mais de uma cidade com mesmo nome e estado em obterRetornoUnicoDaQuery() OperacoesBanco-FuncoesGenericas.h ekjjha\n");
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
					return NULL;
				}
				mysql_free_result(resultado);
				free(query);
				query = NULL;
				resultado = NULL;
				return retorno;//Retorna o ultimo resultado, mesmo se houver erro
			}
			else if (mysql_num_rows(resultado) == 1)
			{
				geraLog(LOG, "Encontrado exatamente um resultado para a query em questao");
				MYSQL_ROW linha;
				
				linha = mysql_fetch_row(resultado);
				
				if(linha == NULL)
				{
					printf(" ERRO: Não era pra poder retornar nulo aqui em obterRetornoUnicoDaQuery() OperacoesBanco-FuncoesGenericas.h abv84eu9h89hbd\n");
					mysql_free_result(resultado);
					free(query);
					query = NULL;
					resultado = NULL;
					return NULL;
				}
				else
				{
					char *retorno;
					if (linha[0] == NULL)
					{
						retorno = NULL;
					}
					else
					{
						retorno = strdup(linha[0]);						
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
				printf(" Warning: nenhum resultado encontrado em obterRetornoUnicoDaQuery() OperacoesBanco-FuncoesGenericas.h\n");
				mysql_free_result(resultado);
				resultado = NULL;
				free(query);
				query = NULL;
				return NULL;
			}
		}
	}
}

/** 
 * @brief  Retorna uma informação obtida na query na primeira posição
 * @note   Libera a query, retorna direto ao usuario, função externa
 * @param  *query: 
 * @retval char *
 */
char *retornaUnicoRetornoDaQuery(Usuario *usuario, char *query)
{
	if (!usuario_conexaoAtiva(usuario))
	{
		printf(" Warning: Conexao inativa detectada em OperacoesBanco-FuncoesGenericas.h retornaUnicoRetornoDaQuery()\n");
		return RETORNO_ERRO_INTERNO_BANCO_STR_DINAMICA;
	}
	if( query == NULL )
	{
		printf(" ERRO: query nula em OperacoesBanco-FuncoesGenericas.h retornaUnicoRetornoDaQuery()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	printf(" LOG: Executando query: |%s| em OperacoesBanco-FuncoesGenericas.h obterRetornoUnicoDaQuery()\n", query);
	
	bool tentarDenovo = false;
	int tentativas = 0;
	do
	{
		if(mysql_query(usuario->conexao, query))
		{
			if( mysql_errno(usuario->conexao) == ERRO_CONEXAO_ENCERRADA_MYSQL)
			{
				printf(" ERRO: Conexão com o banco de dados encerrada detectada em OperacoesBanco-FuncoesGenericas.h obterRetornoUnicoDaQuery() artb9vcn\n");
				usuario->conexao = NULL;
				usuario_conectarBanco(usuario);
				if( tentativas > 2 )
				{
					tentarDenovo = false;
				}
				else
				{
					tentarDenovo = true;
				}
				tentativas = tentativas + 1;
			}
			printf(" ERRO: Falha ao executar query em retornaUnicoRetornoDaQuery() OperacoesBanco-FuncoesGenericas.h sakdjh\n");
			printf(" ERRO MYSQL nº%d = \t|%s|\n", mysql_errno(usuario->conexao) ,mysql_error(usuario->conexao));
			return RETORNO_ERRO_INTERNO_STR_DINAMICA;
		}
	}while(tentarDenovo);
	
	MYSQL_RES *resultado = mysql_store_result(usuario->conexao);
	
	if(resultado == NULL)// Se não houver consulta
	{
		printf(" Warning: Resultado nulo em OperacoesBanco-FuncoesGenericas.h retornaUnicoRetornoDaQuery() kqht\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
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
				return RETORNO_ERRO_NOT_FOUND_STR_DINAMICA;
			}
			else
			{
				printf(" Warning: Resultado inconsistente em OperacoesBanco-FuncoesGenericas.h retornaUnicoRetornoDaQuery() hekja\n");
				mysql_free_result(resultado);
				free(query);
				query = NULL;
				resultado = NULL;
				return RETORNO_ERRO_INTERNO_STR_DINAMICA;
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
					printf(" Warning: Erro interno em OperacoesBanco-FuncoesGenericas.h retornaUnicoRetornoDaQuery() q9hnyugvxd\n");
					return RETORNO_ERRO_INTERNO_STR_DINAMICA;
				}
				mysql_free_result(resultado);
				free(query);
				query = NULL;
				resultado = NULL;
				printf(" LOG: Retornando informações em OperacoesBanco-FuncoesGenericas.h retornaUnicoRetornoDaQuery() q9hnyugvxd\n");
				return retorno;//Retorna o ultimo resultado, mesmo se houver erro
			}
			else if (mysql_num_rows(resultado) == 1)
			{
				MYSQL_ROW linha;
				
				linha = mysql_fetch_row(resultado);
				
				if(linha == NULL)
				{
					printf(" ERRO: Não era pra poder retornar nulo aqui abv84eu9h89hbd em OperacoesBanco-FuncoesGenericas.h retornaUnicoRetornoDaQuery()\n");
					mysql_free_result(resultado);
					free(query);
					query = NULL;
					resultado = NULL;
					return RETORNO_ERRO_INTERNO_STR_DINAMICA;
				}
				else
				{
					char *retorno = strdup(linha[0]);
					if (retorno == NULL)
					{
						printf(" ERRO: Falha ao duplicar linha[0] em OperacoesBanco-FuncoesGenericas.h retornaUnicoRetornoDaQuery() arty0hgvsy\n");
						return RETORNO_ERRO_INTERNO_STR_DINAMICA;
					}
					mysql_free_result(resultado);
					free(query);
					query = NULL;
					resultado = NULL;
					printf(" LOG: Retornando ao cliente em OperacoesBanco-FuncoesGenericas.h retornaUnicoRetornoDaQuery() qty9nh7c\n");
					return retorno;
				}
			}
			else
			{
				printf(" Warning: nenhum objeto localizado em OperacoesBanco-FuncoesGenericas.h retornaUnicoRetornoDaQuery()\n");
				mysql_free_result(resultado);
				resultado = NULL;
				free(query);
				query = NULL;
				return RETORNO_ERRO_NOT_FOUND_STR_DINAMICA;
			}
		}
	}
}

/** 
 * @brief  Checa se o produto existe na base de dados, retorna true caso exista
 * @note   Não libera *id  (função interna)
 * @param  *id: Id do produto a ser checado se existe
 * @retval true, se existir, false caso contrario
 */
bool checarIdProduto(Usuario *usuario, char *id)//OK
{
	if(id == NULL)
	{
		printf("ERRO: Não há login para checar (id == NULL) (OperacoesBanco-FuncoesGenericas.h) (checarIdProduto())\n");
		return false;
	}
	if(!usuario_conexaoAtiva(usuario))
	{
		printf(" Warning: conexao inativa detectada em OperacoesBanco-FuncoesGenericas.h checarIdProduto()\n");
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

	bool retorno = queryRetornaConteudo(usuario, query);
	query = NULL;
	return retorno;
}

/** 
 * @brief  Checa se o contratante existe no banco de dados, retorna true caso exista
 * @note   não libera *idContratante
 * @param  *idContratante: Id do contratante a ser checado se existe na base de dados
 * @retval true, caso exista, false, caso contrario
 */
bool checarIdContratante(Usuario *usuario, char *idContratante)
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
	
		if(queryRetornaConteudo(usuario, query))//queryRetornaConteudo libera a query
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

/** 
 * @brief  Obtem o ID do contratante do usuário, caso seja contratante.
 * @note   chame usuario_obterID(usuario)
 * @param  *usuario: Usuario que precisa saber oo ID
 * @retval NULL caso falhe, ID do usuario caso dê certo.
 */
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
		return usuario_obterId(usuario);
	}
}


//retorna TRUE se o produto estiver vencido, false se estiver dentro do prazo de validade
/** 
 * @brief  Checa se o produto está vencido pra esse usuario. Retorna false caso esteja dentro do "prazo de validade"
 * @note   Não libera *idProduto, nem *usuario
 * @param  *idProduto: O produto o qual vai ser checada a validade
 * @param  *usuario: O usuario que quer saber se está na validade o produto
 * @retval true, caso esteja vencido. False, caso contrário
 */
bool produtoVencido(char *idProduto, Usuario *usuario)
{

	if (!usuario_conexaoAtiva(usuario))
	{
		printf(" Warning: Conexão inativa detectada em OperacoesBanco-FuncoesGenericas.h produtoVencido()\n");
		return true;
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
	
	//int tamanho = 100+strlen(idProduto)+1;
	int tamanho = 111;
	char *query = (char *)malloc(sizeof(char) * tamanho);
	if(query == NULL)
	{
		printf(" Warning: não foi possível alocar memoria para query em OperacoesBanco-FuncoesGenericas.h produtoVencido() qkjhjvgdf\n");
		return true;
	}
	snprintf(query, tamanho, "SELECT DATEDIFF(CURDATE(), P.datacriacao)+P.duracao FROM produto P WHERE P.idproduto LIKE BINARY \'%s\';", idProduto);
	if(query == NULL)
	{
		printf(" Warning: não foi possível formatar query em OperacoesBanco-FuncoesGenericas.h produtoVencido() sajkdhjbr\n");
		return true;
	}

	if(!executaQuery(usuario, query))
	{
		printf(" Warning: Houveram erros ao executar a query desejada |%s| em OperacoesBanco-FuncoesGenericas.h produtoVencido() wsd456ngh\n", query);
		free(query);
		query = NULL;
		return true;
	}

	MYSQL_RES *resultado = mysql_store_result(usuario->conexao);
	
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

/** 
 * @brief  Retorna strings de ERRO caso dê erro, ou as informações requerida na query, concatenadas, com um espaço separando cada informação
 * @note   usada em obterInformacoesProduto, libera a query
 * @param  *query: query a ser executada no banco de dados
 * @retval retorna direto ao usuario, um ponteiro para caracter não estático (strdup(algumaCoisa))
 */
char *retornaInformacoesObtidasNaQuery(Usuario *usuario, char *query)
{
	if(query == NULL)
	{
		printf(" ERRO: query nula em retornaInformacoesObtidasNaQuery() em OperacoesBanco-FuncoesGenericas.h\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (!usuario_conexaoAtiva(usuario))
	{
		return RETORNO_ERRO_INTERNO_BANCO_STR_DINAMICA;
	}
	
	bool tentarDenovo = false;
	int tentativas = 0;
	do
	{
		if(mysql_query(usuario->conexao, query))
		{
			if( mysql_errno(usuario->conexao) == ERRO_CONEXAO_ENCERRADA_MYSQL)
			{
				printf(" ERRO: Conexão com o banco de dados encerrada detectada em OperacoesBanco-FuncoesGenericas.h obterRetornoUnicoDaQuery() artb9vcn\n");
				usuario->conexao = NULL;
				usuario_conectarBanco(usuario);
				if( tentativas > 2 )
				{
					tentarDenovo = false;
				}
				else
				{
					tentarDenovo = true;
				}
				tentativas = tentativas + 1;
			}
			printf(" ERRO: Falha ao executar query em retornaInformacoesObtidasNaQuery() OperacoesBanco-FuncoesGenericas.h sakdjh\n");
			printf(" ERRO MYSQL nº%d = \t|%s|\n", mysql_errno(usuario->conexao) ,mysql_error(usuario->conexao));
			return RETORNO_ERRO_INTERNO_STR_DINAMICA;
		}
	}while(tentarDenovo);
	
	MYSQL_RES *resultado = mysql_store_result(usuario->conexao);
	if(resultado == NULL)// Se não houver consulta
	{
		printf(" Warning: Consulta não realizada em OperacoesBanco-FuncoesGenericas.h retornaInformacoesObtidasNaQuery()\n");
		free(query);
		query = NULL;
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	
	if(mysql_num_fields(resultado) <= 0)
	{
		printf(" Warning: Nada encontrado em OperacoesBanco-FuncoesGenericas.h retornaInformacoesObtidasNaQuery()\n");
		free(query);
		query = NULL;
		mysql_free_result(resultado);
		resultado = NULL;
		return RETORNO_ERRO_NOT_FOUND_STR_DINAMICA;
	}

	if (mysql_num_rows(resultado) <= 0)
	{
		printf(" Warning: nada encontrado em OperacoesBanco-FuncoesGenericas.h retornaInformacoesObtidasNaQuery() abr0a9sfdb\n");
		free(query);
		query = NULL;
		mysql_free_result(resultado);
		resultado = NULL;
		return RETORNO_ERRO_NOT_FOUND_STR_DINAMICA;
	}

	// int quantidadeDeCampos = mysql_num_fields(resultado);
	//Como esse comando vai varrer somente uma linha, não precisa de ter iteração pra isso
	
	// MYSQL_ROW linha = NULL;
	free(query);
	query = NULL;


	MYSQL_ROW linha = mysql_fetch_row(resultado);
	if (linha == NULL)
	{
		printf(" Warning: linha nula depois de fetch em OperacoesBanco.h obterInformacoesBasicasDeProdutoDoBanco() sahve89as7dgerw\n");
		mysql_free_result(resultado);
		resultado = NULL;
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	int tamanho = mysql_num_fields(resultado) + 1;// o \0 e os espaços entre os comandos
	int i;
	for(i = 0;i < mysql_num_fields(resultado);i++)
	{
		if (linha[i] == NULL)
		{
			// tamanho = strlen("NULL");
			tamanho = tamanho + 4;
		}
		else
		{
			tamanho = tamanho + strlen(linha[i]);
		}
	}

	char *informacoes = (char *)malloc(sizeof(char) * tamanho);
	if (informacoes == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para string de retorno em OperacoesBanco-FuncoesGenericas.h retornaInformacoesObtidasNaQuery() asifuhuiasd\n");
		mysql_free_result(resultado);
		resultado = NULL;
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	memset(informacoes, '\0', tamanho);
	for(i = 0;i < mysql_num_fields(resultado);i++)
	{
		if (linha[i] == NULL)
		{
			strcat(informacoes, "NULL");
		}
		else
		{
			strcat(informacoes, linha[i]);
		}
		if (i != (mysql_num_fields(resultado)-1))
		{
			strcat(informacoes, " ");
		}
	}
	if (informacoes == NULL)
	{
		printf(" ERRO: Falha ao concatenar informacoes de retorno em OperacoesBanco-FuncoesGenericas.h retornaInformacoesObtidasNaQuery() a165w8b79asd\n");
		mysql_free_result(resultado);
		resultado = NULL;
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	mysql_free_result(resultado);
	resultado = NULL;
	return informacoes;
}

/*
	Comando utilizado pelo obterTop10Alguma coisa
	Retorna no máximo, o numero de informações (numero de linhas) informado.
	A query enviada deve ter somente um retorno, ou a função retornará ao usuario somente a primeira coluna desse retorno
	Essa função retorna diretamente ao usuario
	
	Padrão de retorno para retornaNIteracoesDaQuery("SELECT resultado FROM tabela", 4); :
		resultado1 resultado2 resultado3 resultado4
*/

/** 
 * @brief  Retorna uma informação concatenada numeroIteracoes vezes, libera query
 * @note   Usada pela função obterTop10produtos...
 * @param  *query: 
 * @param  numeroIteracoes: 
 * @retval retorna direto ao usuario (char *)
 */
char *retornaNIteracoesDaQuery(Usuario *usuario, char *query, int numeroIteracoes)
{
	if (query == NULL)
	{
		printf(" Warning: query nula detectada em OperacoesBanco-FuncoesGenericas.h retornaNIteracoesDaQuery()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (numeroIteracoes <= 0)
	{
		printf(" Warning: numeroIteracoes invalido detectado em OperacoesBanco-FuncoesGenericas.h retornaNIteracoesDaQuery()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (!usuario_conexaoAtiva(usuario))
	{
		printf(" Warning: conexao inativa detectada em OperacoesBanco-FuncoesGenericas.h retornaNIteracoesDaQuery()\n");
		return RETORNO_ERRO_INTERNO_BANCO_STR_DINAMICA;
	}

	if (!executaQuery(usuario, query))
	{
		free(query);
		query = NULL;
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	free(query);
	query = NULL;

	MYSQL_RES *resultado = mysql_store_result(usuario->conexao);
	if (resultado == NULL)
	{
		printf(" Warning: Consulta não realizada em OperacoesBanco-FuncoesGenericas.h retornaNIteracoesDaQuery()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	
	if(mysql_num_fields(resultado) <= 0)
	{
		printf(" Warning: Nada encontrado em OperacoesBanco-FuncoesGenericas.h retornaNIteracoesDaQuery()\n");
		mysql_free_result(resultado);
		resultado = NULL;
		return RETORNO_ERRO_NOT_FOUND_STR_DINAMICA;
	}

	if (mysql_num_rows(resultado) <= 0)
	{
		printf(" Warning: nada encontrado em OperacoesBanco-FuncoesGenericas.h retornaNIteracoesDaQuery() abr0a9sfdb\n");
		mysql_free_result(resultado);
		resultado = NULL;
		return RETORNO_ERRO_NOT_FOUND_STR_DINAMICA;
	}

	MYSQL_ROW linha;
	int contador = 1;
	int tamanho = 0;
	char *retorno = NULL;

	linha = mysql_fetch_row(resultado);
	if (linha[0] == NULL)
	{
		return RETORNO_ERRO_NOT_FOUND_STR_DINAMICA;
	}
	tamanho = strlen(linha[0]) + 2;
	retorno = (char *)malloc(sizeof(char) * tamanho);
	if (retorno == NULL)
	{
		mysql_free_result(resultado);
		resultado = NULL;
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	strcpy(retorno, linha[0]);
	// retorno = strdup(linha[0]);
	strcat(retorno, " ");

	while((linha = mysql_fetch_row(resultado)))
	{
		if (contador >= numeroIteracoes)
		{
			break;
		}
		else
		{
			if(linha[0] == NULL)
			{
				return RETORNO_ERRO_INTERNO_BANCO_STR_DINAMICA;
			}
			tamanho = tamanho + strlen(linha[0]) + 1;
			retorno = (char *)realloc(retorno, tamanho);
			if (retorno == NULL)
			{
				return RETORNO_ERRO_INTERNO_STR_DINAMICA;
			}
			strcat(retorno, linha[0]);
			strcat(retorno, " ");
			printf(" DEBUG: RETORNO = |%s|\n", retorno);
		}
		contador = contador + 1;
	}
	mysql_free_result(resultado);
	resultado = NULL;
	return retorno;
}


/** 
 * @brief  Retorna informações paginadas
 * @note   Diga quais informações precisa (sem o ; do final), e a função faz o resto
 * @param  *query: "SELECT * FROM isto "
 * @param  *pagina: "1" == LIMIT 1, 10;  "2" == LIMIT 11, 20
 * @retval informações concatenadas direto pro usuario
 */
char *retornaPaginado(Usuario *usuario, char *query, char *pagina)
{
	if (query == NULL)
	{
		printf(" ERRO: Query nula detectada em OperacoesBanco-FuncoesGenericas.h retornaPaginado() as1b8982\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (pagina == NULL)
	{
		printf(" ERRO: Pagina nula detectada em OperacoesBanco-FuncoesGenericas.h retornaPaginado() asgf54ro9hsdf\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (stringMenor(query, 0))
	{
		printf(" ERRO: String muito pequena detectada em OperacoesBanco-FuncoesGenericas.h retornaPaginado() ruiogwhnu\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	int pagint = atoi(pagina);
	char *x = intToString((pagint - 1) * 10);
	if (x == NULL)
	{
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	char *y = intToString(((pagint - 1) * 10) + 10);
	if (y == NULL)
	{
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	int tamanho = strlen(query) + strlen(x) + strlen(y) + 10 + 1;
	char *novaQuery = (char *)calloc(sizeof(char), tamanho);
	
	if (novaQuery == NULL)
	{
		printf(" ERRO: Falha ao realocar memoria em OperacoesBanco-FuncoesGenericas.h retornaPaginado() kjasdhbvjrjh\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	printf(" DEBUG: Query recebida: |%s| em OperacoesBanco-FuncoesGenericas.h retornaPaginado() asjlbt\n", query);
	snprintf(novaQuery, tamanho, "%s LIMIT %s,%s;", query, x, y);
	if (novaQuery == NULL)
	{
		printf(" ERRO: Falha ao formatar string em OperacoesBanco-FuncoesGenericas.h retornaPaginado() vkdfjsuri\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	printf(" DEBUG: Query formatada: |%s| em OperacoesBanco-FuncoesGenericas.h retornaPaginado() asjlbt\n", query);

	free(query);
	free(x);
	free(y);
	free(pagina);
	pagina = NULL;
	y = NULL;
	x = NULL;
	query = NULL;

	return retornaNIteracoesDaQuery(usuario, novaQuery, 10);
}

/** 
 * @brief  Checa se a cidade informada existe no banco de dados, retorna true caso positivo
 * @note   não libera idCidade, nem nomeEstado
 * @param  *nomeCidade: Nome da cidade no banco de dados
 * @param  *nomeEstado: Nome do estado no banco de dados (SP, GO...)
 * @retval true, caso cidade exista no banco de dados, false caso contrário
 */
bool cidadeExisteNoBanco(Usuario *usuario, char *nomeCidade, char *nomeEstado)
{
	if (!usuario_conexaoAtiva(usuario))
	{
		geraLog(ERRO, "Conexão perdida com o banco de dados");
		return false;
	}
	if (nomeCidade == NULL || nomeEstado == NULL)
	{
		geraLog(ERRO, "nomeCidade ou nomeEstado == NULL");
		return false;
	}
	int tamanho = 106 + strlen(nomeCidade) + strlen(nomeEstado) + 1;
	char *query = (char *)calloc(sizeof(char), tamanho);
	if (query == NULL)
	{
		geraLog(ERRO, "Falha ao alocar memoria para query");
		return false;
	}

	snprintf(query, tamanho, "SELECT idcidade FROM cidade C JOIN estado E ON E.idestado=C.estado_idestado WHERE C.nome=\'%s\' AND E.nome=\'%s\';", nomeCidade, nomeEstado);
	// char *query = strdup("SELECT idcidade FROM cidade C JOIN estado E ON E.idestado=C.estado_idestado WHERE C.nome='São_Paulo' AND E.nome='SP';");
	if (query == NULL)
	{
		geraLog(ERRO, "Falha ao duplicar query");
		return false;
	}

	printf(" DEBUG: Query formatada = |%s|\n", query);

	char *retorno = obterRetornoUnicoDaQuery(usuario, query);
	if (retorno == NULL)
	{
		geraLog(ERRO, "Cidade não existe no banco de dados");
		return false;
	}
	else
	{
		geraLog(LOG, "Cidade existe no banco de dados");
		return true;
	}
}
