#include <stdio.h>
#include <string.h>
#include "Comandos.h"
#include "../OperacoesBanco/OperacoesBanco.h"

#include "Comando-Obter_Visualizacoes.h"


char *obterIdCidade(void);//APP 4 $
char *obterIdContratante(void);// APP 4 ;
char *obterIdLocalizacao(void);// APP 4 i0
char *obterTop10NovosProdutos();//APP 4 J
char *obterTop10ProdutosMelhorAvaliados();//Retorna os 10 produtos melhor avaliados do banco de dados, ou NULL quando dá erro
char *obterDescricaoProduto(char *email);// APP 4 Q idProduto
char *obterNomeProduto(char *email);// APP 4 1. idProduto
char *obterAvaliacaoProduto(char *email);// APP 4 kW idProduto                       (Retorna NULL quando ocorre algum erro)

char *obterDados(char *email)// APP 4 algumaCoisa
{
	printf(" ********************** obterDados()\n");
	if(email == NULL)
	{
		printf(" Warning: email == NULL em Comando-Obter.h obterDados() a465e\n");
	}
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
				printf(" Warning: ocorreu um erro ao processar top 10 produtos novos em Comando-Obter.h obterDados()\n");
				return NULL;
			}
			else
			{
				printf(" Log: Retorno obtido com sucesso de obterTop10NovosProdutos() em Comando-Obter.h obterDados()\n");
				return top10;
			}
		}
		else if(strcmp(token, TOP_10_MELHORES) == 0)
		{
			char *top = obterTop10ProdutosMelhorAvaliados();
			if(top == NULL)
			{
				printf(" Warning: ocorreu um erro ao processar top 10 melhores produtos em Comando-Obter. obterDados()\n");
				return NULL;
			}
			else
			{
				printf(" LOG: Retorno obtido com sucesso de obterTop10ProdutosMelhorAvaliados() em Comando-Obter.h obterDados()\n");
				return top;
			}
		}
		else if(strcmp(token, TIPO_DESCRICAO_PRODUTO) == 0)// APP 4 Q idProduto
		{
			printf(" LOG: Requisitando descricao de produto em Comando-Obter.h obterDados()\n");
			return obterDescricaoProduto(email);
		}
		else if(strcmp(token, TIPO_NOME_PRODUTO) == 0)// APP 4 1. idProduto
		{
			printf(" LOG: Requisitando nome de produto em Comando-Obter.h obterDados()\n");
			return obterNomeProduto(email);
		}
		else if(strcmp(token, TIPO_AVALIACAO) == 0)// APP 4 kW * idProduto
		{
			char *retorno;
			if((retorno = obterAvaliacaoProduto(email)) != NULL)
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
					// printf(" Warning: Comando incompleto em Comando-Obter.h obterDados()\n");
					return obterQuantidadeDeVisualizacoesGerais(email);
				}
				else if(strcmp(token, TIPO_VISUALIZACAO_ANONIMA_CHAR) == 0)// APP 4 2 @ 3
				{
					// DESEJA obter a QUANTIDADE de VISUALIZACOES ANONIMAS
					printf(" LOG: Solicitando Obtenção de quantidade de visualizações anonimas em Comando-Obter.h obterDados()\n");
					return obterQuantidadeDeVisualizacoesAnonimas(email);
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
		else if(strcmp(token, TIPO_ID_CIDADE) == 0)// APP 4 $
		{
			return obterIdCidade();
		}
		else if( strcmp(token, TIPO_ID_CONTRATANTE) == 0 )
		{
			return obterIdContratante();
		}
		else if( strcmp(token, TIPO_ID_LOCALIZACAO) == 0 )
		{
			return obterIdLocalizacao();
		}
		printf(" Warning: Comando inexistente: |%s| em Comando-Obter.h obterDados() rbjak\n", token);
		return NULL;
	}
	printf(" ERRO: deu uma merda muito grande aqui em Comando-Obter.h obterDados() asdejkjhjvkerwbhsajk\n");
	return NULL;
}

