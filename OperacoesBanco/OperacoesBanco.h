#ifndef __OPERACOES_BANCO__
#define __OPERACOES_BANCO__

#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
//#include <stdarg.h>

#ifndef bool
#define bool char
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif


#include "OperacoesBanco-FuncoesGenericas.h"
#include "OperacoesBanco-Visualizacoes.h"

//Retorna TRUE se o login estiver correto
//bool checarLogin(MYSQL *conexao, char *email)//Funcional
//bool checarLogin(char *email)//OK

bool addUsuarioAnonimoAoBanco(char *email, char *senha)//DONE
{
	if(email == NULL)
	{
		printf(" ERRO: email passado igual a NULL OperacoesBanco.h addUsuarioAnonimoAoBanco()\n");
		return false;
	}
	if(senha == NULL)
	{
		printf(" ERRO: senha passada igual a NULL OperacoesBanco.h addUsuarioAnonimoAoBanco()\n");
		return false;
	}
	if(conexao == NULL)
	{
		printf(" ERRO: Não há conexao com o banco de dados OperacoesBanco.h addUsuarioAnonimoAoBanco()\n");
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
	}
	printf(" LOG: Chegou com sucesso até a adição de usuario ao banco (anonimo) OperacoesBanco.h addUsuarioAnonimoAoBanco()\n");
	
	char *query = NULL;
	//int tamanho = sizeof(char) * (47 + strlen(email) + TAMANHO_SENHA_PADRAO_USUARIO_ANONIMO + 1);//O +1 é do '/0'
	int tamanho = sizeof(char) * (52 + strlen(email));//otimizado
	query = (char *)malloc(tamanho);
	
	if(query == NULL)
	{
		printf(" Warning: falha ao alocar memoria para query (OperacoesBanco.h) addUsuarioAnonimoAoBanco()\n");
		return false;
	}
	snprintf(query, tamanho, "INSERT INTO cliente(email,senha) VALUES(\'%s\',\'%s\');", email, senha);
	
	if(query == NULL)
	{
		printf(" ERRO: não foi possível alocar memória para a query (10) (OperacoesBanco.h) (addUsuarioAnonimoAoBanco())\n");
		return false;
	}
	
	if(mysql_query(conexao, query))//Se ocorrer algum erro
	{
		printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addUsuarioAnonimoAoBanco())\n");
		printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
		printf("\t Query enviada =  |%s|\n", query);
		free(query);
		query = NULL;
		if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
		{
			printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addUsuarioAnonimoAoBanco() 1qa5456dasd\n");
			if(conectarBanco())
			{
				printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addUsuarioAnonimoAoBanco() asd456\n");
			}
			else
			{
				conexao = NULL;
				mysql_close(conexao);
				mysql_thread_end();
				free(conexao);
				printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addUsuarioAnonimoAoBanco() as4d5asd\n");
			}
		}
		return false;
	}
	free(query);
	query = NULL;
	return true;
}

bool addUsuarioAoBanco(char *emailAnterior, char *email, char *senha, char* sexo, char *dataNascimento)//DONE
{
	if(conexao == NULL)
	{
		printf(" Warning: Conexao com o banco de dados é NULA em OperacoesBanco.h addUsuarioAoBanco() cviuhqwenmkxz\n");
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
	}
	if(emailAnterior == NULL)
	{
		printf(" ERRO: email anterior não foi informado devidamente em OperacoesBanco.h addUsuarioAoBanco() 456q5w1c\n");
		return false;
	}
	if(email == NULL)
	{
		printf(" ERRO: email == NULL em OperacoesBanco.h addUsuarioAoBanco(char *email, char *senha, int idade, int sexo, char *dataNascimento)\n");
		return false;
	}
	if(senha == NULL)
	{
		printf(" ERRO: senha == NULL em OperacoesBanco.h addUsuarioAoBanco(char *email, char *senha, int idade, int sexo, char *dataNascimento)\n");
		return false;
	}
	// if(sexo == NULL)
	// {
	// 	printf(" ERRO: sexo == NULL em OperacoesBanco.h addUsuarioAoBanco() 544qwe654b89a32d\n");
	// 	return false;
	// }
	// if(dataNascimento == NULL)
	// {
	// 	printf(" ERRO: email == NULL em OperacoesBanco.h addUsuarioAoBanco(char *email, char *senha, int idade, int sexo, char *dataNascimento)\n");
	// 	return false;
	// }
	
	printf(" LOG: Operação em testes → OperacoesBanco.h addUsuarioAoBanco()\n");
	
	char *query = NULL;
	
	size_t tamanho = 0;
	tamanho = 51 + strlen(emailAnterior) + 1;
	
	query = (char *)malloc(sizeof(char) * tamanho);
	
	if(query == NULL)
	{
		printf(" Warning: Não foi possível alocar memoria suficiente para query em OperacoesBanco.h addUsuarioAoBanco() sadjhgsahjveb378sxcf\n");
		return false;
	}
	
	snprintf(query, tamanho, "SELECT C.idcliente FROM cliente C WHERE C.email=\'%s\';", emailAnterior);
	
	if(query == NULL)
	{
		printf(" Warning: Não foi possível formatar string para query em OperacoesBanco.h addUsuarioAoBanco() sadjhgsahjveb378sa13\n");
		return false;
	}
	
	if(!queryRetornaConteudo(query))/* Query já é liberada após essa função */
	{
		printf(" Warning: Não existe nenhum usuario anonimo com esse email cadastrado em OperacoesBanco.h addUsuarioAoBanco() 13as54q987cd\n");
		query = NULL;
		return false;
	}
	else
	{
		//TODO (leia a proxima linha)
		/* Fazer tratamento de erros pra caso tenha algum problema aqui, senão usuario vai ficar com a senha trocada e o email não :/ */
		
		if(sexo != NULL && dataNascimento != NULL)
		{
			query = NULL;
			// tamanho = 113 + strlen(senha) + strlen(dataNascimento) + strlen(sexo) + strlen(email) + strlen(emailAnterior) + 1;
			tamanho = strlen(senha) + strlen(dataNascimento) + strlen(sexo) + strlen(email) + strlen(emailAnterior) + 114;
			
			query = (char *)malloc(sizeof(char) * tamanho);
			
			if(query == NULL)
			{
				printf(" Warning: Não foi possível alocar memoria para query em OperacoesBanco.h addUsuarioAoBanco() sadjhk3b78vwiuy\n");
				return false;
			}
			
			snprintf(query, tamanho, "UPDATE cliente C SET C.datanascimento=STR_TO_DATE(\'%s\',\'%%d/%%m/%%Y\'),C.sexo=%s,C.senha=\'%s\',C.email=\'%s\' WHERE C.email=\'%s\';", dataNascimento, sexo, senha, email, emailAnterior);
			
			
			if(query == NULL)
			{
				printf(" Warning: Não foi possível formatar string para query em OperacoesBanco.h addUsuarioAoBanco() qjbv9438ts82l\n");
				return false;
			}
			
			if(mysql_query(conexao, query))//Se ocorrer algum erro
			{
				printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addUsuarioAoBanco()) c1q658r04s\n");
				printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
				printf("\t Query enviada =  |%s|\n", query);
				free(query);
				query = NULL;
				if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
				{
					printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addUsuarioAoBanco() 1rwv66dasd\n");
					if(conectarBanco())
					{
						printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addUsuarioAoBanco() asd414q6e8e56\n");
					}
					else
					{
						conexao = NULL;
						mysql_close(conexao);
						mysql_thread_end();
						free(conexao);
						printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addUsuarioAoBanco() 4dsad4qasd\n");
					}
				}
				return false;
			}
			free(query);
			query = NULL;
			return true;
		}
		else
		{
			query = NULL;
			// tamanho = 60 + strlen(senha) +  strlen(sexo) + strlen(email) + strlen(emailAnterior) + 1;
			tamanho = strlen(senha) + strlen(email) + strlen(emailAnterior) + 61;
			
			query = (char *)malloc(sizeof(char) * tamanho);
			
			if(query == NULL)
			{
				printf(" Warning: Não foi possível alocar memoria para query em OperacoesBanco.h addUsuarioAoBanco() sadjhk3b78vwiuy\n");
				return false;
			}
			
			snprintf(query, tamanho, "UPDATE cliente C SET C.senha=\'%s\',C.email=\'%s\' WHERE C.email=\'%s\';", senha, email, emailAnterior);
			
			
			if(query == NULL)
			{
				printf(" Warning: Não foi possível formatar string para query em OperacoesBanco.h addUsuarioAoBanco() qjbv9438ts82l\n");
				return false;
			}
			
			if(mysql_query(conexao, query))//Se ocorrer algum erro
			{
				printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addUsuarioAoBanco()) c1q658r04s\n");
				printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
				printf("\t Query enviada =  |%s|\n", query);
				free(query);
				query = NULL;
				if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
				{
					printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addUsuarioAoBanco() 1rwv66dasd\n");
					if(conectarBanco())
					{
						printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addUsuarioAoBanco() asd414q6e8e56\n");
					}
					else
					{
						conexao = NULL;
						mysql_close(conexao);
						mysql_thread_end();
						free(conexao);
						printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addUsuarioAoBanco() 4dsad4qasd\n");
					}
				}
				return false;
			}
			free(query);
			query = NULL;
			return true;
		}

		
	}
	
	
	printf(" ERRO: O pior erro aconteceu, todos os ifs falharam em OperacoesBanco.h addUsuarioAoBanco() cqebhuy8u4bhajskybt7c8onhyauxis4f\n");
	return false;
}


bool addCidadeAoBanco(char *nomeDoEstado, char *nomeCidade)//DONE
{
	if(nomeCidade == NULL)
	{
		printf(" ERRO: Nome de cidade == NULL em OperacoesBanco.h addCidadeAoBanco()\n");
		return false;
	}
	if(nomeDoEstado == NULL)
	{
		printf(" ERRO: Nome de estado informado == NULL em OperacoesBanco.h addCidadeAoBanco()\n");
		return false;
	}
	if(conexao == NULL)
	{
		printf(" ERRO: Não há conexao com o banco de dados em OperacoesBanco.h addCidadeAoBanco()\n");
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
	}
	
	char *query = NULL;
	int tamanho = sizeof(char) * (100 + strlen(nomeDoEstado) + strlen(nomeCidade) + 1);
	query = (char *)malloc(tamanho);
	if(query == NULL)
	{
		printf(" Warning: não foi possível alocar memoria para query (1) em OperacoesBanco.h addCidadeAoBanco()\n");
		return false;
	}
	snprintf(query, tamanho, "SELECT cidade.nome, estado.nome from cidade JOIN estado WHERE cidade.nome = \'%s\' and estado.nome = \'%s\';", nomeCidade ,nomeDoEstado);
	
	if(query == NULL)
	{
		printf(" Warning: falha ao formatar query em OperacoesBanco.h addCidadeAoBanco()\n");
		return false;
	}
	
	/* Checa se existe estado com esse nome */
	if(!queryRetornaConteudo(query))
	{
		/* Se entrar aqui, então não existe cidade com esse nome nesse estado */
		
		query = NULL;
		
		tamanho = sizeof(char) * (101 + strlen(nomeDoEstado) + strlen(nomeCidade) + 1);
		query = (char *)malloc(tamanho);
		if(query == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h addCidadeAoBanco() (2)\n");
			return false;
		}
		else
		{
			snprintf(query, tamanho, "INSERT INTO cidade(nome, estado_idestado) SELECT \'%s\',estado.idestado FROM estado WHERE estado.nome=\'%s\';", nomeCidade, nomeDoEstado);
			if(query == NULL)
			{
				printf(" Warning: não foi possivel formatar query OperacoesBanco.h addCidadeAoBanco() (2)\n");
				return false;
			}
			if(mysql_query(conexao, query))//Se ocorrer algum erro
			{
				printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addCidadeAoBanco())\n");
				printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
				printf("\t Query enviada =  |%s|\n", query);
				free(query);
				query = NULL;
				if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
				{
					printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addCidadeAoBanco() 1qa5456dasd\n");
					if(conectarBanco())
					{
						printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addCidadeAoBanco() asd456\n");
					}
					else
					{
						conexao = NULL;
						mysql_close(conexao);
						mysql_thread_end();
						free(conexao);
						printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addCidadeAoBanco() as4d5asd\n");
					}
				}
				return false;
			}
			free(query);
			query = NULL;
			return true;
		}
		printf(" ERRO: Exceção não manipulada OperacoesBanco.h addCidadeAoBanco()\n");
		return false;
	}
	else
	{
		printf(" Warning: Pode haver mais de uma cidade com esse nome nesse estado em OperacoesBanco.h addCidadeAoBanco()\n");
		return false;
	}
	printf(" ERRO: Exceção não manipulada OperacoesBanco.h addCidadeAoBanco()\n");
	return false;
}


