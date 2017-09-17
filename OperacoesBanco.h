#ifndef __OPERACOES_BANCO__
#define __OPERACOES_BANCO__

#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
//#include <stdarg.h>
MYSQL *conexao;


#ifndef bool
#define bool char
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#define DATABASE_HOST "127.0.0.1"//TESTES
// #define DATABASE_HOST "35.186.190.243"//GOOGLE CLOUD
#define DATABASE_USER "interface"
#define DATABASE_PASSWORD "senha"
#define DATABASE_SCHEMA "teste"
#define DATABASE_PORT 0
#define DATABASE_DEFAULT_SOCKET NULL
#define DATABASE_DEFAULT_FLAGS 0


// bool conectarBanco(MYSQL *conexao)
bool conectarBanco()//OK
{
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

bool checarSeVoltaAlgumaCoisaDaQuery(char *query)
{
	if(query == NULL)
	{
		printf(" ERRO: Query recebida igual a NULL (OperacoesBanco.h) rodarQuery()\n");
		return false;
	}

	if(mysql_query(conexao, query))//Se ocorrer algum erro
	{
		printf(" ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (rodarQuery)\n");
		printf(" ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
		printf(" Query executada: |%s|\n", query);
		free(query);
		query = NULL;
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


// bool checarLogin(MYSQL *conexao, char *email)//Funcional
bool checarLogin(char *email)//OK
{
	if(conexao == NULL)
	{
		printf("ERRO DE CONEXÃO (OperacoesBanco.h) (checarLogin)\n");
		return false;
	}
	if(email == NULL)
	{
		printf("ERRO: Não há login para checar (email == NULL) (OperacoesBanco.h) (checarLogin)\n");
		return false;
	}

	char *query = NULL;

	query = malloc(sizeof(char) * ( strlen(email) + 60 + 2));
	if(query == NULL)
	{
		printf(" ERRO: não foi possível alocar memória para a query (OperacoesBanco.h) (checarLogin)\n");
		return false;
	}

	snprintf(query, sizeof(char) * ( strlen(email) + 60 + 2  ), "SELECT cliente.email from cliente WHERE cliente.email = \'%s\';", email );

	if(query == NULL)
	{
		printf(" ERRO: não foi possível alocar memória para a query (2) (OperacoesBanco.h) (checarLogin)\n");
		return false;
	}

	bool retorno = checarSeVoltaAlgumaCoisaDaQuery(query);
	query = NULL;
	return retorno;
}

// bool checarIdProduto(MYSQL *conexao, char *id)
bool checarIdProduto(char *id)//OK
{
	if(conexao == NULL)
	{
		printf("ERRO DE CONEXÃO (OperacoesBanco.h) (checarIdProduto())\n");
		return false;
	}
	if(id == NULL)
	{
		printf("ERRO: Não há login para checar (id == NULL) (OperacoesBanco.h) (checarIdProduto())\n");
		return false;
	}
	if(strlen(id) != 10)
	{
		printf("ERRO: Id informado contém quantidade incorreta de caracteres (OperacoesBanco.h) checarIdProduto())\n");
		return false;
	}

	char *query = NULL;

	query = malloc(sizeof(char) * ( strlen(id) + 69));
	if(query == NULL)
	{
		printf(" ERRO: não foi possível alocar memória para a query (OperacoesBanco.h) (checarIdProduto())\n");
		return false;
	}

	snprintf(query, sizeof(char) * ( strlen(id) + 69 ), "SELECT produto.idproduto from produto WHERE produto.idproduto = \'%s\';", id );

	if(query == NULL)
	{
		printf(" ERRO: não foi possível alocar memória para a query (2) (OperacoesBanco.h) (checarIdProduto())\n");
		return false;
	}

	bool retorno = checarSeVoltaAlgumaCoisaDaQuery(query);
	query = NULL;
	return retorno;
}


int checaExistenciaDeVisualizacaoDeProdutoComPessoa(char *idproduto, char *email)//Se retorna RETORNO_OK, existe algo ali, se retornar RETORNO_NULO, não existe
{
	if(idproduto == NULL)
	{
		printf("Produto com id nulo em OperacoesBanco.h,  checaExistenciaDeVisualizacaoDeProdutoComPessoa()\n");
		return RETORNO_ERRO_DE_PARAMETRO;
	}
	if(email == NULL)
	{
		printf("Pessoa com id nulo em OperacoesBanco.h,  checaExistenciaDeVisualizacaoDeProdutoComPessoa()\n");
		return RETORNO_ERRO_DE_PARAMETRO;
	}
	if(conexao == NULL)
	{
		printf("conexao == NULL em OperacoesBanco.h, checaExistenciaDeVisualizacaoDeProdutoComPessoa()\n");
		return RETORNO_ERRO_DE_CONEXAO;
	}
	if(strlen(idproduto) != 10)
	{
		printf("ERRO: Id de produto informado contém quantidade incorreta de caracteres (OperacoesBanco.h) checaExistenciaDeVisualizacaoDeProdutoComPessoa()\n");
		return RETORNO_ERRO_DE_PARAMETRO;
	}

	char *query = NULL;
	int tamanho = sizeof(char) * ( strlen(idproduto) + strlen(email) + 180);

	query = malloc(tamanho);
	if(query == NULL)
	{
		printf(" ERRO: não foi possível alocar memória para a query (OperacoesBanco.h) (checaExistenciaDeVisualizacaoDeProdutoComPessoa())\n");
		return RETORNO_ERRO_FALTA_DE_MEMORIA;
	}

	snprintf(query, tamanho, "SELECT quantidade FROM visualizacaoDeUsuario JOIN cliente ON cliente.email=\'%s\' JOIN produto ON produto.idproduto=visualizacaoDeUsuario.produto_idproduto WHERE produto.idproduto=\'%s\';", email, idproduto );

	if(query == NULL)
	{
		printf(" ERRO: não foi possível alocar memória para a query (2) (OperacoesBanco.h) (checarIdProduto())\n");
		return RETORNO_ERRO_FALTA_DE_MEMORIA;
	}

	if(mysql_query(conexao, query))//Se ocorrer algum erro
	{
		printf(" Ocorreram erros durante a execução da query (OperacoesBanco.h) (checaExistenciaDeVisualizacaoDeProdutoComPessoa())\n");
		printf(" ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
		printf(" Query enviada =  |%s|\n", query);
		free(query);
		query = NULL;
		return RETORNO_ERRO_DE_CONEXAO;
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
					free(query);
					query = NULL;
					// printf(" EMAIL (OperacoesBanco.h) = %s\n",email);
					// printf(" POSICAO DE MEMORIA DE EMAIL (OperacoesBanco.h) = %x\n", &email );
					return RETORNO_OK;
				}
				else
				{
					mysql_free_result(resposta);
					resposta = NULL;
					free(query);
					query = NULL;
					return RETORNO_NULO;
				}
			}
			else
			{
				mysql_free_result(resposta);
				resposta = NULL;
				free(query);
				query = NULL;
				return RETORNO_NULO;
			}
		}	
		else
		{
			mysql_free_result(resposta);
			resposta = NULL;
			free(query);
			query = NULL;
			return RETORNO_NULO;
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
		return RETORNO_ERRO;
	}
}

// bool addVisualizacoesAoBanco(MYSQL *conexao, char *id, int quantidade)
bool addVisualizacoesAoBanco(char *id, int quantidade, char *email, bool usuarioAnonimo)
{
	/*UPDATE produto SET produto.visualizacaoanom = 0 WHERE produto.idproduto = 'cocacolavc';*/
	if(email == NULL)
	{
		printf("ERRO: Email == NULL OperacoesBanco.h (addVisualizacoesAoBanco())\n");
		return false;
	}

	if(id == NULL)
	{
		printf("ERRO: Não há login para checar (id == NULL) (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
		return false;
	}

	if(conexao == NULL)
	{
		printf("ERRO DE CONEXÃO (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
		return false;
	}
	
	if(quantidade < 0  || quantidade > 9999)
	{
		printf("ERRO: Quantidade informada é inválida (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
		return false;
	}

	if(strlen(id) != 10)
	{
		printf("ERRO: Id informado contém quantidade incorreta de caracteres (OperacoesBanco.h) addVisualizacoesAoBanco())\n");
		return false;
	}


	if(usuarioAnonimo)
	{
		printf(" LOG: USUARIO ANONIMO DETECTADO\n");
		char *query = NULL;

		int tamanhoDaQuery = sizeof(char) * (strlen(id) + 107 + 2 + 1);

		query = malloc(tamanhoDaQuery);
		if(query == NULL)
		{
			printf(" ERRO: não foi possível alocar memória para a query (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
			return false;
		}
		memset(query, '\0', tamanhoDaQuery);

		snprintf(query, tamanhoDaQuery, "UPDATE produto SET produto.visualizacaoanom = produto.visualizacaoanom + %d WHERE produto.idproduto = \'%s\';", quantidade, id );

		if(query == NULL)
		{
			printf(" ERRO: não foi possível alocar memória para a query (2) (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
			return false;
		}

		if(mysql_query(conexao, query))//Se ocorrer algum erro
		{
			printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
			printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
			printf("\t Query enviada =  |%s|\n", query);
			free(query);
			query = NULL;
			return false;
		}
		else
		{


			free(query);
			query = NULL;

			switch(checaExistenciaDeVisualizacaoDeProdutoComPessoa(id, email))
			{
				case RETORNO_OK://Se já tiver criado a tabela de visualizacaDeUsuario
					//Usar update para atualizar dados já existentes
					printf(" LOG: Informações já existem\n");
					tamanhoDaQuery = sizeof(char) * (strlen(id) + strlen(email) + 301);

					query = malloc(tamanhoDaQuery);
					if(query == NULL)
					{
						printf(" ERRO: não foi possível alocar memória para a query(3) (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
						return false;
					}
					memset(query, '\0', tamanhoDaQuery);

					snprintf(query, tamanhoDaQuery, "UPDATE visualizacaoDeUsuario JOIN cliente ON cliente.idcliente=visualizacaoDeUsuario.cliente_idcliente JOIN produto ON produto.idproduto=visualizacaoDeUsuario.produto_idproduto SET visualizacaoDeUsuario.quantidade=visualizacaoDeUsuario.quantidade+%d WHERE produto.idproduto=\'%s\' AND cliente.email=\'%s\';", quantidade, id, email );

					if(query == NULL)
					{
						printf(" ERRO: não foi possível alocar memória para a query (4) (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
						return false;
					}

					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
					break;
				case RETORNO_NULO:
					//Usar Insert para inserir dados
					printf(" LOG: Informações NÃO existem ainda\n");
					tamanhoDaQuery = sizeof(char) * (strlen(id) + strlen(email) + 204);

					query = malloc(tamanhoDaQuery);
					if(query == NULL)
					{
						printf(" ERRO: não foi possível alocar memória para a query(5) (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
						return false;
					}
					memset(query, '\0', tamanhoDaQuery);

					snprintf(query, tamanhoDaQuery, "INSERT INTO visualizacaoDeUsuario(quantidade,cliente_idcliente,produto_idproduto) SELECT %d,cliente.idcliente,produto.idproduto FROM cliente JOIN produto ON produto.idproduto=\'%s\' WHERE cliente.email=\'%s\';", quantidade, id, email);

					if(query == NULL)
					{
						printf(" ERRO: não foi possível alocar memória para a query(6) (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
						return false;
					}

					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
					break;
				case RETORNO_ERRO_DE_PARAMETRO:
					printf(" ERRO: Parametros incorretos foram passados, OperacoesBanco.h addVisualizacoesAoBanco()\n");
					return false;
					break;
				case RETORNO_ERRO_DE_CONEXAO:
					printf(" ERRO: Não foi possível estabelecer conexão com o banco de dados OperacoesBanco.h addVisualizacoesAoBanco()\n");
					return false;
					break;
				case RETORNO_ERRO_FALTA_DE_MEMORIA:
					printf(" ERRO: Falta de memória em OperacoesBanco.h addVisualizacoesAoBanco()\n");
					return false;
					break;
				case RETORNO_ERRO:
					printf(" ERRO: Desconhecido em OperacoesBanco.h,  addVisualizacoesAoBanco()\n");
					return false;
					break;
			}
			return false;
		}
		return false;
	}
	else
	{
		printf("USUARIO NÃO É ANONIMO\n");

		char *query = NULL;

		int tamanhoDaQuery = sizeof(char) * (strlen(id) + 102);

		query = malloc(tamanhoDaQuery);
		if(query == NULL)
		{
			printf(" ERRO: não foi possível alocar memória para a query(7) (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
			return false;
		}
		memset(query, '\0', tamanhoDaQuery);

		snprintf(query, tamanhoDaQuery, "UPDATE produto SET produto.visualizacoes=produto.visualizacoes + %d WHERE produto.idproduto=\'%s\';", quantidade, id );

		if(query == NULL)
		{
			printf(" ERRO: não foi possível alocar memória para a query (8) (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
			return false;
		}

		if(mysql_query(conexao, query))//Se ocorrer algum erro
		{
			printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
			printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
			printf("\t Query enviada =  |%s|\n", query);
			free(query);
			query = NULL;
			return false;
		}
		else
		{
			
			free(query);
			query = NULL;

			// int existencia = checaExistenciaDeVisualizacaoDeProdutoComPessoa(id, email);

			// switch(existencia)
			switch(checaExistenciaDeVisualizacaoDeProdutoComPessoa(id, email))
			{
				case RETORNO_OK://Se já tiver criado a tabela de visualizacaDeUsuario
					//Usar update para atualizar dados já existentes
					printf(" LOG: Informações já existem\n");
					tamanhoDaQuery = sizeof(char) * (strlen(id) + strlen(email) + 301);

					query = malloc(tamanhoDaQuery);
					if(query == NULL)
					{
						printf(" ERRO: não foi possível alocar memória para a query(9) (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
						return false;
					}
					memset(query, '\0', tamanhoDaQuery);

					snprintf(query, tamanhoDaQuery, "UPDATE visualizacaoDeUsuario JOIN cliente ON cliente.idcliente=visualizacaoDeUsuario.cliente_idcliente JOIN produto ON produto.idproduto=visualizacaoDeUsuario.produto_idproduto SET visualizacaoDeUsuario.quantidade=visualizacaoDeUsuario.quantidade+%d WHERE produto.idproduto=\'%s\' AND cliente.email=\'%s\';", quantidade, id, email );

					if(query == NULL)
					{
						printf(" ERRO: não foi possível alocar memória para a query (10) (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
						return false;
					}

					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
					break;
				case RETORNO_NULO:
					//Usar Insert para inserir dados
					printf(" LOG: Informações NÃO existem ainda\n");
					tamanhoDaQuery = sizeof(char) * (strlen(id) + strlen(email) + 204);

					query = malloc(tamanhoDaQuery);
					if(query == NULL)
					{
						printf(" ERRO: não foi possível alocar memória para a query(11) (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
						return false;
					}
					memset(query, '\0', tamanhoDaQuery);

					snprintf(query, tamanhoDaQuery, "INSERT INTO visualizacaoDeUsuario(quantidade,cliente_idcliente,produto_idproduto) SELECT %d,cliente.idcliente,produto.idproduto FROM cliente JOIN produto ON produto.idproduto=\'%s\' WHERE cliente.email=\'%s\';", quantidade, id, email);

					if(query == NULL)
					{
						printf(" ERRO: não foi possível alocar memória para a query(12) (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
						return false;
					}

					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addVisualizacoesAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
					break;
				case RETORNO_ERRO_DE_PARAMETRO:
					printf(" ERRO: Parametros incorretos foram passados, OperacoesBanco.h addVisualizacoesAoBanco()\n");
					return false;
					break;
				case RETORNO_ERRO_DE_CONEXAO:
					printf(" ERRO: Não foi possível estabelecer conexão com o banco de dados OperacoesBanco.h addVisualizacoesAoBanco()\n");
					return false;
					break;
				case RETORNO_ERRO_FALTA_DE_MEMORIA:
					printf(" ERRO: Falta de memória em OperacoesBanco.h addVisualizacoesAoBanco()\n");
					return false;
					break;
				case RETORNO_ERRO:
					printf(" ERRO: Desconhecido em OperacoesBanco.h,  addVisualizacoesAoBanco()\n");
					return false;
					break;
			}
			return false;
		}
		return false;	}
	return false;
}

bool addUsuarioAnonimoAoBanco(char *email, char *senha)
{
	if(email == NULL)
	{
		printf(" ERRO: email passado igual a NULL OperacoesBanco.h addUsuarioAnonimoAoBanco()\n");
		return false;
	}
	if(senha == NULL)
	{
		printf(" ERRO: senha passada igual a NULL OperacoesBanco.h addUsuarioAnonimoAoBanco()\n");
		return false;
	}
	if(conexao == NULL)
	{
		printf(" ERRO: Não há conexao com o banco de dados OperacoesBanco.h addUsuarioAnonimoAoBanco()\n");
		return false;
	}
	printf(" LOG: Chegou com sucesso até a adição de usuario ao banco (anonimo) OperacoesBanco.h addUsuarioAnonimoAoBanco()\n");

	char *query = NULL;
	int tamanho = sizeof(char) * (61 + strlen(email) + TAMANHO_SENHA_PADRAO_USUARIO_ANONIMO + 1);//O +1 é do '/0'
	query = malloc(tamanho);

	if(query == NULL)
	{
		printf(" Warning: falha ao alocar memoria para query (OperacoesBanco.h) addUsuarioAnonimoAoBanco()\n");
		return false;
	}
	snprintf(query, tamanho, "INSERT INTO cliente(email, senha, anonimo) VALUES(\'%s\', \'%s\', 1);", email, senha);

	if(query == NULL)
	{
		printf(" ERRO: não foi possível alocar memória para a query (10) (OperacoesBanco.h) (addUsuarioAnonimoAoBanco())\n");
		return false;
	}

	if(mysql_query(conexao, query))//Se ocorrer algum erro
	{
		printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addUsuarioAnonimoAoBanco())\n");
		printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
		printf("\t Query enviada =  |%s|\n", query);
		free(query);
		query = NULL;
		return false;
	}
	free(query);
	query = NULL;
	return true;
}

bool addUsuarioAoBanco(char *email, char *senha, int idade, int sexo, char *dataNascimento)
{
	if(email == NULL)
	{
		printf(" ERRO: email == NULL em OperacoesBanco.h addUsuarioAoBanco(char *email, char *senha, int idade, int sexo, char *dataNascimento)\n");
		return false;
	}
	if(senha == NULL)
	{
		printf(" ERRO: senha == NULL em OperacoesBanco.h addUsuarioAoBanco(char *email, char *senha, int idade, int sexo, char *dataNascimento)\n");
		return false;
	}
	if(dataNascimento == NULL)
	{
		printf(" ERRO: email == NULL em OperacoesBanco.h addUsuarioAoBanco(char *email, char *senha, int idade, int sexo, char *dataNascimento)\n");
		return false;
	}
	//TODO
	printf(" LOG: Ainda não implementado OperacoesBanco.h addUsuarioAoBanco()\n");
	return false;
}



bool addCidadeAoBanco(char *nomeDoEstado, char *nomeCidade)
{
	if(nomeCidade == NULL)
	{
		printf(" ERRO: Nome de cidade == NULL em OperacoesBanco.h addCidadeAoBanco()\n");
		return false;
	}
	if(nomeDoEstado == NULL)
	{
		printf(" ERRO: Nome de estado informado == NULL em OperacoesBanco.h addCidadeAoBanco()\n");
		return false;
	}
	if(conexao == NULL)
	{
		printf(" ERRO: Não há conexao com o banco de dados em OperacoesBanco.h addCidadeAoBanco()\n");
		return false;
	}

	char *query = NULL;
	int tamanho = sizeof(char) * (100 + strlen(nomeDoEstado) + strlen(nomeCidade) + 1);
	query = malloc(tamanho);
	if(query == NULL)
	{
		printf(" Warning: não foi possível alocar memoria para query (1) em OperacoesBanco.h addCidadeAoBanco()\n");
		return false;
	}
	snprintf(query, tamanho, "SELECT cidade.nome, estado.nome from cidade JOIN estado WHERE cidade.nome = \'%s\' and estado.nome = \'%s\';", nomeCidade ,nomeDoEstado);

	if(query == NULL)
	{
		printf(" Warning: falha ao formatar query em OperacoesBanco.h addCidadeAoBanco()\n");
		return false;
	}

	/* Checa se existe estado com esse nome */
	if(!checarSeVoltaAlgumaCoisaDaQuery(query))
	{
		/* Se entrar aqui, então não existe cidade com esse nome nesse estado */
//		free(query);  //Dá free(query) no checarSeVoltaAlgumaCoisaDaQuery mesmo se retornar false
		query = NULL;

		tamanho = sizeof(char) * (101 + strlen(nomeDoEstado) + strlen(nomeCidade) + 1);
		query = malloc(tamanho);
		if(query == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h addCidadeAoBanco() (2)\n");
			return false;
		}
		else
		{
			snprintf(query, tamanho, "INSERT INTO cidade(nome, estado_idestado) SELECT \'%s\',estado.idestado FROM estado WHERE estado.nome=\'%s\';", nomeCidade, nomeDoEstado);
			if(query == NULL)
			{
				printf(" Warning: não foi possivel formatar query OperacoesBanco.h addCidadeAoBanco() (2)\n");
				return false;
			}
			if(mysql_query(conexao, query))//Se ocorrer algum erro
			{
				printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addCidadeAoBanco())\n");
				printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
				printf("\t Query enviada =  |%s|\n", query);
				free(query);
				query = NULL;
				return false;
			}
			free(query);
			query = NULL;
			return true;
		}
		printf(" ERRO: Exceção não manipulada OperacoesBanco.h addCidadeAoBanco()\n");
		return false;
	}
	else
	{
		printf(" Warning: Pode haver mais de uma cidade com esse nome nesse estado em OperacoesBanco.h addCidadeAoBanco()\n");
		return false;
	}
	printf(" ERRO: Exceção não manipulada OperacoesBanco.h addCidadeAoBanco()\n");
	return false;
}




bool addLocalizacaoAoBanco(char* idCidade, char *cep, char *bairro, char *rua, char *numero, char *complemento)// TODO Limitar criação de localizacoes repetidas
{
	if(conexao == NULL)
	{
		printf(" Warning: Conexao nula com banco de dados (OperacoesBanco.h addLocalizacaoAoBanco())\n");
		return false;
	}
	if(idCidade == NULL)
	{
		printf(" Warning: Impossivel existir cidade nula, abortando em OperacoesBanco.h addLocalizacaoAoBanco()\n");
		return false;
	}
	if(cep == NULL)
	{
		printf(" Warning: Não é possível adicionar localizacao sem informar CEP em OperacoesBanco.h addLocalizacaoAoBanco()\n");
		return false;
	}
	if(bairro == NULL)
	{
		if(rua == NULL)
		{
			if(numero == NULL)
			{
				if(complemento == NULL)
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + 57 + 1);
					char *query = malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (1)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep) VALUES(%s,\'%s\');", idCidade, cep);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (1)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
				else
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(complemento) + 70 + 1);
					char *query = malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (2)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,complemento) VALUES(%s,\'%s\',%s);", idCidade, cep, complemento);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (2)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
			}
			else
			{
				if(complemento == NULL)//Numero nao nulo e todo o resto nulo
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(numero) + 65 + 1);
					char *query = malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (3)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,numero) VALUES(%s,\'%s\',%s);", idCidade, cep, numero);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (3)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
				else//Tudo nulo menos numero e complemento
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(numero) + strlen(complemento) + 81 + 1);
					char *query = malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (4)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,numero,complemento) VALUES(%s,\'%s\',%s, \'%s\');", idCidade, cep, numero, complemento);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (4)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
			}
		}
		else// Se rua não for nulo
		{
			if(numero == NULL)// rua não nula mas numero sim
			{
				if(complemento == NULL)// somente rua nao nula
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(rua) + 64 + 1);
					char *query = malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (7)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,rua) VALUES(%s,\'%s\',\'%s\');", idCidade, cep, rua);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (7)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
				else// Somente rua e complemento nao nulos
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(rua) + strlen(complemento) + 79 + 1);
					char *query = malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (8)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,rua,complemento) VALUES(%s,\'%s\',\'%s\',\'%s\');", idCidade, cep, rua, complemento);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (8)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
			}
			else// Rua e numero nao nulos
			{
				if(complemento == NULL)// Somente rua e numero nao nulos
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(numero) + strlen(rua) + 73 + 1);
					char *query = malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (5)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,numero,rua) VALUES(%s,\'%s\',%s, \'%s\');", idCidade, cep, numero, rua);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (5)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
				else//Somente rua, numero e complemento nao nulos
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(numero) + strlen(rua) + strlen(complemento) + 87 + 1);
					char *query = malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (6)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,numero,rua,complemento) VALUES(%s,\'%s\',%s,\'%s\',\'%s\');", idCidade, cep, numero, rua, complemento);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (6)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
			}
		}
	}
	else// Bairro nao nulo
	{
		if(rua == NULL)// Somente bairro nao nulo
		{
			if(numero == NULL)//Somente bairro nao nulo
			{
				if(complemento == NULL)//Somente bairro nao nulo
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(bairro) + 67 + 1);
					char *query = malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (9)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,bairro) VALUES(%s,\'%s\',\'%s\');", idCidade, cep, bairro);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (9)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
				else// Bairro e complemento nao nulos
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(bairro) + strlen(complemento) + 83 + 1);
					char *query = malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (10)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,bairro, complemento) VALUES(%s,\'%s\',\'%s\',\'%s\');", idCidade, cep, bairro, complemento);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (10)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
			}
			else// Bairro, numero nao nulos
			{
				if(complemento == NULL)// Somente bairro e numero nao nulos (RUA NULA)
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(bairro) + strlen(numero) + 76 + 1);
					char *query = malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (11)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,bairro,numero) VALUES(%s,\'%s\',\'%s\', %s);", idCidade, cep, bairro, numero);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (11)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
				else//Bairo, numero complemento nao nulos  (RUA NULA)
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(bairro) + strlen(numero) + strlen(complemento) + 78 + 1);
					char *query = malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (12)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,bairro,numero) VALUES(%s,\'%s\',\'%s\',%s,\'%s\');", idCidade, cep, bairro, numero, complemento);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (12)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
			}
		}
		else//Bairro e rua nao nulos
		{	
			if(numero == NULL)//Somente bairro e rua nao nulos
			{
				if(complemento == NULL)//Somente bairo e rua nao nulos
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(bairro) + strlen(rua) + 74 + 1);
					char *query = malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (15)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,bairro,rua) VALUES(%s,\'%s\',\'%s\',\'%s\');", idCidade, cep, bairro, rua);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (15)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
				else//Bairro, rua e complemento nao nulos
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(bairro) + strlen(rua) + strlen(complemento) + 89 + 1);
					char *query = malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (16)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,bairro,rua,complemento) VALUES(%s,\'%s\',\'%s\',\'%s\',\'%s\');", idCidade, cep, bairro, rua, complemento);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (16)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
			}
			else// Bairro, rua e numero nao nulos
			{
				if(complemento == NULL)//Bairro, rua e numero nao nulos, mas complemento nulo
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(bairro) + strlen(numero) + strlen(rua) + 82 + 1);
					char *query = malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (14)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,bairro,numero,rua) VALUES(%s,\'%s\',\'%s\',%s,\'%s\');", idCidade, cep, bairro, numero, rua);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (14)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
				else// nada nulo, armazenar bairo, numero, rua, complemento
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(bairro) + strlen(numero) + strlen(complemento) + strlen(rua) + 97 + 1);
					char *query = malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (13)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,bairro,numero,rua,complemento) VALUES(%s,\'%s\',\'%s\',%s,\'%s\',\'%s\');", idCidade, cep, bairro, numero, rua, complemento);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (13)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
			}
		}
	}
	printf(" ERRO: Exceção não manipulada em OperacoesBanco.h addLocalizacaoAoBanco()\n");
	return false;
}