char *obterIdCidade()
{
	char *token;
	char *nomeCidade = NULL;
	int tamanhoCidade;
	char *nomeEstado;
	int tamanhoEstado;

	token = strtok(NULL, " ");// APP 4 $ nomeCidadeInformado
	
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Obter.h obterIdCidade() dksajdjkheb\n");
		return NULL;
	}

	tamanhoCidade = strlen(token);
	if(tamanhoCidade > TAMANHO_DO_NOME_DA_CIDADE_COM_MAIOR_NOME_DO_MUNDO)
	{
		printf(" Warning: Comando exageradamente grande em Comando-Obter.h obterIdCidade() ebk1ud8\n");
		return NULL;
	}
	
	nomeCidade = strdup(token);

	if(nomeCidade == NULL)
	{
		printf(" Warning: Falha ao copiar de token para nomeCidade em Comando-Obter.h obterIdCidade aksjd()\n");
		return NULL;
	}
	
	token = strtok(NULL, " ");// APP 4 $ nomeCidadeInformado nomeEstadoInformado

	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Obter.h obterIdCidade() 1gha5\n");
		free(nomeCidade);
		nomeCidade = NULL;
		return NULL;
	}

	tamanhoEstado = strlen(token);
	if(tamanhoEstado != TAMANHO_ESTADO)
	{
		printf(" Warning: Tamanho de Comando incorreto em Comando-Obter.h obterIdCidade() n4yw5\n");
		free(nomeCidade);
		nomeCidade = NULL;
		return NULL;
	}

	nomeEstado = strdup(token);

	if(nomeEstado == NULL)
	{
		printf(" Warning: Falha ao copiar de token para nomeEstado em Comando-Obter.h obterIdCidade() eqkjh3\n");
		free(nomeCidade);
		nomeCidade = NULL;
		return NULL;
	}
	else
	{
		char *retorno = obterIdCidadeDoBanco(nomeCidade, nomeEstado);
		free(nomeCidade);
		free(nomeEstado);
		nomeCidade = NULL;
		nomeEstado = NULL;
		return retorno;// Retorna NOT FOUND ou idCidade 
	}

	printf(" ERRO: Deu Ruim em Comando-Obter.h obterIdCidade()jhdsagjhdsaghjjhgjhgq\n");
	return NULL;
}


char *obterIdContratante()// APP 4 ; cnpj
{
	char *token = strtok(NULL, " "); // cnpj

	if( token == NULL )
	{
		printf(" Warning: Comando insuficiente em Comando-Obter.h obterIdContratante() asjhvbruw0f7878s756f6d\n");
		return NULL;
	}
	if( strlen(token) > TAMANHO_CNPJ )
	{
		printf(" Warning: Comando exageradamente grande em Comando-Obter.h obterIdContratante() abq78vas98wbjhax\n");
		return NULL;
	}
	char *cnpj = strdup(token);
	if( cnpj == NULL )
	{
		printf(" Warning: Falha ao duplicar email informado em Comando-Obter.h obterIdContratante() abv849a8sd72bhjht65w\n");
		return NULL;
	}
	return obterIdContratanteDoBanco(cnpj);
}

