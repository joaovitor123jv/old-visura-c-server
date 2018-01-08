#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Comandos/Comandos.h"
#include <mysql/mysql.h>


int checaExistenciaDeVisualizacaoDeProdutoComPessoa(char *idproduto, Usuario *usuario)//Se retorna RETORNO_OK, existe algo ali, se retornar RETORNO_NULO, não existe
{
	if(idproduto == NULL)
	{
		printf(" Warning: idproduto nulo detectado em OperacoesBanco-Visualizacoes.h,  checaExistenciaDeVisualizacaoDeProdutoComPessoa()\n");
		return RETORNO_ERRO_DE_PARAMETRO;
	}
	if(usuario_obterLogin(usuario) == NULL)
	{
		printf(" Warning: usuario não conectado detectado em OperacoesBanco-Visualizacoes.h,  checaExistenciaDeVisualizacaoDeProdutoComPessoa()\n");
		return RETORNO_ERRO_DE_PARAMETRO;
	}
	if(conexao == NULL)
	{
		printf("conexao == NULL em OperacoesBanco-Visualizacoes.h, checaExistenciaDeVisualizacaoDeProdutoComPessoa()\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(conectarBanco())
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return RETORNO_ERRO_DE_CONEXAO;
		}
	}
	if(strlen(idproduto) != 10)
	{
		printf("ERRO: Id de produto informado contém quantidade incorreta de caracteres (OperacoesBanco-Visualizacoes.h) checaExistenciaDeVisualizacaoDeProdutoComPessoa()\n");
		return RETORNO_ERRO_DE_PARAMETRO;
	}
	
	char *query = NULL;
    // int tamanho = sizeof(char) * ( strlen(idproduto) + strlen(email) + 183 + 1);
    int tamanho = sizeof(char) * (194 + usuario_obterTamanhoLogin(usuario));
	
	query = malloc(tamanho);
	if(query == NULL)
	{
		printf(" ERRO: não foi possível alocar memória para a query (OperacoesBanco-Visualizacoes.h) (checaExistenciaDeVisualizacaoDeProdutoComPessoa())\n");
		return RETORNO_ERRO_FALTA_DE_MEMORIA;
	}
	
    // snprintf(query, tamanho, "SELECT quantidade FROM visualizacaoDeUsuario JOIN cliente ON cliente.email=visualizacaoDeUsuario.cliente_idcliente JOIN produto ON P.idproduto=V.produto_idproduto WHERE P.idproduto=\'%s\';", email, idproduto );
    snprintf(query, tamanho, "SELECT quantidade FROM visualizacaoDeUsuario V JOIN cliente C ON C.idcliente=V.cliente_idcliente JOIN produto P ON P.idproduto=V.produto_idproduto WHERE P.idproduto=\'%s\' AND C.email=\'%s\';", idproduto, usuario_obterLogin(usuario));
	
	if(query == NULL)
	{
		printf(" ERRO: não foi possível alocar memória para a query (2) (OperacoesBanco-Visualizacoes.h) (checarIdProduto())\n");
		return RETORNO_ERRO_FALTA_DE_MEMORIA;
	}
	
	if(mysql_query(conexao, query))//Se ocorrer algum erro
	{
		printf(" Ocorreram erros durante a execução da query (OperacoesBanco-Visualizacoes.h) (checaExistenciaDeVisualizacaoDeProdutoComPessoa())\n");
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
                
                MYSQL_ROW linha = NULL;
				if((linha = mysql_fetch_row(resposta)) != NULL)
				{
                    if (strlen(linha[0]) > 1)
                    {
                        printf(" LOG: Encontrou algo no banco de dados: |%s| em OperacoesBanco-Visualizacoes.h checaExistenciaDeVisualizacaoDeProdutoComPessoa() 6q48r9s8e\n", linha[0]);
                        printf(" LOG: QUERY: |%s| em OperacoesBanco-Visualizacoes.h checaExistenciaDeVisualizacaoDeProdutoComPessoa() 6q48r9s8e\n", query);
                        mysql_free_result(resposta);
                        resposta = NULL;
                        // free()
                        free(query);
                        query = NULL;
                        // printf(" EMAIL (OperacoesBanco-Visualizacoes.h) = %s\n",email);
                        // printf(" POSICAO DE MEMORIA DE EMAIL (OperacoesBanco-Visualizacoes.h) = %x\n", &email );
                        return RETORNO_OK;
                    }
                    else
                    {
                        printf(" LOG: Nada foi encontrado no banco de dados para a query: |%s| em OperacoesBanco-Visualizacoes.h checaExistenciaDeVisualizacaoDeProdutoComPessoa() qkjhvera\n", query);
                        mysql_free_result(resposta);
                        resposta = NULL;
                        free(query);
                        query = NULL;
                        return RETORNO_NULO;
                    }
					// printf("Obteve uma ou mais respostas\n");
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
bool addVisualizacoesAoBanco(char *id, char *quantidade, Usuario *usuario)// APP 2 2 idProduto quantidade
{
	/*UPDATE produto SET produto.visualizacaoanom = 0 WHERE produto.idproduto = 'cocacolavc';*/
	if (usuario == NULL)
	{
		printf(" ERRO: usuario nulo detectado em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
		return false;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" ERRO: usuario não conectado detectado em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
		return false;
	}
	// if(email == NULL)
	// {
	// 	printf("ERRO: Email == NULL OperacoesBanco-Visualizacoes.h (addVisualizacoesAoBanco())\n");
	// 	return false;
	// }
	
	if(id == NULL)
	{
		printf("ERRO: Não há login para checar (id == NULL) (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
		return false;
	}
	
	if(conexao == NULL)
	{
		printf("ERRO DE CONEXÃO (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
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
	
	// if(quantidade < 0  || quantidade > 9999)
	// {
	// 	printf("ERRO: Quantidade informada é inválida (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
	// 	return false;
	// }

    if(quantidade == NULL)
    {
        printf(" Warning: Não foi informada quantidade de produtos a ser adicionados em addVisualizacoesAoBanco() OperacoesBanco-Visualizacoes.h\n");
    }

    //TODO Checar se a quantidade informada é válida

	if(strlen(id) != 10)
	{
		printf("ERRO: Id informado contém quantidade incorreta de caracteres (OperacoesBanco-Visualizacoes.h) addVisualizacoesAoBanco())\n");
		return false;
	}
    
    if (produtoVencido(id, usuario))
    {
        printf(" Warning: Produto vencido detectado em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
        return false;
    }
	
	if(usuario_PermissaoAnonimo(usuario))
	{
		printf(" LOG: USUARIO ANONIMO DETECTADO em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
		char *query = NULL;
		
        //int tamanhoDaQuery = sizeof(char) * (strlen(id) + strlen(quantidade) + 99 + 1);
        int tamanhoDaQuery = sizeof(char) * (strlen(quantidade) + 110);
		
		query = malloc(tamanhoDaQuery);
		if(query == NULL)
		{
			printf(" ERRO: não foi possível alocar memória para a query (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
			return false;
		}
		memset(query, '\0', tamanhoDaQuery);
        
		snprintf(query, tamanhoDaQuery, "UPDATE produto SET produto.visualizacaoanom = produto.visualizacaoanom+%s WHERE produto.idproduto=\'%s\';", quantidade, id );
		
		if(query == NULL)
		{
			printf(" ERRO: não foi possível alocar memória para a query (2) (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
			return false;
		}
        
        //Adiciona quantidade de visualizações anonimas à TABELA de produto, em vizualizacaoanom
		if(mysql_query(conexao, query))//Se ocorrer algum erro
		{
			printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
			printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
			printf("\t Query enviada =  |%s|\n", query);
			free(query);
			query = NULL;
			if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
			{
				printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
				if(conectarBanco())
				{
					printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
				}
				else
				{
					conexao = NULL;
					mysql_close(conexao);
					mysql_thread_end();
					free(conexao);
					printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
				}
			}
			return false;
		}
		else
		{	
			free(query);
			query = NULL;
			
			switch(checaExistenciaDeVisualizacaoDeProdutoComPessoa(id, usuario))
			{
				case RETORNO_OK://Se já tiver criado a tabela de visualizacaDeUsuario
                    //Usar update para atualizar dados já existentes
                    printf(" LOG: Informações já existem em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
                    // tamanhoDaQuery = sizeof(char) * (strlen(id) + strlen(email) + strlen(quantidade) + 198 + 1);
                    tamanhoDaQuery = sizeof(char) * ( usuario_obterTamanhoLogin(usuario) + strlen(quantidade) + 219);
                    
                    query = malloc(tamanhoDaQuery);
                    if(query == NULL)
                    {
                        printf(" ERRO: não foi possível alocar memória para a query(9) (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
                        return false;
                    }
                    memset(query, '\0', tamanhoDaQuery);
                    
                    snprintf(query, tamanhoDaQuery, "UPDATE visualizacaoDeUsuario V JOIN cliente C ON C.idcliente=V.cliente_idcliente JOIN produto P ON P.idproduto=V.produto_idproduto SET V.quantidade=V.quantidade+%s WHERE P.idproduto=\'%s\' AND C.email=\'%s\';", quantidade, id, usuario_obterLogin(usuario) );
                    
                    if(query == NULL)
                    {
                        printf(" ERRO: não foi possível alocar memória para a query (10) (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
                        return false;
                    }
                    
                    if(mysql_query(conexao, query))//Se ocorrer algum erro
                    {
                        printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
                        printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
                        printf("\t Query enviada =  |%s|\n", query);
                        free(query);
                        query = NULL;
                        if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
                        {
                            printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco() 1qa5456dasd\n");
                            if(conectarBanco())
                            {
                                printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco() asd456\n");
                            }
                            else
                            {
                                conexao = NULL;
                                mysql_close(conexao);
                                mysql_thread_end();
                                free(conexao);
                                printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco() as4d5asd\n");
                            }
                        }
                        return false;
                    }
                    free(query);
                    query = NULL;
                    return true;
                    break;
				case RETORNO_NULO:
                    //Usar Insert para inserir dados
                    printf(" LOG: Informações NÃO existem ainda\n");
                    // tamanhoDaQuery = sizeof(char) * (strlen(id) + strlen(email) + strlen(quantidade) + 179 + 1);
                    tamanhoDaQuery = sizeof(char) *  (usuario_obterTamanhoLogin(usuario) + strlen(quantidade) + 190);
                    
                    query = malloc(tamanhoDaQuery);
                    if(query == NULL)
                    {
                        printf(" ERRO: não foi possível alocar memória para a query(5) (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
                        return false;
                    }
                    memset(query, '\0', tamanhoDaQuery);
                    
                    snprintf(query, tamanhoDaQuery, "INSERT INTO visualizacaoDeUsuario(quantidade,cliente_idcliente,produto_idproduto) SELECT %s,C.idcliente,P.idproduto FROM cliente C JOIN produto P ON P.idproduto=\'%s\' WHERE C.email=\'%s\';", quantidade, id, usuario_obterLogin(usuario));
                    
                    if(query == NULL)
                    {
                        printf(" ERRO: não foi possível alocar memória para a query(6) (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
                        return false;
                    }
                    
                    if(mysql_query(conexao, query))//Se ocorrer algum erro
                    {
                        printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
                        printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
                        printf("\t Query enviada =  |%s|\n", query);
                        free(query);
                        query = NULL;
                        if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
                        {
                            printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco() 1qa5456dasd\n");
                            if(conectarBanco())
                            {
                                printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco() asd456\n");
                            }
                            else
                            {
                                conexao = NULL;
                                mysql_close(conexao);
                                mysql_thread_end();
                                free(conexao);
                                printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco() as4d5asd\n");
                            }
                        }
                        return false;
                    }
                    free(query);
                    query = NULL;
                    return true;
                    break;
				case RETORNO_ERRO_DE_PARAMETRO:
                    printf(" ERRO: Parametros incorretos foram passados, OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
                    return false;
                    break;
				case RETORNO_ERRO_DE_CONEXAO:
                    printf(" ERRO: Não foi possível estabelecer conexão com o banco de dados OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
                    return false;
                    break;
				case RETORNO_ERRO_FALTA_DE_MEMORIA:
                    printf(" ERRO: Falta de memória em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
                    return false;
                    break;
				case RETORNO_ERRO:
                    printf(" ERRO: Desconhecido em OperacoesBanco-Visualizacoes.h,  addVisualizacoesAoBanco()\n");
                    return false;
                    break;
                default:
                    printf(" ERRO: Retorno desconhecido por checaExistenciaDeVisualizacaoDeProdutoComPessoa() em addVisualizacoesAoBanco() OperacoesBanco-Visualizacoes.h\n");
                    return false;
                    break;                    
			}
		}
		return false;
	}
	else
	{
		printf(" LOG: USUARIO NÃO É ANONIMO em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
		
		char *query = NULL;
		
        // int tamanhoDaQuery = sizeof(char) * (strlen(id) + strlen(quantidade) + 77 + 1);
        int tamanhoDaQuery = sizeof(char) * ( strlen(quantidade) + 88);
		query = malloc(tamanhoDaQuery);
		if(query == NULL)
		{
			printf(" ERRO: não foi possível alocar memória para a query(7) (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
			return false;
		}
		memset(query, '\0', tamanhoDaQuery);
		
		snprintf(query, tamanhoDaQuery, "UPDATE produto P SET P.visualizacoes=P.visualizacoes + %s WHERE P.idproduto=\'%s\';", quantidade, id );
		
		if(query == NULL)
		{
			printf(" ERRO: não foi possível alocar memória para a query (8) (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
			return false;
		}
		
		if(mysql_query(conexao, query))//Se ocorrer algum erro
		{
			printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
			printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
			printf("\t Query enviada =  |%s|\n", query);
			free(query);
			query = NULL;
			if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
			{
				printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco() 1qa5456dasd\n");
				if(conectarBanco())
				{
					printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco() asd456\n");
				}
				else
				{
					conexao = NULL;
					mysql_close(conexao);
					mysql_thread_end();
					free(conexao);
					printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco() as4d5asd\n");
				}
			}
			return false;
		}
		else
		{
			
			free(query);
			query = NULL;
			
			// int existencia = checaExistenciaDeVisualizacaoDeProdutoComPessoa(id, email);
			
			// switch(existencia)
			switch(checaExistenciaDeVisualizacaoDeProdutoComPessoa(id, usuario))
			{
				case RETORNO_OK://Se já tiver criado a tabela de visualizacaDeUsuario
                    //Usar update para atualizar dados já existentes
                    printf(" LOG: Informações já existem em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
                    // tamanhoDaQuery = sizeof(char) * (strlen(id) + strlen(email) + strlen(quantidade) + 208 + 1);
                    tamanhoDaQuery = sizeof(char) * ( usuario_obterTamanhoLogin(usuario) + strlen(quantidade) + 219);
                    
                    query = malloc(tamanhoDaQuery);
                    if(query == NULL)
                    {
                        printf(" ERRO: não foi possível alocar memória para a query(9) (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
                        return false;
                    }
                    memset(query, '\0', tamanhoDaQuery);
                    
                    snprintf(query, tamanhoDaQuery, "UPDATE visualizacaoDeUsuario V JOIN cliente C ON C.idcliente=V.cliente_idcliente JOIN produto P ON P.idproduto=V.produto_idproduto SET V.quantidade=V.quantidade+%s WHERE P.idproduto=\'%s\' AND C.email=\'%s\';", quantidade, id, usuario_obterLogin(usuario) );
                    
                    if(query == NULL)
                    {
                        printf(" ERRO: não foi possível alocar memória para a query (10) (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
                        return false;
                    }
                    
                    if(mysql_query(conexao, query))//Se ocorrer algum erro
                    {
                        printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
                        printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
                        printf("\t Query enviada =  |%s|\n", query);
                        free(query);
                        query = NULL;
                        if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
                        {
                            printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco() 1qa5456dasd\n");
                            if(conectarBanco())
                            {
                                printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco() asd456\n");
                            }
                            else
                            {
                                conexao = NULL;
                                mysql_close(conexao);
                                mysql_thread_end();
                                free(conexao);
                                printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco() as4d5asd\n");
                            }
                        }
                        return false;
                    }
                    free(query);
                    query = NULL;
                    return true;
                    break;
				case RETORNO_NULO:
                    //Usar Insert para inserir dados
                    printf(" LOG: Informações NÃO existem ainda\n");
                    // tamanhoDaQuery = sizeof(char) * (strlen(id) + strlen(email) + strlen(quantidade) + 179 + 1);
                    tamanhoDaQuery = sizeof(char) *  (usuario_obterTamanhoLogin(usuario) + strlen(quantidade) + 190);
                    
                    query = malloc(tamanhoDaQuery);
                    if(query == NULL)
                    {
                        printf(" ERRO: não foi possível alocar memória para a query(5) (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
                        return false;
                    }
                    memset(query, '\0', tamanhoDaQuery);
                    
                    snprintf(query, tamanhoDaQuery, "INSERT INTO visualizacaoDeUsuario(quantidade,cliente_idcliente,produto_idproduto) SELECT %s,C.idcliente,P.idproduto FROM cliente C JOIN produto P ON P.idproduto=\'%s\' WHERE C.email=\'%s\';", quantidade, id, usuario_obterLogin(usuario));
                    
                    if(query == NULL)
                    {
                        printf(" ERRO: não foi possível alocar memória para a query(6) (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
                        return false;
                    }
                    
                    if(mysql_query(conexao, query))//Se ocorrer algum erro
                    {
                        printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
                        printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
                        printf("\t Query enviada =  |%s|\n", query);
                        free(query);
                        query = NULL;
                        if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
                        {
                            printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco() 1qa5456dasd\n");
                            if(conectarBanco())
                            {
                                printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco() asd456\n");
                            }
                            else
                            {
                                conexao = NULL;
                                mysql_close(conexao);
                                mysql_thread_end();
                                free(conexao);
                                printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco() as4d5asd\n");
                            }
                        }
                        return false;
                    }
                    free(query);
                    query = NULL;
                    return true;
                    break;
				case RETORNO_ERRO_DE_PARAMETRO:
                    printf(" ERRO: Parametros incorretos foram passados, OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
                    return false;
                    break;
				case RETORNO_ERRO_DE_CONEXAO:
                    printf(" ERRO: Não foi possível estabelecer conexão com o banco de dados OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
                    return false;
                    break;
				case RETORNO_ERRO_FALTA_DE_MEMORIA:
                    printf(" ERRO: Falta de memória em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
                    return false;
                    break;
				case RETORNO_ERRO:
                    printf(" ERRO: Desconhecido em OperacoesBanco-Visualizacoes.h,  addVisualizacoesAoBanco()\n");
                    return false;
                    break;
			}
			return false;
		}
		return false;	
	}
	return false;
}


char *obterQuantidadeDeVisualizacoesAnonimasDoBanco(char *idProduto, Usuario *usuario)// APP 4 2 @ 3 * idProduto
{
    if(conexao == NULL)
    {
        printf(" Warning: Conexao nula detectada em obterQuantidadeDeVisualizacoesAnonimasDoBanco() OperacoesBanco-Visualizacoes.h qkjhfd\n");
        if(!conectarBanco())
        {
            printf(" Warning: Falha ao tentar reconectar-se ao banco de dados em obterQuantidadeDeVisualizacoesAnonimasDoBanco() OperacoesBanco-Visualizacoes.h\n");
            return NULL;
        }
        else
        {
            printf(" LOG: Conexao recuperada, continuando operacao em obterQuantidadeDeVisualizacoesAnonimasDoBanco() OperacoesBanco-Visualizacoes.h\n");
        }
    }
    if(idProduto == NULL)
    {
        printf(" Warning: idProduto == NULL em obterQuantidadeDeVisualizacoesAnonimasDoBanco() OperacoesBanco-Visualizacoes.h\n");
        return NULL;
    }

    if (produtoVencido(idProduto, usuario))
    {
        printf(" Warning: Produto vencido detectado em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesAnonimasDoBanco()\n");
        return NULL;
    }
    // int tamanho = 62 + TAMANHO_ID_PRODUTO + 1;// Jamais esqueça o '\0'
    int tamanho = 73;
    char *query = (char *)malloc(sizeof(char) * tamanho);
    if(query == NULL)
    {
        printf(" Warning: falha ao alocar memoria para query em obterQuantidadeDeVisualizacoesAnonimasDoBanco() OperacoesBanco-Visualizacoes.h qkjhetf\n");
        return NULL;
    }

    snprintf(query, tamanho, "SELECT P.visualizacaoanom FROM produto P WHERE P.idproduto=\'%s\';", idProduto);
    if(query == NULL)
    {
        printf(" Warning: falha ao formatar query em obterQuantidadeDeVisualizacoesAnonimasDoBanco() OperacoesBanco-Visualizacoes.h kjheb\n");
        return NULL;
    }
    /* 
    if(mysql_query(conexao, query))
    {
        printf(" Warning: falha ao executar query em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesAnonimasDoBanco() 4654q8\n");

        free(query);
        query = NULL;
        return NULL;
    }
     */

    if(mysql_query(conexao, query))//Se ocorrer algum erro
	{
		printf(" ERRO: Ocorreram erros durante a execução da query (OperacoesBanco-Visualizacoes.h) obterQuantidadeDeVisualizacoesAnonimasDoBanco()\n");
		printf(" ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
		printf(" Query executada: |%s|\n", query);
		free(query);
		query = NULL;

		if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
		{
			printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesAnonimasDoBanco()\n");
			if(conectarBanco())
			{
				printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesAnonimasDoBanco()\n");
			}
			else
			{
				conexao = NULL;
				mysql_close(conexao);
				mysql_thread_end();
				free(conexao);
				printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesAnonimasDoBanco()\n");
			}
		}
		return NULL;
	}


    MYSQL_RES *resultado = mysql_store_result(conexao);
    
    if(resultado == NULL)
    {
        printf(" Warning: Consulta não realizada em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesAnonimasDoBanco() nao houve conexao\n");
        free(query);
        query = NULL;
        return NULL;
    }
    
    if(mysql_num_fields(resultado) == 0)
    {
        printf(" Warning: Nao houve resultados na pesquisa em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesAnonimasDoBanco() bnje\n");
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
        printf(" Warning: Resposta Nula em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesAnonimasDoBanco()\n");
        free(query);
        mysql_free_result(resultado);
        resultado = NULL;
        query = NULL;
        return NULL;
    }
    else
    {
        if(strlen(linha[0]) > 0)
        {
            printf(" LOG: Retorno obtido com sucesso em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesAnonimasDoBanco() kjh3as\n"); 
        }
        else
        {
            printf(" Warning: Produto nao possui visualizações anonimas cadastradas no banco de dados em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesAnonimasDoBanco() kejhqkfge3\n");
            free(query);
            mysql_free_result(resultado);
            resultado = NULL;
            query = NULL;
            return NULL;
        }
        
        int tamanho = strlen(linha[0]) + 1;
        retorno = malloc(sizeof(char) * tamanho);
        
        if(retorno == NULL)
        {
            
            printf(" Warning: Nao foi possivel alocar memoria para retorno em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesAnonimasDoBanco() sakdjhjkredf\n");
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
            
            printf(" Warning: Nao foi possivel copiar memoria para retorno em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesAnonimasDoBanco() sakdjhjkredf\n");
            free(query);
            mysql_free_result(resultado);
            resultado = NULL;
            query = NULL;
            return NULL;
            
        }
        else
        {
            
            printf(" LOG: Operação concluida com sucesso em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesAnonimasDoBanco() ckjahjksd\n");
            free(query);
            mysql_free_result(resultado);
            resultado = NULL;
            query = NULL;
            return retorno;
            
        }
    }
    printf(" ERRO: erro desconhecido em obterQuantidadeDeVisualizacoesAnonimasDoBanco() OperacoesBanco-Visualizacoes.h q654r888r77q65a22d\n");
    return NULL;
}

char *obterQuantidadeDeVisualizacoesGeraisDoBanco(char *idProduto, Usuario *usuario)// APP 4 2 @ 2 * idProduto (chama também, essa funcao)
{
    if(conexao == NULL)
    {
        printf(" Warning: Conexao nula detectada em obterQuantidadeDeVisualizacoesGeraisDoBanco() OperacoesBanco-Visualizacoes.h qkjhfd\n");
        if(!conectarBanco())
        {
            printf(" Warning: Falha ao tentar reconectar-se ao banco de dados em obterQuantidadeDeVisualizacoesGeraisDoBanco() OperacoesBanco-Visualizacoes.h\n");
            return NULL;
        }
        else
        {
            printf(" LOG: Conexao recuperada, continuando operacao em obterQuantidadeDeVisualizacoesGeraisDoBanco() OperacoesBanco-Visualizacoes.h\n");
        }
    }
    if(idProduto == NULL)
    {
        printf(" Warning: idProduto == NULL em obterQuantidadeDeVisualizacoesGeraisDoBanco() OperacoesBanco-Visualizacoes.h\n");
        return NULL;
    }
    if (produtoVencido(idProduto, usuario))
    {
        printf(" Warning: Produto vencido detectado em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesGeraisDoBanco()\n");
        return NULL;
    }

    // int tamanho = 59 + TAMANHO_ID_PRODUTO + 1;// Jamais esqueça o '\0'
    int tamanho = 70;
    char *query = (char *)malloc(sizeof(char) * tamanho);
    if(query == NULL)
    {
        printf(" Warning: falha ao alocar memoria para query em obterQuantidadeDeVisualizacoesGeraisDoBanco() OperacoesBanco-Visualizacoes.h qkjhetf\n");
        return NULL;
    }

    snprintf(query, tamanho, "SELECT P.visualizacoes FROM produto P WHERE P.idproduto=\'%s\';", idProduto);
    if(query == NULL)
    {
        printf(" Warning: falha ao formatar query em obterQuantidadeDeVisualizacoesGeraisDoBanco() OperacoesBanco-Visualizacoes.h kjheb\n");
        return NULL;
    }
    /* 
    if(mysql_query(conexao, query))
    {
        printf(" Warning: falha ao executar query em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesAnonimasDoBanco() 4654q8\n");

        free(query);
        query = NULL;
        return NULL;
    }
     */

    if(mysql_query(conexao, query))//Se ocorrer algum erro
	{
		printf(" ERRO: Ocorreram erros durante a execução da query (OperacoesBanco-Visualizacoes.h) obterQuantidadeDeVisualizacoesGeraisDoBanco()\n");
		printf(" ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
		printf(" Query executada: |%s|\n", query);
		free(query);
		query = NULL;

		if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
		{
			printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesGeraisDoBanco()\n");
			if(conectarBanco())
			{
				printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesGeraisDoBanco()\n");
			}
			else
			{
				conexao = NULL;
				mysql_close(conexao);
				mysql_thread_end();
				free(conexao);
				printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesGeraisDoBanco()\n");
			}
		}
		return NULL;
	}


    MYSQL_RES *resultado = mysql_store_result(conexao);
    
    if(resultado == NULL)
    {
        printf(" Warning: Consulta não realizada em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesGeraisDoBanco() nao houve conexao\n");
        free(query);
        query = NULL;
        return NULL;
    }
    
    if(mysql_num_fields(resultado) == 0)
    {
        printf(" Warning: Nao houve resultados na pesquisa em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesGeraisDoBanco() bnje\n");
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
        printf(" Warning: Resposta Nula em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesGeraisDoBanco()\n");
        free(query);
        mysql_free_result(resultado);
        resultado = NULL;
        query = NULL;
        return NULL;
    }
    else
    {
        if(strlen(linha[0]) > 0)
        {
            printf(" LOG: Retorno obtido com sucesso em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesGeraisDoBanco() kjh3as\n"); 
        }
        else
        {
            printf(" Warning: Produto nao possui visualizações anonimas cadastradas no banco de dados em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesGeraisDoBanco() kejhqkfge3\n");
            free(query);
            mysql_free_result(resultado);
            resultado = NULL;
            query = NULL;
            return NULL;
        }
        
        int tamanho = strlen(linha[0]) + 1;
        retorno = malloc(sizeof(char) * tamanho);
        
        if(retorno == NULL)
        {
            
            printf(" Warning: Nao foi possivel alocar memoria para retorno em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesGeraisDoBanco() sakdjhjkredf\n");
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
            
            printf(" Warning: Nao foi possivel copiar memoria para retorno em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesGeraisDoBanco() sakdjhjkredf\n");
            free(query);
            mysql_free_result(resultado);
            resultado = NULL;
            query = NULL;
            return NULL;
            
        }
        else
        {
            
            printf(" LOG: Operação concluida com sucesso em OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesGeraisDoBanco() ckjahjksd\n");
            free(query);
            mysql_free_result(resultado);
            resultado = NULL;
            query = NULL;
            return retorno;
            
        }
    }
    printf(" ERRO: erro desconhecido em obterQuantidadeDeVisualizacoesGeraisDoBanco() OperacoesBanco-Visualizacoes.h q654r888r77q65a22d\n");
    return NULL;
}
