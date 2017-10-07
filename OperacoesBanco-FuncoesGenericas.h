//Arquivo que contem os "Scripts" mais genericos usados pelo OperaçõesBanco.h

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0 
#endif

#ifndef bool
#define bool char
#endif


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
	conexao = malloc(sizeof(MYSQL));
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

//Retorna TRUE se o produto existe na base de dados
// bool checarIdProduto(MYSQL *conexao, char *id)
bool checarIdProduto(char *id)//OK
{
	if(conexao == NULL)
	{
		printf("ERRO DE CONEXÃO (OperacoesBanco.h) (checarIdProduto())\n");
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

// Retorna true caso seja encontrado contratante com esse ID na base de dados
bool checarIdContratante(char *idContratante)
{
	char *query = NULL;
	int tamanho = 53 + 1 + strlen(idContratante);
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
			return true;
		}
		else
		{
			printf(" Warning: Não existe empresa com esse id (%s)(contratante) no banco de dados, encerrando operacao em addProdutoAoBanco() OperacoesBanco.h jcvkdhkc\n", idContratante);
			return false;
		}
}