#include <stdio.h>
#include <string.h>
#include "Comandos.h"
#include "../OperacoesBanco/OperacoesBanco.h"

#include "Comando-Obter_Visualizacoes.h"


char *obterIdCidade(Usuario *usuario);//APP 4 $
char *obterIdContratante(Usuario *usuario);// APP 4 ;
char *obterIdLocalizacao(Usuario *usuario);// APP 4 i0

char *obterTop10NovosProdutos(Usuario *usuario);//APP 4 J
//Essa função reencaminha pra APP 4 J (1, 2 e 3)

char *obterTop10ProdutosMelhorAvaliados(Usuario *usuario);//Retorna os 10 produtos melhor avaliados do banco de dados, ou NULL quando dá erro

char *obterDescricaoProduto(Usuario *usuario);// APP 4 Q idProduto
char *obterNomeProduto(Usuario *usuario);// APP 4 1. idProduto
char *obterAvaliacaoProduto(Usuario *usuario);// APP 4 kW idProduto                       (Retorna NULL quando ocorre algum erro)
char *obterInformacoesProduto(Usuario *usuario);// APP 4 iP idProduto			//Retorna informações do produto separadas por espaço, de acordo com o nível de permissão de usuario

char *obter10ProdutosDaEmpresa(Usuario *usuario);

char *obterInformacaoPaginada(Usuario *usuario);

char *obterInformacoesUsuario(Usuario *usuario);

char *obterIdDeEmpresaDadoProduto(Usuario *usuario);

char *obterListaDesejos(Usuario *usuario);

char *obterQuantidadeDeHabitantesDaCidade(Usuario *usuario);

