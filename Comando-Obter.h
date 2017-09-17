#include <stdio.h>
#include <string.h>
#include "Comandos.h"
#include "OperacoesBanco.h"

char *obterIdCidade();//APP 4 $
char *obterIdContratante();// APP 4 ;
char *obterTop10NovosProdutos();//APP 4 J
char *obterDescricaoProduto();// APP 4 Q
char *obterNomeProduto();

char *obterDados()// APP 4 algumaCoisa
{
	printf(" ********************** obterDados()\n");
	char* token = NULL;
	token = strtok(NULL, " ");
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Obter.h obterDados()\n");
		return NULL;
	}
	else
	{
		if(strcmp(token, TOP_10_NOVOS) == 0)// APP 4 J
		{
			char *top10 = obterTop10NovosProdutos();// Obtem os ultimos produtos adicionados
			if(top10 == NULL)
			{
				printf(" Warning: ocorreu um erro ao processar top 10 produtos novos\n");
				return NULL;
			}
			else
			{
				printf(" Log: Retorno obtido com sucesso de obterTop10NovosProdutos()\n");
				return top10;
			}
		}
		else if(strcmp(token, TIPO_DESCRICAO_PRODUTO) == 0)// APP 4 Q
		{
			printf(" LOG: Requisitando descricao de produto em Comando-Obter.h obterDados()\n");
			return obterDescricaoProduto();
		}
		else if(strcmp(token, TIPO_NOME_PRODUTO) == 0)// APP 4 1.
		{
			printf(" LOG: Requisitando nome de produto em Comando-Obter.h obterDados()\n");
			return obterNomeProduto();
		}
		printf(" Warning: Comando exageradamente grande em Comando-Obter.h obterIdCidade() rbjak\n");
		return NULL;
	}
	// INICIO REFORMA
}

char *obterIdCidade()
{
	char *token;

	token = strtok(NULL, " ");// APP 4 $ nomeCidadeInformado
	
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Obter.h obterIdCidade() dksajdjkheb\n");
		return NULL;
	}

	int tamanhoCidade = strlen(token);
	if(tamanhoCidade > TAMANHO_DO_NOME_DA_CIDADE_COM_MAIOR_NOME_DO_MUNDO)
	{
		printf(" Warning: Comando exageradamente grande em Comando-Obter.h obterIdCidade() ebk1ud8\n");
		return NULL;
	}

	// FIM REFORMA

	char *nomeCidade = NULL;

	nomeCidade = malloc(sizeof(char) * (tamanhoCidade + 1));

	if(nomeCidade == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para Cidade em Comando-Obter.h obterIdCidade()\n");
		return NULL;
	}

	strcpy(nomeCidade, token);

	if(nomeCidade== NULL)
	{
		printf(" Warning: Falha ao copiar de token para nomeCidade em Comando-Obter.h obterIdCidade aksjd()\n");
	}
	else// Usar como anotacao pra desalocar memoria depos
	{
		token = strtok(NULL, " ");// APP 4 $ { nomeCidadeInformado nomeEstadoInformado

		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente em Comando-Obter.h obterIdCidade() 1gha5\n");
			free(nomeCidade);
			nomeCidade = NULL;
			return NULL;
		}

		int tamanhoEstado = strlen(token);
		if(tamanhoEstado != TAMANHO_ESTADO)
		{
			printf(" Warning: Tamanho de Comando incorreto em Comando-Obter.h obterIdCidade() n4yw5\n");
			free(nomeCidade);
			nomeCidade = NULL;
			return NULL;
		}

		char *nomeEstado = malloc(sizeof(char) * (tamanhoEstado + 1));

		if(nomeEstado == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para nome de estado em Comando-Obter.h obterIdCidade() vhqjek\n");
			free(nomeCidade);
			nomeCidade = NULL;
			return NULL;
		}

		strcpy(nomeEstado, token);

		if(nomeEstado == NULL)
		{
			printf(" Warning: Falha ao alocar copiar de token para nomeEstado em Comando-Obter.h obterIdCidade() eqkjh3\n");
			free(nomeCidade);
			nomeCidade = NULL;
			return NULL;
		}
		else// Necessario liberar nome de cidade e nome de estado
		{
			char *retorno = obterIdCidadeDoBanco(nomeCidade, nomeEstado);
			free(nomeCidade);
			free(nomeEstado);
			nomeCidade = NULL;
			nomeEstado = NULL;
			return retorno;// Retorna NOT FOUND ou idCidade 
		}
	}

	printf(" ERRO: Deu Ruim em Comando-Obter.h obterIdCidade()jhdsagjhdsaghjjhgjhgq\n");
	return NULL;
}



char *obterIdContratante()// APP 4 ;
{

	//TODO
	char *token = strtok(NULL, " ");// APP 4 ; emailInformado

	token = malloc(sizeof(char) * (strlen(token) + 1));
	if(token == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para token em Comando-Obter.h obterIdContratante()\n");
		return NULL;
	}


	printf(" ERRO: Comando executado de forma incorreta em Comando-Obter.h obterIdContratante()\n");
	return NULL;
}