bool addLocalizacaoAoBanco(char* idCidade, char *cep, char *bairro, char *rua, char *numero, char *complemento)// TODO Limitar criação de localizacoes repetidas
{
	if(conexao == NULL)
	{
		printf(" Warning: Conexao nula com banco de dados (OperacoesBanco.h addLocalizacaoAoBanco())\n");
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
	}
	if(idCidade == NULL)
	{
		printf(" Warning: Impossivel existir cidade nula, abortando em OperacoesBanco.h addLocalizacaoAoBanco()\n");
		return false;
	}
	if(cep == NULL)
	{
		printf(" Warning: Não é possível adicionar localizacao sem informar CEP em OperacoesBanco.h addLocalizacaoAoBanco()\n");
		return false;
	}
	if(bairro == NULL)
	{
		if(rua == NULL)
		{
			if(numero == NULL)
			{
				if(complemento == NULL)
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + 57 + 1);
					char *query = (char *)malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (1)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep) VALUES(%s,\'%s\');", idCidade, cep);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (1)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(conectarBanco())
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								conexao = NULL;
								mysql_close(conexao);
								mysql_thread_end();
								free(conexao);
								printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() as4d5asd\n");
							}
						}
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
				else
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(complemento) + 70 + 1);
					char *query = (char *)malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (2)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,complemento) VALUES(%s,\'%s\',%s);", idCidade, cep, complemento);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (2)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(conectarBanco())
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								conexao = NULL;
								mysql_close(conexao);
								mysql_thread_end();
								free(conexao);
								printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() as4d5asd\n");
							}
						}
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
			}
			else
			{
				if(complemento == NULL)//Numero nao nulo e todo o resto nulo
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(numero) + 65 + 1);
					char *query = (char *)malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (3)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,numero) VALUES(%s,\'%s\',%s);", idCidade, cep, numero);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (3)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(conectarBanco())
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								conexao = NULL;
								mysql_close(conexao);
								mysql_thread_end();
								free(conexao);
								printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() as4d5asd\n");
							}
						}
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
				else//Tudo nulo menos numero e complemento
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(numero) + strlen(complemento) + 81 + 1);
					char *query = (char *)malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (4)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,numero,complemento) VALUES(%s,\'%s\',%s, \'%s\');", idCidade, cep, numero, complemento);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (4)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(conectarBanco())
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								conexao = NULL;
								mysql_close(conexao);
								mysql_thread_end();
								free(conexao);
								printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() as4d5asd\n");
							}
						}
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
			}
		}
		else// Se rua não for nulo
		{
			if(numero == NULL)// rua não nula mas numero sim
			{
				if(complemento == NULL)// somente rua nao nula
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(rua) + 64 + 1);
					char *query = (char *)malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (7)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,rua) VALUES(%s,\'%s\',\'%s\');", idCidade, cep, rua);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (7)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(conectarBanco())
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								conexao = NULL;
								mysql_close(conexao);
								mysql_thread_end();
								free(conexao);
								printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() as4d5asd\n");
							}
						}
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
				else// Somente rua e complemento nao nulos
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(rua) + strlen(complemento) + 79 + 1);
					char *query = (char *)malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (8)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,rua,complemento) VALUES(%s,\'%s\',\'%s\',\'%s\');", idCidade, cep, rua, complemento);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (8)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(conectarBanco())
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								conexao = NULL;
								mysql_close(conexao);
								mysql_thread_end();
								free(conexao);
								printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() as4d5asd\n");
							}
						}
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
			}
			else// Rua e numero nao nulos
			{
				if(complemento == NULL)// Somente rua e numero nao nulos
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(numero) + strlen(rua) + 73 + 1);
					char *query = (char *)malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (5)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,numero,rua) VALUES(%s,\'%s\',%s, \'%s\');", idCidade, cep, numero, rua);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (5)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(conectarBanco())
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								conexao = NULL;
								mysql_close(conexao);
								mysql_thread_end();
								free(conexao);
								printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() as4d5asd\n");
							}
						}
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
				else//Somente rua, numero e complemento nao nulos
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(numero) + strlen(rua) + strlen(complemento) + 87 + 1);
					char *query = (char *)malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (6)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,numero,rua,complemento) VALUES(%s,\'%s\',%s,\'%s\',\'%s\');", idCidade, cep, numero, rua, complemento);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (6)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(conectarBanco())
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								conexao = NULL;
								mysql_close(conexao);
								mysql_thread_end();
								free(conexao);
								printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() as4d5asd\n");
							}
						}
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
			}
		}
	}
	else// Bairro nao nulo
	{
		if(rua == NULL)// Somente bairro nao nulo
		{
			if(numero == NULL)//Somente bairro nao nulo
			{
				if(complemento == NULL)//Somente bairro nao nulo
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(bairro) + 67 + 1);
					char *query = (char *)malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (9)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,bairro) VALUES(%s,\'%s\',\'%s\');", idCidade, cep, bairro);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (9)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(conectarBanco())
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								conexao = NULL;
								mysql_close(conexao);
								mysql_thread_end();
								free(conexao);
								printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() as4d5asd\n");
							}
						}
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
				else// Bairro e complemento nao nulos
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(bairro) + strlen(complemento) + 83 + 1);
					char *query = (char *)malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (10)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,bairro, complemento) VALUES(%s,\'%s\',\'%s\',\'%s\');", idCidade, cep, bairro, complemento);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (10)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(conectarBanco())
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								conexao = NULL;
								mysql_close(conexao);
								mysql_thread_end();
								free(conexao);
								printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() as4d5asd\n");
							}
						}
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
			}
			else// Bairro, numero nao nulos
			{
				if(complemento == NULL)// Somente bairro e numero nao nulos (RUA NULA)
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(bairro) + strlen(numero) + 76 + 1);
					char *query = (char *)malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (11)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,bairro,numero) VALUES(%s,\'%s\',\'%s\', %s);", idCidade, cep, bairro, numero);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (11)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(conectarBanco())
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								conexao = NULL;
								mysql_close(conexao);
								mysql_thread_end();
								free(conexao);
								printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() as4d5asd\n");
							}
						}
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
				else//Bairo, numero complemento nao nulos  (RUA NULA)
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(bairro) + strlen(numero) + strlen(complemento) + 78 + 1);
					char *query = (char *)malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (12)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,bairro,numero) VALUES(%s,\'%s\',\'%s\',%s,\'%s\');", idCidade, cep, bairro, numero, complemento);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (12)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(conectarBanco())
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								conexao = NULL;
								mysql_close(conexao);
								mysql_thread_end();
								free(conexao);
								printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() as4d5asd\n");
							}
						}
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
			}
		}
		else//Bairro e rua nao nulos
		{	
			if(numero == NULL)//Somente bairro e rua nao nulos
			{
				if(complemento == NULL)//Somente bairo e rua nao nulos
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(bairro) + strlen(rua) + 74 + 1);
					char *query = (char *)malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (15)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,bairro,rua) VALUES(%s,\'%s\',\'%s\',\'%s\');", idCidade, cep, bairro, rua);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (15)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(conectarBanco())
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								conexao = NULL;
								mysql_close(conexao);
								mysql_thread_end();
								free(conexao);
								printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() as4d5asd\n");
							}
						}
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
				else//Bairro, rua e complemento nao nulos
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(bairro) + strlen(rua) + strlen(complemento) + 89 + 1);
					char *query = (char *)malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (16)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,bairro,rua,complemento) VALUES(%s,\'%s\',\'%s\',\'%s\',\'%s\');", idCidade, cep, bairro, rua, complemento);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (16)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(conectarBanco())
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								conexao = NULL;
								mysql_close(conexao);
								mysql_thread_end();
								free(conexao);
								printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() as4d5asd\n");
							}
						}
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
			}
			else// Bairro, rua e numero nao nulos
			{
				if(complemento == NULL)//Bairro, rua e numero nao nulos, mas complemento nulo
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(bairro) + strlen(numero) + strlen(rua) + 82 + 1);
					char *query = (char *)malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (14)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,bairro,numero,rua) VALUES(%s,\'%s\',\'%s\',%s,\'%s\');", idCidade, cep, bairro, numero, rua);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (14)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(conectarBanco())
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								conexao = NULL;
								mysql_close(conexao);
								mysql_thread_end();
								free(conexao);
								printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() as4d5asd\n");
							}
						}
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
				else// nada nulo, armazenar bairo, numero, rua, complemento
				{
					int tamanho = sizeof(char) * (strlen(cep) + strlen(idCidade) + strlen(bairro) + strlen(numero) + strlen(complemento) + strlen(rua) + 97 + 1);
					char *query = (char *)malloc(tamanho);
					if(query == NULL)
					{
						printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addLocalizacaoAoBanco() (13)\n");
						return false;
					}
					snprintf(query, tamanho, "INSERT INTO localizacao(cidade_idcidade,cep,bairro,numero,rua,complemento) VALUES(%s,\'%s\',\'%s\',%s,\'%s\',\'%s\');", idCidade, cep, bairro, numero, rua, complemento);
					if(query == NULL)
					{
						printf(" Warning: falha ao formatar query em OperacoesBanco.h addLocalizacaoAoBanco() (13)\n");
						return false;
					}
					if(mysql_query(conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(conectarBanco())
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								conexao = NULL;
								mysql_close(conexao);
								mysql_thread_end();
								free(conexao);
								printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() as4d5asd\n");
							}
						}
						return false;
					}
					free(query);
					query = NULL;
					return true;
				}
			}
		}
	}
	printf(" ERRO: Exceção não manipulada em OperacoesBanco.h addLocalizacaoAoBanco()\n");
	return false;
}


