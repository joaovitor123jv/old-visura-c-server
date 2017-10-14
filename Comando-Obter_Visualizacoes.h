#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AdaptadorDeString.h"
#include "Comandos.h"
#include "OperacoesBanco.h"

char *obterQuantidadeDeVisualizacoesAnonimas(void)// APP 4 2 @ 3 * idProduto
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

char *obterQuantidadeDeVisualizacoesGerais(void)// APP 4 2 @ 2 * idProduto
{
    char *token = strtok(NULL, " ");// APP 4 2 @ 2 *
    if (token == NULL)
    {
        printf(" Warning: Comando insuficiente em obterQuantidadeDeVisualizacoesGerais() Comando-Obter_Visualizacoes.h hjhakjqbne \n");
        return NULL;
    }
    if (strlen(token) > TAMANHO_TIPO)
    {
        printf(" Warning: Tamanho de argumento inválido em obterQuantidadeDeVisualizacoesGerais() Comando-Obter_Visualizacoes.h 4q65e88\n");
        printf("\t\tToken = |%s|\n", token);
        return NULL;
    }
    if(strcmp(token, TIPO_ID_PRODUTO) == 0)
    {
        printf(" LOG: Solicitação de obtenção de quantidade de visualizações gerais de um produto em Coman-Obter_Visualizacoes.h obterQuantidadeDeVisualizacoesGerais()\n");
    }
    else
    {
        printf(" Warning: Comando incorreto em obterQuantidadeDeVisualizacoesGerais() Comando-Obter_Visualizacoes.h 1q65e88\n");
        return NULL;
    }

    token = strtok(NULL, " ");// APP 4 2 @ 2 * idProduto
    if(token == NULL)
    {
        printf(" LOG: Comando insuficiente em obterQuantidadeDeVisualizacoesGerais() Comando-Obter_Visualizacoes.h 65q54e88t7\n");
        return NULL;
    }

    if(strlen(token) != TAMANHO_ID_PRODUTO)
    {
        printf(" Warning: Tamanho de ID de produto informado inválido em obterQuantidadeDeVisualizacoesGerais() Comando-Obter_Visualizacoes.h qkjhe\n");
        return NULL;
    }

    char *idProduto = NULL;

    idProduto = strdup(token);//DUPLICA UMA STRING (pq nn usou isso de inicio JV ? (T-T) )
    if(idProduto == NULL)
    {
        printf(" Warning: Não foi possível duplicar a string contida em token em obterQuantidadeDeVisualizacoesGerais() Comando-Obter_Visualizacoes.h q654e87dd\n");
        return NULL;
    }

    char *qtdVisualAnom;
    if((qtdVisualAnom = obterQuantidadeDeVisualizacoesAnonimasDoBanco(idProduto)) != NULL)
    {
        printf(" LOG: Quantidade de visualizações retornada com sucesso em obterQuantidadeDeVisualizacoesGerais() Comando-Obter_Visualizacoes.h\n");
    }
    else
    {
        printf(" Warning: não foi possível obter os dados do banco em obterQuantidadeDeVisualizacoesGerais() Comando-Obter_Visualizacoes.h qalkjwf\n");
        free(idProduto);
        idProduto = NULL;
        return NULL;
    }


    char *qtdVisualNorm;
    if((qtdVisualNorm = obterQuantidadeDeVisualizacoesGeraisDoBanco(idProduto)) != NULL)
    {
        printf(" LOG: Quantidade de visualizações retornada com sucesso em obterQuantidadeDeVisualizacoesGerais() Comando-Obter_Visualizacoes.h\n");
    }
    else
    {
        printf(" Warning: não foi possível obter os dados do banco em obterQuantidadeDeVisualizacoesGerais() Comando-Obter_Visualizacoes.h qalkjwf\n");
        free(qtdVisualAnom);
        qtdVisualAnom = NULL;
        free(idProduto);
        idProduto = NULL;
        return NULL;
    }

    // long int qtd1 = strtol(qtdVisualAnom, NULL, 10);//Converte string que comeca em qtdVisualAnom na base 10 (decimal)
    // long int qtd2 = strtol(qtdVisualNorm, NULL, 10);
    // int qtd1 = atoi(qtdVisualAnom);
    // int qtd2 = atoi(qtdVisualNorm);

    // if(qtd1<0 || qtd2<0)
    // {
    //     printf(" Warning: dados inconsistentes detectados em Comando-Obter_Visualizacoes.h obterQuantidadeDeVisualizacoesGerais() qkjhdfe\n");
    //     free(idProduto);
    //     idProduto = NULL;
    //     qtdVisualAnom = NULL;
    //     qtdVisualNorm = NULL;
    //     return NULL;
    // }


    // snprintf(retorno, tamanho, "%d", atoi(qtdVisualAnom)+atoi(qtdVisualNorm));

    char *retorno = intToString(atoi(qtdVisualAnom) + atoi(qtdVisualNorm));
    free(idProduto);
    idProduto = NULL;
    free(qtdVisualAnom);
    qtdVisualAnom = NULL;
    free(qtdVisualNorm);
    qtdVisualNorm = NULL;

    if(retorno != NULL)
    {
        printf(" LOG: Quantidade total de visualizações obtido com sucesso em Comando-Obter_Visualizacoes.h obterQuantidadeDeVisualizacoesGerais()ds456\n");
        return retorno;
    }
    return NULL;
}