bool addContratanteAoBanco(char *nome, char *cnpj, char *plano, char *email, char *telefone, char *idLocalizacao)
{
	if(conexao == NULL)
	{
		printf(" Warning: conexão nula em addContratanteAoBanco() OperacoesBanco.h\n");
		return false;
	}

	if(idLocalizacao == NULL)
	{
		printf(" Warning: Não foi informado idLocalizacao em addContratanteAoBanco() OperacoesBanco.h \n");
		return false;
	}

	if(nome == NULL)
	{
		printf(" Warning: nome informado é nulo em addContratanteAoBanco()  OperacoesBanco.h\n");
		return false;
	}

	if(plano == NULL)
	{
		printf(" Warning: plano nulo em addContratanteAoBanco() OperacoesBanco.h\n");
		return false;
	}

	if(email == NULL)
	{
		printf(" Warning: email informado é nule em addContratanteAoBanco() OperacoesBanco.h\n");
		return false;
	}

	int tamanho;
	char *query = NULL;


	/* Checa se a já existe empresa com esse cnpj e esse email cadastrada */
	tamanho = 68 + strlen(email) + strlen(cnpj) + 1; // Esse +1 é o do bendito \0 kkk
	query = malloc(sizeof(char) * tamanho);

	if(query == NULL)
	{

		printf(" Warning: Falha ao alocar memoria para query em addContratanteAoBanco() OperacoesBanco.h (qbbabdkhhhf)");
		return false;

	}
	else
	{

		snprintf(query, tamanho, "SELECT idcontratante FROM contratante C WHERE C.email=\'%s\' OR C.cnpj=%s;", email, cnpj);
		if(checarSeVoltaAlgumaCoisaDaQuery(query))
		{
			printf(" Warning: Já existe Uma empresa com esse email e/ou esse cnpj cadastrados no banco de dados em addContratanteAoBanco()  OperacoesBanco.h bqkjshqoxiusya\n");
			query= NULL;
			return false;
		}
		else
		{
			printf(" LOG: Não existe empresa com esse cnpj nem esse email no banco de dados em addContratanteAoBanco() OperacoesBanco.h \n");

			query = NULL;
			tamanho = 51 + 1 + strlen(idLocalizacao);
			query = malloc(sizeof(char) * tamanho);

			if(query == NULL)
			{
				printf(" Warning: Falha ao alocar memoria para query em addContratanteAoBanco() OperacoesBanco.h 56827d\n");
				return false;
			}
			else
			{
				snprintf(query, tamanho, "SELECT * FROM localizacao L WHERE L.idlocalizacao=%s;", idLocalizacao);
				if(query == NULL)
				{
					printf(" Warning: Falha ao formatar query em addContratanteAoBanco() OperacoesBanco.h qcbid\n");
					return false;
				}
				else
				{
					if(checarSeVoltaAlgumaCoisaDaQuery(query))
					{
						//Entra aqui se  query retornar algo
						printf(" LOG: Localizacao informada realmente existe em addContratanteAoBanco() OperacoesBanco.h qkcb\n");
					}
					else
					{
						printf(" Warning: Localizacao informada é inexistente, encerrando operacao em addContratanteAoBanco() OperacoesBanco.h");
						query = NULL;
						return false;
					}
				}
			}
			
			printf(" \t Continuando adição ao banco\n");
		}

	}

	if(telefone == NULL)
	{
		printf(" LOG: Telefone não informado, prosseguindo em addContratanteAoBanco() OperacoesBanco.h\n");
		tamanho = 90 + 1 + strlen(plano) + strlen(cnpj) + strlen(nome) + strlen(email) + strlen(idLocalizacao);
		query = malloc(sizeof(char) * tamanho );
		if(query == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para query em addContratanteAoBanco() OperacoesBanco.h\n");
			return false;
		}
		else
		{
			snprintf(query, tamanho, "INSERT INTO contratante(email,plano,cnpj,nome,localizacao_idlocalizacao) VALUES(\'%s\',%s,%s,\'%s\',%s);", email, plano, cnpj, nome, idLocalizacao);

			if(query == NULL)
			{
				printf(" Warning: Não foi possível formatar aquery para enviar ao banco de dados em addContratanteAoBanco() OperacoesBanco.h \n");
				return false;
			}
		}
	}
	else
	{
		printf(" LOG: Telefone informado pelo cliente em addContratanteAoBanco() OperacoesBanco.h\n");

		tamanho = 102 + 1 + strlen(plano) + strlen(cnpj) + strlen(nome) + strlen(email) + strlen(telefone) + strlen(idLocalizacao);
		query = malloc(sizeof(char) * tamanho );
		if(query == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para query em addContratanteAoBanco() OperacoesBanco.h (dasbkdbqwjkd)\n");
			return false;
		}
		else
		{
			snprintf(query, tamanho, "INSERT INTO contratante(email,plano,cnpj,nome,telefone,localizacao_idlocalizacao) VALUES(\'%s\',%s,%s,\'%s\',\'%s\',%s);", email, plano, cnpj, nome, telefone, idLocalizacao);
			if(query == NULL)
			{
				printf(" Warning: Não foi possível formatar aquery para enviar ao banco de dados em addContratanteAoBanco() OperacoesBanco.h (oqvervinsjk)\n");
				return false;
			}
		}
	}

	if(query == NULL)
	{
		printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addContratanteAoBanco() (sakdjhjkhq)\n");
		return false;
	}

	if(mysql_query(conexao, query))//Se ocorrer algum erro
	{
		printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addContratanteAoBanco())\n");
		printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
		printf("\t Query enviada =  |%s|\n", query);
		free(query);
		query = NULL;
		return false;
	}
	free(query);
	query = NULL;
	return true;
}


