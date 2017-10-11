#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include "Comandos.h"
#include <mysql/mysql.h>

char *obterQuantidadeDeVisualizacoesAnonimasDoBanco(char *idProduto)// APP 4 2 @ 3 * idProduto
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