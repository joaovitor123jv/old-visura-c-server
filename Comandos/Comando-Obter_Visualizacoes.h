#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../AdaptadorDeString/AdaptadorDeString.h"
#include "Comandos.h"
#include "../OperacoesBanco/OperacoesBanco.h"

char *obterQuantidadeDeVisualizacoesAnonimas(Usuario *usuario)// APP 4 2 @ 3 idProduto
{
    char *token = strtok(NULL, " ");// APP 4 2 @ 3 idProduto

    token = strtok(NULL, " ");// APP 4 2 @ 3 idProduto
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
    if((retorno = obterQuantidadeDeVisualizacoesAnonimasDoBanco(idProduto, usuario)) != NULL)
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

char *obterQuantidadeDeVisualizacoesGerais(Usuario *usuario)// APP 4 2 @ 2 idProduto
{
    char *token = strtok(NULL, " ");// APP 4 2 @ 2 idProduto
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
    if((qtdVisualAnom = obterQuantidadeDeVisualizacoesAnonimasDoBanco(idProduto, usuario)) != NULL)
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
    if((qtdVisualNorm = obterQuantidadeDeVisualizacoesGeraisDoBanco(idProduto, usuario)) != NULL)
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