// Só existe pra manter o controle
char *obterTop10NovosProdutos()// APP 4 J
{
	/*
	 * PADRÃO DE RETORNO: Retorna os id's de produtos separados por um espaco
	 *
	 * Linha 1 ->  "idProduto1 idproduto2 idProduto3 ...\0"
	 *
	 * Em caso de erro -> NULL
	 */
	char *top10 = obterTop10NovosProdutosDoBanco();
	if(top10 == NULL)
	{
		printf(" Warning: top 10 produtos não obteve resultado em Comando-Obter.h obterIdContratante()\n");
		return NULL;
	}
	else
	{
		printf(" LOG: Retorno obtido com sucesso de obterTop10NovosProdutos em Comando-Obter.h obterIdContratante()\n");
		return top10;
	}

	return NULL;
}


char *obterDescricaoProduto()// APP 4 Q
{
	/*
	*	Retorna descricao do produto caso sucesso, ou "ERRO" caso algo dê errado
	*/
	char *token;
	token = strtok(NULL, " ");// APP 4 Q *
	if(token == NULL)
	{
		printf(" Warning: Comando muito pequeno em Comando-Obter.h obterDescricaoProduto() eqde\n");
		return NULL;
	}
	else if(strlen(token) > TAMANHO_TIPO)
	{
		printf(" Warning: comando exageradamente grande detectado em Comando-Obter.h obterDescricaoProduto() eqjkh\n");
		return NULL;
	}
	else if(strcmp(token, TIPO_ID_PRODUTO) == 0)
	{
		printf(" LOG: Cliente informou ID do produto em Comando-Obter.h obterDescricaoProduto() bqwt\n");

		token = strtok(NULL, " ");// APP 4 Q * idProdutoInformado
		if(token == NULL)
		{
			printf(" Warning: comando insuficiente em Comando-Obter.h obterDescricaoProduto() 4f51q\n");
			return NULL;
		}
		else if(strlen(token) != TAMANHO_ID_PRODUTO)
		{
			printf(" Warning: Comando exageradamente grande em Comando-Obter.h obterDescricaoProduto()  44h5w11c\n");
			return NULL;
		}
		else
		{
			char *idProduto = NULL;
			idProduto = malloc(sizeof(char) * (TAMANHO_ID_PRODUTO + 1));
			printf(" LOG: Memoria para produto alocada em Comando-Obter.h obterDescricaoProduto() eb19cc7\n");

			if(idProduto == NULL)
			{
				printf(" Warning: Falha ao alocar memoria para idProduto em Comando-Obter.h obterIdCidade() hjk4ba72\n");
				return NULL;
			}
			else
			{
				strcpy(idProduto, token);
				if(idProduto == NULL)
				{
					printf(" Warning: Falha ao copiar de token para idProduto em Comando-Obter.h obterIdCidade() eqkuhjgbvt120\n");
					return NULL;
				}
				return obterDescricaoProdutoDoBanco(idProduto);
			}

			printf(" ERRO: Desconhecido 45652022200018448974113 em Comando-Obter.h obterDescricaoProduto()\n");
			return NULL;
		}
		printf(" ERRO: Desconhecido rb1625f0j66 em Comando-Obter.h obterDescricaoProduto()\n");
		return NULL;// TEMP
	}
	else// Se der PAU
	{
		printf(" ERRO: Comando incorreto em Comando-Obter.h obterDescricaoProduto()\n");
		return NULL;
	}
}



char *obterNomeProduto()
{
	char *token  = NULL;
	printf(" LOG: Iniciando interpretação em obterNomeProduto() Comando-Obter.h\n");

	token = strtok(NULL, " ");// APP 4 1. idProdutoInformado

	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em obterNomeProduto() Comando-Obter.h akjbbe\n");
		return NULL;
	}
	if(strlen(token) != TAMANHO_ID_PRODUTO)
	{
		printf(" Warning: Comando incorreto em obterNomeProduto() Comando-Obter.h ohrbbzzk\n");
		return NULL;
	}

	char *idProduto = NULL;
	idProduto = malloc(sizeof(char) * (TAMANHO_ID_PRODUTO + 1));

	if(idProduto == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para idProduto em obterNomeProduto() Comando-Obter.h n4oi2d\n");
		return NULL;
	}

	strcpy(idProduto, token);

	if(idProduto == NULL)
	{
		printf(" Warning: Ocorreu um erro ao tentar copiar string de token para idProduto em obterNomeProduto() Comando-Obter.h\n");
		return NULL;
	}

	char *nomeProduto = NULL;
	//TODO obterNomeProdutoDoBanco(idProduto)
	nomeProduto = obterNomeProdutoDoBanco(idProduto);
	free(idProduto);
	return nomeProduto;
}

