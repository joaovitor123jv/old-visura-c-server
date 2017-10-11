#include <stdio.h>
#include <string.h>
#include "Comandos.h"
#include "OperacoesBanco.h"

char *obterQuantidadeDeVisualizacoesAnonimas()// APP 4 2 @ 3
{
    // Fazer isso aqui !!!
    char *token = strtok(NULL, " ");// APP 4 2 @ 3 *
    if (token == NULL)
    {
        printf(" Warning: Comando insuficiente em obterQuantidadeDeVisualizacoesAnonimas() Comando-Obter_Visualizacoes.h hjhakjqbne \n");
        return NULL;
    }
    if (strlen(token) > TAMANHO_TIPO)
    {
        printf(" Warning: Tamanho de argumento inválido em obterQuantidadeDeVisualizacoesAnonimas Comando-Obter_Visualizacoes.h 4q65e88\n");
        printf("\t\tToken = |%s|\n", token);
        return NULL;
    }
    if(strcmp(token, TIPO_ID_PRODUTO) == 0)
    {
        printf(" LOG: Solicitação de obtenção de quantidade de visualizações anonimas de um produto\n");
    }
    else
    {
        printf(" Warning: Comando incorreto em obterQuantidadeDeVisualizacoesAnonimas() Comando-Obter_Visualizacoes.h 1q65e88\n");
        return NULL;
    }

    token = strtok(NULL, " ");// APP 4 2 @ 3 * idProduto
    if(token == NULL)
    {
        printf(" LOG: Comando insuficiente em obterQuantidadeDeVisualizacoesAnonimas() Comando-Obter_Visualizacoes.h 65q54e88t7\n");
        return NULL;
    }

    if(strlen(token) != TAMANHO_ID_PRODUTO)
    {
        printf(" Warning: Tamanho de ID de produto informado inválido em obterQuantidadeDeVisualizacoesAnonimas() Comando-Obter_Visualizacoes.h qkjhe\n");
        return NULL;
    }

    char *idProduto = NULL;

    idProduto = strdup(token);//DUPLICA UMA STRING (pq nn usou isso de inicio JV ? (T-T) )
    if(idProduto == NULL)
    {
        printf(" Warning: Não foi possível duplicar a string contida em token em obterQuantidadeDeVisualizacoesAnonimas() Comando-Obter_Visualizacoes.h q654e87dd\n");
        return NULL;
    }

    char *retorno;
    if((retorno = obterQuantidadeDeVisualizacoesAnonimasDoBanco(idProduto)) != NULL)
    {
        printf(" LOG: Quantidade de visualizações retornada com sucesso em obterQuantidadeDeVisualizacoesAnonimas() Comando-Obter_Visualizacoes.h\n");
        free(idProduto);
        idProduto = NULL;
        return retorno;
    }
    else
    {
        printf(" Warning: não foi possível obter os dados do banco em obterQuantidadeDeVisualizacoesAnonimas() Comando-Obter_Visualizacoes.h qalkjwf\n");
        free(idProduto);
        idProduto = NULL;
        return NULL;
    }


    printf(" ERRO: Deu algum erro que não previ em obterQuantidadeDeVisualizacoesAnonimas() Comando-Obter_Visualizacoes.h qkjhejkvjhea\n");
    printf(" \tLiberando memorias talvez utilizadas\n");
    if(idProduto != NULL)
    {
        free(idProduto);
        idProduto = NULL;
    }
    return NULL;
}