bool addProdutoAoBanco(char *idContratante, char *idProduto, char *duracao, char *nomeProduto)
{
	if(conexao == NULL)
	{
		printf(" Warning: Conexao nula em addProdutoAoBanco() OperacoesBanco.h 68sg2\n");
		return false;
	}

	if(idContratante == NULL)
	{
		printf(" Warning: idContratante == NULL em addProdutoAoBanco() OperacoesBanco.h jhdsak\n");
		return false;
	}

	if(idProduto == NULL)
	{
		printf(" Warning: idProduto == NULL em addProdutoAoBanco() OperacoesBanco.h dshjk\n");
		return false;
	}

	if(duracao == NULL)
	{
		printf(" Warning: duracao == NULL em addProdutoAoBanco() OperacoesBanco.h dshajks\n");
		return false;
	}

	if(nomeProduto == NULL)
	{
		printf(" Warning: Nome do produto não foi informado em addProdutoAoBanco() OperacoesBanco.h 6jk29df\n");
		return false;
	}

	int tamanho;
	tamanho = 53 + 1 + strlen(idContratante);
	char *query;

//INICIO DA CHECAGEM DE DADOS
	query = malloc(sizeof(char) * tamanho);

	if(query == NULL)
	{
		printf(" Warning: falha ao alocar memoria para query em addProdutoAoBanco() OperacoesBanco.h ijhvbhsd87vbe\n");
		return false;
	}

	snprintf(query, tamanho, "SELECT * FROM contratante C WHERE C.idcontratante=\'%s\';", idContratante);

	if(query == NULL)
	{
		printf(" Warning: Falha ao formatar query para checagem de dados em addProdutoAoBanco() OperacoesBanco.h dvksjbkj\n");
		return false;
	}

	if(checarSeVoltaAlgumaCoisaDaQuery(query))
	{
		//Se voltar algo da query
		printf(" LOG: Existe empresa com esse ID no banco de dados, continuando com a adição do produto em addProdutoAoBanco() OperacoesBanco.h\n");
	}
	else
	{
		printf(" Warning: Não existe empresa com esse id (contratante) no banco de dados, encerrando operacao em addProdutoAoBanco() OperacoesBanco.h jcvkdhkc\n");
		return false;
	}

//FIM DA CHECAGEM DE DADOS
	query = NULL;
	tamanho = 0;



	tamanho = 93 + 1 + strlen(idContratante) + strlen(idProduto) + strlen(duracao) + strlen(nomeProduto);
	query = malloc(sizeof(char) * tamanho);

	if(query == NULL)
	{
		printf(" Warning: Não foi possivel alocar memoria para query em addProdutoAoBanco() OperacoesBanco.h 3kjhv73b\n");
		return false;
	}

	snprintf(query, tamanho, "INSERT INTO produto(idproduto,nomeproduto,duracao,contratante_idcontratante) VALUES(\'%s\',\'%s\',%s,%s);", idProduto, nomeProduto, duracao, idContratante);

	if(query == NULL)
	{
		printf(" Warning: Não foi possivel formatar query em addProdutoAoBanco() OperacoesBanco.h c3jjhvr87jh\n");
		return false;
	}

	if(mysql_query(conexao, query))//Se ocorrer algum erro
	{
		printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addProdutoAoBanco())\n");
		printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
		printf("\t Query enviada =  |%s|\n", query);
		free(query);
		query = NULL;
		return false;
	}
	free(query);
	query = NULL;

	return true;
	
}

