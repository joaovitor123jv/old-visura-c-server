/** 
 * @brief  Documento que toma conta de todas as funções responsáveis pelas Visualizações, relacionadas ao banco de dados
 * @note   Todos os comentários onde está escrito "retorna direto ao usuário", entenda "através da interface, retorna direto ao usuário"
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Comandos/Comandos.h"
#include "../Bibliotecas/Usuario.h"
#include "OperacoesBanco-FuncoesGenericas.h"
#include <mysql/mysql.h>


bool produtoVencido(char *idProduto, Usuario *usuario);

/** 
 * @brief  Checa a existência de visualização com usuário especificado (função interna)
 * @note   não libera Usuario, nem idProduto
 * @param  *idproduto: O produto que será checado se existe visualização
 * @param  *usuario: Será checado se o usuário especificado tem alguma visualização com o produto
 * @retval RETORNO_OK, caso houver visualização com aquele usuário. RETORNO_NULO, caso contrário
 */
int checaExistenciaDeVisualizacaoDeProdutoComPessoa(char *idproduto, Usuario *usuario)
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
    // int tamanho = sizeof(char) * (194 + usuario_obterTamanhoLogin(usuario));
    int tamanho = sizeof(char) *(207 + 10 + usuario_obterTamanhoLogin(usuario) + 1);
	query = (char *)malloc(tamanho);
	if(query == NULL)
	{
		printf(" ERRO: não foi possível alocar memória para a query (OperacoesBanco-Visualizacoes.h) (checaExistenciaDeVisualizacaoDeProdutoComPessoa())\n");
		return RETORNO_ERRO_FALTA_DE_MEMORIA;
	}
    snprintf(query, tamanho, "SELECT quantidade FROM visualizacaoDeUsuario V JOIN cliente C ON C.idcliente=V.cliente_idcliente JOIN produto P ON P.idproduto=V.produto_idproduto WHERE P.idproduto LIKE BINARY \'%s\' AND C.email LIKE BINARY \'%s\';", idproduto, usuario_obterLogin(usuario));
	
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
		campos = mysql_fetch_fields(resposta);
		if(campos != NULL)
		{
			if(mysql_num_fields(resposta) >= 1)
			{
                
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

/** 
 * @brief  Adiciona a quantidade de visualizações informada ao banco
 * @note   Comando que invoca esse comando: APP 2 2 idProduto quantidade
 * @note   Não libera a id, nem produto.
 * @param  *id: Id do produto que deve ser chamado
 * @param  *quantidade: A quantidade de visualizações que precisa ser adicionada
 * @param  *usuario: 
 * @retval true, caso a visualização seja adicionada, false caso contrário (não retorna direto ao usuario)
 */
bool addVisualizacoesAoBanco(char *id, char *quantidade, Usuario *usuario)// APP 2 2 idProduto quantidade
{
	char *query = NULL;
	int tamanhoDaQuery = 0;
	/*UPDATE produto SET produto.visualizacaoanom = 0 WHERE produto.idproduto = 'cocacolavc';*/
	if (usuario == NULL)
	{
		geraLog(ERRO, "Usuario nulo detectado");
		return false;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		geraLog(ERRO, "Usuario nao conectado detectado");
		return false;
	}
	if(id == NULL)
	{
		geraLog(ERRO, "idProduto == NULL");
		if( quantidade != NULL )
		{
			free( quantidade );
			quantidade = NULL;
		}
		return false;
	}
	if( !conexaoAtiva() )
	{
		geraLog(WARNING, "Falha ao conectar-se ao banco de dados" );
		return false;
	}

/* if(conexao == NULL)
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
	}*/

    if(quantidade == NULL)
    {
		geraLog(ERRO, "quantidade == NULL detectado");
		if( id != NULL )
		{
			free( id );
			id = NULL;
		}
		return false;
    }

    //TODO Checar se a quantidade informada é válida
    if( !stringTamanhoIgual(id, 10) )
	{
		geraLog(ERRO, "Quantidade inválida de caracteres para idProduto detectada");
		return false;
	}
    
    if (produtoVencido(id, usuario))
    {
		geraLog(WARNING, "Produto vencido para esse usuario");
        return false;
    }
	
	if(usuario_PermissaoAnonimo(usuario))
	{
		geraLog(LOG, "Usuario anonimo identificado");
		
        //int tamanhoDaQuery = sizeof(char) * (strlen(id) + strlen(quantidade) + 99 + 1);
        tamanhoDaQuery = sizeof(char) * (strlen(quantidade) + 112 + TAMANHO_ID_PRODUTO);
		
		query = (char *)malloc(tamanhoDaQuery);
		if(query == NULL)
		{
			//printf(" ERRO: não foi possível alocar memória para a query (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
			geraLog(ERRO, "Falha ao alocar memoria para query");
			return false;
		}
		memset(query, '\0', tamanhoDaQuery);
        
		snprintf(query, tamanhoDaQuery, "UPDATE produto SET produto.visualizacaoanom = produto.visualizacaoanom+%s WHERE produto.idproduto LIKE BINARY \'%s\';", quantidade, id );

        printf(" LOG: QUERY GERADA PRA ESSE TROÇO = |%s|\n", query);
		
		if(query == NULL)
		{
			//printf(" ERRO: não foi possível alocar memória para a query (2) (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
			geraLog(ERRO, "falha ao formatar query");
			return false;
		}
        
        //Adiciona quantidade de visualizações anonimas à TABELA de produto, em vizualizacaoanom
		if(mysql_query(conexao, query))//Se ocorrer algum erro
		{
			//printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
			geraLog(ERRO, "Ocorreram erros durante a execução da query");
			printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
			printf("\t Query enviada =  |%s|\n", query);
			free(query);
			query = NULL;
			if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
			{
				//printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
				geraLog(LOG, "Tentando reconexão com o banco de dados");
				if(conectarBanco())
				{
					//printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
					geraLog(LOG, "Reconexão efetuada com sucesso");
				}
				else
				{
					conexao = NULL;
					mysql_close(conexao);
					mysql_thread_end();
					free(conexao);
					//printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
					geraLog(ERRO, "Falha ao reconectar-se ao banco de dados");
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
                    //printf(" LOG: Informações já existem em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
					geraLog(LOG, "Visualização de usuario já existem");
                    // tamanhoDaQuery = sizeof(char) * (strlen(id) + strlen(email) + strlen(quantidade) + 198 + 1);
                    // tamanhoDaQuery = sizeof(char) * ( usuario_obterTamanhoLogin(usuario) + strlen(quantidade) + 219);
                    tamanhoDaQuery = sizeof(char) * (usuario_obterTamanhoLogin(usuario), + strlen(quantidade) + TAMANHO_ID_PRODUTO + 234 + 1);
                    
                    query = (char *)malloc(tamanhoDaQuery);
                    if(query == NULL)
                    {
						geraLog(ERRO, "Falha ao alocar memoria para query");
                        return false;
                    }
                    memset(query, '\0', tamanhoDaQuery);
                    
                    snprintf(query, tamanhoDaQuery, "UPDATE visualizacaoDeUsuario V JOIN cliente C ON C.idcliente=V.cliente_idcliente JOIN produto P ON P.idproduto LIKE BINARY V.produto_idproduto SET V.quantidade=V.quantidade+%s WHERE P.idproduto LIKE BINARY \'%s\' AND C.email LIKE BINARY \'%s\';", quantidade, id, usuario_obterLogin(usuario) );
                    
                    if(query == NULL)
                    {
						geraLog(ERRO, "Falha ao formatar query");
                        return false;
                    }
                    
//                    if(mysql_query(conexao, query))//Se ocorrer algum erro
//                    {
//                        printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
//                        printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
//                        printf("\t Query enviada =  |%s|\n", query);
//                        free(query);
//                        query = NULL;
//                        if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
//                        {
//                            printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco() 1qa5456dasd\n");
//                            if(conectarBanco())
//                            {
//                                printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco() asd456\n");
//                            }
//                            else
//                            {
//                                conexao = NULL;
//                                mysql_close(conexao);
//                                mysql_thread_end();
//                                free(conexao);
//                                printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco() as4d5asd\n");
//                            }
//                        }
//                        return false;
//                    }

					if( !executaQuery(usuario, query) )
					{
						geraLog(ERRO, "Falha ao executar query no banco de dados");
						free( query );
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
                    // tamanhoDaQuery = sizeof(char) * (strlen(id) + strlen(email) + strlen(quantidade) + 179 + 1);
                    tamanhoDaQuery = sizeof(char) *  (usuario_obterTamanhoLogin(usuario) + strlen(quantidade) + 214);
                    
                    query = (char *)malloc(tamanhoDaQuery);
                    if(query == NULL)
                    {
                        printf(" ERRO: não foi possível alocar memória para a query(5) (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
                        return false;
                    }
                    memset(query, '\0', tamanhoDaQuery);
                    
                    snprintf(query, tamanhoDaQuery, "INSERT INTO visualizacaoDeUsuario(quantidade,cliente_idcliente,produto_idproduto) SELECT %s,C.idcliente,P.idproduto FROM cliente C JOIN produto P ON P.idproduto LIKE BINARY \'%s\' WHERE C.email LIKE BINARY \'%s\';", quantidade, id, usuario_obterLogin(usuario));
                    
                    if(query == NULL)
                    {
                        printf(" ERRO: não foi possível alocar memória para a query(6) (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
                        return false;
                    }
                    printf(" LOG: Inserindo visualização anônma em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
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
                    printf(" LOG: Dados adicionados com sucesso em OperacoesBanco-Visualizacoes.h addVisualizacoesAoBanco()\n");
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
        int tamanhoDaQuery = sizeof(char) * ( strlen(quantidade) + 100);
		query = (char *)malloc(tamanhoDaQuery);
		if(query == NULL)
		{
			printf(" ERRO: não foi possível alocar memória para a query(7) (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
			return false;
		}
		memset(query, '\0', tamanhoDaQuery);
		
		snprintf(query, tamanhoDaQuery, "UPDATE produto P SET P.visualizacoes=P.visualizacoes + %s WHERE P.idproduto LIKE BINARY \'%s\';", quantidade, id );
		
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
                    tamanhoDaQuery = sizeof(char) * ( usuario_obterTamanhoLogin(usuario) + strlen(quantidade) + 243);
                    
                    query = (char *)malloc(tamanhoDaQuery);
                    if(query == NULL)
                    {
                        printf(" ERRO: não foi possível alocar memória para a query(9) (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
                        return false;
                    }
                    memset(query, '\0', tamanhoDaQuery);
                    
                    snprintf(query, tamanhoDaQuery, "UPDATE visualizacaoDeUsuario V JOIN cliente C ON C.idcliente=V.cliente_idcliente JOIN produto P ON P.idproduto=V.produto_idproduto SET V.quantidade=V.quantidade+%s WHERE P.idproduto LIKE BINARY \'%s\' AND C.email LIKE BINARY \'%s\';", quantidade, id, usuario_obterLogin(usuario) );
                    
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
                    tamanhoDaQuery = sizeof(char) *  (usuario_obterTamanhoLogin(usuario) + strlen(quantidade) + 214);
                    
                    query = (char *)malloc(tamanhoDaQuery);
                    if(query == NULL)
                    {
                        printf(" ERRO: não foi possível alocar memória para a query(5) (OperacoesBanco-Visualizacoes.h) (addVisualizacoesAoBanco())\n");
                        return false;
                    }
                    memset(query, '\0', tamanhoDaQuery);
                    
                    snprintf(query, tamanhoDaQuery, "INSERT INTO visualizacaoDeUsuario(quantidade,cliente_idcliente,produto_idproduto) SELECT %s,C.idcliente,P.idproduto FROM cliente C JOIN produto P ON P.idproduto LIKE BINARY \'%s\' WHERE C.email LIKE BINARY \'%s\';", quantidade, id, usuario_obterLogin(usuario));
                    
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
    int tamanho = 85;
    char *query = (char *)malloc(sizeof(char) * tamanho);
    if(query == NULL)
    {
        printf(" Warning: falha ao alocar memoria para query em obterQuantidadeDeVisualizacoesAnonimasDoBanco() OperacoesBanco-Visualizacoes.h qkjhetf\n");
        return NULL;
    }

    snprintf(query, tamanho, "SELECT P.visualizacaoanom FROM produto P WHERE P.idproduto LIKE BINARY \'%s\';", idProduto);
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
        return (char *)RETORNO_NOT_FOUND;
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
        retorno = (char *)malloc(sizeof(char) * tamanho);
        
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

/** 
 * @brief  obtém a quantidade de visualizações "totais" do banco de dados, de acordo com o produto especificado
 * @note   retorna direto ao usuário, libera idProduto
 * @note   pode ser acessado pelo comando (APP 4 2 @ 2 * idProduto)
 * @param  *idProduto: produto o qual será checado visualizações
 * @param  *usuario: O usuário que está solicitando a quantidade de visualizações
 * @retval retorna direto ao usuário
 */
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
    int tamanho = 82;
    char *query = (char *)malloc(sizeof(char) * tamanho);
    if(query == NULL)
    {
        printf(" Warning: falha ao alocar memoria para query em obterQuantidadeDeVisualizacoesGeraisDoBanco() OperacoesBanco-Visualizacoes.h qkjhetf\n");
        return NULL;
    }

    snprintf(query, tamanho, "SELECT P.visualizacoes FROM produto P WHERE P.idproduto LIKE BINARY \'%s\';", idProduto);
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
        return (char *)RETORNO_NOT_FOUND;
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
        retorno = (char *)malloc(sizeof(char) * tamanho);
        
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

/** 
 * @brief  Obtem a quantidade de visualizações do produto especificado, do banco de dados, e retorna diretamente ao cliente
 * @note   Retorna diretamente ao cliente, logo, libera as variáveis
 * @param  *usuario: O usuário que solicitou o acesso a esses dados (contratante, provavelmente)
 * @param  *idProduto: o id do produto em questão
 * @param  *nomeCidade: o nome da cidade que deseja saber de onde veio as visualizações
 * @param  *nomeEstado: o nome do estado, que a cidade acima mencionada se localiza
 * @retval strdup(algumaCoisa). um ponteiro alocado que deve ser liberado em algum lugar;
 */
char *obterQuantidadeDeVisualizacoesDoProdutoNaCidadeDoBanco(Usuario *usuario, char *idProduto, char *nomeCidade, char *nomeEstado)
{
    static const char *localizacao = "OperacoesBanco-Visualizacoes.h obterQuantidadeDeVisualizacoesDoProdutoNaCidadeDoBanco()";
    if (!usuarioValido(usuario, localizacao))
    {
        return RETORNO_ERRO_INTERNO_STR_DINAMICA;
    }
    if (idProduto == NULL)
    {
        geraLog(ERRO, "idProduto nulo identificado");        
        if (nomeCidade != NULL)
        {
            free(nomeCidade);
            nomeCidade = NULL;
        }
        if (nomeEstado != NULL)
        {
            free(nomeEstado);
            nomeEstado = NULL;
        }
        return RETORNO_ERRO_INTERNO_STR_DINAMICA;
    }
    if (nomeCidade == NULL)
    {
        geraLog(ERRO, "nomeCidade nulo identificado");
        if (nomeEstado != NULL)
        {
            free(nomeEstado);
            nomeEstado = NULL;
        }
        free(idProduto);
        idProduto = NULL;
        return RETORNO_ERRO_INTERNO_STR_DINAMICA;
    }
    if (nomeEstado == NULL)
    {
        geraLog(ERRO, "nomeEstado nulo identificado");
        free(idProduto);
        free(nomeCidade);
        nomeCidade = NULL;
        idProduto = NULL;
        return RETORNO_ERRO_INTERNO_STR_DINAMICA;
    }

    int tamanho = 331 + strlen(nomeCidade) + strlen(nomeEstado) + TAMANHO_ID_PRODUTO + 1;
    char *query = (char *)calloc(sizeof(char), tamanho);
    if (query == NULL)
    {
        geraLog(ERRO, "Falha ao alocar memoria para query");
        free(idProduto);
        free(nomeCidade);
        free(nomeEstado);
        nomeEstado = NULL;
        nomeCidade = NULL;
        idProduto = NULL;
        return RETORNO_ERRO_INTERNO_STR_DINAMICA;
    }

    //MAL OTIMIZADO!!!
    snprintf(query, tamanho, "SELECT count(V.quantidade) FROM visualizacaoDeUsuario V JOIN cliente C ON V.cliente_idcliente=idcliente JOIN localizacao L ON C.localizacao_idlocalizacao=L.idlocalizacao JOIN cidade M ON L.cidade_idcidade=M.idcidade JOIN estado E ON M.estado_idestado=E.idestado WHERE E.nome=\'%s\' AND C.nome=\'%s\' AND V.produto_idproduto LIKE BINARY \'%s\';", nomeEstado, nomeEstado, idProduto);

    free(idProduto);
    free(nomeCidade);
    free(nomeEstado);
    nomeEstado = NULL;
    nomeCidade = NULL;
    idProduto = NULL;

    if (query == NULL)
    {
        geraLog(ERRO, "Falha ao formatar query");
        return RETORNO_ERRO_INTERNO_STR_DINAMICA;
    }
    return retornaUnicoRetornoDaQuery(usuario, query);
}