bool addContratanteAoBanco(char *nome, char *cnpj, char *plano, char *email, char *telefone, char *idLocalizacao, char *senha)//DONE
{
	if(conexao == NULL)
	{
		printf(" Warning: conexão nula em addContratanteAoBanco() OperacoesBanco.h\n");
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
	}
	
	if(idLocalizacao == NULL)
	{
		printf(" Warning: Não foi informado idLocalizacao em addContratanteAoBanco() OperacoesBanco.h \n");
		return false;
	}
	
	if(nome == NULL)
	{
		printf(" Warning: nome informado é nulo em addContratanteAoBanco()  OperacoesBanco.h\n");
		return false;
	}
	
	if(plano == NULL)
	{
		printf(" Warning: plano nulo em addContratanteAoBanco() OperacoesBanco.h\n");
		return false;
	}
	
	if(email == NULL)
	{
		printf(" Warning: email informado é nule em addContratanteAoBanco() OperacoesBanco.h\n");
		return false;
	}
	if( senha == NULL )
	{
		printf(" Warning: senha informada é nula em addContratanteAoBanco() OperacoesBanco.h sajbveiosd89\n");
		return false;
	}
	
	int tamanho;
	char *query = NULL;
	
	
	/* Checa se a já existe empresa com esse cnpj e esse email cadastrada */
	/* QUERYs para checagem de dados corretos */
	tamanho = 68 + strlen(email) + strlen(cnpj) + 1; // Esse +1 é o do bendito \0 kkk
	query = (char *)malloc(sizeof(char) * tamanho);
	
	if(query == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para query em addContratanteAoBanco() OperacoesBanco.h (qbbabdkhhhf)");
		return false;
	}
	else
	{
		snprintf(query, tamanho, "SELECT idcontratante FROM contratante C WHERE C.email=\'%s\' OR C.cnpj=%s;", email, cnpj);
		if(queryRetornaConteudo(query))
		{
			printf(" Warning: Já existe Uma empresa com esse email e/ou esse cnpj cadastrados no banco de dados em addContratanteAoBanco()  OperacoesBanco.h bqkjshqoxiusya\n");
			query= NULL;
			return false;
		}
		else
		{
			printf(" LOG: Não existe empresa com esse cnpj nem esse email no banco de dados em addContratanteAoBanco() OperacoesBanco.h \n");
			
			query = NULL;
			tamanho = 51 + 1 + strlen(idLocalizacao);
			query = (char *)malloc(sizeof(char) * tamanho);
			
			if(query == NULL)
			{
				printf(" Warning: Falha ao alocar memoria para query em addContratanteAoBanco() OperacoesBanco.h 56827d\n");
				return false;
			}
			else
			{
				snprintf(query, tamanho, "SELECT * FROM localizacao L WHERE L.idlocalizacao=%s;", idLocalizacao);
				if(query == NULL)
				{
					printf(" Warning: Falha ao formatar query em addContratanteAoBanco() OperacoesBanco.h qcbid\n");
					return false;
				}
				else
				{
					if(queryRetornaConteudo(query))
					{
						//Entra aqui se  query retornar algo
						printf(" LOG: Localizacao informada realmente existe em addContratanteAoBanco() OperacoesBanco.h qkcb\n");
					}
					else
					{
						printf(" Warning: Localizacao informada é inexistente (%s), encerrando operacao em addContratanteAoBanco() OperacoesBanco.h", idLocalizacao);
						query = NULL;
						return false;
					}
				}
			}
			printf(" \t LOG: Continuando adição ao banco em OperacoesBanco.h addContratanteAoBanco() e8q77aa456w5q1c52s\n");
		}
	}
	
	if(telefone == NULL)/* Se o cliente não informou telefone */
	{
		printf(" LOG: Telefone não informado, prosseguindo em addContratanteAoBanco() OperacoesBanco.h\n");
		tamanho = 99 + 1 + strlen(plano) + strlen(cnpj) + strlen(nome) + strlen(email) + strlen(idLocalizacao) + strlen(senha);
		query = (char *)malloc(sizeof(char) * tamanho );
		if(query == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para query em addContratanteAoBanco() OperacoesBanco.h\n");
			return false;
		}
		else
		{
			snprintf(query, tamanho, "INSERT INTO contratante(email,plano,cnpj,nome,localizacao_idlocalizacao,senha) VALUES(\'%s\',%s,%s,\'%s\',%s,\'%s\');", email, plano, cnpj, nome, idLocalizacao, senha);
			if(query == NULL)
			{
				printf(" Warning: Não foi possível formatar aquery para enviar ao banco de dados em addContratanteAoBanco() OperacoesBanco.h \n");
				return false;
			}
		}
	}
	else/* Se o cliente informou telefone */
	{
		printf(" LOG: Telefone informado pelo cliente em addContratanteAoBanco() OperacoesBanco.h\n");
		
		tamanho = 111 + 1 + strlen(plano) + strlen(cnpj) + strlen(nome) + strlen(email) + strlen(telefone) + strlen(idLocalizacao) + strlen(senha);
		query = (char *)malloc(sizeof(char) * tamanho );
		if(query == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para query em addContratanteAoBanco() OperacoesBanco.h (dasbkdbqwjkd)\n");
			return false;
		}
		else
		{
			snprintf(query, tamanho, "INSERT INTO contratante(email,plano,cnpj,nome,telefone,localizacao_idlocalizacao,senha) VALUES(\'%s\',%s,%s,\'%s\',\'%s\',%s,\'%s\');", email, plano, cnpj, nome, telefone, idLocalizacao, senha);
			if(query == NULL)
			{
				printf(" Warning: Não foi possível formatar aquery para enviar ao banco de dados em addContratanteAoBanco() OperacoesBanco.h (oqvervinsjk)\n");
				return false;
			}
		}
	}
	
	if(query == NULL)
	{
		printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addContratanteAoBanco() (sakdjhjkhq)\n");
		return false;
	}

	if(mysql_query(conexao, query))//Se ocorrer algum erro
	{
		printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addContratanteAoBanco())\n");
		printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
		printf("\t Query enviada =  |%s|\n", query);
		free(query);
		query = NULL;
		if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
		{
			printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addContratanteAoBanco() 1qa5456dasd\n");
			if(conectarBanco())
			{
				printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addContratanteAoBanco() asd456\n");
			}
			else
			{
				conexao = NULL;
				mysql_close(conexao);
				mysql_thread_end();
				free(conexao);
				printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addContratanteAoBanco() as4d5asd\n");
			}
		}
		return false;
	}
	free(query);
	query = NULL;
	return true;
}


bool addProdutoAoBanco(char *idContratante, char *idProduto, char *duracao, char *nomeProduto, char *descricao)//DONE
{
	if(conexao == NULL)
	{
		printf(" Warning: Conexao nula em addProdutoAoBanco() OperacoesBanco.h 68sg2\n");
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
	}
	
	if(idContratante == NULL)
	{
		printf(" Warning: idContratante == NULL em addProdutoAoBanco() OperacoesBanco.h jhdsak\n");
		return false;
	}
	
	if(idProduto == NULL)
	{
		printf(" Warning: idProduto == NULL em addProdutoAoBanco() OperacoesBanco.h dshjk\n");
		return false;
	}
	
	if(duracao == NULL)
	{
		printf(" Warning: duracao == NULL em addProdutoAoBanco() OperacoesBanco.h dshajks\n");
		return false;
	}
	
	if(nomeProduto == NULL)
	{
		printf(" Warning: Nome do produto não foi informado em addProdutoAoBanco() OperacoesBanco.h 6jk29df\n");
		return false;
	}
	
	if(checarIdContratante(idContratante))
	{
		printf(" LOG: ID de contratante encontrado no banco de dados em addProdutoAoBanco() OperacoesBanco.h asd4581r8w\n");
	}
	else
	{
		printf(" Warning: Não foi encontrado tal contratante na base de dados em addProdutoAoBanco() OperacoesBanco.h asd156eq\n");
		return false;
	}
	
	int tamanho;
	char *query = NULL;
	
	if(descricao == NULL)
	{
		tamanho = 93 + 1 + strlen(idContratante) + strlen(idProduto) + strlen(duracao) + strlen(nomeProduto);
		query = (char *)malloc(sizeof(char) * tamanho);
		
		if(query == NULL)
		{
			printf(" Warning: Não foi possivel alocar memoria para query em addProdutoAoBanco() OperacoesBanco.h 3kjhv73b\n");
			return false;
		}
		
		snprintf(query, tamanho, "INSERT INTO produto(idproduto,nomeproduto,duracao,contratante_idcontratante) VALUES(\'%s\',\'%s\',%s,%s);", idProduto, nomeProduto, duracao, idContratante);
		
		if(query == NULL)
		{
			printf(" Warning: Não foi possivel formatar query em addProdutoAoBanco() OperacoesBanco.h c3jjhvr87jh\n");
			return false;
		}
		
		if(mysql_query(conexao, query))//Se ocorrer algum erro
		{
			printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addProdutoAoBanco())\n");
			printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
			printf("\t Query enviada =  |%s|\n", query);
			free(query);
			query = NULL;
			if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
			{
				printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addProdutoAoBanco() 1qa5456dasd\n");
				if(conectarBanco())
				{
					printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addProdutoAoBanco() asd456\n");
				}
				else
				{
					conexao = NULL;
					mysql_close(conexao);
					mysql_thread_end();
					free(conexao);
					printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addProdutoAoBanco() as4d5asd\n");
				}
			}
			return false;
		}
		free(query);
		query = NULL;
		
		return true;
	}
	else
	{
		// Se a descricao for informada
		//tamanho = 106 + 1 + strlen(idContratante) + strlen(idProduto) + strlen(duracao) + strlen(nomeProduto) + strlen(descricao);
		tamanho = 117 + strlen(idContratante) + strlen(duracao) + strlen(nomeProduto) + strlen(descricao);
		query = (char *)malloc(sizeof(char) * tamanho);
		
		if(query == NULL)
		{
			printf(" Warning: Não foi possivel alocar memoria para query em addProdutoAoBanco() OperacoesBanco.h 3kjhv73b\n");
			return false;
		}
		
		snprintf(query, tamanho, "INSERT INTO produto(idproduto,nomeproduto,duracao,contratante_idcontratante,descricao) VALUES(\'%s\',\'%s\',%s,%s,\'%s\');", idProduto, nomeProduto, duracao, idContratante, descricao);
		
		if(query == NULL)
		{
			printf(" Warning: Não foi possivel formatar query em addProdutoAoBanco() OperacoesBanco.h c3jjhvr87jh\n");
			return false;
		}
		
		if(mysql_query(conexao, query))//Se ocorrer algum erro
		{
			printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addProdutoAoBanco())\n");
			printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
			printf("\t Query enviada =  |%s|\n", query);
			free(query);
			query = NULL;
			if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
			{
				printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addProdutoAoBanco() 1qa5456dasd\n");
				if(conectarBanco())
				{
					printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addProdutoAoBanco() asd456\n");
				}
				else
				{
					conexao = NULL;
					mysql_close(conexao);
					mysql_thread_end();
					free(conexao);
					printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addProdutoAoBanco() as4d5asd\n");
				}
			}
			return false;
		}
		free(query);
		query = NULL;
		
		return true;
	}
	
	
	
}