char *obterIdCidadeDoBanco(char *nomeCidade, char *nomeEstado)// APP 4 CHAVE_DE_SEGURANCA_PHP $ { nomeCidadeInformada nomeEstadoInformado
{
	if(conexao == NULL)
	{
		printf(" ERRO: Conexao nula em OperacoesBanco.h obterIdCidadeDoBanco() !!!\n");
		return NULL;
	}
	
	if(nomeCidade == NULL)
	{
		printf(" ERRO: Comunicacao entre funcoes falhou, nomeCidade == NULL em OperacoesBanco.h obterIdCidadeDoBanco()\n");
		return NULL;
	}

	if(nomeEstado == NULL)
	{
		printf(" ERRO: Comunicacao entre funcoes falhou, nomeEstado == NULL em OperacoesBanco.h obterIdCidadeDoBanco()\n");
		return NULL;
	}

	int tamanho = 108 + 1 + strlen(nomeEstado) + strlen(nomeCidade);
	char *query = malloc(sizeof(char) * (tamanho));

	if(query == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h obterIdCidadeDoBanco() qkjeh\n");
		return NULL;
	}

	snprintf(query, tamanho, "SELECT C.idcidade FROM cidade C JOIN estado E ON E.idestado=C.estado_idestado WHERE E.nome=\'%s\' AND C.nome=\'%s\';", nomeEstado, nomeCidade);


	if(query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterIdCidadeDoBanco() bqjek\n");
		return NULL;
	}

	if(mysql_query(conexao, query))
	{
		printf(" ERRO: Falha ao executar query em OperacoesBanco.h obterIdCidadeDoBanco() sakdjh\n");
		printf(" \t%s\n", mysql_error(conexao));
		return NULL;
	}

	MYSQL_RES *resultado = mysql_store_result(conexao);

	if(resultado == NULL)// Se não houver consulta
	{
		printf(" Warning: Resultado nulo em OperacoesBanco.h obterIdCidadeDoBanco() kqht\n");
		return NULL;
	}
	else
	{

//		MYSQL_FIELD *campos;
//		campos = mysql_fetch_fields(resultado);//Passa campos com resposta para a variavel campos

		if(mysql_num_fields(resultado) != 1)
		{
			if(mysql_num_fields(resultado) == 0)
			{
				printf(" LOG: Nada encontrado na base de dados para:\n");
				printf(" \t%s\n", query);
				printf(" \t em OperacoesBanco.h obterIdCidadeDoBanco() 4rr455\n");
				mysql_free_result(resultado);
				free(query);
				resultado = NULL;
				query = NULL;
				return RETORNO_NOT_FOUND;
			}
			else
			{
				printf(" Warning: Resultado inconsistente em OperacoesBanco.h obterIdCidadeDoBanco() hekja\n");
				mysql_free_result(resultado);
				free(query);
				query = NULL;
				resultado = NULL;
				return NULL;
			}
		}
		else
		{
			if(mysql_num_rows(resultado) != 1)
			{
				printf(" Warning: Mais de uma cidade com mesmo nome e estado em OperacoesBanco.h obterIdCidadeDoBanco() ekjjha\n");
				printf(" \tRespostas:\n");
				MYSQL_ROW linhas;
				while((linhas = mysql_fetch_row(resultado)) != NULL)
				{
					printf(" linhas[0] -> %s\n", linhas[0]);
				}
				printf(" \n");
				mysql_free_result(resultado);
				free(query);
				query = NULL;
				resultado = NULL;
				return linhas[0];//Retorna o ultimo resultado, mesmo se houver erro
			}
			else
			{
				MYSQL_ROW linha;

				linha = mysql_fetch_row(resultado);

				if(linha == NULL)
				{
					printf(" ERRO: Não era pra poder retornar nulo aqui\n");
					mysql_free_result(resultado);
					free(query);
					query = NULL;
					resultado = NULL;
					return NULL;
				}
				else
				{
					mysql_free_result(resultado);
					free(query);
					query = NULL;
					resultado = NULL;
					return linha[0];
				}
			}
		}

	}
	return NULL;
}



