#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Bibliotecas/AdaptadorDeString/AdaptadorDeString.h"
#include "Comandos.h"
#include "../OperacoesBanco/OperacoesBanco.h"

char *obterQuantidadeDeVisualizacoesAnonimas(Usuario *usuario)// APP 4 2 @ 3 idProduto
{
    char *token = usuario_getNextToken(usuario);// APP 4 2 @ 3 idProduto
    if(token == NULL)
    {
        geraLog(WARNING, "Comando insuficiente detectado, esperado idProduto");
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
    char *token = usuario_getNextToken(usuario);// APP 4 2 @ 2 idProduto
    if(token == NULL)
    {
        geraLog(LOG, "Comando insuficiente detectado, esperado idProduto");
        return NULL;
    }

    if(strlen(token) != TAMANHO_ID_PRODUTO)
    {
        geraLog(WARNING, "Tamanho de ID de produto invalido informado");
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

/** 
 * @brief  Retorna a quantidade de visualizações de produto na cidade ao usuario
 * @note    Retorna direto ao usuario, função externa
 * @note2   Resultado do comando: APP 4 2 @ vC idProduto nomeCidade nomeEstado
 * @param  *usuario: Usuario o qual deseja saber a quantidade de visualizações desse produto na cidade
 * @retval ERRO:%s caso falhe, a quantidade caso tenha sucesso.
 */
char *obterQuantidadeDeVisualizacoesDoProdutoNaCidade(Usuario *usuario)
{
    static const char *localizacao = "Comando-Obter_Visualizacoes.h obterQuantidadeDeVisualizacoesDoProdutoNaCidade()";
    char *token = NULL;
    char *idProduto = NULL;
    char *nomeCidade = NULL;
    char *nomeEstado = NULL;
    if (!usuarioValido(usuario, localizacao))
    {
        return RETORNO_ERRO_INTERNO_STR_DINAMICA;
    }
    if (!usuario_ContratanteTemPermissao(usuario, PERMISSAO_OBTER_QUANTIDADE_DE_HABITANTES_NA_CIDADE))
    {
        return RETORNO_ERRO_NAO_AUTORIZADO_STR_DINAMICA;
    }

    token = usuario_getNextToken(usuario);
    if (token == NULL)
    {
        geraLog(WARNING, "Comando insuficiente detectado, token NULO");
        return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
    }
    if (!stringTamanhoIgual(token, TAMANHO_ID_PRODUTO))
    {
        geraLog(WARNING, "Entrada de tamanho incorreto detectada (esperando idProduto)");
        return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
    }
    idProduto = strdup(token);
    if (idProduto == NULL)
    {
        geraLog(ERRO, "Falha ao duplicar token");
        return RETORNO_ERRO_INTERNO_STR_DINAMICA;
    }

    token = usuario_getNextToken(usuario);
    if (token == NULL)
    {
        geraLog(WARNING, "Comando insuficiente detectado, token NULO");
        return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
    }
    if (!stringMaior(token, TAMANHO_DO_NOME_DA_CIDADE_COM_MAIOR_NOME_DO_MUNDO))
    {
        geraLog(WARNING, "Entrada de tamanho incorreto detectada (esperando nomeCidade)");
        return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
    }
    nomeCidade = strdup(token);
    if (nomeCidade == NULL)
    {
        geraLog(ERRO, "Falha ao duplicar token");
        return RETORNO_ERRO_INTERNO_STR_DINAMICA;
    }

    token = usuario_getNextToken(usuario);
    if (token == NULL)
    {
        geraLog(WARNING, "Comando insuficiente detectado, token NULO");
        return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
    }
    if (!stringMaior(token, TAMANHO_ESTADO))
    {
        geraLog(WARNING, "Entrada de tamanho incorreto detectada (esperando nomeEstado)");
        return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
    }
    nomeEstado = strdup(token);
    if (nomeEstado == NULL)
    {
        geraLog(ERRO, "Falha ao duplicar token");
        return RETORNO_ERRO_INTERNO_STR_DINAMICA;
    }


    //TODO
    return obterQuantidadeDeVisualizacoesDoProdutoNaCidadeDoBanco(usuario, idProduto, nomeCidade, nomeEstado);

    // return RETORNO_ERRO_COMANDO_NAO_CONSTRUIDO_STR_DINAMICA;
}