char *comandoObter(Usuario *usuario)// APP 4 algumaCoisa
{
	printf(" ********************** obterDados()\n");
	if (usuario == NULL)
	{
		printf(" Warning: Usuario nulo detectado em Comando-Obter.h comandoObter()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" Warning: usuario nao conectado detectado em Comando-Obter.h comandoObter()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	char* token = NULL;

	token = usuario_getNextToken(usuario);
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Obter.h comandoObter()\n");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	else if (stringMaior(token, TAMANHO_TIPO))
	{
		printf(" Warning: Comando exageradamente grande detectado em Comando-Obter.h comandoObter()\n");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}


	if(strcmp(token, TOP_10_NOVOS) == 0)// APP 4 J
	{
		return obterTop10NovosProdutos(usuario);
	}// --TOP_10_NOVOS
	else if(strcmp(token, TOP_10_MELHORES) == 0)
	{
		return obterTop10ProdutosMelhorAvaliados(usuario);//APP 4 9& 
	}
	else if(strcmp(token, TIPO_INFORMACOES_PRODUTO) == 0)
	{
		printf(" LOG: Requisitando informacoes de produto em Comando-Obter.h comandoObter()\n");
		return obterInformacoesProduto(usuario);
	}
	else if(strcmp(token, TIPO_DESCRICAO_PRODUTO) == 0)// APP 4 Q idProduto
	{
		printf(" LOG: Requisitando descricao de produto em Comando-Obter.h comandoObter()\n");
		return obterDescricaoProduto(usuario);
	}
	else if(strcmp(token, TIPO_INFORMACOES_MINHAS) == 0)// APP 4 iM
	{
		return obterInformacoesUsuario(usuario);
	}
	else if( strcmp(token, TIPO_INFORMACAO_PAGINADA) == 0 )// APP 4 ##
	{
		return obterInformacaoPaginada(usuario);
	}
	else if( strcmp(token, TIPO_ID_DE_EMPRESA_POR_PRODUTO) == 0 )// APP 4 EP idProduto
	{
		printf(" LOG: Requisitando ID de empresa, dado id de produto em Comando-Obter.h comandoObter() kjhgt\n");
		return obterIdDeEmpresaDadoProduto(usuario);
	}
	else if(strcmp(token, TIPO_ID_CIDADE) == 0)// APP 4 $
	{
		return obterIdCidade(usuario);
	}
	else if(strcmp(token, TIPO_QUANTIDADE_DE_HABITANTES_DA_CIDADE) == 0)// APP 4 qC nomeCidade nomeEstado
	{
		return obterQuantidadeDeHabitantesDaCidade(usuario);
	}
	else if( strcmp(token, TIPO_ID_CONTRATANTE) == 0 )
	{
		return obterIdContratante(usuario);
	}
	else if( strcmp(token, TIPO_ID_LOCALIZACAO) == 0 )
	{
		return obterIdLocalizacao(usuario);
	}
	else if(strcmp(token, TIPO_NOME_PRODUTO) == 0)// APP 4 1. idProduto
	{
		printf(" LOG: Requisitando nome de produto em Comando-Obter.h comandoObter()\n");
		return obterNomeProduto(usuario);
	}
	else if(strcmp(token, TIPO_AVALIACAO) == 0)// APP 4 kW * idProduto
	{
		char *retorno;
		if((retorno = obterAvaliacaoProduto(usuario)) != NULL)//TODO MECHER ATUALIZAR
		{
			printf(" LOG: Obtenção de produto realizada com sucesso em Comando-Obter.h comandoObter() 1q654f8tgsd\n");
			return retorno;
		}
		else
		{
			printf(" Warning: Ocorreu um erro durante a realização do comando de obtenção em Comando-Obter.h comandoObter() asdkjhjbtffs\n");
			return RETORNO_ERRO_INTERNO_STR_DINAMICA;
		}
	}
	else if(strcmp(token, TIPO_VISUALIZACAO) == 0)// APP 4 2
	{
		// IMPORTANTE: Leia isso como "Desejo obter ALGO relacionado a visualizações"
		// 		A pergunta que surge com isso: "O que ?"

		token = usuario_getNextToken(usuario);
		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente em Comando-Obter.h comandoObter() 45a654q8e\n");
			return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
		}
		if (strcmp(token, TIPO_QUANTIDADE) == 0)// APP 4 2 @
		{
			// DESEJA obter a QUANTIDADE de algo relacionado a visualizações
			token = usuario_getNextToken(usuario);
			if(token == NULL)
			{
				printf(" Warning: Comando insuficiente em Comando-Obter.h comandoObter() 5q68r87s\n");
				return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
			}
			if (strcmp(token, TIPO_VISUALIZACAO) == 0)// APP 4 2 @ 2
			{
				// DESEJA obter a QUANTIDADE de VISUALIZACOES (NÃO ANONIMAS)
				printf(" LOG: Solicitando Obtenção de quantidade de visualizações de usuario não anonimos em Comando-Obter.h comandoObter()\n");
				// printf(" Warning: Comando incompleto em Comando-Obter.h obterDados()\n");
				return obterQuantidadeDeVisualizacoesGerais(usuario);
			}
			else if(strcmp(token, TIPO_VISUALIZACAO_ANONIMA_CHAR) == 0)// APP 4 2 @ 3
			{
				// DESEJA obter a QUANTIDADE de VISUALIZACOES ANONIMAS
				printf(" LOG: Solicitando Obtenção de quantidade de visualizações anonimas em Comando-Obter.h comandoObter()\n");
				return obterQuantidadeDeVisualizacoesAnonimas(usuario);
			}
			else if(strcmp(token, TIPO_ESPECIFICO) == 0)// APP 4 2 @ =
			{
				//DESEJA obter QUANTIDADE de VISUALIZACOES de ALGO ESPECIFICO
				printf(" LOG: Solicitando Obtenção de quantidade de visualizações de algo específico em Comando-Obter.h comandoObter()\n");
				printf(" Warning: Comando incompleto em Comando-Obter.h comandoObter()\n");
				return RETORNO_ERRO_COMANDO_NAO_CONSTRUIDO_STR_DINAMICA;
			}
		}
		else
		{
			printf(" Warning: Comando não compreendido (%s) em Comando-Obter.h comandoObter() aq468e7ca\n", token);
			return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
		}
	}

	printf(" Warning: Comando inexistente: |%s| em Comando-Obter.h comandoObter() rbjak\n", token);
	return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
}

char *obterIdCidade(Usuario *usuario)
{
	char *token;
	char *nomeCidade = NULL;
	char *nomeEstado;

	// APP 4 $ nomeCidadeInformado
	token = usuario_getNextToken(usuario);

	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Obter.h obterIdCidade() dksajdjkheb\n");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}

	if( stringMaior(token, TAMANHO_DO_NOME_DA_CIDADE_COM_MAIOR_NOME_DO_MUNDO) )
	{
		printf(" Warning: Comando exageradamente grande em Comando-Obter.h obterIdCidade() ebk1ud8\n");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}

	nomeCidade = strdup(token);

	if(nomeCidade == NULL)
	{
		printf(" Warning: Falha ao copiar de token para nomeCidade em Comando-Obter.h obterIdCidade aksjd()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	// APP 4 $ nomeCidadeInformado nomeEstadoInformado
	token = usuario_getNextToken(usuario);

	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Obter.h obterIdCidade() 1gha5\n");
		free(nomeCidade);
		nomeCidade = NULL;
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}

	if( !stringTamanhoIgual(token, TAMANHO_ESTADO) )
	{
		printf(" Warning: Tamanho de Comando incorreto em Comando-Obter.h obterIdCidade() n4yw5\n");
		free(nomeCidade);
		nomeCidade = NULL;
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}

	nomeEstado = strdup(token);

	if(nomeEstado == NULL)
	{
		printf(" Warning: Falha ao copiar de token para nomeEstado em Comando-Obter.h obterIdCidade() eqkjh3\n");
		free(nomeCidade);
		nomeCidade = NULL;
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
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
	return RETORNO_ERRO_INTERNO_STR_DINAMICA;
}


char *obterIdContratante(Usuario *usuario)// APP 4 ; cnpj
{
	char *token = usuario_getNextToken(usuario); // cnpj

	if( token == NULL )
	{
		if (usuario == NULL)
		{
			return RETORNO_ERRO_INTERNO_STR_DINAMICA;
		}
		if (usuario_obterLogin(usuario) == NULL)
		{
			return RETORNO_ERRO_INTERNO_STR_DINAMICA;
		}
		if (usuario_PermissaoContratante(usuario))
		{
			char *retorno= usuario_obterId(usuario);
			if (retorno == NULL)
			{
				return RETORNO_ERRO_INTERNO_STR_DINAMICA;
			}
			return retorno;
		}
		printf(" Warning: Comando insuficiente em Comando-Obter.h obterIdContratante() asjhvbruw0f7878s756f6d\n");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	//	if( strlen(token) > TAMANHO_CNPJ )
	if( stringMaior(token, TAMANHO_CNPJ) )
	{
		printf(" Warning: Comando exageradamente grande em Comando-Obter.h obterIdContratante() abq78vas98wbjhax\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	char *cnpj = strdup(token);
	if( cnpj == NULL )
	{
		printf(" Warning: Falha ao duplicar email informado em Comando-Obter.h obterIdContratante() abv849a8sd72bhjht65w\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	return obterIdContratanteDoBanco(cnpj);
}

char *obterIdLocalizacao(Usuario *usuario)// APP 4 i0 idCidade cep bairro rua numero complemento//TODO Teoricamente pronto
{
	char *token = usuario_getNextToken(usuario);// APP 4 i0 idCidade
	if( token == NULL )
	{
		printf(" Warning: Comando insuficiente em Comando-Obter.h obterIdLocalizacao() qiuwbv89r4sf\n");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}

	//	if( strlen(token) > TAMANHO_CHAVE_PRIMARIA )
	if( stringMaior(token, TAMANHO_CHAVE_PRIMARIA) )
	{
		printf(" Warning: Argumento \"idCidade\" informado é exageradamente grande em Comando-Obter.h obterIdLocalizacao()\n");
		return NULL;
	}
	char *idCidade = strdup(token);
	if( idCidade == NULL )
	{
		printf(" Warning: Falha ao duplicar idCidade em Comando-Obter.h obterIdLocalizacao()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	// AOO 4 i0 idCidade cep
	token = usuario_getNextToken(usuario);
	if( token == NULL )
	{
		printf(" Warning: Comando insuficiente em Comando-Obter.h akiuvbruiasjd\n");
		free( idCidade );
		idCidade = NULL;
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}

	//TODO Adicionar suporte a checagem de tamanho de CEP

	char *cep = strdup(token);
	if( cep == NULL )
	{
		printf(" Warning: Falha ao duplicar dados de cep em Comando-Obter.h obterIdLocalizacao() sadkjahsfjkruiosdidds\n");
		free( idCidade );
		idCidade = NULL;
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	// APP 4 i0 idCidade cep bairro
	token = usuario_getNextToken(usuario);
	if( token == NULL )// Se o cliente informar somente idCidade e cep
	{
		printf(" LOG: Cliente não informou bairro em Comando-Obter.h obterIdLocalizacao() askjfjkhqsg4 \n");
		char *retorno = obterIdLocalizacaoDoBanco(idCidade, cep, NULL, NULL, NULL, NULL);
		free( idCidade );
		free( cep );
		cep = NULL;
		idCidade = NULL;
		return retorno;
	}

	//	if( strlen(token) > TAMANHO_BAIRRO )
	if( stringMaior(token, TAMANHO_BAIRRO) )
	{
		printf(" Warning: Tamanho de bairro exageradamente grande em Comando-Obter.h obterIdLocalizacao() asjvhbroasd\n");
		free( idCidade );
		free( cep );
		cep = NULL;
		idCidade = NULL;
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}

	char *bairro = strdup(token);

	if( bairro == NULL )
	{
		printf(" Warning: Falha ao tentar duplicar token para bairro em Comando-Obter.h obterIdLocalizacao() abnrs\n");
		free( idCidade );
		free( cep );
		cep = NULL;
		idCidade = NULL;
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	// APP 4 i0 idCidade cep bairro rua
	token = usuario_getNextToken(usuario);

	if( token == NULL )
	{
		printf(" LOG: Cliente não informou rua em Comando-Obter.h obterIdLocalizacao()\n");
		char *retorno = obterIdLocalizacaoDoBanco(idCidade, cep, bairro, NULL, NULL, NULL);
		free( idCidade );
		free( cep );
		free( bairro );
		bairro = NULL;
		cep = NULL;
		idCidade = NULL;
		return retorno;
	}

	//	if( strlen(token) > TAMANHO_RUA )
	if( stringMaior(token, TAMANHO_RUA) )
	{
		printf(" Warning: Tamanho da rua é exageradamente grande em Comando-Obter.h obterIdLocalizacao() aceghieoads\n");
		free( idCidade );
		free( cep );
		free( bairro );
		bairro = NULL;
		cep = NULL;
		idCidade = NULL;
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}

	char *rua = strdup(token);
	if( rua == NULL )
	{
		printf(" Warning: Falha ao duplicar string de token para rua em Comando-Obter.h obterIdLocalizacao() asjhjvkeasf\n");
		free( idCidade );
		free( cep );
		free( bairro );
		bairro = NULL;
		cep = NULL;
		idCidade = NULL;
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	// APP 4 i0 idCidade cep bairro rua numero
	token = usuario_getNextToken(usuario);
	if( token == NULL )
	{
		printf(" LOG: Cliente não informou numero em Comando-Obter.h obterIdLocalizacao() ashjvjk\n");
		char *retorno = obterIdLocalizacaoDoBanco(idCidade, cep, bairro, rua, NULL, NULL);
		free( idCidade );
		free( cep );
		free( bairro );
		free( rua );
		rua = NULL;
		bairro = NULL;
		cep = NULL;
		idCidade = NULL;
		return retorno;
	}
	//	 if( strlen(token) > TAMANHO_NUMERO )
	if( stringMaior(token, TAMANHO_NUMERO) )
	{
		printf(" Warning: Numero informado muto grande em Comando-Obter.h obterIdLocalizacao() sajhveasd\n");
		free( idCidade );
		free( cep );
		free( bairro );
		free( rua );
		rua = NULL;
		bairro = NULL;
		cep = NULL;
		idCidade = NULL;
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}

	char *numero = strdup(token);
	if( numero == NULL )
	{
		printf(" Warning: Falha ao duplicar numero em Comando-Obter.h obterIdLocalizacao() sajhveasd\n");
		free( idCidade );
		free( cep );
		free( bairro );
		free( rua );
		rua = NULL;
		bairro = NULL;
		cep = NULL;
		idCidade = NULL;
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}

	// APP 4 i0 idCidade cep bairro rua numero complemento
	token = usuario_getNextToken(usuario);
	if( token == NULL )
	{
		printf(" LOG: Cliente não informou numero em Comando-Obter.h obterIdLocalizacao() ashjvjk\n");
		char *retorno = obterIdLocalizacaoDoBanco(idCidade, cep, bairro, rua, NULL, NULL);
		free( idCidade );
		free( cep );
		free( bairro );
		free( rua );
		free( numero );
		numero = NULL;
		rua = NULL;
		bairro = NULL;
		cep = NULL;
		idCidade = NULL;
		return retorno;
	}

	//	if( strlen(token) > TAMANHO_COMPLEMENTO )
	if( stringMaior(token, TAMANHO_COMPLEMENTO) )
	{
		printf(" Warning: Complemento exageradamente grande em Comando-Obter.h obterIdLocalizacao() askjdhjkvd\n");
		free( idCidade );
		free( cep );
		free( bairro );
		free( rua );
		rua = NULL;
		bairro = NULL;
		cep = NULL;
		idCidade = NULL;
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}

	char *complemento = strdup(token);
	if( complemento == NULL )
	{
		printf(" Warning: Falha ao duplicar complemento em Comando-Obter.h obterIdLocalizacao() kjhbrostf\n");
		free( idCidade );
		free( cep );
		free( bairro );
		free( rua );
		rua = NULL;
		bairro = NULL;
		cep = NULL;
		idCidade = NULL;
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	char *retorno = obterIdLocalizacaoDoBanco(idCidade, cep, bairro, rua, numero, complemento);
	free( idCidade );
	free( cep );
	free( bairro );
	free( rua );
	free( complemento );
	complemento = NULL;
	rua = NULL;
	bairro = NULL;
	cep = NULL;
	idCidade = NULL;
	return retorno;
}


// decide quais tipos de produto estão aptos a serem enviados
char *obterTop10NovosProdutos(Usuario *usuario)// APP 4 J
{
	/*
	 * PADRÃO DE RETORNO: Retorna os id's de produtos separados por um espaco
	 *
	 * Linha 1 ->  "idProduto1 idproduto2 idProduto3 ...\0"
	 *
	 */
	if (usuario == NULL)
	{
		printf(" ERRO: erro interno, usuario nulo detectado em Comando-Obter.h obterTop10NovosProdutos()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" ERRO: Usuario nao conectado detectado em Comando-Obter.h obterTop10NovosProdutos()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	char *token;
	token = usuario_getNextToken(usuario);
	if (token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Obter.h obterTop10NovosProdutos()\n");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	if (stringMaior(token, TAMANHO_TIPO_PRODUTO))
	{
		printf(" Warning: Tipo de produto exageradamente grande em Comando-Obter.h obterTop10NovosProdutos()\n");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}

	if (usuario_PermissaoContratante(usuario))
	{
		if (strcmp(token, TIPO_REALIDADE_AUMENTADA_E_VIRTUAL) == 0)// APP 4 J 3
		{
			printf(" LOG: Contratante Solicitando top 10 novos produtos, realidade aumentada e virtual em Comando-Obter.h obterTop10NovosProdutos()\n");
			return obterUltimos10ProdutosAdicionadosPeloContratanteDoBanco(usuario);
		}
		else if(strcmp(token, TIPO_REALIDADE_AUMENTADA) == 0)// APP 4 J 1
		{
			printf(" LOG: Contratante Solicitando top 10 novos produtos, realidade aumentada em Comando-Obter.h obterTop10NovosProdutos()\n");
			return obterUltimos10ProdutosAdicionadosRealidadeAumentadaPeloContratanteDoBanco(usuario);
			// return RETORNO_ERRO_COMANDO_NAO_CONSTRUIDO_STR_DINAMICA;
		}
		else if(strcmp(token, TIPO_REALIDADE_VIRTUAL) == 0)// APP 4 J 2
		{
			printf(" LOG: Contratante Solicitando top 10 novos produtos, realidade virtual em Comando-Obter.h obterTop10NovosProdutos()\n");
			return obterUltimos10ProdutosAdicionadosRealidadeVirtualPeloContratanteDoBanco(usuario);
			// return RETORNO_ERRO_COMANDO_NAO_CONSTRUIDO_STR_DINAMICA;
		}
		else
		{
			return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
		}
	}
	else
	{
		if (strcmp(token, TIPO_REALIDADE_AUMENTADA_E_VIRTUAL) == 0)// APP 4 J 3
		{
			printf(" LOG: Solicitando top 10 novos produtos, realidade aumentada e virtual em Comando-Obter.h obterTop10NovosProdutos()\n");
			return obterTop10NovosProdutosDoBanco();
		}
		else if(strcmp(token, TIPO_REALIDADE_AUMENTADA) == 0)// APP 4 J 1
		{
			printf(" LOG: Solicitando top 10 novos produtos, realidade aumentada em Comando-Obter.h obterTop10NovosProdutos()\n");
			return obterTop10NovosProdutosRealidadeAumentadaDoBanco();
		}
		else if(strcmp(token, TIPO_REALIDADE_VIRTUAL) == 0)// APP 4 J 2
		{
			printf(" LOG: Solicitando top 10 novos produtos, realidade virtual em Comando-Obter.h obterTop10NovosProdutos()\n");
			return obterTop10NovosProdutosRealidadeVirtualDoBanco();
		}
		else
		{
			return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
		}
	}
}


char *obterDescricaoProduto(Usuario *usuario)// APP 4 Q
{
	/*
	 *	Retorna descricao do produto caso sucesso, ou "ERRO" caso algo dê errado
	 */
	char *token;

	// APP 4 Q idProdutoInformado
	token = usuario_getNextToken(usuario);
	if(token == NULL)
	{
		printf(" Warning: comando insuficiente em Comando-Obter.h obterDescricaoProduto() 4f51q\n");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	else if( !stringTamanhoIgual(token, TAMANHO_ID_PRODUTO) )
	{
		printf(" Warning: Comando de tamanho incorreto identificado em Comando-Obter.h obterDescricaoProduto()  44h5w11c\n");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
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
		char *retorno = obterDescricaoProdutoDoBanco(idProduto, usuario);
		free(idProduto);
		idProduto = NULL;
		return retorno;
	}
	printf(" ERRO: Desconhecido rb1625f0j66 em Comando-Obter.h obterDescricaoProduto()\n");
	return RETORNO_ERRO_INTERNO_STR_DINAMICA;// TEMP
}


char *obterNomeProduto(Usuario *usuario)
{
	char *token  = NULL;
	printf(" LOG: Iniciando interpretação em obterNomeProduto() Comando-Obter.h\n");

	// APP 4 1. idProdutoInformado
	token = usuario_getNextToken(usuario);

	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em obterNomeProduto() Comando-Obter.h akjbbe\n");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	//	if(strlen(token) != TAMANHO_ID_PRODUTO)
	if( !stringTamanhoIgual(token, TAMANHO_ID_PRODUTO) )
	{
		printf(" Warning: Comando incorreto em obterNomeProduto() Comando-Obter.h ohrbbzzk\n");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}

	char *idProduto = strdup(token);
	if (idProduto == NULL)
	{
		printf(" Warning: Falha ao duplicar produto em Comando-Obter.h obterNomeProduto() aaasd156 \n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	char *nomeProduto = NULL;
	//	obterNomeProdutoDoBanco(idProduto);
	nomeProduto = obterNomeProdutoDoBanco(idProduto, usuario);
	free(idProduto);
	return nomeProduto;
}


/* Obtema avaliação do produto e retorna da seguinte forma:  "numeroDeAvaliacoesPositivas NumeroDeAvaliacoesNegativas" */
char *obterAvaliacaoProduto(Usuario *usuario)// APP 4 kW idProduto                       (Retorna NULL quando ocorre algum erro)
{
	char *token = usuario_getNextToken(usuario);// APP 4 kW idProduto
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Obter.h obterAvaliacaoProduto() sadkjhjvrsad\n");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	//	if(strlen(token) != TAMANHO_ID_PRODUTO)
	if( !stringTamanhoIgual(token, TAMANHO_ID_PRODUTO) )
	{
		printf(" Warning: Comando incorreto detectado em Comando-Obter.h obterAvaliacaoProduto() asdhjkvrsdf\n");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}

	char *idProduto = strdup(token);
	if(idProduto == NULL)
	{
		printf(" Warning: Não foi possível duplicar token em Comando-Obter.h obterAvaliacaoProduto() qhjjvrsad\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	char *retorno;
	if((retorno = obterAvaliacaoProdutoDoBanco(idProduto, usuario)) != NULL)
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
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	printf(" ERRO: Erro desconhecido em Comando-Obter.h obterAvaliacaoProduto() askjhgvrdf\n");
	return RETORNO_ERRO_INTERNO_STR_DINAMICA;
}

char *obterTop10ProdutosMelhorAvaliados(Usuario *usuario)
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
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	else
	{
		printf(" LOG: Retorno obtido com sucesso de obterTop10NovosProdutos em Comando-Obter.h obterTop10ProdutosMelhorAvaliados()\n");
		return top;
	}

	return RETORNO_ERRO_INTERNO_STR_DINAMICA;
}


char *obterInformacoesProduto(Usuario *usuario)
{
	if (usuario == NULL)
	{
		printf(" ERRO: usuario nulo detectado em Comando-Obter.h obterInformacoesProduto() sakjh\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" Warning: usuario nao conectado identificado em Comando-Obter.h obterInformacoesProduto() saviusad\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	char *idProduto = usuario_getNextToken(usuario);
	if (idProduto == NULL)
	{
		printf(" Warning: Comando insuficiente detectado em Comando-Obter.h obterInformacoesProduto() asdsaehtusdr\n");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	if (usuario_PermissaoContratante(usuario) || usuario_PermissaoRoot(usuario))// TODO Remover comando de ROOT
	{
		return obterInformacoesAvancadasDeProdutoDoBanco(usuario, idProduto);
		// return RETORNO_ERRO_COMANDO_NAO_CONSTRUIDO_STR_DINAMICA;
	}
	else if(usuario_PermissaoRoot(usuario))
	{
		return strdup("Comando ainda não concluído para ROOT... logue como contratante ou usuario normal/anonimo");
	}
	else if(usuario_PermissaoCliente(usuario))
	{
		return obterInformacoesBasicasDeProdutoDoBanco(usuario, idProduto);
	}
	else
	{
		return RETORNO_ERRO_NAO_AUTORIZADO_STR_DINAMICA;	
	}
	return RETORNO_ERRO_INTERNO_STR_DINAMICA;
}

char *obterInformacoesUsuario(Usuario *usuario)
{
	if (usuario == NULL)
	{
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (usuario_PermissaoCliente(usuario))
	{
		return obterInformacoesClienteDoBanco(usuario);
	}
	else if (usuario_PermissaoContratante(usuario))
	{
		return obterInformacoesContratanteDoBanco(usuario);
		// return RETORNO_ERRO_COMANDO_NAO_CONSTRUIDO_STR_DINAMICA;
	}
	else if (usuario_PermissaoRoot(usuario))
	{
		return strdup("ROOT");
	}
	else
	{
		return RETORNO_ERRO_NAO_AUTORIZADO_STR_DINAMICA;
	}
}


char *obter10ProdutosDaEmpresa(Usuario *usuario)//APP 4 ## 33
{
	const char *localizacao = "Comando-Obter.h obter10ProdutosDaEmpresa()";
	if (!usuarioValido(usuario, localizacao))
	{
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	// if( !usuario_PermissaoContratante(usuario) )
	// {
	// 	printf(" Warning: Usuario normal tentou acessar comando disponível somente para contratante em Comando-Obter.h obter10ProdutosDaEmpresa() aisubru2wqiw\n");
	// 	return RETORNO_ERRO_NAO_AUTORIZADO_STR_DINAMICA;
	// }

	char *token;
	token = usuario_getNextToken(usuario);
	if( token == NULL )
	{
		return obter10ProdutosDaEmpresaDoBanco(usuario, strdup("1"), NULL);
	}
	char *pagina = strdup(token);
	if (usuario_PermissaoCliente(usuario) || usuario_PermissaoAnonimo(usuario))
	{

		token = usuario_getNextToken(usuario);
		if (token == NULL)
		{
			printf(" Warning: Comando insuficiente em Comando-Obter.h obterInformacaoPaginada() dfwjbht\n");
			return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
		}
		if (stringMaior(token, TAMANHO_DE_INTEIRO_EM_BANCO_DE_DADOS))
		{
			printf(" Warning: Comando exageradamente grande em Comando-Obter.h obterInformacaoPaginada() bouithfjt\n");
			return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
		}
		char *idContratante = strdup(token);
		return obter10ProdutosDaEmpresaDoBanco(usuario, pagina, idContratante);	
	}
	return obter10ProdutosDaEmpresaDoBanco(usuario, pagina, NULL);

}


/** 
 * @brief  Retorna a lista de desejos na pagina especificada ao usuario
 * @note   Requisita informações ao banco de dados antes de retornar
 * @param  *usuario:
 * @retval string que precisa ser liberada
 */
char *obterListaDesejos(Usuario *usuario)
{
	const char *localizacao = "Comando-Obter.h obterListaDesejos()";
	if (!usuarioValido(usuario, localizacao))
	{
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	char *token = usuario_getNextToken(usuario);
	if (token == NULL)
	{
		geraLog(LOG, "Usuario solicitando primeira pagina da lista de desejos");
		return retornaListaDesejosDoBanco(usuario, strdup("1"));
	}
	else
	{
		return retornaListaDesejosDoBanco(usuario, strdup(token));
	}
}



/** 
 * @brief  Redirecionador, cuida de funções que retornam informações que podem ser paginadas
 * @note   Wrapper
 * @param  *usuario: 
 * @retval Mensagem para o usuario
 */
char *obterInformacaoPaginada(Usuario *usuario)// APP 4 ##
{
	geraLog(LOG, "Usuario requisitando informação paginada");
	if (!usuarioValido(usuario, "Comando-Obter.h obterInformacaoPaginada() 156rf"))
	{
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	char * token = usuario_getNextToken(usuario);
	if( token == NULL )
	{
		geraLog(WARNING, "Comando insuficiente detectado askgbre");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	if( stringMaior(token, TAMANHO_TIPO) )
	{
		geraLog(ERRO, "Comando exageradamente grande detectado dfjkvbhuif");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	if( strcmp(token, TIPO_ID_DE_PRODUTO_POR_EMPRESA) == 0 )//APP 4 ## 33
	{
		return obter10ProdutosDaEmpresa(usuario);
	}
	else if( strcmp(token, TIPO_LISTA_DESEJOS) == 0 )// AAPP 4 ## LD
	{
		return obterListaDesejos(usuario);
	}
	else
	{
		geraLog(WARNING, "Usuario solicitou um comando que não existe");
		printf("\tComando → |%s|\n", token);
		return RETORNO_ERRO_COMANDO_NAO_CONSTRUIDO_STR_DINAMICA;
	}
}


char *obterIdDeEmpresaDadoProduto(Usuario *usuario)
{
	if (!usuarioValido(usuario, "Comando-Obter.h obterIdDeEmpresaDadoProduto() ndvjko"))
	{
		printf("Usuario invalido detectado nessa merda\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	printf(" asjkdhskjahdjksahdkjashdkjashkjdashjkdsahjkdhasjkdhsakjdhsajkdk\n");
	char *token = usuario_getNextToken(usuario);//APP 4 EP idProduto

	if (token == NULL)
	{
		printf(" Warning: Comando insuficiente detectado em Comando-Obter.h obterIdDeEmpresaDadoProduto() daskjbte\n");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	if (!stringTamanhoIgual(token, TAMANHO_ID_PRODUTO))
	{
		printf(" Warning: Coamndo incorreto detectado em Comando-Obter.h obterIdDeEmpresaDadoProduto() daskjbter\n");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	char *idProduto = strdup(token);
	if (idProduto == NULL)
	{
		printf(" Warning: Falha ao duplicar string em Comando-Obter.h obterIdDeEmpresaDadoProduto() vbruoiwehgtug\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	printf(" LOG: Obtendo retorno de id de empresa do banco de dados em Comando-Obter.h obterIdDeEmpresaDadoProduto() djkgbr\n");
	return retornaIdDeEmpresaDadoProdutoDoBanco(usuario, idProduto);

}


char *obterQuantidadeDeHabitantesDaCidade(Usuario *usuario)
{
	if (!usuarioValido(usuario, "Comando-Obter.h obterQuantidadeDeHabitantesDaCidade(Usuario *usuario)"))
	{
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	char *token = usuario_getNextToken(usuario);
	if (token == NULL)
	{
		geraLog(WARNING, "Comando insuficiente detectado (esperando nome de cidade)");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	if (stringMaior(token, TAMANHO_DO_NOME_DA_CIDADE_COM_MAIOR_NOME_DO_MUNDO))
	{
		geraLog(WARNING, "Comando exageradamente grande detectado (esperado nome de cidade)");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	char *nomeDaCidade = strdup(token);
	if (nomeDaCidade == NULL)
	{
		geraLog(WARNING, "Falha ao duplicar nome de cidade");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	token = usuario_getNextToken(usuario);
	if (token == NULL)
	{
		geraLog(WARNING, "Comando insuficiente detectado (esperado nome de estado)");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (!stringTamanhoIgual(token, TAMANHO_ESTADO))
	{
		geraLog(ERRO, "Tamanho de nome de estado informado não condiz com o esperado");
		return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
	}
	char *nomeDoEstado = strdup(token);

	if (nomeDoEstado == NULL)
	{
		geraLog(ERRO, "Falha ao duplicar nome de estado");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	if (!cidadeExisteNoBanco(nomeDaCidade, nomeDoEstado))
	{
		geraLog(WARNING, "Cidade pesquisada não existe no banco de dados");
		return RETORNO_ERRO_NOT_FOUND_STR_DINAMICA;
	}

	return obterQuantidadeDeHabitantesDaCidadeDoBanco(usuario, nomeDaCidade, nomeDoEstado);
}