char *obterTop10NovosProdutosDoBanco()
{
	if(conexao == NULL)
	{
		printf(" ERRO: conexao nula em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		return NULL;
	}

	int tamanho = 65 + 1;
	char *query = malloc(sizeof(char) * (tamanho));

	if(query == NULL)
	{
		printf(" Warning: Falha ao tentar alocar memoria para query em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		return NULL;
	}

	snprintf(query, tamanho, "SELECT idproduto FROM produto ORDER BY datacriacao DESC LIMIT 10;");

	if(query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		return NULL;
	}

	if(mysql_query(conexao, query))
	{
		printf(" Warning: falhao ao executar query em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		free(query);
		query = NULL;
		return NULL;
	}

	MYSQL_RES *resultado = mysql_store_result(conexao);
	if(resultado == NULL)// Se não houver consulta
	{
		printf(" Warning: Consulta não realizada em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		free(query);
		query = NULL;
		return NULL;
	}

	if(mysql_num_fields(resultado) == 0)
	{
		printf(" Warning: Nada encontrado em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		free(query);
		query = NULL;
		mysql_free_result(resultado);
		resultado = NULL;
		return RETORNO_NOT_FOUND;
	}

	MYSQL_ROW linha = NULL;
	char *retorno = NULL;

	tamanho = ((mysql_num_rows(resultado) * 10) + 1 + mysql_num_rows(resultado)-1);

	retorno = malloc(sizeof(char) * tamanho);// numero de resultados * tamanho de cada resultado + \0 + numero de resultados -1

	if(retorno == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para retorno em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		free(query);
		query = NULL;
		mysql_free_result(resultado);
		resultado = NULL;
		return NULL;
	}
	memset(retorno, '\0', tamanho);

//	 int i = 0;

	while((linha = mysql_fetch_row(resultado)) != NULL)
	{
//		snprintf(retorno, tamanho, "%s %s", retorno, linha);
		
		printf(" LINHA OBTIDA = |%s| em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n", linha[0]);
		if(strlen(linha[0]) > 0)
		{
			strcat(retorno, linha[0]);
		}

		printf(" Retorno = |%s| até agora em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n", retorno);

		if(retorno == NULL)
		{
			printf(" Warning: Falha ao formatar resultado em OperacoesBanco.h obterTop10NovosProdutosDoBanco() vbrjka\n");
			free(query);
			mysql_free_result(resultado);
			resultado = NULL;
			query = NULL;
			return NULL;
		}
		if(strlen(retorno) < tamanho - 1)
		{
			strcat(retorno, " ");
		}
	}

	if(retorno == NULL)
	{
		printf(" ERRO: Falha ao obter dados em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		free(query);
		mysql_free_result(resultado);
		resultado = NULL;
		query = NULL;
		return NULL;
	}
	else
	{
		free(query);
		mysql_free_result(resultado);
		resultado = NULL;
		query = NULL;
		return retorno;
	}

	return NULL;
}


char *obterDescricaoProdutoDoBanco(char *idProduto)
{
	if (conexao == NULL)
	{
		printf(" Warning: Conexao perdida com o banco de dados em OperacoesBanco.h obterDescricaoProdutoDoBanco() 4e5b11eea\n");
		return NULL;
	}
	else if(idProduto == NULL)
	{
		printf(" Warning: Id de produto nulo em OperacoesBanco.h obterDescricaoProdutoDoBanco() veqf45\n");
		return NULL;
	}
	else
	{
		char *query = NULL;
		//int tamanho = 55 + TAMANHO_ID_PRODUTO + 1;// Usando TAMANHO_ID_PRODUTO para otimização
		int tamanho = 66;// Usando TAMANHO_ID_PRODUTO para otimização

		query = malloc(sizeof(char) * tamanho);
		if(query == NULL)
		{
			printf(" Warning: nao foi possível alocar memoria para query em OperacoesBanco.h obterDescricaoProdutoDoBanco()\n");
			return false;
		}

		snprintf(query, tamanho, "SELECT P.descricao FROM produto P WHERE P.idproduto=\'%s\';", idProduto);

		if(query == NULL)
		{
			printf(" Warning: nao foi possível formatar query em OperacoesBanco.h obterDescricaoProdutoDoBanco() cqhkhbjvr\n");
			return false;
		}

		if(mysql_query(conexao, query))
		{
			printf(" Warning: falha ao executar query em OperacoesBanco.h obterDescricaoProdutoDoBanco() kjelkzj\n");
			free(query);
			query = NULL;
			return NULL;
		}

		MYSQL_RES *resultado = mysql_store_result(conexao);

		if(resultado == NULL)
		{
			printf(" Warning: Consulta não realizada em OperacoesBanco.h obterDescricaoProdutoDoBanco() nao houve conexao\n");
			free(query);
			query = NULL;
			return NULL;
		}

		if(mysql_num_fields(resultado) == 0)
		{
			printf(" Warning: Nao houve resultados na pesquisa em OperacoesBanco.h obterDescricaoProdutoDoBanco() bnje\n");
			free(query);
			mysql_free_result(resultado);
			resultado = NULL;
			query = NULL;
			return RETORNO_NOT_FOUND;
		}

		MYSQL_ROW linha = NULL;
		char *retorno = NULL;

		linha = mysql_fetch_row(resultado);
		if(linha == NULL)
		{
			printf(" Warning: Resposta Nula em OperacoesBanco.h obterDescricaoProdutoDoBanco()\n");
			free(query);
			mysql_free_result(resultado);
			resultado = NULL;
			query = NULL;
			return NULL;
		}
		else
		{
			if(linha[0] == NULL)/* EDITADO */
			{
				printf(" Warning: Pesquisa por linha não retornou nada em OperacoesBanco.h obterDescricaoProdutoDoBanco() 38ahah1kkj\n");
				printf(" \tQuery utilizada: |%s|\n", query);
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return NULL;
			}
			if(strlen(linha[0]) > 1)
			{
				printf(" LOG: Retorno obtido com sucesso em OperacoesBanco.h obterDescricaoProdutoDoBanco() kjh3as\n");
			}
			else
			{

				printf(" Warning: Produto nao possui descrição cadastrada no banco de dados em OperacoesBanco.h obterDescricaoProdutoDoBanco() kejhqkfge3\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return RETORNO_NOT_FOUND;

			}

			int tamanho = strlen(linha[0]) + 1;
			retorno = malloc(sizeof(char) * tamanho);

			if(retorno == NULL)
			{

				printf(" Warning: Nao foi possivel alocar memoria para retorno em OperacoesBanco.h obterDescricaoProdutoDoBanco() sakdjhjkredf\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return NULL;

			}

			memset(retorno, '\0', tamanho);

			strcpy(retorno, linha[0]);

			if(retorno == NULL)
			{

				printf(" Warning: Nao foi possivel copiar memoria para retorno em OperacoesBanco.h obterDescricaoProdutoDoBanco() sakdjhjkredf\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return NULL;

			}
			else
			{

				printf(" LOG: Operação concluida com sucesso em OperacoesBanco.h obterDescricaoProdutoDoBanco() ckjahjksd\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return retorno;

			}


		}

	}
	printf(" ERRO: erro Desconhecido chave: ew54b510530fv8qw em OperacoesBanco.h obterDescricaoProdutoDoBanco()\n");
	return NULL;
}




char *obterNomeProdutoDoBanco(char *idProduto)
{
	if(conexao == NULL)
	{
		printf(" Warning: Conexao nula com o banco de dados em obterNomeProdutoDoBanco() OperacoesBanco.h \n");
		return NULL;
	}
	if(idProduto == NULL)
	{
		printf(" Warning: idProduto informado NULO em obterNomeProdutoDoBanco() OperacoesBanco.h\n");
		return NULL;
	}
	else
	{
		char *query = NULL;
		//int tamanho = 57 + TAMANHO_ID_PRODUTO + 1;// Usando TAMANHO_ID_PRODUTO para otimização
		int tamanho = 68;// Usando TAMANHO_ID_PRODUTO para otimização

		query = malloc(sizeof(char) * tamanho);
		if(query == NULL)
		{
			printf(" Warning: nao foi possível alocar memoria para query em OperacoesBanco.h obterNomeProdutoDoBanco()\n");
			return false;
		}

		snprintf(query, tamanho, "SELECT P.nomeProduto FROM produto P WHERE P.idproduto=\'%s\';", idProduto);

		if(query == NULL)
		{
			printf(" Warning: nao foi possível formatar query em OperacoesBanco.h obterNomeProdutoDoBanco() cqhkhbjvr\n");
			return false;
		}

		if(mysql_query(conexao, query))
		{
			printf(" Warning: falha ao executar query em OperacoesBanco.h obterNomeProdutoDoBanco() kjelkzj\n");
			free(query);
			query = NULL;
			return NULL;
		}

		MYSQL_RES *resultado = mysql_store_result(conexao);

		if(resultado == NULL)
		{
			printf(" Warning: Consulta não realizada em OperacoesBanco.h obterNomeProdutoDoBanco() nao houve conexao\n");
			free(query);
			query = NULL;
			return NULL;
		}

		if(mysql_num_fields(resultado) == 0)
		{
			printf(" Warning: Nao houve resultados na pesquisa em OperacoesBanco.h obterNomeProdutoDoBanco() bnje\n");
			free(query);
			mysql_free_result(resultado);
			resultado = NULL;
			query = NULL;
			return RETORNO_NOT_FOUND;
		}

		MYSQL_ROW linha = NULL;
		char *retorno = NULL;

		linha = mysql_fetch_row(resultado);
		if(linha == NULL)
		{
			printf(" Warning: Resposta Nula em OperacoesBanco.h obterNomeProdutoDoBanco()\n");
			free(query);
			mysql_free_result(resultado);
			resultado = NULL;
			query = NULL;
			return NULL;
		}
		else
		{
			if(strlen(linha[0]) > 1)
			{

				printf(" LOG: Retorno obtido com sucesso em OperacoesBanco.h obterNomeProdutoDoBanco() kjh3as\n");

			}
			else
			{

				printf(" Warning: Produto nao possui descrição cadastrada no banco de dados em OperacoesBanco.h obterNomeProdutoDoBanco() kejhqkfge3\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return RETORNO_NOT_FOUND;

			}

			int tamanho = strlen(linha[0]) + 1;
			retorno = malloc(sizeof(char) * tamanho);

			if(retorno == NULL)
			{

				printf(" Warning: Nao foi possivel alocar memoria para retorno em OperacoesBanco.h obterNomeProdutoDoBanco() sakdjhjkredf\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return NULL;

			}

			memset(retorno, '\0', tamanho);

			strcpy(retorno, linha[0]);

			if(retorno == NULL)
			{

				printf(" Warning: Nao foi possivel copiar memoria para retorno em OperacoesBanco.h obterNomeProdutoDoBanco() sakdjhjkredf\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return NULL;

			}
			else
			{

				printf(" LOG: Operação concluida com sucesso em OperacoesBanco.h obterNomeProdutoDoBanco() ckjahjksd\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return retorno;

			}


		}

	}
	printf(" ERRO: erro Desconhecido chave: ew54b510530fv8qw em OperacoesBanco.h obterNomeProdutoDoBanco()\n");
	return NULL;
}






#endif