char *obterIdLocalizacaoDoBanco(char *idCidade, char *cep, char *bairro, char *rua, char *numero, char *complemento)
{
	if(conexao == NULL)
	{
		printf(" ERRO: Conexao nula em OperacoesBanco.h obterIdLocalizacaoDoBanco() !!!\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(conectarBanco())
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return NULL;
		}
	}

	if( idCidade == NULL )
	{
		printf(" ERRO: idCidade nulo detectada em OperacoesBanco.h obterIdLocalizacaoDoBanco() sabfjieoq\n");
		return NULL;
	}

	if( cep == NULL )
	{
		printf(" ERRO: cep nulo detectada em OperacoesBanco.h obterIdLocalizacaoDoBanco() sabfjieoq\n");
		return NULL;
	}

//TODO Daqui pra baixo
	int tamanho;
	char *query;

	if( bairro == NULL )// Retorna a localizacao geral
	{
		tamanho = 88+strlen(idCidade)+strlen(cep)+1;
		query = (char *)malloc(sizeof(char) * tamanho);
		if( query == NULL )
		{
			printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h obterIdLocalizacaoDoBanco() aoirnisdf\n");
			return NULL;
		}
		snprintf(query, tamanho, "SELECT L.idlocalizacao FROM localizacao L WHERE L.cidade_idcidade=%s AND L.cep=\'%s\' LIMIT 1;", idCidade, cep);
		if( query == NULL )
		{
			printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterIdLocalizacaoDoBanco() abuvirkasd\n");
			return NULL;
		}
		return retornaUnicoRetornoDaQuery(query);
	}
	else
	{
		if( rua == NULL )// Busca somente "idCidade, cep, bairro"
		{
			tamanho = 104+strlen(idCidade)+strlen(bairro)+strlen(cep)+1;
			query = (char *)malloc(sizeof(char) * tamanho);
			if( query == NULL )
			{
				printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h obterIdLocalizacaoDoBanco() aoirnisdf\n");
				return NULL;
			}
			snprintf(query, tamanho, "SELECT L.idlocalizacao FROM localizacao L WHERE L.cidade_idcidade=%s AND L.bairro=\'%s\' AND L.cep=\'%s\' LIMIT 1;", idCidade, bairro, cep);
			if( query == NULL )
			{
				printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterIdLocalizacaoDoBanco() abuvirkasd\n");
				return NULL;
			}
			return retornaUnicoRetornoDaQuery(query);
		}
		else
		{
			if( numero == NULL )// Busca somente "idCidade, cep, bairro, rua"
			{
				tamanho = 117+strlen(idCidade)+strlen(bairro)+strlen(cep)+strlen(rua)+1;
				query = (char *)malloc(sizeof(char) * tamanho);
				if( query == NULL )
				{
					printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h obterIdLocalizacaoDoBanco() aoirnisdf\n");
					return NULL;
				}
				snprintf(query, tamanho, "SELECT L.idlocalizacao FROM localizacao L WHERE L.cidade_idcidade=%s AND L.bairro=\'%s\' AND L.cep=\'%s\' AND L.rua=\'%s\' LIMIT 1;", idCidade, bairro, cep, rua);
				if( query == NULL )
				{
					printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterIdLocalizacaoDoBanco() abuvirkasd\n");
					return NULL;
				}
				return retornaUnicoRetornoDaQuery(query);
			}
			else
			{
				if( complemento == NULL )// Busca somente "idCidade, cep, bairro, rua, numero"
				{
					tamanho = 133+strlen(idCidade)+strlen(bairro)+strlen(cep)+strlen(rua)+strlen(numero)+1;
					query = (char *)malloc(sizeof(char) * tamanho);
					if( query == NULL )
					{
						printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h obterIdLocalizacaoDoBanco() aoirnisdf\n");
						return NULL;
					}
					snprintf(query, tamanho, "SELECT L.idlocalizacao FROM localizacao L WHERE L.cidade_idcidade=%s AND L.bairro=\'%s\' AND L.cep=\'%s\' AND L.rua=\'%s\' AND L.numero=\'%s\' LIMIT 1;", idCidade, bairro, cep, rua, numero);
					if( query == NULL )
					{
						printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterIdLocalizacaoDoBanco() abuvirkasd\n");
						return NULL;
					}
					return retornaUnicoRetornoDaQuery(query);
				}
				else// Busca com todos os dados
				{
					tamanho = 154+strlen(idCidade)+strlen(bairro)+strlen(cep)+strlen(rua)+strlen(numero)+strlen(complemento)+1;
					query = (char *)malloc(sizeof(char) * tamanho);
					if( query == NULL )
					{
						printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h obterIdLocalizacaoDoBanco() aoirnisdf\n");
						return NULL;
					}
					snprintf(query, tamanho, "SELECT L.idlocalizacao FROM localizacao L WHERE L.cidade_idcidade=%s AND L.bairro=\'%s\' AND L.cep=\'%s\' AND L.rua=\'%s\' AND L.numero=\'%s\' AND L.complemento=\'%s\' LIMIT 1;", idCidade, bairro, cep, rua, numero, complemento);
					if( query == NULL )
					{
						printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterIdLocalizacaoDoBanco() abuvirkasd\n");
						return NULL;
					}
					return retornaUnicoRetornoDaQuery(query);
				}
			}
		}
	}

/* int tamanho = 126 + 1 + 2 * (strlen(cidade) + strlen(estado));
	char *query = (char *)malloc(sizeof(char) * (tamanho));
	
	if(query == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h obterIdLocalizacaoDoBanco() qkjeh\n");
		return NULL;
	}
	
	snprintf(query, tamanho, "SELECT L.idlocalizacao FROM localizacao L JOIN cidade C ON C.nome=\'%s\' JOIN estado E ON E.nome=\'%s\' WHERE C.nome=\'%s\' AND E.nome=\'%s\';", cidade, estado, cidade, estado);
	
	if(query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterIdLocalizacaoDoBanco() bqjek\n");
		return NULL;
	}*/

	//TODO Daqui pra cima
	return NULL;
}

char *obterIdContratanteDoBanco(char *cnpj)// APP 4 ; cnpj
{
	if(conexao == NULL)
	{
		printf(" ERRO: Conexao nula em OperacoesBanco.h obterIdContratanteDoBanco() !!!\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(conectarBanco())
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return NULL;
		}
	}

	if( cnpj == NULL )
	{
		printf(" Warning: Cliente não informou cnpj em OperacoesBanco.h obterIdContratanteDoBanco() asvbeuasd\n");
		return NULL;
	}


	int tamanho = 58 + 1 + strlen(cnpj);
	char *query = (char *)malloc(sizeof(char) * (tamanho));
	
	if(query == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h obterIdContratanteDoBanco() qkjeh\n");
		return NULL;
	}
	
	snprintf(query, tamanho, "SELECT C.idcontratante FROM contratante C WHERE C.cnpj=\'%s\';", cnpj);
	
	
	if(query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterIdContratanteDoBanco() bqjek\n");
		return NULL;
	}
	
	if(mysql_query(conexao, query))
	{
		printf(" ERRO: Falha ao executar query em OperacoesBanco.h obterIdContratanteDoBanco() sakdjh\n");
		printf(" \t%s\n", mysql_error(conexao));
		return NULL;
	}
	
	MYSQL_RES *resultado = mysql_store_result(conexao);
	
	if(resultado == NULL)// Se não houver consulta
	{
		printf(" Warning: Resultado nulo em OperacoesBanco.h obterIdContratanteDoBanco() kqht\n");
		return NULL;
	}
	else
	{
		
		if(mysql_num_fields(resultado) != 1)
		{
			if(mysql_num_fields(resultado) == 0)
			{
				printf(" LOG: Nada encontrado na base de dados para:\n");
				printf(" \t%s\n", query);
				printf(" \t em OperacoesBanco.h obterIdContratanteDoBanco() 4rr455\n");
				mysql_free_result(resultado);
				free(query);
				resultado = NULL;
				query = NULL;
				return (char *)RETORNO_NOT_FOUND;
			}
			else
			{
				printf(" Warning: Resultado inconsistente em OperacoesBanco.h obterIdContratanteDoBanco() hekja\n");
				mysql_free_result(resultado);
				free(query);
				query = NULL;
				resultado = NULL;
				return NULL;
			}
		}
		else
		{
			if(mysql_num_rows(resultado) != 1)
			{
				printf(" Warning: Mais de uma cidade com mesmo nome e estado em OperacoesBanco.h obterIdContratanteDoBanco() ekjjha\n");
				printf(" \tRespostas:\n");
				MYSQL_ROW linhas;
				while((linhas = mysql_fetch_row(resultado)) != NULL)
				{
					printf(" linhas[0] -> %s\n", linhas[0]);
				}
				printf(" \n");
				mysql_free_result(resultado);
				free(query);
				query = NULL;
				resultado = NULL;
				return linhas[0];//Retorna o ultimo resultado, mesmo se houver erro
			}
			else
			{
				MYSQL_ROW linha;
				
				linha = mysql_fetch_row(resultado);
				
				if(linha == NULL)
				{
					printf(" ERRO: Não era pra poder retornar nulo aqui abv84eu9h89hbd\n");
					mysql_free_result(resultado);
					free(query);
					query = NULL;
					resultado = NULL;
					return NULL;
				}
				else
				{
					mysql_free_result(resultado);
					free(query);
					query = NULL;
					resultado = NULL;
					return linha[0];
				}
			}
		}
		
	}
	return NULL;



}

