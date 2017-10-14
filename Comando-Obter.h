#include <stdio.h>
#include <string.h>
#include "Comandos.h"
#include "OperacoesBanco.h"

#include "Comando-Obter_Visualizacoes.h"

char *obterIdCidade(void);//APP 4 $
char *obterIdContratante(void);// APP 4 ;
char *obterTop10NovosProdutos(void);//APP 4 J
char *obterDescricaoProduto(void);// APP 4 Q * idProduto
char *obterNomeProduto(void);// APP 4 1. idProduto
char *obterAvaliacaoProduto(void);// APP 4 kW * idProduto                       (Retorna NULL quando ocorre algum erro)

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
		else if(strcmp(token, TIPO_AVALIACAO) == 0)// APP 4 kW * idProduto
		{
			char *retorno;
			if((retorno = obterAvaliacaoProduto()) != NULL)
			{
				printf(" LOG: Obtenção de produto realizada com sucesso em Comando-Obter.h obterDados() 1q654f8tgsd\n");
				return retorno;
			}
			else
			{
				printf(" Warning: Ocorreu um erro durante a realização do comando de obtenção em Comando-Obter.h obterDados() asdkjhjbtffs\n");
				return NULL;
			}
		}
		else if(strcmp(token, TIPO_VISUALIZACAO) == 0)// APP 4 2
		{
			// IMPORTANTE: Leia isso como "Desejo obter ALGO relacionado a visualizações"
			// 		A pergunta que surge com isso: "O que ?"

			token = strtok(NULL, " ");
			if(token == NULL)
			{
				printf(" Warning: Comando insuficiente em Comando-Obter.h obterDados() 45a654q8e\n");
				return NULL;
			}
			if (strcmp(token, TIPO_QUANTIDADE) == 0)// APP 4 2 @
			{
				// DESEJA obter a QUANTIDADE de algo relacionado a visualizações
				token = strtok(NULL, " ");
				if(token == NULL)
				{
					printf(" Warning: Comando insuficiente em Comando-Obter.h obterDados() 5q68r87s\n");
					return NULL;
				}
				if (strcmp(token, TIPO_VISUALIZACAO) == 0)// APP 4 2 @ 2
				{
					// DESEJA obter a QUANTIDADE de VISUALIZACOES (NÃO ANONIMAS)
					printf(" LOG: Solicitando Obtenção de quantidade de visualizações de usuario não anonimos em Comando-Obter.h obterDados()\n");
					printf(" Warning: Comando incompleto em Comando-Obter.h obterDados()\n");
					return NULL;
				}
				else if(strcmp(token, TIPO_VISUALIZACAO_ANONIMA_CHAR) == 0)// APP 4 2 @ 3
				{
					// DESEJA obter a QUANTIDADE de VISUALIZACOES ANONIMAS
					printf(" LOG: Solicitando Obtenção de quantidade de visualizações anonimas em Comando-Obter.h obterDados()\n");
					return obterQuantidadeDeVisualizacoesAnonimas();
				}
				else if(strcmp(token, TIPO_ESPECIFICO) == 0)// APP 4 2 @ =
				{
					//DESEJA obter QUANTIDADE de VISUALIZACOES de ALGO ESPECIFICO
					printf(" LOG: Solicitando Obtenção de quantidade de visualizações de algo específico em Comando-Obter.h obterDados()\n");
					printf(" Warning: Comando incompleto em Comando-Obter.h obterDados()\n");
					return NULL;
				}
			}
			else
			{
				printf(" Warning: Comando não compreendido (%s) em Comando-Obter.h obterDados() aq468e7ca\n", token);
				return false;
			}
		}
		printf(" Warning: Comando exageradamente grande: |%s| em Comando-Obter.h obterIdCidade() rbjak\n", token);
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
	//	obterNomeProdutoDoBanco(idProduto);
	nomeProduto = obterNomeProdutoDoBanco(idProduto);
	free(idProduto);
	return nomeProduto;
}


/* Obtema avaliação do produto e retorna da seguinte forma:  "numeroDeAvaliacoesPositivas NumeroDeAvaliacoesNegativas" */
char *obterAvaliacaoProduto()// APP 4 kW * idProduto                       (Retorna NULL quando ocorre algum erro)
{
	char *token = strtok(NULL, " ");// APP 4 kW *
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente com Comando-Obter.h obterAvaliacaoProduto() 41q5v8r\n");
		return NULL;
	}
	// if(token[0] != '*' || token[1] != '\0')// Comparação otimizada
	// {
	// 	printf(" Warning: Comando incorreto detectado em Comando-Obter.h obterAvaliacaoProduto() qhjkves\n");
	// 	return NULL;
	// }
	if(strlen(token) > TAMANHO_TIPO)
	{
		printf(" Warning: Comando exageradamente grande em Comando-Obter.h obterAvaliacaoProduto() q1568dsaed\n");
		return NULL;
	}
	if (strcmp(token, TIPO_ID_PRODUTO) != 0)
	{
		printf(" Warning: Comando incorreto detectado em Comando-Obter.h obterAvaliacaoProduto() qhjkves\n");
		return NULL;
	}

	token = strtok(NULL, " ");// APP 4 kW * idProduto
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Obter.h obterAvaliacaoProduto() sadkjhjvrsad\n");
		return NULL;
	}
	if(strlen(token) != TAMANHO_ID_PRODUTO)
	{
		printf(" Warning: Comando incorreto detectado em Comando-Obter.h obterAvaliacaoProduto() asdhjkvrsdf\n");
		return NULL;
	}

	char *idProduto = strdup(token);
	if(idProduto == NULL)
	{
		printf(" Warning: Não foi possível duplicar token em Comando-Obter.h obterAvaliacaoProduto() qhjjvrsad\n");
		return NULL;
	}

	char *retorno;
	if((retorno = obterAvaliacaoProdutoDoBanco(idProduto)) != NULL)
	{
		printf(" LOG: Produto obtido do banco de dados com sucesso em Comando-Obter.h obterAvaliacaoProduto() chjksdf\n");
		free(idProduto);
		idProduto = NULL;
		return retorno;
	}
	else
	{
		printf(" Warning: Não foi possível obter obter avaliacao do produto do banco em Comando-Obter.h obterAvaliacaoProduto() qw65b41df\n");
		free(idProduto);
		idProduto = NULL;
		return NULL;
	}
	printf(" ERRO: Erro desconhecido em Comando-Obter.h obterAvaliacaoProduto() askjhgvrdf\n");
	return NULL;
}