char *obterIdLocalizacao(void)// APP 4 i0 nomeCidade nomeEstado	//TODO FAZENDO
{
	char *token = strtok(NULL, " ");
	if( token == NULL )
	{
		printf(" Warning: Comando insuficiente em Comando-Obter.h obterIdLocalizacao() qiuwbv89r4sf\n");
		return NULL;
	}
	if( strlen(token) > TAMANHO_DO_NOME_DA_CIDADE_COM_MAIOR_NOME_DO_MUNDO )
	{
		printf(" Warning: Comando exageradamente grande em Comando-Obter.h obterIdLocalizacao() asbvreiaoisdij58\n");
		return NULL;
	}

	char *nomeCidade = strdup(token);
	if( nomeCidade == NULL )
	{
		printf(" Warning: Falha ao duplicar nomeCidade em Comando-Obter.h obterIdLocalizacao() askjvbruos8d\n");
		return NULL;
	}

	strtok(NULL, " ");
	
	if( token == NULL )
	{
		printf(" Warning: Comando insuficiente em Comando-Obter.h obterIdLocalizacao() asdfbuvieuhw74\n");
		free( nomeCidade );
		nomeCidade = NULL;
		return NULL;
	}
	if( strlen(token) > TAMANHO_ESTADO )
	{
		printf(" Warning: Comando exageradamente grande em Comando-Obter.h obterIdLocalizacao() 590cvy73dsvkve\n");
		free( nomeCidade );
		nomeCidade = NULL;
		return NULL;
	}

	char *estado = strdup(token);
	if( estado == NULL )
	{
		printf(" Warning: Falha ao duplicar nomeCidade em Comando-Obter.h obterIdLocalizacao() askjdhvbru\n");
		free( nomeCidade );
		nomeCidade = NULL;
		return NULL;
	}
	//TODO

	return obterIdLocalizacaoDoBanco(nomeCidade, estado);
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
		printf(" Warning: top 10 produtos não obteve resultado em Comando-Obter.h obterTop10NovosProdutos()\n");
		return NULL;
	}
	else
	{
		printf(" LOG: Retorno obtido com sucesso de obterTop10NovosProdutos em Comando-Obter.h obterTop10NovosProdutos()\n");
		return top10;
	}

	return NULL;
}


char *obterDescricaoProduto(char *email)// APP 4 Q
{
	/*
	*	Retorna descricao do produto caso sucesso, ou "ERRO" caso algo dê errado
	*/
	char *token;

	token = strtok(NULL, " ");// APP 4 Q idProdutoInformado
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
		idProduto = strdup(token);

		if(idProduto == NULL)
		{
			printf(" Warning: Falha ao duplicar idProduto em Comando-Obter.h obterIdCidade() hjk4ba72\n");
			return NULL;
		}
		char *retorno = obterDescricaoProdutoDoBanco(idProduto, email);
		free(idProduto);
		idProduto = NULL;
		return retorno;
	}
	printf(" ERRO: Desconhecido rb1625f0j66 em Comando-Obter.h obterDescricaoProduto()\n");
	return NULL;// TEMP
}


char *obterNomeProduto(char *email)
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

	char *idProduto = strdup(token);
	if (idProduto == NULL)
	{
		printf(" Warning: Falha ao duplicar produto em Comando-Obter.h obterNomeProduto() aaasd156 \n");
		return NULL;
	}

	char *nomeProduto = NULL;
	//	obterNomeProdutoDoBanco(idProduto);
	nomeProduto = obterNomeProdutoDoBanco(idProduto, email);
	free(idProduto);
	return nomeProduto;
}


/* Obtema avaliação do produto e retorna da seguinte forma:  "numeroDeAvaliacoesPositivas NumeroDeAvaliacoesNegativas" */
char *obterAvaliacaoProduto(char *email)// APP 4 kW idProduto                       (Retorna NULL quando ocorre algum erro)
{
	char *token = strtok(NULL, " ");// APP 4 kW idProduto
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
	if((retorno = obterAvaliacaoProdutoDoBanco(idProduto, email)) != NULL)
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

char *obterTop10ProdutosMelhorAvaliados()
{
	/*
	 * PADRÃO DE RETORNO: Retorna os id's de produtos separados por um espaco
	 *
	 * Linha 1 ->  "idProduto1 idproduto2 idProduto3 ...\0"
	 *
	 * Em caso de erro -> NULL
	 */
	char *top = obterTop10ProdutosMelhorAvaliadosDoBanco();
	if(top == NULL)
	{
		printf(" Warning: top 10 produtos não obteve resultado em Comando-Obter.h obterTop10ProdutosMelhorAvaliados()\n");
		return NULL;
	}
	else
	{
		printf(" LOG: Retorno obtido com sucesso de obterTop10NovosProdutos em Comando-Obter.h obterTop10ProdutosMelhorAvaliados()\n");
		return top;
	}

	return NULL;
}