char *obterIdCidadeDoBanco(char *nomeCidade, char *nomeEstado)/* APP 4 CHAVE_DE_SEGURANCA_PHP $ { nomeCidadeInformada nomeEstadoInformado*/
{
	if(conexao == NULL)
	{
		printf(" ERRO: Conexao nula em OperacoesBanco.h obterIdCidadeDoBanco() !!!\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(conectarBanco())
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return NULL;
		}
	}
	
	if(nomeCidade == NULL)
	{
		printf(" ERRO: Comunicacao entre funcoes falhou, nomeCidade == NULL em OperacoesBanco.h obterIdCidadeDoBanco()\n");
		return NULL;
	}
	
	if(nomeEstado == NULL)
	{
		printf(" ERRO: Comunicacao entre funcoes falhou, nomeEstado == NULL em OperacoesBanco.h obterIdCidadeDoBanco()\n");
		return NULL;
	}
	
	int tamanho = 108 + 1 + strlen(nomeEstado) + strlen(nomeCidade);
	char *query = (char *)malloc(sizeof(char) * (tamanho));
	
	if(query == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h obterIdCidadeDoBanco() qkjeh\n");
		return NULL;
	}
	
	snprintf(query, tamanho, "SELECT C.idcidade FROM cidade C JOIN estado E ON E.idestado=C.estado_idestado WHERE E.nome=\'%s\' AND C.nome=\'%s\';", nomeEstado, nomeCidade);
	
	
	if(query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterIdCidadeDoBanco() bqjek\n");
		return NULL;
	}
	
	if(mysql_query(conexao, query))
	{
		printf(" ERRO: Falha ao executar query em OperacoesBanco.h obterIdCidadeDoBanco() sakdjh\n");
		printf(" \t%s\n", mysql_error(conexao));
		return NULL;
	}
	
	MYSQL_RES *resultado = mysql_store_result(conexao);
	
	if(resultado == NULL)// Se não houver consulta
	{
		printf(" Warning: Resultado nulo em OperacoesBanco.h obterIdCidadeDoBanco() kqht\n");
		return NULL;
	}
	else
	{
		
		//		MYSQL_FIELD *campos;
		//		campos = mysql_fetch_fields(resultado);//Passa campos com resposta para a variavel campos
		
		if(mysql_num_fields(resultado) != 1)
		{
			if(mysql_num_fields(resultado) == 0)
			{
				printf(" LOG: Nada encontrado na base de dados para:\n");
				printf(" \t%s\n", query);
				printf(" \t em OperacoesBanco.h obterIdCidadeDoBanco() 4rr455\n");
				mysql_free_result(resultado);
				free(query);
				resultado = NULL;
				query = NULL;
				return (char *)RETORNO_NOT_FOUND;
			}
			else
			{
				printf(" Warning: Resultado inconsistente em OperacoesBanco.h obterIdCidadeDoBanco() hekja\n");
				mysql_free_result(resultado);
				free(query);
				query = NULL;
				resultado = NULL;
				return NULL;
			}
		}
		else
		{
			if(mysql_num_rows(resultado) != 1)
			{
				printf(" Warning: Mais de uma cidade com mesmo nome e estado em OperacoesBanco.h obterIdCidadeDoBanco() ekjjha\n");
				printf(" \tRespostas:\n");
				MYSQL_ROW linhas;
				while((linhas = mysql_fetch_row(resultado)) != NULL)
				{
					printf(" linhas[0] -> %s\n", linhas[0]);
				}
				printf(" \n");
				mysql_free_result(resultado);
				free(query);
				query = NULL;
				resultado = NULL;
				return linhas[0];//Retorna o ultimo resultado, mesmo se houver erro
			}
			else
			{
				MYSQL_ROW linha;
				
				linha = mysql_fetch_row(resultado);
				
				if(linha == NULL)
				{
					printf(" ERRO: Não era pra poder retornar nulo aqui\n");
					mysql_free_result(resultado);
					free(query);
					query = NULL;
					resultado = NULL;
					return NULL;
				}
				else
				{
					mysql_free_result(resultado);
					free(query);
					query = NULL;
					resultado = NULL;
					return linha[0];
				}
			}
		}
		
	}
	return NULL;
}


char *obterTop10NovosProdutosDoBanco()//DONE
{
	if(conexao == NULL)
	{
		printf(" ERRO: conexao nula em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(conectarBanco())
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return NULL;
		}
	}
	
	int tamanho = 65 + 1;
	char *query = (char *)malloc(sizeof(char) * (tamanho));
	
	if(query == NULL)
	{
		printf(" Warning: Falha ao tentar alocar memoria para query em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		return NULL;
	}
	
	snprintf(query, tamanho, "SELECT idproduto FROM produto ORDER BY datacriacao DESC LIMIT 10;");
	
	if(query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		return NULL;
	}
	
	if(mysql_query(conexao, query))
	{
		printf(" Warning: falhao ao executar query em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		free(query);
		query = NULL;
		return NULL;
	}
	
	MYSQL_RES *resultado = mysql_store_result(conexao);
	if(resultado == NULL)// Se não houver consulta
	{
		printf(" Warning: Consulta não realizada em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		free(query);
		query = NULL;
		return NULL;
	}
	
	if(mysql_num_fields(resultado) == 0)
	{
		printf(" Warning: Nada encontrado em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		free(query);
		query = NULL;
		mysql_free_result(resultado);
		resultado = NULL;
		return (char *)RETORNO_NOT_FOUND;
	}
	
	MYSQL_ROW linha = NULL;
	char *retorno = NULL;
	
	tamanho = ((mysql_num_rows(resultado) * 10) + 1 + mysql_num_rows(resultado)-1);
	
	retorno = (char *)malloc(sizeof(char) * tamanho);// numero de resultados * tamanho de cada resultado + \0 + numero de resultados -1
	
	if(retorno == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para retorno em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		free(query);
		query = NULL;
		mysql_free_result(resultado);
		resultado = NULL;
		return NULL;
	}
	memset(retorno, '\0', tamanho);
	
	//	 int i = 0;
	
	while((linha = mysql_fetch_row(resultado)) != NULL)
	{
		//		snprintf(retorno, tamanho, "%s %s", retorno, linha);
		
		printf(" LOG: LINHA OBTIDA = |%s| em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n", linha[0]);
		if(strlen(linha[0]) > 0)
		{
			strcat(retorno, linha[0]);
		}
		
		printf(" LOG: Retorno = |%s| até agora em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n", retorno);
		
		if(retorno == NULL)
		{
			printf(" Warning: Falha ao formatar resultado em OperacoesBanco.h obterTop10NovosProdutosDoBanco() vbrjka\n");
			free(query);
			mysql_free_result(resultado);
			resultado = NULL;
			query = NULL;
			return NULL;
		}
		if(strlen(retorno) < tamanho - 1)
		{
			strcat(retorno, " ");
		}
	}
	
	if(retorno == NULL)
	{
		printf(" ERRO: Falha ao obter dados em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		free(query);
		mysql_free_result(resultado);
		resultado = NULL;
		query = NULL;
		return NULL;
	}
	else
	{
		free(query);
		mysql_free_result(resultado);
		resultado = NULL;
		query = NULL;
		return retorno;
	}
	
	return NULL;
}

char *obterTop10ProdutosMelhorAvaliadosDoBanco()
{
	if (conexao == NULL)
	{
		printf(" ERRO: conexao nula em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(conectarBanco())
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return NULL;
		}
	}
	//int tamanho = 149 + 1;
	int tamanho = 150;
	char *query = (char *)malloc(sizeof(char) * tamanho);
	if(query == NULL)
	{
		printf(" Warning: não foi possível alocar memoria para query em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n");
		return NULL;
	}
	query = strdup("SELECT idproduto FROM produto P WHERE DATEDIFF(CURDATE(), P.datacriacao)-P.duracao<=0 ORDER BY P.avaliacaoPositiva-P.avaliacaoNegativa DESC LIMIT 10;");
	if (query == NULL)
	{
		printf(" Warning: Falha ao copiar string para query em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n");
		return NULL;
	}

	if(mysql_query(conexao, query))
	{
		printf(" Warning: falhao ao executar query: |%s| em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n", query);
		free(query);
		query = NULL;
		return NULL;
	}

	free(query);
	query = NULL;//Não preciso mais de query daqui pra frente
	
	MYSQL_RES *resultado = mysql_store_result(conexao);
	if(resultado == NULL)// Se não houver consulta
	{
		printf(" Warning: Consulta não realizada em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n");
		return NULL;
	}
	
	if(mysql_num_fields(resultado) == 0)
	{
		printf(" Warning: Nada encontrado em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n");
		mysql_free_result(resultado);
		resultado = NULL;
		return (char *)RETORNO_NOT_FOUND;
	}
	
	MYSQL_ROW linha = NULL;
	char *retorno = NULL;
	
	tamanho = ((mysql_num_rows(resultado) * 10) + 1 + mysql_num_rows(resultado)-1);
	
	retorno = (char *)malloc(sizeof(char) * tamanho);// numero de resultados * tamanho de cada resultado + \0 + numero de resultados -1
	
	if(retorno == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para retorno em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n");
		mysql_free_result(resultado);
		resultado = NULL;
		return NULL;
	}
	memset(retorno, '\0', tamanho);
	
	//	 int i = 0;
	
	while((linha = mysql_fetch_row(resultado)) != NULL)
	{
		//		snprintf(retorno, tamanho, "%s %s", retorno, linha);
		
		printf(" LOG: LINHA OBTIDA = |%s| em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n", linha[0]);
		if(strlen(linha[0]) > 0)
		{
			strcat(retorno, linha[0]);
		}
		
		printf(" LOG: Retorno = |%s| até agora em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n", retorno);
		
		if(retorno == NULL)
		{
			printf(" Warning: Falha ao formatar resultado em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco() vbrjka\n");
			mysql_free_result(resultado);
			resultado = NULL;
			return NULL;
		}
		if(strlen(retorno) < tamanho - 1)
		{
			strcat(retorno, " ");
		}
	}
	
	if(retorno == NULL)
	{
		printf(" ERRO: Falha ao obter dados em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n");
		mysql_free_result(resultado);
		resultado = NULL;
		return NULL;
	}
	else
	{
		mysql_free_result(resultado);
		resultado = NULL;
		return retorno;
	}
	
	return NULL;
}


char *obterDescricaoProdutoDoBanco(char *idProduto, char *email)//DONE
{
	if (conexao == NULL)
	{
		printf(" Warning: Conexao perdida com o banco de dados em OperacoesBanco.h obterDescricaoProdutoDoBanco() 4e5b11eea\n");
		return NULL;
	}
	else if(idProduto == NULL)
	{
		printf(" Warning: Id de produto nulo em OperacoesBanco.h obterDescricaoProdutoDoBanco() veqf45\n");
		return NULL;
	}
	else
	{
		if (produtoVencido(idProduto, email))
		{
			printf(" Warning: produto vencido em OperacoesBanco.h obterDescricaoProdutoDoBanco\n");
			return NULL;
		}
		char *query = NULL;
		//int tamanho = 55 + TAMANHO_ID_PRODUTO + 1;// Usando TAMANHO_ID_PRODUTO para otimização
		int tamanho = 66;// Usando TAMANHO_ID_PRODUTO para otimização
		
		query = (char *)malloc(sizeof(char) * tamanho);
		if(query == NULL)
		{
			printf(" Warning: nao foi possível alocar memoria para query em OperacoesBanco.h obterDescricaoProdutoDoBanco()\n");
			return false;
		}
		
		snprintf(query, tamanho, "SELECT P.descricao FROM produto P WHERE P.idproduto=\'%s\';", idProduto);
		
		if(query == NULL)
		{
			printf(" Warning: nao foi possível formatar query em OperacoesBanco.h obterDescricaoProdutoDoBanco() cqhkhbjvr\n");
			return false;
		}
		
		if(mysql_query(conexao, query))
		{
			printf(" Warning: falha ao executar query em OperacoesBanco.h obterDescricaoProdutoDoBanco() kjelkzj\n");
			free(query);
			query = NULL;
			return NULL;
		}
		
		MYSQL_RES *resultado = mysql_store_result(conexao);
		
		if(resultado == NULL)
		{
			printf(" Warning: Consulta não realizada em OperacoesBanco.h obterDescricaoProdutoDoBanco() nao houve conexao\n");
			free(query);
			query = NULL;
			return NULL;
		}
		
		if(mysql_num_fields(resultado) == 0)
		{
			printf(" Warning: Nao houve resultados na pesquisa em OperacoesBanco.h obterDescricaoProdutoDoBanco() bnje\n");
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
			printf(" Warning: Resposta Nula em OperacoesBanco.h obterDescricaoProdutoDoBanco()\n");
			free(query);
			mysql_free_result(resultado);
			resultado = NULL;
			query = NULL;
			return NULL;
		}
		else
		{
			if(linha[0] == NULL)/* EDITADO */
			{
				printf(" Warning: Pesquisa por linha não retornou nada em OperacoesBanco.h obterDescricaoProdutoDoBanco() 38ahah1kkj\n");
				printf(" \tQuery utilizada: |%s|\n", query);
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return NULL;
			}
			if(strlen(linha[0]) > 1)
			{
				printf(" LOG: Retorno obtido com sucesso em OperacoesBanco.h obterDescricaoProdutoDoBanco() kjh3as\n");
			}
			else
			{
				
				printf(" Warning: Produto nao possui descrição cadastrada no banco de dados em OperacoesBanco.h obterDescricaoProdutoDoBanco() kejhqkfge3\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return (char *)RETORNO_NOT_FOUND;
				
			}
			
			int tamanho = strlen(linha[0]) + 1;
			retorno =(char *)malloc(sizeof(char) * tamanho);
			
			if(retorno == NULL)
			{
				
				printf(" Warning: Nao foi possivel alocar memoria para retorno em OperacoesBanco.h obterDescricaoProdutoDoBanco() sakdjhjkredf\n");
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
				
				printf(" Warning: Nao foi possivel copiar memoria para retorno em OperacoesBanco.h obterDescricaoProdutoDoBanco() sakdjhjkredf\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return NULL;
				
			}
			else
			{
				
				printf(" LOG: Operação concluida com sucesso em OperacoesBanco.h obterDescricaoProdutoDoBanco() ckjahjksd\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return retorno;
				
			}
			
			
		}
		
	}
	printf(" ERRO: erro Desconhecido chave: ew54b510530fv8qw em OperacoesBanco.h obterDescricaoProdutoDoBanco()\n");
	return NULL;
}


char *obterNomeProdutoDoBanco(char *idProduto, char *email)//DONE
{
	if(conexao == NULL)
	{
		printf(" Warning: Conexao nula com o banco de dados em obterNomeProdutoDoBanco() OperacoesBanco.h \n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(conectarBanco())
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return NULL;
		}
	}
	if(idProduto == NULL)
	{
		printf(" Warning: idProduto informado NULO em obterNomeProdutoDoBanco() OperacoesBanco.h\n");
		return NULL;
	}
	else
	{
		if(produtoVencido(idProduto, email))
		{
			printf(" Warning: produto vencido em OperacoesBanco.h obterNomeProdutoDoBanco()\n");
			return NULL;
		}

		char *query = NULL;
		//int tamanho = 57 + TAMANHO_ID_PRODUTO + 1;// Usando TAMANHO_ID_PRODUTO para otimização
		int tamanho = 68;// Usando TAMANHO_ID_PRODUTO para otimização
		
		query = (char *)malloc(sizeof(char) * tamanho);
		if(query == NULL)
		{
			printf(" Warning: nao foi possível alocar memoria para query em OperacoesBanco.h obterNomeProdutoDoBanco()\n");
			return false;
		}
		
		snprintf(query, tamanho, "SELECT P.nomeProduto FROM produto P WHERE P.idproduto=\'%s\';", idProduto);
		
		if(query == NULL)
		{
			printf(" Warning: nao foi possível formatar query em OperacoesBanco.h obterNomeProdutoDoBanco() cqhkhbjvr\n");
			return false;
		}
		
		if(mysql_query(conexao, query))
		{
			printf(" Warning: falha ao executar query em OperacoesBanco.h obterNomeProdutoDoBanco() kjelkzj\n");
			free(query);
			query = NULL;
			return NULL;
		}
		
		MYSQL_RES *resultado = mysql_store_result(conexao);
		
		if(resultado == NULL)
		{
			printf(" Warning: Consulta não realizada em OperacoesBanco.h obterNomeProdutoDoBanco() nao houve conexao\n");
			free(query);
			query = NULL;
			return NULL;
		}
		
		if(mysql_num_fields(resultado) == 0)
		{
			printf(" Warning: Nao houve resultados na pesquisa em OperacoesBanco.h obterNomeProdutoDoBanco() bnje\n");
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
			printf(" Warning: Resposta Nula em OperacoesBanco.h obterNomeProdutoDoBanco()\n");
			free(query);
			mysql_free_result(resultado);
			resultado = NULL;
			query = NULL;
			return NULL;
		}
		else
		{
			if (linha[0] == NULL)
			{
				printf(" Warning: Produto nao possui nome cadastrado no banco de dados em OperacoesBanco.h obterNomeProdutoDoBanco() kejhqkfge3\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return (char *)RETORNO_NOT_FOUND;
			}
			if(strlen(linha[0]) > 1)
			{
				printf(" LOG: Retorno obtido com sucesso em OperacoesBanco.h obterNomeProdutoDoBanco() kjh3as\n");
			}
			
			int tamanho = strlen(linha[0]) + 1;
			retorno = (char *)malloc(sizeof(char) * tamanho);
			
			if(retorno == NULL)
			{
				
				printf(" Warning: Nao foi possivel alocar memoria para retorno em OperacoesBanco.h obterNomeProdutoDoBanco() sakdjhjkredf\n");
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
				printf(" Warning: Nao foi possivel copiar memoria para retorno em OperacoesBanco.h obterNomeProdutoDoBanco() sakdjhjkredf\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return NULL;
			}
			else
			{
				printf(" LOG: Operação concluida com sucesso em OperacoesBanco.h obterNomeProdutoDoBanco() ckjahjksd\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return retorno;
			}
		}
	}
	printf(" ERRO: erro Desconhecido chave: ew54b510530fv8qw em OperacoesBanco.h obterNomeProdutoDoBanco()\n");
	return NULL;
}


char *obterAvaliacaoProdutoDoBanco(char *idProduto, char *email)// APP 4 kW * idProduto
{
	printf(" LOG: iniciando obtenção de avaliação em OperacoesBanco.h obterAvaliacaoProdutoDoBanco()\n");
	if(conexao == NULL)
	{
		printf(" Warning: Conexao nula com o banco de dados em obterAvaliacaoProdutoDoBanco() OperacoesBanco.h \n");
		printf(" \tLOG: Tentando reconexão com banco de dados \n");
		if(conectarBanco())
		{
			printf(" \tLOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" \tWarning: Falha ao reconectar-se, encerrando interpretação\n");
			return NULL;
		}
	}
	if(idProduto == NULL)
	{
		printf(" Warning: idProduto informado NULO em obterAvaliacaoProdutoDoBanco() OperacoesBanco.h\n");
		return NULL;
	}
	else
	{
		if(produtoVencido(idProduto, email))
		{
			printf(" Warning: Produto vencido em OperacoesBanco.h obterAvaliacaoProdutoDoBanco()\n");
			return NULL;
		}
		char *query = NULL;
		// int tamanho = 79 + 1 + TAMANHO_ID_PRODUTO;
		int tamanho = 99;// Usando TAMANHO_ID_PRODUTO para otimização
		
		query = (char *)malloc(sizeof(char) * tamanho);
		if(query == NULL)
		{
			printf(" Warning: nao foi possível alocar memoria para query em OperacoesBanco.h obterAvaliacaoProdutoDoBanco()\n");
			return false;
		}
		
		snprintf(query, tamanho, "SELECT P.avaliacaoPositiva,P.avaliacaoNegativa FROM produto P WHERE P.idproduto=\'%s\';", idProduto);
		
		if(query == NULL)
		{
			printf(" Warning: nao foi possível formatar query em OperacoesBanco.h obterAvaliacaoProdutoDoBanco() cqhkhbjvr\n");
			return NULL;
		}
		
		// if(mysql_query(conexao, query))
		// {
		// 	printf(" Warning: falha ao executar query em OperacoesBanco.h obterAvaliacaoProdutoDoBanco() kjelkzj\n");
		// 	free(query);
		// 	query = NULL;
		// 	return NULL;
		// }
		if(!executaQuery(query))
		{
			printf(" Warning: Falha ao executar query em OperacoesBanco.h obterAvaliacaoProdutoDoBanco() qa468fsx2er\n");
			free(query);
			query = NULL;
			return NULL;
		}
		else
		{
			printf(" LOG: Query executada, continuando para obtenção de dados em OperacoesBanco.h obterAvaliacaoProdutoDoBanco() 49q87b2df\n");
		}
		
		MYSQL_RES *resultado = mysql_store_result(conexao);
		
		if(resultado == NULL)
		{
			printf(" Warning: Consulta não realizada em OperacoesBanco.h obterAvaliacaoProdutoDoBanco() nao houve conexao\n");
			free(query);
			query = NULL;
			return NULL;
		}
		
		if(mysql_num_fields(resultado) == 0)
		{
			printf(" Warning: Nao houve resultados na pesquisa em OperacoesBanco.h obterAvaliacaoProdutoDoBanco() bnje\n");
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
			printf(" Warning: Resposta Nula em OperacoesBanco.h obterAvaliacaoProdutoDoBanco()\n");
			free(query);
			mysql_free_result(resultado);
			resultado = NULL;
			query = NULL;
			return NULL;
		}
		else
		{
			if(strlen(linha[0]) > 0 && strlen(linha[1]) > 0)
			{
				printf(" LOG: Retorno obtido com sucesso em OperacoesBanco.h obterAvaliacaoProdutoDoBanco() kjh3as\n");
				printf(" LOG TEMPORARIO: linha[0]=|%s|    ;  linha[1] =|%s|\n", linha[0], linha[1]);
			}
			else
			{
				printf(" Warning: Produto nao possui descrição cadastrada no banco de dados em OperacoesBanco.h obterAvaliacaoProdutoDoBanco() kejhqkfge3\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return (char *)RETORNO_NOT_FOUND;
			}
			
			int tamanho = strlen(linha[0])+ 1 + strlen(linha[1]) + 1;
			retorno = (char *)malloc(sizeof(char) * tamanho);
			
			if(retorno == NULL)
			{
				
				printf(" Warning: Nao foi possivel alocar memoria para retorno em OperacoesBanco.h obterAvaliacaoProdutoDoBanco() sakdjhjkredf\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return NULL;
				
			}
			
			memset(retorno, '\0', tamanho);
			
			// strcpy(retorno, linha[0]);
			snprintf(retorno, tamanho, "%s %s", linha[0], linha[1]);
			
			if(retorno == NULL)
			{
				
				printf(" Warning: Nao foi possivel copiar memoria para retorno em OperacoesBanco.h obterAvaliacaoProdutoDoBanco() sakdjhjkredf\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return NULL;
				
			}
			else
			{
				
				printf(" LOG: Operação concluida com sucesso em OperacoesBanco.h obterAvaliacaoProdutoDoBanco() ckjahjksd\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return retorno;
				
			}
			
			
		}
		
	}
	printf(" ERRO: erro Desconhecido chave: ew54b510530fv8qw em OperacoesBanco.h obterAvaliacaoProdutoDoBanco()\n");
	return NULL;

}


bool addNomeDeUsuarioAoBanco(char *email, char *nome)// TESTAR        APP 2 & # nome
{
	if(nome == NULL)
	{
		printf(" Warning: Nome recebido é nulo em OperacoesBanco.h addNomeDeUsuarioAoBanco() 4654egg\n");
		return false;
	}
	if (conexao == NULL)
	{
		printf(" Warning: Conexao nula detectada em OperacoesBanco.h addNomeDeUsuarioAoBanco() 56456q489b87ts\n");
		printf(" LOG: Tentando reconexão com banco de dados em OperacoesBanco.h addNomeDeUsuarioAoBanco()\n");
		if(conectarBanco())
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return false;
		}
	}
	

	char *query = NULL;
	// int tamanho = strlen(email) + strlen(nome) + 48 +1;
	int tamanho = strlen(email) + strlen(nome) + 49;
	query = (char *)malloc(sizeof(char) * tamanho);
	if(query == NULL)
	{
		printf(" Warning: nao foi possível alocar memoria para query em OperacoesBanco.h addNomeDeUsuarioAoBanco()\n");
		return false;
	}
	
	snprintf(query, tamanho, "UPDATE cliente C SET C.nome=\'%s\' WHERE C.email=\'%s\';", nome, email);
	
	if(query == NULL)
	{
		printf(" Warning: nao foi possível formatar query em OperacoesBanco.h addNomeDeUsuarioAoBanco() cqhkhbjvr\n");
		return false;
	}
	
	if(mysql_query(conexao, query))
	{

		printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addNomeDeUsuarioAoBanco())\n");
		printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
		printf("\t Query enviada =  |%s|\n", query);
		free(query);
		query = NULL;
		if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
		{
			printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addNomeDeUsuarioAoBanco() 1qa5456dasd\n");
			if(conectarBanco())
			{
				printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addNomeDeUsuarioAoBanco() asd456\n");
			}
			else
			{
				conexao = NULL;
				mysql_close(conexao);
				mysql_thread_end();
				free(conexao);
				printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addNomeDeUsuarioAoBanco() as4d5asd\n");
			}
		}
		return false;
	}
	else
	{
		printf(" LOG: Nome %s adicionado ao usuario %s com sucesso em Comando-Adicao.h addNomeDeUsuarioAoBanco\n", nome, email);
		free(query);
		query = NULL;
		return true;
	}

}

bool addSobrenomeDeUsuarioAoBanco(char *email, char *sobrenome)//TESTAR          APP 2 & Y sobrenome
{
	if(sobrenome == NULL)
	{
		printf(" Warning: Nome recebido é nulo em OperacoesBanco.h addSobrenomeDeUsuarioAoBanco() 4654egg\n");
		return false;
	}
	if (conexao == NULL)
	{
		printf(" Warning: Conexao nula detectada em OperacoesBanco.h addSobrenomeDeUsuarioAoBanco() 56456q489b87ts\n");
		printf(" LOG: Tentando reconexão com banco de dados em OperacoesBanco.h addSobrenomeDeUsuarioAoBanco()\n");
		if(conectarBanco())
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return false;
		}
	}
	

	char *query = NULL;
	// int tamanho = strlen(email) + strlen(sobrenome) + 53 +1;
	int tamanho = strlen(email) + strlen(sobrenome) + 54;
	query = (char *)malloc(sizeof(char) * tamanho);
	if(query == NULL)
	{
		printf(" Warning: nao foi possível alocar memoria para query em OperacoesBanco.h addSobrenomeDeUsuarioAoBanco()\n");
		return false;
	}
	
	snprintf(query, tamanho, "UPDATE cliente C SET C.sobrenome=\'%s\' WHERE C.email=\'%s\';", sobrenome, email);
	
	if(query == NULL)
	{
		printf(" Warning: nao foi possível formatar query em OperacoesBanco.h addSobrenomeDeUsuarioAoBanco() cqhkhbjvr\n");
		return false;
	}
	
	if(mysql_query(conexao, query))
	{

		printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addSobrenomeDeUsuarioAoBanco())\n");
		printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
		printf("\t Query enviada =  |%s|\n", query);
		free(query);
		query = NULL;
		if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
		{
			printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addSobrenomeDeUsuarioAoBanco() 1qa5456dasd\n");
			if(conectarBanco())
			{
				printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addSobrenomeDeUsuarioAoBanco() asd456\n");
			}
			else
			{
				conexao = NULL;
				mysql_close(conexao);
				mysql_thread_end();
				free(conexao);
				printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addSobrenomeDeUsuarioAoBanco() as4d5asd\n");
			}
		}
		return false;
	}
	else
	{
		printf(" LOG: sobrenome %s adicionado ao usuario %s com sucesso em Comando-Adicao.h addSobrenomeDeUsuarioAoBanco\n", sobrenome, email);
		free(query);
		query = NULL;
		return true;
	}

}

bool addSexoDeUsuarioAoBanco(char *email, char *sexo)// TESTAR        APP 2 & SQ sexo
{
	if(sexo == NULL)
	{
		printf(" Warning: Nome recebido é nulo em OperacoesBanco.h addSexoDeUsuarioAoBanco() 4654egg\n");
		return false;
	}
	if (conexao == NULL)
	{
		printf(" Warning: Conexao nula detectada em OperacoesBanco.h addSexoDeUsuarioAoBanco() 56456q489b87ts\n");
		printf(" LOG: Tentando reconexão com banco de dados em OperacoesBanco.h addSexoDeUsuarioAoBanco()\n");
		if(conectarBanco())
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return false;
		}
	}
	

	char *query = NULL;
	// int tamanho = strlen(email) + strlen(sobrenome) + 48 +1;
	int tamanho = strlen(email) + strlen(sexo) + 49;
	query = (char *)malloc(sizeof(char) * tamanho);
	if(query == NULL)
	{
		printf(" Warning: nao foi possível alocar memoria para query em OperacoesBanco.h addSexoDeUsuarioAoBanco()\n");
		return false;
	}
	
	snprintf(query, tamanho, "UPDATE cliente C SET C.sexo=\'%s\' WHERE C.email=\'%s\';", sexo, email);
	
	if(query == NULL)
	{
		printf(" Warning: nao foi possível formatar query em OperacoesBanco.h addSexoDeUsuarioAoBanco() cqhkhbjvr\n");
		return false;
	}
	
	if(mysql_query(conexao, query))
	{

		printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addSexoDeUsuarioAoBanco())\n");
		printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
		printf("\t Query enviada =  |%s|\n", query);
		free(query);
		query = NULL;
		if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
		{
			printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addSexoDeUsuarioAoBanco() 1qa5456dasd\n");
			if(conectarBanco())
			{
				printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addSexoDeUsuarioAoBanco() asd456\n");
			}
			else
			{
				conexao = NULL;
				mysql_close(conexao);
				mysql_thread_end();
				free(conexao);
				printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addSexoDeUsuarioAoBanco() as4d5asd\n");
			}
		}
		return false;
	}
	else
	{
		printf(" LOG: sexo %s adicionado ao usuario %s com sucesso em Comando-Adicao.h addSexoDeUsuarioAoBanco\n", sexo, email);
		free(query);
		query = NULL;
		return true;
	}

}

bool addDataNascimentoDeUsuarioAoBanco(char *email, char *dataNascimento)
{
	if(dataNascimento == NULL)
	{
		printf(" Warning: Data de nascimento não informada em OperacoesBanco.h addDataNascimentoDeUsuarioAoBanco()\n");
		return false;
	}
	if (conexao == NULL)
	{
		printf(" Warning: Conexao nula detectada em OperacoesBanco.h addDataNascimentoDeUsuarioAoBanco() 56456q489b87ts\n");
		printf(" LOG: Tentando reconexão com banco de dados em OperacoesBanco.h addDataNascimentoDeUsuarioAoBanco()\n");
		if(conectarBanco())
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return false;
		}
	}
	

	char *query = NULL;
	// int tamanho = strlen(email) + strlen(dataNascimento) + 82 +1;
	int tamanho = strlen(email) + strlen(dataNascimento) + 83;
	query = (char *)malloc(sizeof(char) * tamanho);
	if(query == NULL)
	{
		printf(" Warning: nao foi possível alocar memoria para query em OperacoesBanco.h addDataNascimentoDeUsuarioAoBanco()\n");
		return false;
	}
	
	snprintf(query, tamanho, "UPDATE cliente C SET C.dataNascimento=STR_TO_DATE(\'%s\',\'%%d/%%m/%%Y\') WHERE C.email=\'%s\';", dataNascimento, email);
	
	if(query == NULL)
	{
		printf(" Warning: nao foi possível formatar query em OperacoesBanco.h addDataNascimentoDeUsuarioAoBanco() cqhkhbjvr\n");
		return false;
	}
	
	if(mysql_query(conexao, query))
	{

		printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addDataNascimentoDeUsuarioAoBanco())\n");
		printf("\t ERRO nº%d  ->  %s\n", mysql_errno(conexao), mysql_error(conexao));
		printf("\t Query enviada =  |%s|\n", query);
		free(query);
		query = NULL;
		if(mysql_errno(conexao) == 2006)// SERVER MYSQL SUMIU
		{
			printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addDataNascimentoDeUsuarioAoBanco() 1qa5456dasd\n");
			if(conectarBanco())
			{
				printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addDataNascimentoDeUsuarioAoBanco() asd456\n");
			}
			else
			{
				conexao = NULL;
				mysql_close(conexao);
				mysql_thread_end();
				free(conexao);
				printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addDataNascimentoDeUsuarioAoBanco() as4d5asd\n");
			}
		}
		return false;
	}
	else
	{
		printf(" LOG: data de nascimento %s adicionado ao usuario %s com sucesso em Comando-Adicao.h addDataNascimentoDeUsuarioAoBanco\n", dataNascimento, email);
		free(query);
		query = NULL;
		return true;
	}
	return false;
}

bool addAvaliacaoAProdutoAoBanco(char *email, char *idProduto, char *avaliacao)// APP 2 kW * idProduto avaliacao
{
	if (conexao == NULL)
	{
		printf(" Warning: Conexao nula detectada em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() 56456q489b87ts\n");
		printf(" LOG: Tentando reconexão com banco de dados em OperacoesBanco.h addAvaliacaoAProdutoAoBanco()\n");
		if(conectarBanco())
		{
			printf(" \tLOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" \tWarning: Falha ao reconectar-se, encerrando interpretação\n");
			return false;
		}
	}
	if(email == NULL)
	{
		printf(" Warning: email == NULL em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() 65q4w5e\n");
		return false;
	}
	if(idProduto == NULL)
	{
		printf(" Warning: idProduto == NULL em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() 1q6w54bve\n");
		return false;
	}
	if(avaliacao == NULL)
	{
		printf(" Warning: avaliacao == NULL em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() hkjqwd\n");
		return false;
	}
	if(produtoVencido(idProduto, email))
	{
		printf(" Warning: produto vencido detectado em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() akjhvs\n");
		return false;
	}

	int checou = checaExistenciaDeVisualizacaoDeProdutoComPessoa(idProduto, email);
	if(checou == RETORNO_OK)
	{
		printf(" LOG: Existe visualização de usuario cadastrada em OperacoesBanco.h addAvaliacaoAProdutoAoBanco()\n");
	}
	else if(checou == RETORNO_NULO)
	{
		printf(" Warning: não existe visualização para esse usuario em OperacoesBanco.h addAvaliacaoAProdutoAoBanco()\n");
		return false;
	}
	else
	{
		printf(" ERRO: Usuario ou produto não existem, ou algum outro erro possível em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() qkjhejcv\n");
		return false;
	}

	//int tamanho = 151 + strlen(email) + TAMANHO_ID_PRODUTO + 1;
	int tamanho = strlen(email) + 162;//otimizado
	char *query = (char *)malloc(sizeof(char) * tamanho);

	if (query == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() q648484eas\n");
		return false;
	}
	snprintf(query, tamanho, "UPDATE visualizacaoDeUsuario V JOIN cliente C ON C.idcliente=V.cliente_idcliente SET V.avaliacaoDoUsuario=%c WHERE V.produto_idproduto=\'%s\' AND C.email=\'%s\';", avaliacao[0], idProduto, email );
	if(query == NULL)
	{
		printf(" Warning: Falhao ao formatar query para executar em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() qkjhhdjkasd\n");
		return false;
	}

	if(!executaQuery(query))
	{
		printf(" Warning: problemas foram encontrados durante a execução da query em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() q46f874asd\n");
		free(query);
		query = NULL;
		return false;
	}
	else
	{
		free(query);
		query = NULL;

		if(avaliacao[0] == '1')
		{
			//tamanho = 84+TAMANHO_ID_PRODUTO+1;
			tamanho = 95;//otimizado
			query = (char *)malloc(sizeof(char) * tamanho);
			if(query == NULL)
			{
				printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() akhkvejjd81779c\n");
				return false;
			}
			snprintf(query, tamanho, "UPDATE produto P SET P.avaliacaoPositiva=P.avaliacaoPositiva+1 WHERE P.idproduto=\'%s\';", idProduto);
			if (query == NULL)
			{
				printf(" Warning: Falha ao formatar query em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() qbhbr25300sdf44*-asd\n");
				return false;
			}
			if(!executaQuery(query))
			{
				printf(" Warning: Foram encontrados erros na execucao da query em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() a646e88bra\n");
				free(query);
				query = NULL;
				return false;
			}
			printf(" LOG: Avaliacao adicionada com sucesso ao banco de dados em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() chkah389s9827\n");
			free(query);
			query = NULL;
			return true;
		}
		else if(avaliacao[0] == '0')
		{
			//tamanho = 84+TAMANHO_ID_PRODUTO+1;
			tamanho = 95;//otimizado
			query = (char *)malloc(sizeof(char) * tamanho);
			if(query == NULL)
			{
				printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() akhkvejjd81779c\n");
				return false;
			}
			snprintf(query, tamanho, "UPDATE produto P SET P.avaliacaoPositiva=P.avaliacaoNegativa+1 WHERE P.idproduto=\'%s\';", idProduto);
			if (query == NULL)
			{
				printf(" Warning: Falha ao formatar query em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() qbhbr25300sdf44*-asd\n");
				return false;
			}
			if(!executaQuery(query))
			{
				printf(" Warning: Foram encontrados erros na execucao da query em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() a646e88bra\n");
				free(query);
				query = NULL;
				return false;
			}
			printf(" LOG: Avaliacao adicionada com sucesso ao banco de dados em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() chkah389s9827\n");
			free(query);
			query = NULL;
			return true;
		}
		else
		{
			printf(" ERRO: Lógica anterior à inserção FALHOU, DADOS NÃO CONFIÁVEIS DETECTADOS em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() ashdjksarb\n");
			free(query);
			query = NULL;
			return false;
		}
	}

	printf(" ERRO: DEU PAU !!!!! em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() qjwhejikhjvs6545 \n");
	return false;
}

bool addFeedBackDeProdutoAoBanco(char *email, char *idProduto, char *titulo, char *conteudo)
{
	if (conexao == NULL)
	{
		printf(" Warning: Conexao nula detectada em OperacoesBanco.h addFeedBackDeProdutoAoBanco() 56456q489b87ts\n");
		printf(" LOG: Tentando reconexão com banco de dados em OperacoesBanco.h addFeedBackDeProdutoAoBanco()\n");
		if(conectarBanco())
		{
			printf(" \tLOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" \tWarning: Falha ao reconectar-se, encerrando interpretação\n");
			return false;
		}
	}
	if(email == NULL)
	{
		printf(" Warning: email == NULL detectado em addFeedBackDeProdutoAoBanco() OperacoesBanco.h ashjgvrs\n");
		return false;
	}
	if (idProduto == NULL)
	{
		printf(" Warning: idProduto == NULL detectado em addFeedBackDeProdutoAoBanco() OperacoesBanco.h ashjgvrs\n");
		return false;
	}
	if (titulo == NULL)
	{
		printf(" Warning: titulo == NULL detectado em addFeedBackDeProdutoAoBanco() OperacoesBanco.h ashjgvrs\n");
		return false;
	}
	if (conteudo == NULL)
	{
		printf(" Warning: conteudo == NULL detectado em addFeedBackDeProdutoAoBanco() OperacoesBanco.h ashjgvrs\n");
		return false;
	}
	
	if( produtoVencido(idProduto, email) )
	{
		printf(" Warning: Produto vencido detectado em OperacoesBanco.h addFeedBackDeProdutoAoBanco() sa6b54xdad\n");
		return false;
	}
	else
	{
		if(checaExistenciaDeVisualizacaoDeProdutoComPessoa(idProduto, email) != RETORNO_OK)
		{
			printf(" Warning: usuario cadastrado não possui nenhuma visualização desse produto registrada em OperacoesBanco.h addFeedBackDeProdutoAoBanco() askjdhjvbdsd5\n");
			return false;
		}

		// int tamanho = 93 + strlen(email), TAMANHO_ID_PRODUTO + 1;
		int tamanho = 104 + strlen(email);
		char *query = (char *)malloc(sizeof(char) * tamanho);
		if(query == NULL)
		{
			printf(" Warning: não foi possivel alocar memoria para query em OperacoesBanco.h addFeedBackDeProdutoAoBanco() asdjkhkvdn");
			return false;
		}
		snprintf(query, tamanho, "SELECT F.idfeedBackCliente FROM feedBackCliente F WHERE F.emailCliente=\'%s\' AND F.idproduto=\'%s\';", email, idProduto);
		if(query == NULL)
		{
			printf(" Warning: não foi possivel formatar query em OperacoesBanco.h addFeedBackDeProdutoAoBanco() asdjkhkvdn");
			return false;
		}
		if(queryRetornaConteudo(query))
		{
			printf(" LOG: Feedback existente detectado em OperacoesBanco.h addFeedBackDeProdutoAoBanco() asdjhjkbvscf\n");
			// tamanho = 115 + strlen(titulo) + strlen(conteudo) + strlen(email) + TAMANHO_ID_PRODUTO + 1;
			tamanho = 126 + strlen(titulo) + strlen(conteudo) + strlen(email);
			query = (char *)malloc(sizeof(char) * tamanho);
			if(query == NULL)
			{
				printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addFeedBackDeProdutoAoBanco() asdjhkvdsdf65025s\n");
				return false;
			}
			snprintf(query, tamanho, "UPDATE feedBackCliente F SET F.titulo=\'%s\', F.conteudo=\'%s\' WHERE F.emailCliente=\'%s\' AND F.idproduto=\'%s\';", titulo, conteudo, email, idProduto);
			if(query == NULL)
			{
				printf(" Warning: falha ao formatar query em OperacoesBanco.h addFeedBackDeProdutoAoBanco() bhjvbhjcfdsfr\n");
				return false;
			}
			if(!executaQuery(query))
			{
				printf(" Warning: Ocorreram erros ao executar a query em OperacoesBanco.h addFeedBackDeProdutoAoBanco() 1684ws89bvd\n");
				free(query);
				query = NULL;
				return false;
			}
			else
			{
				printf(" LOG: Dados adicionados ao banco com sucesso em OperacoesBanco.h addFeedBackDeProdutoAoBanco() sadjhkvdsg\n");
				free(query);
				query = NULL;
				return true;
			}
		}
		else
		{
			printf(" LOG: Adicionando conteudo ao banco de dados em OperacoesBanco.h addFeedBackDeProdutoAoBanco() sadjhkvrd\n");
			// tamanho = 88 + strlen(titulo) + strlen(conteudo) + strlen(email) + TAMANHO_ID_PRODUTO + 1;
			tamanho = strlen(titulo) + strlen(conteudo) + strlen(email) + 99;//OTIMIZADO
			query = (char *)malloc(sizeof(char) * tamanho);
			if (query == NULL)
			{
				printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h addFeedBackDeProdutoAoBanco() sadkhjgvhresd\n");
				return false;
			}
			snprintf(query, tamanho, "INSERT INTO feedBackCliente(titulo,conteudo,emailCliente,idproduto) VALUES(\'%s\',\'%s\',\'%s\',\'%s\');", titulo, conteudo, email, idProduto);
			if (query == NULL)
			{
				printf(" Warning: Falha ao formatar query para ser executada em OperacoesBanco.h addFeedBackDeProdutoAoBanco() sadhjgjbvrg5456f\n");
				return false;
			}
			if(executaQuery(query))
			{
				printf(" LOG: Produto adicionado ao banco de dados com sucesso em OperacoesBanco.h addFeedBackDeProdutoAoBanco() asdhjkvxd\n");
				free(query);
				query = NULL;
				return true;
			}
			else
			{
				printf(" Warning: Falha ao adicionar produto ao banco de dados em OperacoesBanco.h addFeedBackDeProdutoAoBanco() sadjhksafhjkgbr\n");
				free(query);
				query = NULL;
				return false;
			}
		}
	}
	printf(" ERRO: erro desconhecido detectado em OperacoesBanco.h addFeedBackDeProdutoAoBanco() ajkhfjgsdf462\n");
	return false;
}


bool addPontosDeUsuarioAoBanco(char *email, char *quantidade)
{
	if (conexao == NULL)
	{
		printf(" Warning: Conexao nula detectada em OperacoesBanco.h addPontosDeUsuarioAoBanco() 56456q489b87ts\n");
		printf(" LOG: Tentando reconexão com banco de dados em OperacoesBanco.h addPontosDeUsuarioAoBanco()\n");
		if(conectarBanco())
		{
			printf(" \tLOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" \tWarning: Falha ao reconectar-se, encerrando interpretação\n");
			return false;
		}
	}
	if(email == NULL)
	{
		printf(" Warning: email == NULL em OperacoesBanco.h addPontosDeUsuarioAoBanco() cqufh20nasdi\n");
		return false;
	}
	if(quantidade == NULL)
	{
		printf(" Warning: quantidade == NULL em OperacoesBanco.h addPontosDeUsuarioAoBanco() cqufh20nasdi\n");
		return false;
	}

	// int tamanho =  57 + strlen(email) + strlen(quantidade) + 1;
	int tamanho = 58 + strlen(email) + strlen(quantidade);// que merda de otimizacao hein ?
	char *query = (char *)malloc(sizeof(char) * tamanho);
	if (query == NULL)
	{
		printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addPontosDeUsuarioAoBanco() vq4b0s8sq\n");
		return false;
	}
	snprintf(query, tamanho, "UPDATE cliente C SET C.pontos=C.pontos+%s WHERE C.email=\'%s\';", quantidade, email);
	if (query == NULL)
	{
		printf(" Warning: falha ao formatar query em OperacoesBanco.h addPontosDeUsuarioAoBanco() ab4iua13tb\n");
		return false;
	}
	if(executaQuery(query))
	{
		printf(" LOG: Pontos atualizados com sucesso em OperacoesBanco.h addPontosDeUsuarioAoBanco() qbv98hqf\n");
		free(query);
		query = NULL;
		return true;
	}
	else
	{
		printf(" Warning: Falha ao atualizar pontos no banco de dados em OperacoesBanco.h addPontosDeUsuarioAoBanco() ahr8q9888xxai1b\n");
		free(query);
		query = NULL;
		return false;
	}
}

#endif

