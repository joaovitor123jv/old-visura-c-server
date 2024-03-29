// #pragma once
#ifndef __OPERACOES_BANCO__
#define __OPERACOES_BANCO__

#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
#include "../Bibliotecas/Usuario.h"
// #include "../Comandos/Comando-Atualizar.h"
//#include <stdarg.h>

bool interno_atualizarQuantidadeDeHabitantesDaCidade(Usuario *usuario, char *nomeDaCidade, char *nomeDoEstado);

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

/* COMANDOS DE ADICAO */

//Retorna TRUE se o login estiver correto
//bool checarLogin(MYSQL *conexao, char *email)//Funcional
//bool checarLogin(char *email)//OK

bool addUsuarioAnonimoAoBanco(Usuario *usuario, char *email)//DONE
{
	if(email == NULL)
	{
		geraLog(ERRO, "ERRO: email passado igual a NULL OperacoesBanco.h addUsuarioAnonimoAoBanco()");
		return false;
	}

	if(usuario->conexao == NULL)
	{
		geraLog(ERRO, "Não há conexao com o banco de dados OperacoesBanco.h addUsuarioAnonimoAoBanco()");
		geraLog(LOG, "Tentando reconexão com banco de dados");
		if(usuario_conectarBanco(usuario))
		{
			geraLog(LOG, "Reconectado com sucesso, continuando interpretação");
		}
		else
		{
			geraLog(WARNING, "Falha ao recontectar-se, encerrando interpretação");
			return false;
		}
	}
	geraLog(LOG, "Chegou com sucesso até a adição de usuario ao banco (anonimo)");


	char *query = NULL;
	//int tamanho = sizeof(char) * (47 + strlen(email) + TAMANHO_SENHA_PADRAO_USUARIO_ANONIMO + 1);//O +1 é do '/0'
	int tamanho = sizeof(char) * (52 + strlen(email));//otimizado
	query = (char *)malloc(tamanho);

	if(query == NULL)
	{
		geraLog(WARNING, "Falha ao alocar memoria para query");
		return false;
	}
	snprintf(query, tamanho, "INSERT INTO cliente(email,senha) VALUES(\'%s\',\'nulo\');", email);

	if(query == NULL)
	{
		geraLog(ERRO, "Não foi possível alocar memória para a query");
		return false;
	}

	if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
	{
		geraLog(ERRO, "Ocorreram erros durante a execução da query");
		printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
		printf("\t Query enviada =  |%s|\n", query);
		free(query);
		query = NULL;
		if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
		{
			geraLog(LOG, "Tentando reconexão com o banco de dados");
			if(usuario_conectarBanco(usuario))
			{
				geraLog(LOG, " Re-conexão efetuada com sucesso");
			}
			else
			{
				usuario->conexao = NULL;
				mysql_close(usuario->conexao);
				mysql_thread_end();
				free(usuario->conexao);
				geraLog(ERRO, "Não foi possivel reconectar-se ao banco de dados");
			}
		}
		return false;
	}
	free(query);
	query = NULL;
	return true;
}

bool addUsuarioAoBanco(Usuario *usuario, char *email, char *senha, char* sexo, char *dataNascimento)//DONE
{
	if(usuario->conexao == NULL)
	{
		geraLog(WARNING, "Conexao com o banco de dados é nula, tentando reconexão");
		if(usuario_conectarBanco(usuario))
		{
			geraLog(LOG, "Reconectado com sucesso, continuando interpretação");
		}
		else
		{
			geraLog(WARNING, "Falha ao reconectar-se, encerrando interpretação");
			return false;
		}
	}
	// if(emailAnterior == NULL)
	// {
	// 	printf(" ERRO: email anterior não foi informado devidamente em OperacoesBanco.h addUsuarioAoBanco() 456q5w1c\n");
	// 	return false;
	// }
	if (usuario == NULL)
	{
		geraLog(ERRO, "Usuario nulo detectado");
		return false;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		geraLog(WARNING, "Usuario nao conectado detectado");
		return false;
	}
	if(email == NULL)
	{
		geraLog(ERRO, "email nulo detectado");
		return false;
	}
	if(senha == NULL)
	{
		geraLog(ERRO, "Senha nula detectada");
		return false;
	}

	geraLog(LOG, "Operação em testes → OperacoesBanco.h addUsuarioAoBanco()");

	char *query = NULL;

	size_t tamanho = 0;
	tamanho = 63 + usuario_obterTamanhoLogin(usuario) + 1;

	query = (char *)malloc(sizeof(char) * tamanho);

	if(query == NULL)
	{
		geraLog(WARNING, "Não foi possivel alocar memoria para query");
		return false;
	}

	snprintf(query, tamanho, "SELECT C.idcliente FROM cliente C WHERE C.email LIKE BINARY \'%s\';", usuario_obterLogin(usuario));

	if(query == NULL)
	{
		geraLog(WARNING, "Não foi possivel formatar string para query");
		return false;
	}

	if(!queryRetornaConteudo(usuario, query))/* Query já é liberada após essa função */
	{
		geraLog(WARNING, "Não existe nenhum usuario anonimo com esse email cadastrado");
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
			tamanho = strlen(senha) + strlen(dataNascimento) + strlen(sexo) + strlen(email) + usuario_obterTamanhoLogin(usuario) + 126;

			query = (char *)malloc(sizeof(char) * tamanho);

			if(query == NULL)
			{
				geraLog(WARNING, "Não foi possivel alocar memoria para query");
				return false;
			}

			snprintf(query, tamanho, "UPDATE cliente C SET C.datanascimento=STR_TO_DATE(\'%s\',\'%%d/%%m/%%Y\'),C.sexo=%s,C.senha=\'%s\',C.email=\'%s\' WHERE C.email LIKE BINARY \'%s\';", dataNascimento, sexo, senha, email, usuario_obterLogin(usuario));


			if(query == NULL)
			{
				geraLog(WARNING, "Não foi possivel formatar string para query");
				return false;
			}

			if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
			{
				geraLog(ERRO, "Ocorreram erros durante a execução da query");
				printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
				printf("\t Query enviada =  |%s|\n", query);
				free(query);
				query = NULL;
				if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
				{
					geraLog(LOG, "Tentando reconexão com o banco de dados");
					if(usuario_conectarBanco(usuario))
					{
						geraLog(LOG, "Re-conexão efetuada com sucesso");
					}
					else
					{
						usuario->conexao = NULL;
						mysql_close(usuario->conexao);
						mysql_thread_end();
						free(usuario->conexao);
						geraLog(ERRO, "Nao foi possível reconectar-se ao banco de dados");
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
			tamanho = strlen(senha) + strlen(email) + usuario_obterTamanhoLogin(usuario) + 73;

			query = (char *)malloc(sizeof(char) * tamanho);

			if(query == NULL)
			{
				geraLog(WARNING, "Não foi possivel alocar memoria para query");
				return false;
			}

			snprintf(query, tamanho, "UPDATE cliente C SET C.senha=\'%s\',C.email=\'%s\' WHERE C.email LIKE BINARY \'%s\';", senha, email, usuario_obterLogin(usuario));


			if(query == NULL)
			{
				geraLog(WARNING, "Não foi possível formatar string para query");
				return false;
			}

			if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
			{
				geraLog(ERRO, "Ocorreram erros durante a execução da query");
				printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
				printf("\t Query enviada =  |%s|\n", query);
				free(query);
				query = NULL;
				if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
				{
					geraLog(LOG, "Tentando reconexão com o banco de dados");
					if(usuario_conectarBanco(usuario))
					{
						geraLog(LOG, "Re-conexão com o banco de dados efetuada com sucesso");
					}
					else
					{
						usuario->conexao = NULL;
						mysql_close(usuario->conexao);
						mysql_thread_end();
						free(usuario->conexao);
						geraLog(ERRO, "Não foi possível reconectar-se ao banco de dados");
					}
				}
				return false;
			}
			free(query);
			query = NULL;
			return true;
		}


	}


	geraLog(ERRO, "O pior erro aconteceu, todos os ifs falharam");
	return false;
}

bool addCidadeAoBanco(Usuario *usuario, char *nomeDoEstado, char *nomeCidade)//DONE
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
	if (!usuario_conexaoAtiva(usuario))
	{
		printf(" Warning: Conexao com o banco de dados é inexistente em OperacoesBanco.h addCidadeAoBanco()\n");
		return false;
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
	if(!queryRetornaConteudo(usuario, query))
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
			if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
			{
				printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addCidadeAoBanco())\n");
				printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
				printf("\t Query enviada =  |%s|\n", query);
				free(query);
				query = NULL;
				if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
				{
					printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addCidadeAoBanco() 1qa5456dasd\n");
					if(usuario_conectarBanco(usuario))
					{
						printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addCidadeAoBanco() asd456\n");
					}
					else
					{
						usuario->conexao = NULL;
						mysql_close(usuario->conexao);
						mysql_thread_end();
						free(usuario->conexao);
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

bool addLocalizacaoAoBanco(Usuario *usuario, char* idCidade, char *cep, char *bairro, char *rua, char *numero, char *complemento)// TODO Limitar criação de localizacoes repetidas
{
	if (!usuario_conexaoAtiva(usuario))
	{
		printf(" Warning: Conexao com o banco de dados é inexistente em OperacoesBanco.h addLocalizacaoAoBanco()\n");
		return false;
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
					if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(usuario_conectarBanco(usuario))
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								usuario->conexao = NULL;
								mysql_close(usuario->conexao);
								mysql_thread_end();
								free(usuario->conexao);
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
					if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(usuario_conectarBanco(usuario))
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								usuario->conexao = NULL;
								mysql_close(usuario->conexao);
								mysql_thread_end();
								free(usuario->conexao);
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
					if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(usuario_conectarBanco(usuario))
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								usuario->conexao = NULL;
								mysql_close(usuario->conexao);
								mysql_thread_end();
								free(usuario->conexao);
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
					if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(usuario_conectarBanco(usuario))
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								usuario->conexao = NULL;
								mysql_close(usuario->conexao);
								mysql_thread_end();
								free(usuario->conexao);
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
					if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(usuario_conectarBanco(usuario))
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								usuario->conexao = NULL;
								mysql_close(usuario->conexao);
								mysql_thread_end();
								free(usuario->conexao);
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
					if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(usuario_conectarBanco(usuario))
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								usuario->conexao = NULL;
								mysql_close(usuario->conexao);
								mysql_thread_end();
								free(usuario->conexao);
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
					if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(usuario_conectarBanco(usuario))
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								usuario->conexao = NULL;
								mysql_close(usuario->conexao);
								mysql_thread_end();
								free(usuario->conexao);
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
					if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(usuario_conectarBanco(usuario))
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								usuario->conexao = NULL;
								mysql_close(usuario->conexao);
								mysql_thread_end();
								free(usuario->conexao);
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
					if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(usuario_conectarBanco(usuario))
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								usuario->conexao = NULL;
								mysql_close(usuario->conexao);
								mysql_thread_end();
								free(usuario->conexao);
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
					if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(usuario_conectarBanco(usuario))
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								usuario->conexao = NULL;
								mysql_close(usuario->conexao);
								mysql_thread_end();
								free(usuario->conexao);
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
					if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(usuario_conectarBanco(usuario))
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								usuario->conexao = NULL;
								mysql_close(usuario->conexao);
								mysql_thread_end();
								free(usuario->conexao);
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
					if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(usuario_conectarBanco(usuario))
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								usuario->conexao = NULL;
								mysql_close(usuario->conexao);
								mysql_thread_end();
								free(usuario->conexao);
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
					if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(usuario_conectarBanco(usuario))
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								usuario->conexao = NULL;
								mysql_close(usuario->conexao);
								mysql_thread_end();
								free(usuario->conexao);
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
					if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(usuario_conectarBanco(usuario))
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								usuario->conexao = NULL;
								mysql_close(usuario->conexao);
								mysql_thread_end();
								free(usuario->conexao);
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
					if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(usuario_conectarBanco(usuario))
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								usuario->conexao = NULL;
								mysql_close(usuario->conexao);
								mysql_thread_end();
								free(usuario->conexao);
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
					if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
					{
						printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addLocalizacaoAoBanco())\n");
						printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
						printf("\t Query enviada =  |%s|\n", query);
						free(query);
						query = NULL;
						if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
						{
							printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addLocalizacaoAoBanco() 1qa5456dasd\n");
							if(usuario_conectarBanco(usuario))
							{
								printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addLocalizacaoAoBanco() asd456\n");
							}
							else
							{
								usuario->conexao = NULL;
								mysql_close(usuario->conexao);
								mysql_thread_end();
								free(usuario->conexao);
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

bool addContratanteAoBanco(Usuario *usuario, char *nome, char *cnpj, char *plano, char *email, char *telefone, char *idLocalizacao, char *senha)//DONE
{
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
	if (!usuario_conexaoAtiva(usuario))
	{
		printf(" Warning: Conexao com o banco de dados é inexistente em OperacoesBanco.h addContratanteAoBanco()\n");
		return false;
	}

	int tamanho;
	char *query = NULL;


	/* Checa se a já existe empresa com esse cnpj e esse email cadastrada */
	/* QUERYs para checagem de dados corretos */
	tamanho = 82 + strlen(email) + strlen(cnpj) + 1; // Esse +1 é o do bendito \0 kkk
	query = (char *)malloc(sizeof(char) * tamanho);

	if(query == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para query em addContratanteAoBanco() OperacoesBanco.h (qbbabdkhhhf)");
		return false;
	}
	else
	{
		snprintf(query, tamanho, "SELECT idcontratante FROM contratante C WHERE C.email LIKE BINARY \'%s\' OR C.cnpj=\'%s\';", email, cnpj);
		if(queryRetornaConteudo(usuario, query))
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
					if(queryRetornaConteudo(usuario, query))
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
		tamanho = 101 + 1 + strlen(plano) + strlen(cnpj) + strlen(nome) + strlen(email) + strlen(idLocalizacao) + strlen(senha);
		query = (char *)malloc(sizeof(char) * tamanho );
		if(query == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para query em addContratanteAoBanco() OperacoesBanco.h\n");
			return false;
		}
		else
		{
			snprintf(query, tamanho, "INSERT INTO contratante(email,plano,cnpj,nome,localizacao_idlocalizacao,senha) VALUES(\'%s\',%s,\'%s\',\'%s\',%s,\'%s\');", email, plano, cnpj, nome, idLocalizacao, senha);
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

		tamanho = 113 + 1 + strlen(plano) + strlen(cnpj) + strlen(nome) + strlen(email) + strlen(telefone) + strlen(idLocalizacao) + strlen(senha);
		query = (char *)malloc(sizeof(char) * tamanho );
		if(query == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para query em addContratanteAoBanco() OperacoesBanco.h (dasbkdbqwjkd)\n");
			return false;
		}
		else
		{
			snprintf(query, tamanho, "INSERT INTO contratante(email,plano,cnpj,nome,telefone,localizacao_idlocalizacao,senha) VALUES(\'%s\',%s,\'%s\',\'%s\',\'%s\',%s,\'%s\');", email, plano, cnpj, nome, telefone, idLocalizacao, senha);
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

	if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
	{
		printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addContratanteAoBanco())\n");
		printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
		printf("\t Query enviada =  |%s|\n", query);
		free(query);
		query = NULL;
		if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
		{
			printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addContratanteAoBanco() 1qa5456dasd\n");
			if(usuario_conectarBanco(usuario))
			{
				printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addContratanteAoBanco() asd456\n");
			}
			else
			{
				usuario->conexao = NULL;
				mysql_close(usuario->conexao);
				mysql_thread_end();
				free(usuario->conexao);
				printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addContratanteAoBanco() as4d5asd\n");
			}
		}
		return false;
	}
	free(query);
	query = NULL;
	return true;
}

bool addProdutoAoBanco(Usuario *usuario, char *idContratante, char *idProduto, char *duracao, char *nomeProduto, char *descricao, char *tipoProduto, char *categoria)//DONE COM PROBLEMA
{
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

	if (tipoProduto == NULL)
	{
		printf(" Warning: Tipo de produto não especificado em addProdutoAoBanco() OperacoesBanco.h asbvur98as7fe\n");
		return false;
	}
	if (!usuario_conexaoAtiva(usuario))
	{
		printf(" Warning: Conexao com o banco de dados é inexistente em OperacoesBanco.h addProdutoAoBanco()\n");
		return false;
	}

	if(checarIdContratante(usuario, idContratante))
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

	if (categoria == NULL)// Se a categoria nao for informada
	{
		if(descricao != NULL)
		{
			// Se a descricao for informada
			//tamanho = 127 + 1 + strlen(idContratante) + strlen(idProduto) + strlen(duracao) + strlen(nomeProduto) + strlen(descricao) + strlen(tipoProduto) + strlen(categoria);
			tamanho = 145 + strlen(idContratante) + strlen(duracao) + strlen(nomeProduto) + strlen(descricao) + strlen(tipoProduto);//Otimizado
			query = (char *)malloc(sizeof(char) * tamanho);

			if(query == NULL)
			{
				printf(" Warning: Não foi possivel alocar memoria para query em addProdutoAoBanco() OperacoesBanco.h 3kjhv73b\n");
				return false;
			}

			snprintf(query, tamanho, "INSERT INTO produto(idproduto,nomeproduto,duracao,contratante_idcontratante,descricao,tipo) VALUES(\'%s\',\'%s\',%s,%s,\'%s\',\'%s\');", idProduto, nomeProduto, duracao, idContratante, descricao, tipoProduto);
			if(query == NULL)
			{
				printf(" Warning: Não foi possivel formatar query em addProdutoAoBanco() OperacoesBanco.h c3jjhvr87jh\n");
				return false;
			}

			if (executaQuery(usuario, query))
			{
				printf(" LOG: Produto adicionado com sucesso ao banco de dados em OperacoesBanco.h addProdutoAoBanco() sakfjhgrasd\n");
				printf(" LOG: Produto foi adicionado contendo: idProduto, nomeProduto, duracao, contratante_idcontratante, descricao, tipoProduto\n");
				printf("\n");
				free(query);
				query = NULL;
				return true;
			}
			else
			{
				printf(" Warning: Falha ao adicionar produto ao banco de dados em OperacoesBanco.h addProdutoAoBanco() sadkjhrasd\n");
				free(query);
				query = NULL;
				return false;
			}
		}
		else
		{
			tamanho = 112 + strlen(idContratante) + strlen(duracao) + strlen(nomeProduto) + strlen(tipoProduto);//Otimizado
			query = (char *)malloc(sizeof(char) * tamanho);

			if(query == NULL)
			{
				printf(" Warning: Não foi possivel alocar memoria para query em addProdutoAoBanco() OperacoesBanco.h 3kjhv73b\n");
				return false;
			}

			snprintf(query, tamanho, "INSERT INTO produto(idproduto,nomeproduto,duracao,contratante_idcontratante,tipo) VALUES(\'%s\',\'%s\',%s,%s,\'%s\');", idProduto, nomeProduto, duracao, idContratante, tipoProduto);

			if(query == NULL)
			{
				printf(" Warning: Não foi possivel formatar query em addProdutoAoBanco() OperacoesBanco.h c3jjhvr87jh\n");
				return false;
			}

			if(mysql_query(usuario->conexao, query))//Se ocorrer algum erro
			{
				printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addProdutoAoBanco())\n");
				printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
				printf("\t Query enviada =  |%s|\n", query);
				free(query);
				query = NULL;
				if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
				{
					printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addProdutoAoBanco() 1qa5456dasd\n");
					if(usuario_conectarBanco(usuario))
					{
						printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addProdutoAoBanco() asd456\n");
					}
					else
					{
						usuario->conexao = NULL;
						mysql_close(usuario->conexao);
						mysql_thread_end();
						free(usuario->conexao);
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
	else// Se a categoria for informada
	{
		if(descricao == NULL)// Descricao nao informada
		{
			// tamanho =114 + strlen(categoria) + strlen(tipoProduto) + strlen(idContratante) + strlen(idProduto) + strlen(nomeProduto) + strlen(duracao) + 1;
			tamanho = 125 + strlen(categoria) + strlen(tipoProduto) + strlen(idContratante) + strlen(nomeProduto) + strlen(duracao);
			query = (char *)malloc(sizeof(char) * tamanho);
			if (query == NULL)
			{
				printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h addProdutoAoBanco() sakjdhgrj\n");
				return false;
			}
			snprintf(query, tamanho, "INSERT INTO produto(idproduto,nomeproduto,duracao,contratante_idcontratante,tipo,categoria) VALUES(\'%s\',\'%s\',%s,%s,\'%s\',\'%s\');", idProduto, nomeProduto, duracao, idContratante, tipoProduto, categoria);
			if (query == NULL)
			{
				printf(" Warning: Falha ao formatar string para query em OperacoesBanco.h addProdutoAoBanco() asivrhasd\n");
				return false;
			}
			if (executaQuery(usuario, query))
			{
				printf(" LOG: Produto adicionado com sucesso ao banco de dados em OperacoesBanco.h addProdutoAoBanco() \n");
				free(query);
				query = NULL;
				return true;
			}
			else
			{
				printf(" Warning: Falha ao adicionar produto ao banco de dados em OperacoesBanco.h addProdutoAoBanco() askjghedsa");
				free(query);
				query = NULL;
				return false;
			}
		}
		else// Categoria e descricao informados
		{
			// Se a descricao for informada
			//tamanho = 127 + 1 + strlen(idContratante) + strlen(idProduto) + strlen(duracao) + strlen(nomeProduto) + strlen(descricao) + strlen(tipoProduto) + strlen(categoria);
			tamanho = 138 + strlen(idContratante) + strlen(duracao) + strlen(nomeProduto) + strlen(descricao) + strlen(tipoProduto) + strlen(categoria);//Otimizado
			query = (char *)malloc(sizeof(char) * tamanho);

			if(query == NULL)
			{
				printf(" Warning: Não foi possivel alocar memoria para query em addProdutoAoBanco() OperacoesBanco.h 3kjhv73b\n");
				return false;
			}

			snprintf(query, tamanho, "INSERT INTO produto(idproduto,nomeproduto,duracao,contratante_idcontratante,descricao,tipo,categoria) VALUES(\'%s\',\'%s\',%s,%s,\'%s\',\'%s\',\'%s\');", idProduto, nomeProduto, duracao, idContratante, descricao, tipoProduto,categoria);

			if(query == NULL)
			{
				printf(" Warning: Não foi possivel formatar query em addProdutoAoBanco() OperacoesBanco.h c3jjhvr87jh\n");
				return false;
			}

			if (executaQuery(usuario, query))
			{
				printf(" LOG: Produto adicionado com sucesso ao banco de dados em OperacoesBanco.h addProdutoAoBanco() sakfjhgrasd\n");
				printf(" LOG: Produto foi adicionado contendo: idProduto, nomeProduto, duracao, contratante_idcontratante, descricao, tipoProduto, categoria\n");
				printf("\n");
				free(query);
				query = NULL;
				return true;
			}
			else
			{
				printf(" Warning: Falha ao adicionar produto ao banco de dados em OperacoesBanco.h addProdutoAoBanco() sadkjhrasd\n");
				free(query);
				query = NULL;
				return false;
			}
		}
	}
}

bool addProdutoAListaDeDesejosDoClienteAoBanco(Usuario *usuario, char *idProduto)
{
	if (idProduto == NULL)
	{
		printf(" Warning: Produto nulo detectado em OperacoesBanco.h addProdutoAListaDeDesejosDoClienteAoBanco() r0o9sad0890br\n");
		return false;
	}
	if (usuario == NULL)
	{
		printf(" Warning: usuario nulo detectado em OperacoesBanco.h addProdutoAListaDeDesejosDoClienteAoBanco() aqo8ry8bdsf\n");
		free(idProduto);
		idProduto = NULL;
		return false;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" Warning: usuario não conectado detectado em OperacoesBanco.h addProdutoAListaDeDesejosDoClienteAoBanco() asjkdhsajkd\n");
		free(idProduto);
		idProduto = NULL;
		return false;
	}
	int tamanho = 97 + strlen(idProduto)+ strlen(usuario_obterId(usuario)) + 1;
	char *query = (char *)malloc(sizeof(char) * tamanho);

	//Checa se o usuario já possui o item na lista de desejos   _START_
	if (query == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h addProdutoAListaDeDesejosDoClienteAoBanco() aoijb89asdfe\n");
		free(idProduto);
		idProduto = NULL;
		return false;
	}
	snprintf(query, tamanho, "SELECT L.idproduto FROM listaDesejos L WHERE L.cliente_idcliente=%s AND L.idproduto LIKE BINARY \'%s\';", usuario_obterId(usuario), idProduto);
	if (query == NULL)
	{
		printf(" Warning: Falha ao formatar query para execução em OperacoesBanco.h addProdutoAListaDeDesejosDoClienteAoBanco() asadvgreihuskjrbhasdhy");
		free(idProduto);
		idProduto = NULL;
		return false;
	}
	if (queryRetornaConteudo(usuario, query))
	{
		printf(" LOG: Cliente já possui produto em sua lista de desejos em OperacoesBanco.h addProdutoAListaDeDesejosDoClienteAoBanco() aqoifub980as\n");
		free(idProduto);
		idProduto = NULL;
		return false;
	}//Checa se o usuario já possui o item na lista de desejos _END_
	else
	{
		printf(" LOG: Cliente ainda nao possui produto na sua lista de desejos em OperacoesBanco.h addProdutoAListaDeDesejosDoClienteAoBanco() advr09981\n");
		// tamanho = 67 + strlen(usuario_obterId(usuario)) + strlen(idProduto) + 1;
		tamanho = 78 + strlen(usuario_obterId(usuario));
		query = (char *)malloc(sizeof(char) * tamanho);
		if (query == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h addProdutoAListaDeDesejosDoClienteAoBanco() v987asd89v4vbjhgvcylç\n");
			free(idProduto);
			idProduto = NULL;
			return false;
		}
		snprintf(query, tamanho, "INSERT INTO listaDesejos(idProduto, cliente_idcliente) VALUES(\'%s\',%s);", idProduto, usuario_obterId(usuario));
		if (query == NULL)
		{
			printf(" Warning: Falha ao formatar query em OperacoesBanco.h addProdutoAListaDeDesejosDoClienteAoBanco() vr408dybn7tneweuih6\n");
			free(idProduto);
			idProduto = NULL;
			return false;
		}
		if (!executaQuery(usuario, query))
		{
			printf(" Warning: falha ao executar query, produto não adicionado à lista de desejos em OperacoesBanco.h addProdutoAListaDeDesejosDoClienteAoBanco() dwebr5990nuhsjy\n");
			free( query );
			free( idProduto );
			query = NULL;
			idProduto = NULL;
			return false;
		}
		else
		{
			printf(" LOG: produto adicionaro à lista de desejos em OperacoesBanco.h addProdutoAListaDeDesejosDoClienteAoBanco() 0wrh8h9hjgidsaohu6uj\n");
			free( query );
			free( idProduto );
			query = NULL;
			idProduto = NULL;
			return true;
		}
	}

}

bool addNomeDeUsuarioAoBanco(Usuario *usuario, char *nome)// TESTAR        APP 2 & # nome
{
	if (usuario == NULL)
	{
		printf(" Warning: Usuario nulo detectado em OperacoesBanco.h addNomeDeUsuarioAoBanco()\n");
		return false;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" ERRO: Usuario não conectado detectado em OperacoesBanco.h addNomeDeUsuarioAoBanco()\n");
		return false;
	}
	if(nome == NULL)
	{
		printf(" Warning: Nome recebido é nulo em OperacoesBanco.h addNomeDeUsuarioAoBanco() 4654egg\n");
		return false;
	}
	if (!usuario_conexaoAtiva(usuario))
	{
		printf(" Warning: Conexao com o banco de dados é inexistente em OperacoesBanco.h addNomeDeUsuarioAoBanco()\n");
		return false;
	}


	char *query = NULL;
	// int tamanho = strlen(email) + strlen(nome) + 48 +1;
	int tamanho = usuario_obterTamanhoLogin(usuario) + strlen(nome) + 61;
	query = (char *)malloc(sizeof(char) * tamanho);
	if(query == NULL)
	{
		printf(" Warning: nao foi possível alocar memoria para query em OperacoesBanco.h addNomeDeUsuarioAoBanco()\n");
		return false;
	}

	snprintf(query, tamanho, "UPDATE cliente C SET C.nome=\'%s\' WHERE C.email LIKE BINARY \'%s\';", nome, usuario_obterLogin(usuario));

	if(query == NULL)
	{
		printf(" Warning: nao foi possível formatar query em OperacoesBanco.h addNomeDeUsuarioAoBanco() cqhkhbjvr\n");
		return false;
	}

	if (!executaQuery(usuario, query))
	{
		printf(" Warning: Falha ao adicionar nome de usuario ao banco de dados em OperacoesBanco.h addNomeDeUsuarioAoBanco()\n");
		free(query);
		query = NULL;
		return false;
	}
	else
	{
		printf(" LOG: Nome de Usuario adicionado com sucesso ao banco de dados em OperacoesBanco.h addNomeDeUsuarioAoBanco()\n");
		free(query);
		query = NULL;
		return true;
	}
}

bool addSobrenomeDeUsuarioAoBanco(Usuario *usuario, char *sobrenome)//TESTAR          APP 2 & Y sobrenome
{
	if (usuario == NULL)
	{
		printf(" ERRO: Usuario nulo detectado em OperacoesBanco.h addSobrenomeDeUsuarioAoBanco()\n");
		return false;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" ERRO: Usuario não conectado detectado em OperacoesBanco.h addSobrenomeDeUsuarioAoBanco()\n");
		return false;
	}
	if(sobrenome == NULL)
	{
		printf(" Warning: Nome recebido é nulo em OperacoesBanco.h addSobrenomeDeUsuarioAoBanco() 4654egg\n");
		return false;
	}
	if (!usuario_conexaoAtiva(usuario))
	{
		printf(" Warning: Conexao com o banco de dados é inexistente em OperacoesBanco.h addSobrenomeDeUsuarioAoBanco()\n");
		return false;
	}


	char *query = NULL;
	// int tamanho = strlen(email) + strlen(sobrenome) + 53 +1;
	int tamanho = usuario_obterTamanhoLogin(usuario) + strlen(sobrenome) + 66;
	query = (char *)malloc(sizeof(char) * tamanho);
	if(query == NULL)
	{
		printf(" Warning: nao foi possível alocar memoria para query em OperacoesBanco.h addSobrenomeDeUsuarioAoBanco()\n");
		return false;
	}

	snprintf(query, tamanho, "UPDATE cliente C SET C.sobrenome=\'%s\' WHERE C.email LIKE BINARY \'%s\';", sobrenome, usuario_obterLogin(usuario));

	if(query == NULL)
	{
		printf(" Warning: nao foi possível formatar query em OperacoesBanco.h addSobrenomeDeUsuarioAoBanco() cqhkhbjvr\n");
		return false;
	}

	if(mysql_query(usuario->conexao, query))
	{

		printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addSobrenomeDeUsuarioAoBanco())\n");
		printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
		printf("\t Query enviada =  |%s|\n", query);
		free(query);
		query = NULL;
		if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
		{
			printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addSobrenomeDeUsuarioAoBanco() 1qa5456dasd\n");
			if(usuario_conectarBanco(usuario))
			{
				printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addSobrenomeDeUsuarioAoBanco() asd456\n");
			}
			else
			{
				usuario->conexao = NULL;
				mysql_close(usuario->conexao);
				mysql_thread_end();
				free(usuario->conexao);
				printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addSobrenomeDeUsuarioAoBanco() as4d5asd\n");
			}
		}
		return false;
	}
	else
	{
		printf(" LOG: sobrenome %s adicionado ao usuario %s com sucesso em Comando-Adicao.h addSobrenomeDeUsuarioAoBanco\n", sobrenome, usuario_obterLogin(usuario));
		free(query);
		query = NULL;
		return true;
	}

}

bool addSexoDeUsuarioAoBanco(Usuario *usuario, char *sexo)// TESTAR        APP 2 & SQ sexo
{
	if (usuario == NULL)
	{
		printf(" ERRO: usuario nulo detectado em OperacoesBanco.h addSexoDeUsuarioAoBanco()\n");
		return false;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" ERRO: usuario não conectado detectado em OperacoesBanco.h addSexoDeUsuarioAoBanco()\n");
		return false;
	}
	if(sexo == NULL)
	{
		printf(" Warning: Nome recebido é nulo em OperacoesBanco.h addSexoDeUsuarioAoBanco() 4654egg\n");
		return false;
	}
	if (usuario->conexao == NULL)
	{
		printf(" Warning: Conexao nula detectada em OperacoesBanco.h addSexoDeUsuarioAoBanco() 56456q489b87ts\n");
		printf(" LOG: Tentando reconexão com banco de dados em OperacoesBanco.h addSexoDeUsuarioAoBanco()\n");
		if(usuario_conectarBanco(usuario))
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
	int tamanho = usuario_obterTamanhoLogin(usuario) + strlen(sexo) + 61;
	query = (char *)malloc(sizeof(char) * tamanho);
	if(query == NULL)
	{
		printf(" Warning: nao foi possível alocar memoria para query em OperacoesBanco.h addSexoDeUsuarioAoBanco()\n");
		return false;
	}

	snprintf(query, tamanho, "UPDATE cliente C SET C.sexo=\'%s\' WHERE C.email LIKE BINARY \'%s\';", sexo, usuario_obterLogin(usuario));

	if(query == NULL)
	{
		printf(" Warning: nao foi possível formatar query em OperacoesBanco.h addSexoDeUsuarioAoBanco() cqhkhbjvr\n");
		return false;
	}

	if(mysql_query(usuario->conexao, query))
	{

		printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addSexoDeUsuarioAoBanco())\n");
		printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
		printf("\t Query enviada =  |%s|\n", query);
		free(query);
		query = NULL;
		if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
		{
			printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addSexoDeUsuarioAoBanco() 1qa5456dasd\n");
			if(usuario_conectarBanco(usuario))
			{
				printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addSexoDeUsuarioAoBanco() asd456\n");
			}
			else
			{
				usuario->conexao = NULL;
				mysql_close(usuario->conexao);
				mysql_thread_end();
				free(usuario->conexao);
				printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addSexoDeUsuarioAoBanco() as4d5asd\n");
			}
		}
		return false;
	}
	else
	{
		printf(" LOG: sexo %s adicionado ao usuario %s com sucesso em Comando-Adicao.h addSexoDeUsuarioAoBanco\n", sexo, usuario_obterLogin(usuario));
		free(query);
		query = NULL;
		return true;
	}

}

bool addDataNascimentoDeUsuarioAoBanco(Usuario * usuario, char *dataNascimento)
{
	if (usuario == NULL)
	{
		printf(" ERRO: Usuario nulo detectado em OperacoesBanco.h addDataNascimentoDeUsuarioAoBanco()\n");
		return false;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" ERRO: Usuario não conectado detectado em OperacoesBanco.h addDataNascimentoDeUsuarioAoBanco()\n");
		return false;
	}
	if(dataNascimento == NULL)
	{
		printf(" Warning: Data de nascimento não informada em OperacoesBanco.h addDataNascimentoDeUsuarioAoBanco()\n");
		return false;
	}
	if (usuario->conexao == NULL)
	{
		printf(" Warning: Conexao nula detectada em OperacoesBanco.h addDataNascimentoDeUsuarioAoBanco() 56456q489b87ts\n");
		printf(" LOG: Tentando reconexão com banco de dados em OperacoesBanco.h addDataNascimentoDeUsuarioAoBanco()\n");
		if(usuario_conectarBanco(usuario))
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
	int tamanho = usuario_obterTamanhoLogin(usuario) + strlen(dataNascimento) + 95;
	query = (char *)malloc(sizeof(char) * tamanho);
	if(query == NULL)
	{
		printf(" Warning: nao foi possível alocar memoria para query em OperacoesBanco.h addDataNascimentoDeUsuarioAoBanco()\n");
		return false;
	}

	snprintf(query, tamanho, "UPDATE cliente C SET C.dataNascimento=STR_TO_DATE(\'%s\',\'%%d/%%m/%%Y\') WHERE C.email LIKE BINARY \'%s\';", dataNascimento, usuario_obterLogin(usuario));

	if(query == NULL)
	{
		printf(" Warning: nao foi possível formatar query em OperacoesBanco.h addDataNascimentoDeUsuarioAoBanco() cqhkhbjvr\n");
		return false;
	}

	if(mysql_query(usuario->conexao, query))
	{

		printf("ERRO: Ocorreram erros durante a execução da query (OperacoesBanco.h) (addDataNascimentoDeUsuarioAoBanco())\n");
		printf("\t ERRO nº%d  ->  %s\n", mysql_errno(usuario->conexao), mysql_error(usuario->conexao));
		printf("\t Query enviada =  |%s|\n", query);
		free(query);
		query = NULL;
		if(mysql_errno(usuario->conexao) == 2006)// SERVER MYSQL SUMIU
		{
			printf(" LOG: Tentando reconexão com o banco de dados em OperacoesBanco.h addDataNascimentoDeUsuarioAoBanco() 1qa5456dasd\n");
			if(usuario_conectarBanco(usuario))
			{
				printf(" LOG: Re-conexão efetuada com sucesso em OperacoesBanco.h addDataNascimentoDeUsuarioAoBanco() asd456\n");
			}
			else
			{
				usuario->conexao = NULL;
				mysql_close(usuario->conexao);
				mysql_thread_end();
				free(usuario->conexao);
				printf(" ERRO: Não foi possível reconectar-se ao banco de dados em OperacoesBanco.h addDataNascimentoDeUsuarioAoBanco() as4d5asd\n");
			}
		}
		return false;
	}
	else
	{
		printf(" LOG: data de nascimento %s adicionado ao usuario %s com sucesso em Comando-Adicao.h addDataNascimentoDeUsuarioAoBanco\n", dataNascimento, usuario_obterLogin(usuario));
		free(query);
		query = NULL;
		return true;
	}
	return false;
}

bool addAvaliacaoAProdutoAoBanco(Usuario *usuario, char *idProduto, char *avaliacao)// APP 2 kW * idProduto avaliacao
{
	if (usuario->conexao == NULL)
	{
		printf(" Warning: Conexao nula detectada em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() 56456q489b87ts\n");
		printf(" LOG: Tentando reconexão com banco de dados em OperacoesBanco.h addAvaliacaoAProdutoAoBanco()\n");
		if(usuario_conectarBanco(usuario))
		{
			printf(" \tLOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" \tWarning: Falha ao reconectar-se, encerrando interpretação\n");
			return false;
		}
	}
	if(usuario == NULL)
	{
		printf(" Warning: usuario == NULL em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() 65q4w5e\n");
		return false;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" ERRO: usuario não conectado detectado em OperacoesBanco.h addAvaliacaoAProdutoAoBanco()\n");
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
	if(produtoVencido(idProduto, usuario))
	{
		printf(" Warning: produto vencido detectado em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() akjhvs\n");
		return false;
	}

	int checou = checaExistenciaDeVisualizacaoDeProdutoComPessoa(idProduto, usuario);
	bool visualizacaoJaExistia = false;
	if(checou == RETORNO_OK)
	{
		printf(" LOG: Existe visualização de usuario cadastrada em OperacoesBanco.h addAvaliacaoAProdutoAoBanco()\n");
		visualizacaoJaExistia = true;
	}
	else if(checou == RETORNO_NULO)
	{
		printf(" Warning: não existe visualização para esse usuario em OperacoesBanco.h addAvaliacaoAProdutoAoBanco()\n");
		if(addVisualizacoesAoBanco(idProduto, strdup("1"), usuario))
		{
			geraLog(LOG, "Visualização adicionada a usuario");
		}
		else
		{
			geraLog(ERRO, "Falha ao adicionar visualização ao usuario");
			return false;
		}
		// return false;
	}
	else
	{
		printf(" ERRO: Usuario ou produto não existem, ou algum outro erro possível em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() qkjhejcv\n");
		return false;
	}

	//int tamanho = 151 + strlen(email) + TAMANHO_ID_PRODUTO + 1;
	int tamanho = usuario_obterTamanhoLogin(usuario) + 186;//otimizado
	char *query = (char *)malloc(sizeof(char) * tamanho);

	

	if (query == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() q648484eas\n");
		return false;
	}
	snprintf(query, tamanho, "UPDATE visualizacaoDeUsuario V JOIN cliente C ON C.idcliente=V.cliente_idcliente SET V.avaliacaoDoUsuario=%c WHERE V.produto_idproduto LIKE BINARY \'%s\' AND C.email LIKE BINARY \'%s\';", avaliacao[0], idProduto, usuario_obterLogin(usuario) );
	if(query == NULL)
	{
		printf(" Warning: Falha ao formatar query para executar em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() qkjhhdjkasd\n");
		return false;
	}

	if(!executaQuery(usuario, query))
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


		if (visualizacaoJaExistia)
		{
			return true;//Proíbe usuário de alterar a visualização (por enquanto)
		}
		else
		{
			if(avaliacao[0] == '1')
			{
				//tamanho = 84+TAMANHO_ID_PRODUTO+1;
				tamanho = 107;//otimizado
				query = (char *)malloc(sizeof(char) * tamanho);
				if(query == NULL)
				{
					printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() akhkvejjd81779c\n");
					return false;
				}
				snprintf(query, tamanho, "UPDATE produto P SET P.avaliacaoPositiva=P.avaliacaoPositiva+1  WHERE P.idproduto LIKE BINARY \'%s\';", idProduto);
				if (query == NULL)
				{
					printf(" Warning: Falha ao formatar query em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() qbhbr25300sdf44*-asd\n");
					return false;
				}
				if(!executaQuery(usuario, query))
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
				tamanho = 107;//otimizado
				query = (char *)malloc(sizeof(char) * tamanho);
				if(query == NULL)
				{
					printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() akhkvejjd81779c\n");
					return false;
				}
				snprintf(query, tamanho, "UPDATE produto P SET P.avaliacaoPositiva=P.avaliacaoNegativa+1 WHERE P.idproduto LIKE BINARY \'%s\';", idProduto);
				if (query == NULL)
				{
					printf(" Warning: Falha ao formatar query em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() qbhbr25300sdf44*-asd\n");
					return false;
				}
				if(!executaQuery(usuario, query))
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
			else if(avaliacao[0] == '2')
			{
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
	}

	printf(" ERRO: DEU PAU !!!!! em OperacoesBanco.h addAvaliacaoAProdutoAoBanco() qjwhejikhjvs6545 \n");
	return false;
}

bool addFeedBackDeProdutoAoBanco(Usuario *usuario, char *idProduto, char *titulo, char *conteudo)
{
	if (usuario->conexao == NULL)
	{
		printf(" Warning: Conexao nula detectada em OperacoesBanco.h addFeedBackDeProdutoAoBanco() 56456q489b87ts\n");
		printf(" LOG: Tentando reconexão com banco de dados em OperacoesBanco.h addFeedBackDeProdutoAoBanco()\n");
		if(usuario_conectarBanco(usuario))
		{
			printf(" \tLOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" \tWarning: Falha ao reconectar-se, encerrando interpretação\n");
			return false;
		}
	}
	if(usuario == NULL)
	{
		printf(" Warning: usuario == NULL detectado em addFeedBackDeProdutoAoBanco() OperacoesBanco.h ashjgvrs\n");
		return false;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" Warning: usuario não conectado detectado em addFeedBackDeProdutoAoBanco() OperacoesBanco.h sahvuias\n");
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

	if( produtoVencido(idProduto, usuario) )
	{
		printf(" Warning: Produto vencido detectado em OperacoesBanco.h addFeedBackDeProdutoAoBanco() sa6b54xdad\n");
		return false;
	}
	else
	{
		if(checaExistenciaDeVisualizacaoDeProdutoComPessoa(idProduto, usuario) != RETORNO_OK)
		{
			printf(" Warning: usuario cadastrado não possui nenhuma visualização desse produto registrada em OperacoesBanco.h addFeedBackDeProdutoAoBanco() askjdhjvbdsd5\n");
			if(addVisualizacoesAoBanco(idProduto, strdup("1"), usuario))
			{
				geraLog(LOG, "Visualização adicionada a usuario");
			}
			else
			{
				geraLog(ERRO, "Falha ao adicionar visualização ao usuario");
				return false;
			}
		}

		// int tamanho = 93 + strlen(email), TAMANHO_ID_PRODUTO + 1;
		int tamanho = 116 + usuario_obterTamanhoLogin(usuario);
		char *query = (char *)malloc(sizeof(char) * tamanho);
		if(query == NULL)
		{
			printf(" Warning: não foi possivel alocar memoria para query em OperacoesBanco.h addFeedBackDeProdutoAoBanco() asdjkhkvdn");
			return false;
		}
		snprintf(query, tamanho, "SELECT F.idfeedBackCliente FROM feedBackCliente F WHERE F.emailCliente=\'%s\' AND F.idproduto LIKE BINARY \'%s\';", usuario_obterLogin(usuario), idProduto);
		if(query == NULL)
		{
			printf(" Warning: não foi possivel formatar query em OperacoesBanco.h addFeedBackDeProdutoAoBanco() asdjkhkvdn");
			return false;
		}
		if(queryRetornaConteudo(usuario, query))
		{
			printf(" LOG: Feedback existente detectado em OperacoesBanco.h addFeedBackDeProdutoAoBanco() asdjhjkbvscf\n");
			// tamanho = 115 + strlen(titulo) + strlen(conteudo) + strlen(email) + TAMANHO_ID_PRODUTO + 1;
			tamanho = 138 + strlen(titulo) + strlen(conteudo) + usuario_obterTamanhoLogin(usuario);
			query = (char *)malloc(sizeof(char) * tamanho);
			if(query == NULL)
			{
				printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addFeedBackDeProdutoAoBanco() asdjhkvdsdf65025s\n");
				return false;
			}
			snprintf(query, tamanho, "UPDATE feedBackCliente F SET F.titulo=\'%s\', F.conteudo=\'%s\' WHERE F.emailCliente=\'%s\' AND F.idproduto LIKE BINARY \'%s\';", titulo, conteudo, usuario_obterLogin(usuario), idProduto);
			if(query == NULL)
			{
				printf(" Warning: falha ao formatar query em OperacoesBanco.h addFeedBackDeProdutoAoBanco() bhjvbhjcfdsfr\n");
				return false;
			}
			if(!executaQuery(usuario, query))
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
			tamanho = strlen(titulo) + strlen(conteudo) + usuario_obterTamanhoLogin(usuario) + 99;//OTIMIZADO
			query = (char *)malloc(sizeof(char) * tamanho);
			if (query == NULL)
			{
				printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h addFeedBackDeProdutoAoBanco() sadkhjgvhresd\n");
				return false;
			}
			snprintf(query, tamanho, "INSERT INTO feedBackCliente(titulo,conteudo,emailCliente,idproduto) VALUES(\'%s\',\'%s\',\'%s\',\'%s\');", titulo, conteudo, usuario_obterLogin(usuario), idProduto);
			if (query == NULL)
			{
				printf(" Warning: Falha ao formatar query para ser executada em OperacoesBanco.h addFeedBackDeProdutoAoBanco() sadhjgjbvrg5456f\n");
				return false;
			}
			if(executaQuery(usuario, query))
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

bool addPontosDeUsuarioAoBanco(Usuario *usuario, char *quantidade)
{
	if (usuario->conexao == NULL)
	{
		printf(" Warning: Conexao nula detectada em OperacoesBanco.h addPontosDeUsuarioAoBanco() 56456q489b87ts\n");
		printf(" LOG: Tentando reconexão com banco de dados em OperacoesBanco.h addPontosDeUsuarioAoBanco()\n");
		if(usuario_conectarBanco(usuario))
		{
			printf(" \tLOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" \tWarning: Falha ao reconectar-se, encerrando interpretação\n");
			return false;
		}
	}
	if (usuario == NULL)
	{
		printf(" Warning: Usuario nulo detectado em OperacoesBanco.h addPontosDeUsuarioAoBanco()\n");
		return false;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" Warning: Usuario não conectado detectado em OperacoesBanco.h addPontosDeUsuarioAoBanco()\n");
		return false;
	}
	if(quantidade == NULL)
	{
		printf(" Warning: quantidade == NULL em OperacoesBanco.h addPontosDeUsuarioAoBanco() cqufh20nasdi\n");
		return false;
	}

	// int tamanho =  57 + strlen(email) + strlen(quantidade) + 1;
	int tamanho = 70 + usuario_obterTamanhoLogin(usuario) + strlen(quantidade);// que merda de otimizacao hein ?
	char *query = (char *)malloc(sizeof(char) * tamanho);
	if (query == NULL)
	{
		printf(" Warning: falha ao alocar memoria para query em OperacoesBanco.h addPontosDeUsuarioAoBanco() vq4b0s8sq\n");
		return false;
	}
	snprintf(query, tamanho, "UPDATE cliente C SET C.pontos=C.pontos+%s WHERE C.email LIKE BINARY \'%s\';", quantidade, usuario_obterLogin(usuario));
	if (query == NULL)
	{
		printf(" Warning: falha ao formatar query em OperacoesBanco.h addPontosDeUsuarioAoBanco() ab4iua13tb\n");
		return false;
	}
	if(executaQuery(usuario, query))
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

bool addNumeroDeHabitantesACidadeAoBanco(Usuario *usuario, char *nomeCidade, char *nomeEstado, char *numeroHabitantes)
{
	if (nomeCidade == NULL)
	{
		geraLog(ERRO, "nome de cidade é nulo");
		if (nomeEstado!=NULL)
		{
			free(nomeEstado);
			nomeEstado = NULL;
		}
		if (numeroHabitantes != NULL)
		{
			free(numeroHabitantes);
			numeroHabitantes = NULL;
		}
		return false;
	}
	if (nomeEstado == NULL)
	{
		geraLog(ERRO, "nome de estado é nulo");
		free(nomeCidade);
		nomeCidade = NULL;
		if (numeroHabitantes != NULL)
		{
			free(numeroHabitantes);
			numeroHabitantes = NULL;
		}
		return false;
	}
	if (numeroHabitantes == NULL)
	{
		geraLog(ERRO, "Numero de habitantes nulo");
		free(nomeCidade);
		free(nomeEstado);
		nomeCidade = NULL;
		nomeEstado = NULL;
		return false;
	}

	if (!cidadeExisteNoBanco(usuario, nomeCidade, nomeEstado))
	{
		geraLog(WARNING, "Cidade não existe no banco de dados");
		printf("\tCidade = |%s|\n",nomeCidade);
		free(nomeCidade);
		free(nomeEstado);
		nomeCidade = NULL;
		nomeEstado = NULL;
		return false;
	}

	int tamanho = 122 + strlen(nomeEstado) + strlen(nomeCidade) + strlen(numeroHabitantes) + 1;
	char *query = (char*)calloc(sizeof(char), tamanho);
	if (query == NULL)
	{
		geraLog(ERRO, "Falha ao alocar memoria para query");
		free(nomeCidade);
		free(nomeEstado);
		free(numeroHabitantes);
		numeroHabitantes = NULL;
		nomeEstado = NULL;
		nomeCidade = NULL;
		return false;
	}

	snprintf(query, tamanho, "UPDATE cidade C JOIN estado E ON E.idestado=C.estado_idestado SET C.quantidadeDeHabitantes=%s WHERE C.nome=\'%s\' AND E.nome=\'%s\';", numeroHabitantes, nomeCidade, nomeEstado);//TODO
	if (query == NULL)
	{
		geraLog(ERRO, "Falha ao formatar query");
		free(nomeCidade);
		free(nomeEstado);
		free(numeroHabitantes);
		numeroHabitantes = NULL;
		nomeEstado = NULL;
		nomeCidade = NULL;
		return false;
	}

	if (!executaQuery(usuario, query))
	{
		geraLog(ERRO, "Falha ao executar query");
		printf("\tQuery = |%s|\n", query);
		free(query);
		free(nomeCidade);
		free(nomeEstado);
		free(numeroHabitantes);
		numeroHabitantes = NULL;
		nomeEstado = NULL;
		nomeCidade = NULL;
		query = NULL;
		return false;
	}
	else
	{
		geraLog(LOG, "Quantidade de habitantes alterada com sucesso");
		free(query);
		free(nomeCidade);
		free(nomeEstado);
		free(numeroHabitantes);
		numeroHabitantes = NULL;
		nomeEstado = NULL;
		nomeCidade = NULL;
		query = NULL;
		return true;
	}
}

/* ****FIM COMANDOS DE ADICAO****/
/* COMANDOS DE OBTENÇÃO */





/** 
 * @brief  Retorna direto ao usuario a quantidade de habitantes da cidade, se não houver, atualiza (caso usuario esteja autorizado)
 * @note   retorna direto ao usuario, libera todos os char *
 * @param  *usuario: O usuario que deseja saber a informação obtida
 * @param  *nomeDaCidade: o nome da cidade que deseja saber a quantidade de habitantes
 * @param  *nomeDoEstado: O estado onde se encontra a cidade que deseja saber a quantidade de habitantes
 * @retval retorna direto ao usuario char *
 */
char *obterQuantidadeDeHabitantesDaCidadeDoBanco(Usuario *usuario, char *nomeDaCidade, char *nomeDoEstado)
{
	static const char *localizacao = "OperacoesBanco.h obterQuantidadeDeHabitantesDaCidadeDoBanco(Usuario*, char*, char*)";
	if (!usuarioValido(usuario, localizacao))
	{
		if (nomeDaCidade != NULL)
		{
			free(nomeDaCidade);
			nomeDaCidade = NULL;
		}
		if (nomeDoEstado != NULL)
		{
			free(nomeDoEstado);
			nomeDoEstado = NULL;
		}
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (nomeDaCidade == NULL)
	{
		if (nomeDoEstado != NULL)
		{
			free(nomeDoEstado);
			nomeDoEstado = NULL;
		}
		geraLog(ERRO, "nomeDaCidade nulo detectado");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (nomeDoEstado == NULL)
	{
		free(nomeDaCidade);
		nomeDaCidade = NULL;
		geraLog(ERRO, "nomeDoEstado nulo detectado");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	int tamanho = 122 + strlen(nomeDaCidade) + strlen(nomeDoEstado) + 1;
	char *query = (char *)calloc(sizeof(char), tamanho);
	if (query == NULL)
	{
		geraLog(ERRO, "Falha ao alocar memoria para query");
		free(nomeDaCidade);
		free(nomeDoEstado);
		nomeDaCidade = NULL;
		nomeDoEstado = NULL;
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	snprintf(query, tamanho, "SELECT C.quantidadeDeHabitantes FROM cidade C JOIN estado E ON C.estado_idestado=E.idestado WHERE C.nome=\'%s\' AND E.nome=\'%s\';", nomeDaCidade, nomeDoEstado);
	if (query == NULL)
	{
		geraLog(ERRO, "Falha ao formatar para query");
		free(nomeDaCidade);
		free(nomeDoEstado);
		nomeDaCidade = NULL;
		nomeDoEstado = NULL;
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	char *resposta = obterRetornoUnicoDaQuery(usuario, query);

	if (resposta == NULL)
	{
		if (!interno_atualizarQuantidadeDeHabitantesDaCidade(usuario, nomeDaCidade, nomeDoEstado))
		{
			geraLog(WARNING, "Não foi possível atualizar o numero de habitantes da cidade");
			free(nomeDaCidade);
			free(nomeDoEstado);
			nomeDaCidade = NULL;
			nomeDoEstado = NULL;
			return RETORNO_ERRO_NOT_FOUND_STR_DINAMICA;
		}
		else
		{
			query = NULL;
			free(resposta);
			resposta = NULL;
			query = (char *)calloc(sizeof(char), tamanho);
			if (query == NULL)
			{
				geraLog(ERRO, "Falha ao alocar memoria para query");
				free(nomeDaCidade);
				free(nomeDoEstado);
				nomeDaCidade = NULL;
				nomeDoEstado = NULL;
				return RETORNO_ERRO_INTERNO_STR_DINAMICA;
			}

			snprintf(query, tamanho, "SELECT C.quantidadeDeHabitantes FROM cidade C JOIN estado E ON C.estado_idestado=E.idestado WHERE C.nome=\'%s\' AND E.nome=\'%s\';", nomeDaCidade, nomeDoEstado);
			if (query == NULL)
			{
				geraLog(ERRO, "Falha ao formatar para query");
				free(nomeDaCidade);
				free(nomeDoEstado);
				nomeDaCidade = NULL;
				nomeDoEstado = NULL;
				return RETORNO_ERRO_INTERNO_STR_DINAMICA;
			}
			
			resposta = obterRetornoUnicoDaQuery(usuario, query);

			if (resposta == NULL)
			{
				geraLog(ERRO, "Mesmo após atualizar a quantidade de habitantes da cidade, não deu certo");
				free(nomeDaCidade);
				free(nomeDoEstado);
				nomeDaCidade = NULL;
				nomeDoEstado = NULL;
				return RETORNO_ERRO_INTERNO_STR_DINAMICA;
			}
		}
	}

	free(nomeDaCidade);
	free(nomeDoEstado);
	nomeDoEstado = NULL;
	nomeDaCidade = NULL;

	return resposta;
}



char *obterIdLocalizacaoDoBanco(Usuario *usuario, char *idCidade, char *cep, char *bairro, char *rua, char *numero, char *complemento)
{
	if (!usuario_conexaoAtiva(usuario))
	{
		geraLog(ERRO, "Conexão inativa com o banco de dados detectada");
		return RETORNO_ERRO_INTERNO_BANCO_STR_DINAMICA;
	}

	if( idCidade == NULL )
	{
		printf(" ERRO: idCidade nulo detectada em OperacoesBanco.h obterIdLocalizacaoDoBanco() sabfjieoq\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	if( cep == NULL )
	{
		printf(" ERRO: cep nulo detectada em OperacoesBanco.h obterIdLocalizacaoDoBanco() sabfjieoq\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
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
			return RETORNO_ERRO_INTERNO_STR_DINAMICA;
		}
		snprintf(query, tamanho, "SELECT L.idlocalizacao FROM localizacao L WHERE L.cidade_idcidade=%s AND L.cep=\'%s\' LIMIT 1;", idCidade, cep);
		if( query == NULL )
		{
			printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterIdLocalizacaoDoBanco() abuvirkasd\n");
			return RETORNO_ERRO_INTERNO_STR_DINAMICA;
		}
		return retornaUnicoRetornoDaQuery(usuario, query);
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
				return RETORNO_ERRO_INTERNO_STR_DINAMICA;
			}
			snprintf(query, tamanho, "SELECT L.idlocalizacao FROM localizacao L WHERE L.cidade_idcidade=%s AND L.bairro=\'%s\' AND L.cep=\'%s\' LIMIT 1;", idCidade, bairro, cep);
			if( query == NULL )
			{
				printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterIdLocalizacaoDoBanco() abuvirkasd\n");
				return RETORNO_ERRO_INTERNO_STR_DINAMICA;
			}
			return retornaUnicoRetornoDaQuery(usuario, query);
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
					return RETORNO_ERRO_INTERNO_STR_DINAMICA;
				}
				snprintf(query, tamanho, "SELECT L.idlocalizacao FROM localizacao L WHERE L.cidade_idcidade=%s AND L.bairro=\'%s\' AND L.cep=\'%s\' AND L.rua=\'%s\' LIMIT 1;", idCidade, bairro, cep, rua);
				if( query == NULL )
				{
					printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterIdLocalizacaoDoBanco() abuvirkasd\n");
					return RETORNO_ERRO_INTERNO_STR_DINAMICA;
				}
				return retornaUnicoRetornoDaQuery(usuario, query);
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
						return RETORNO_ERRO_INTERNO_STR_DINAMICA;
					}
					snprintf(query, tamanho, "SELECT L.idlocalizacao FROM localizacao L WHERE L.cidade_idcidade=%s AND L.bairro=\'%s\' AND L.cep=\'%s\' AND L.rua=\'%s\' AND L.numero=\'%s\' LIMIT 1;", idCidade, bairro, cep, rua, numero);
					if( query == NULL )
					{
						printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterIdLocalizacaoDoBanco() abuvirkasd\n");
						return RETORNO_ERRO_INTERNO_STR_DINAMICA;
					}
					return retornaUnicoRetornoDaQuery(usuario, query);
				}
				else// Busca com todos os dados
				{
					tamanho = 154+strlen(idCidade)+strlen(bairro)+strlen(cep)+strlen(rua)+strlen(numero)+strlen(complemento)+1;
					query = (char *)malloc(sizeof(char) * tamanho);
					if( query == NULL )
					{
						printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h obterIdLocalizacaoDoBanco() aoirnisdf\n");
						return RETORNO_ERRO_INTERNO_STR_DINAMICA;
					}
					snprintf(query, tamanho, "SELECT L.idlocalizacao FROM localizacao L WHERE L.cidade_idcidade=%s AND L.bairro=\'%s\' AND L.cep=\'%s\' AND L.rua=\'%s\' AND L.numero=\'%s\' AND L.complemento=\'%s\' LIMIT 1;", idCidade, bairro, cep, rua, numero, complemento);
					if( query == NULL )
					{
						printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterIdLocalizacaoDoBanco() abuvirkasd\n");
						return RETORNO_ERRO_INTERNO_STR_DINAMICA;
					}
					return retornaUnicoRetornoDaQuery(usuario, query);
				}
			}
		}
	}
	return RETORNO_ERRO_INTERNO_STR_DINAMICA;
}

char *obterIdContratanteDoBanco(Usuario *usuario, char *cnpj)// APP 4 ; cnpj
{
	if(usuario->conexao == NULL)
	{
		printf(" ERRO: Conexao nula em OperacoesBanco.h obterIdContratanteDoBanco() !!!\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(usuario_conectarBanco(usuario))
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return RETORNO_ERRO_INTERNO_BANCO_STR_DINAMICA;
		}
	}

	if( cnpj == NULL )
	{
		printf(" Warning: Cliente não informou cnpj em OperacoesBanco.h obterIdContratanteDoBanco() asvbeuasd\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}


	int tamanho = 58 + 1 + strlen(cnpj);
	char *query = (char *)malloc(sizeof(char) * (tamanho));

	if(query == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h obterIdContratanteDoBanco() qkjeh\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	snprintf(query, tamanho, "SELECT C.idcontratante FROM contratante C WHERE C.cnpj=\'%s\';", cnpj);


	if(query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterIdContratanteDoBanco() bqjek\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	return retornaUnicoRetornoDaQuery(usuario, query);
}

char *obterIdCidadeDoBanco(Usuario *usuario, char *nomeCidade, char *nomeEstado)/* APP 4 CHAVE_DE_SEGURANCA_PHP $ { nomeCidadeInformada nomeEstadoInformado*/
{
	if(usuario->conexao == NULL)
	{
		printf(" ERRO: Conexao nula em OperacoesBanco.h obterIdCidadeDoBanco() !!!\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(usuario_conectarBanco(usuario))
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return strdup("ERRO interno, banco off, tente novamente");
		}
	}

	if(nomeCidade == NULL)
	{
		printf(" ERRO: Comunicacao entre funcoes falhou, nomeCidade == NULL em OperacoesBanco.h obterIdCidadeDoBanco()\n");
		return strdup("ERRO interno, tente novamente");
	}

	if(nomeEstado == NULL)
	{
		printf(" ERRO: Comunicacao entre funcoes falhou, nomeEstado == NULL em OperacoesBanco.h obterIdCidadeDoBanco()\n");
		return strdup("ERRO interno, tente novamente");
	}

	int tamanho = 108 + 1 + strlen(nomeEstado) + strlen(nomeCidade);
	char *query = (char *)malloc(sizeof(char) * (tamanho));

	if(query == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h obterIdCidadeDoBanco() qkjeh\n");
		return strdup("ERRO interno, tente novamente");
	}

	snprintf(query, tamanho, "SELECT C.idcidade FROM cidade C JOIN estado E ON E.idestado=C.estado_idestado WHERE E.nome=\'%s\' AND C.nome=\'%s\';", nomeEstado, nomeCidade);


	if(query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterIdCidadeDoBanco() bqjek\n");
		return strdup("ERRO interno, tente novamente");
	}

	return retornaUnicoRetornoDaQuery(usuario, query);
}

char *obterTop10NovosProdutosDoBanco(Usuario *usuario)//DONE
{
	if(usuario->conexao == NULL)
	{
		printf(" ERRO: usuario->conexao nula em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(usuario_conectarBanco(usuario))
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return strdup("ERRO: banco de dados desconectado, tente novamente");
		}
	}

	int tamanho = 65 + 1;
	char *query = (char *)malloc(sizeof(char) * (tamanho));

	if(query == NULL)
	{
		printf(" Warning: Falha ao tentar alocar memoria para query em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		return strdup("ERRO interno, tente novamente");
	}

	snprintf(query, tamanho, "SELECT idproduto FROM produto ORDER BY datacriacao DESC LIMIT 10;");

	if(query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
		return strdup("ERRO interno, tente novamente");
	}

	return retornaNIteracoesDaQuery(usuario, query, 10);

	// if(mysql_query(usuario->conexao, query))
	// {
	// 	printf(" Warning: falhao ao executar query em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
	// 	free(query);
	// 	query = NULL;
	// 	return strdup("ERRO interno, tente novamente");
	// }

	// MYSQL_RES *resultado = mysql_store_result(usuario->conexao);
	// if(resultado == NULL)// Se não houver consulta
	// {
	// 	printf(" Warning: Consulta não realizada em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
	// 	free(query);
	// 	query = NULL;
	// 	return strdup("ERRO interno, tente novamente");
	// }

	// if(mysql_num_fields(resultado) == 0)
	// {
	// 	printf(" Warning: Nada encontrado em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
	// 	free(query);
	// 	query = NULL;
	// 	mysql_free_result(resultado);
	// 	resultado = NULL;
	// 	return strdup("ERRO: Nada encontrado");
	// }

	// MYSQL_ROW linha = NULL;
	// char *retorno = NULL;

	// tamanho = ((mysql_num_rows(resultado) * 10) + 1 + mysql_num_rows(resultado)-1);

	// retorno = (char *)malloc(sizeof(char) * tamanho);// numero de resultados * tamanho de cada resultado + \0 + numero de resultados -1

	// if(retorno == NULL)
	// {
	// 	printf(" Warning: Falha ao alocar memoria para retorno em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
	// 	free(query);
	// 	query = NULL;
	// 	mysql_free_result(resultado);
	// 	resultado = NULL;
	// 	return strdup("ERRO interno, tente novamente");
	// }
	// memset(retorno, '\0', tamanho);

	// //	 int i = 0;
	// bool achouAlgumaCoisa = false;

	// while((linha = mysql_fetch_row(resultado)) != NULL)
	// {
	// 	//		snprintf(retorno, tamanho, "%s %s", retorno, linha);

	// 	printf(" LOG: LINHA OBTIDA = |%s| em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n", linha[0]);
	// 	if(strlen(linha[0]) > 0)
	// 	{
	// 		if (strlen(linha[0]) != TAMANHO_ID_PRODUTO)
	// 		{
	// 			free(query);
	// 			query = NULL;
	// 			mysql_free_result(resultado);
	// 			resultado = NULL;
	// 			free(retorno);
	// 			retorno = NULL;
	// 			return strdup("ERRO interno, ou nada encontrado");
	// 		}
	// 		strcat(retorno, linha[0]);
	// 	}

	// 	printf(" LOG: Retorno = |%s| até agora em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n", retorno);

	// 	if(retorno == NULL)
	// 	{
	// 		printf(" Warning: Falha ao formatar resultado em OperacoesBanco.h obterTop10NovosProdutosDoBanco() vbrjka\n");
	// 		free(query);
	// 		mysql_free_result(resultado);
	// 		resultado = NULL;
	// 		query = NULL;
	// 		return strdup("ERRO interno, tente novamente");
	// 	}
	// 	if(strlen(retorno) < tamanho - 1)
	// 	{
	// 		strcat(retorno, " ");
	// 	}
	// 	if (!achouAlgumaCoisa)
	// 	{
	// 		achouAlgumaCoisa = true;
	// 	}
	// }

	// if(retorno == NULL)
	// {
	// 	printf(" ERRO: Falha ao obter dados em OperacoesBanco.h obterTop10NovosProdutosDoBanco()\n");
	// 	free(query);
	// 	mysql_free_result(resultado);
	// 	resultado = NULL;
	// 	query = NULL;
	// 	return strdup("ERRO interno, tente novamente");
	// }
	// else
	// {
	// 	free(query);
	// 	mysql_free_result(resultado);
	// 	resultado = NULL;
	// 	query = NULL;
	// 	if (!achouAlgumaCoisa)
	// 	{
	// 		if (retorno != NULL)
	// 		{
	// 			free(retorno);
	// 			retorno = NULL;
	// 		}
	// 		return strdup("ERRO, nada encontrado");
	// 	}
	// 	return retorno;
	// }

	// return strdup("ERRO interno, tente novamente");
}

char *obterTop10NovosProdutosRealidadeAumentadaDoBanco(Usuario *usuario)//DONE
{
	if(usuario->conexao == NULL)
	{
		printf(" ERRO: usuario->conexao nula em OperacoesBanco.h obterTop10NovosProdutosRealidadeAumentadaDoBanco()\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(usuario_conectarBanco(usuario))
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return RETORNO_ERRO_INTERNO_BANCO_STR_DINAMICA;
		}
	}
	// int tamanho = 82 + 1;
	int tamanho = 83;
	char *query = (char *)malloc(sizeof(char) * (tamanho));

	if(query == NULL)
	{
		printf(" Warning: Falha ao tentar alocar memoria para query em OperacoesBanco.h obterTop10NovosProdutosRealidadeAumentadaDoBanco()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	snprintf(query, tamanho, "SELECT idproduto FROM produto P WHERE P.tipo=1 ORDER BY datacriacao DESC LIMIT 10;");

	if(query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterTop10NovosProdutosRealidadeAumentadaDoBanco()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	return retornaNIteracoesDaQuery(usuario, query, 10);

	// if(mysql_query(usuario->conexao, query))
	// {
	// 	printf(" Warning: falhao ao executar query em OperacoesBanco.h obterTop10NovosProdutosRealidadeAumentadaDoBanco()\n");
	// 	free(query);
	// 	query = NULL;
	// 	return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	// }

	// MYSQL_RES *resultado = mysql_store_result(usuario->conexao);
	// if(resultado == NULL)// Se não houver consulta
	// {
	// 	printf(" Warning: Consulta não realizada em OperacoesBanco.h obterTop10NovosProdutosRealidadeAumentadaDoBanco()\n");
	// 	free(query);
	// 	query = NULL;
	// 	return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	// }

	// if(mysql_num_fields(resultado) == 0)
	// {
	// 	printf(" Warning: Nada encontrado em OperacoesBanco.h obterTop10NovosProdutosRealidadeAumentadaDoBanco()\n");
	// 	free(query);
	// 	query = NULL;
	// 	mysql_free_result(resultado);
	// 	resultado = NULL;
	// 	return RETORNO_ERRO_NOT_FOUND_STR_DINAMICA;
	// }

	// MYSQL_ROW linha = NULL;
	// char *retorno = NULL;

	// tamanho = ((mysql_num_rows(resultado) * 10) + 1 + mysql_num_rows(resultado)-1);

	// retorno = (char *)malloc(sizeof(char) * tamanho);// numero de resultados * tamanho de cada resultado + \0 + numero de resultados -1

	// if(retorno == NULL)
	// {
	// 	printf(" Warning: Falha ao alocar memoria para retorno em OperacoesBanco.h obterTop10NovosProdutosRealidadeAumentadaDoBanco()\n");
	// 	free(query);
	// 	query = NULL;
	// 	mysql_free_result(resultado);
	// 	resultado = NULL;
	// 	return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	// }
	// memset(retorno, '\0', tamanho);

	// //	 int i = 0;
	// bool achouAlgumaCoisa = false;

	// while((linha = mysql_fetch_row(resultado)) != NULL)
	// {
	// 	//		snprintf(retorno, tamanho, "%s %s", retorno, linha);

	// 	printf(" LOG: LINHA OBTIDA = |%s| em OperacoesBanco.h obterTop10NovosProdutosRealidadeAumentadaDoBanco()\n", linha[0]);
	// 	if(strlen(linha[0]) > 0)
	// 	{
	// 		if (strlen(linha[0]) != TAMANHO_ID_PRODUTO)
	// 		{
	// 			free(query);
	// 			query = NULL;
	// 			mysql_free_result(resultado);
	// 			resultado = NULL;
	// 			free(retorno);
	// 			retorno = NULL;
	// 			return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	// 		}
	// 		strcat(retorno, linha[0]);
	// 	}

	// 	printf(" LOG: Retorno = |%s| até agora em OperacoesBanco.h obterTop10NovosProdutosRealidadeAumentadaDoBanco()\n", retorno);

	// 	if(retorno == NULL)
	// 	{
	// 		printf(" Warning: Falha ao formatar resultado em OperacoesBanco.h obterTop10NovosProdutosRealidadeAumentadaDoBanco() vbrjka\n");
	// 		free(query);
	// 		mysql_free_result(resultado);
	// 		resultado = NULL;
	// 		query = NULL;
	// 		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	// 	}
	// 	if(strlen(retorno) < tamanho - 1)
	// 	{
	// 		strcat(retorno, " ");
	// 	}
	// 	if (!achouAlgumaCoisa)
	// 	{
	// 		achouAlgumaCoisa = true;
	// 	}
	// }

	// if(retorno == NULL)
	// {
	// 	printf(" ERRO: Falha ao obter dados em OperacoesBanco.h obterTop10NovosProdutosRealidadeAumentadaDoBanco()\n");
	// 	free(query);
	// 	mysql_free_result(resultado);
	// 	resultado = NULL;
	// 	query = NULL;
	// 	return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	// }
	// else
	// {
	// 	free(query);
	// 	mysql_free_result(resultado);
	// 	resultado = NULL;
	// 	query = NULL;
	// 	if (!achouAlgumaCoisa)
	// 	{
	// 		if (retorno != NULL)
	// 		{
	// 			free(retorno);
	// 			retorno = NULL;
	// 		}
	// 		return strdup("ERRO, nada encontrado");
	// 	}
	// 	return retorno;
	// }

	// return RETORNO_ERRO_INTERNO_STR_DINAMICA;
}

char *obterTop10NovosProdutosRealidadeVirtualDoBanco(Usuario *usuario)//DONE
{
	if(usuario->conexao == NULL)
	{
		printf(" ERRO: usuario->conexao nula em OperacoesBanco.h obterTop10NovosProdutosRealidadeVirtualDoBanco()\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(usuario_conectarBanco(usuario))
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return RETORNO_ERRO_INTERNO_BANCO_STR_DINAMICA;
		}
	}

	//int tamanho = 82 + 1;
	int tamanho = 83;
	char *query = (char *)malloc(sizeof(char) * (tamanho));

	if(query == NULL)
	{
		printf(" Warning: Falha ao tentar alocar memoria para query em OperacoesBanco.h obterTop10NovosProdutosRealidadeVirtualDoBanco()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	snprintf(query, tamanho, "SELECT idproduto FROM produto P WHERE P.tipo=2 ORDER BY datacriacao DESC LIMIT 10;");

	if(query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterTop10NovosProdutosRealidadeVirtualDoBanco()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	return retornaNIteracoesDaQuery(usuario, query, 10);

	// if(mysql_query(usuario->conexao, query))
	// {
	// 	printf(" Warning: falhao ao executar query em OperacoesBanco.h obterTop10NovosProdutosRealidadeVirtualDoBanco()\n");
	// 	free(query);
	// 	query = NULL;
	// 	return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	// }

	// MYSQL_RES *resultado = mysql_store_result(usuario->conexao);
	// if(resultado == NULL)// Se não houver consulta
	// {
	// 	printf(" Warning: Consulta não realizada em OperacoesBanco.h obterTop10NovosProdutosRealidadeVirtualDoBanco()\n");
	// 	free(query);
	// 	query = NULL;
	// 	return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	// }

	// if(mysql_num_fields(resultado) == 0)
	// {
	// 	printf(" Warning: Nada encontrado em OperacoesBanco.h obterTop10NovosProdutosRealidadeVirtualDoBanco()\n");
	// 	free(query);
	// 	query = NULL;
	// 	mysql_free_result(resultado);
	// 	resultado = NULL;
	// 	return RETORNO_ERRO_NOT_FOUND_STR_DINAMICA;
	// }

	// MYSQL_ROW linha = NULL;
	// char *retorno = NULL;

	// tamanho = ((mysql_num_rows(resultado) * 10) + 1 + mysql_num_rows(resultado)-1);

	// retorno = (char *)malloc(sizeof(char) * tamanho);// numero de resultados * tamanho de cada resultado + \0 + numero de resultados -1

	// if(retorno == NULL)
	// {
	// 	printf(" Warning: Falha ao alocar memoria para retorno em OperacoesBanco.h obterTop10NovosProdutosRealidadeVirtualDoBanco()\n");
	// 	free(query);
	// 	query = NULL;
	// 	mysql_free_result(resultado);
	// 	resultado = NULL;
	// 	return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	// }
	// memset(retorno, '\0', tamanho);

	// //	 int i = 0;
	// bool achouAlgumaCoisa = false;

	// while((linha = mysql_fetch_row(resultado)) != NULL)
	// {
	// 	//		snprintf(retorno, tamanho, "%s %s", retorno, linha);

	// 	printf(" LOG: LINHA OBTIDA = |%s| em OperacoesBanco.h obterTop10NovosProdutosRealidadeVirtualDoBanco()\n", linha[0]);
	// 	if(strlen(linha[0]) > 0)
	// 	{
	// 		if (strlen(linha[0]) != TAMANHO_ID_PRODUTO)
	// 		{
	// 			free(query);
	// 			query = NULL;
	// 			mysql_free_result(resultado);
	// 			resultado = NULL;
	// 			free(retorno);
	// 			retorno = NULL;
	// 			return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	// 		}
	// 		strcat(retorno, linha[0]);
	// 	}

	// 	printf(" LOG: Retorno = |%s| até agora em OperacoesBanco.h obterTop10NovosProdutosRealidadeVirtualDoBanco()\n", retorno);

	// 	if(retorno == NULL)
	// 	{
	// 		printf(" Warning: Falha ao formatar resultado em OperacoesBanco.h obterTop10NovosProdutosRealidadeVirtualDoBanco() vbrjka\n");
	// 		free(query);
	// 		mysql_free_result(resultado);
	// 		resultado = NULL;
	// 		query = NULL;
	// 		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	// 	}
	// 	if(strlen(retorno) < tamanho - 1)
	// 	{
	// 		strcat(retorno, " ");
	// 	}
	// 	if (!achouAlgumaCoisa)
	// 	{
	// 		achouAlgumaCoisa = true;
	// 	}
	// }

	// if(retorno == NULL)
	// {
	// 	printf(" ERRO: Falha ao obter dados em OperacoesBanco.h obterTop10NovosProdutosRealidadeVirtualDoBanco()\n");
	// 	free(query);
	// 	mysql_free_result(resultado);
	// 	resultado = NULL;
	// 	query = NULL;
	// 	return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	// }
	// else
	// {
	// 	free(query);
	// 	mysql_free_result(resultado);
	// 	resultado = NULL;
	// 	query = NULL;
	// 	if (!achouAlgumaCoisa)
	// 	{
	// 		if (retorno != NULL)
	// 		{
	// 			free(retorno);
	// 			retorno = NULL;
	// 		}
	// 		return RETORNO_ERRO_NOT_FOUND_STR_DINAMICA;
	// 	}
	// 	return retorno;
	// }

	// return RETORNO_ERRO_INTERNO_STR_DINAMICA;
}

char *obterTop10ProdutosMelhorAvaliadosDoBanco(Usuario *usuario)
{
	if (usuario->conexao == NULL)
	{
		printf(" ERRO: usuario->conexao nula em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(usuario_conectarBanco(usuario))
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return strdup("ERRO interno(falha de usuario->conexao com o banco), tente novamente");
		}
	}
	//int tamanho = 149 + 1;
	int tamanho = 150;
	char *query = (char *)malloc(sizeof(char) * tamanho);
	if(query == NULL)
	{
		printf(" Warning: não foi possível alocar memoria para query em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n");
		return strdup("ERRO interno, tente novamente");
	}
	query = strdup("SELECT idproduto FROM produto P WHERE DATEDIFF(CURDATE(), P.datacriacao)-P.duracao<=0 ORDER BY P.avaliacaoPositiva-P.avaliacaoNegativa DESC LIMIT 10;");
	if (query == NULL)
	{
		printf(" Warning: Falha ao copiar string para query em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n");
		return strdup("ERRO interno, tente novamente");
	}

	return retornaNIteracoesDaQuery(usuario, query, 10);

	// if(mysql_query(usuario->conexao, query))
	// {
	// 	printf(" Warning: falhao ao executar query: |%s| em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n", query);
	// 	free(query);
	// 	query = NULL;
	// 	return strdup("ERRO interno, tente novamente");
	// }

	// free(query);
	// query = NULL;//Não preciso mais de query daqui pra frente

	// MYSQL_RES *resultado = mysql_store_result(usuario->conexao);
	// if(resultado == NULL)// Se não houver consulta
	// {
	// 	printf(" Warning: Consulta não realizada em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n");
	// 	return strdup("ERRO interno, tente novamente");
	// }

	// if(mysql_num_fields(resultado) == 0)
	// {
	// 	printf(" Warning: Nada encontrado em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n");
	// 	mysql_free_result(resultado);
	// 	resultado = NULL;
	// 	return strdup("ERRO nada encontrado");
	// }

	// MYSQL_ROW linha = NULL;
	// char *retorno = NULL;

	// tamanho = ((mysql_num_rows(resultado) * 10) + 1 + mysql_num_rows(resultado)-1);

	// retorno = (char *)malloc(sizeof(char) * tamanho);// numero de resultados * tamanho de cada resultado + \0 + numero de resultados -1

	// if(retorno == NULL)
	// {
	// 	printf(" Warning: Falha ao alocar memoria para retorno em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n");
	// 	mysql_free_result(resultado);
	// 	resultado = NULL;
	// 	return strdup("ERRO interno, tente novamente");
	// }
	// memset(retorno, '\0', tamanho);

	// //	 int i = 0;
	// bool achouAlgumaCoisa = false;

	// while((linha = mysql_fetch_row(resultado)) != NULL)
	// {
	// 	//		snprintf(retorno, tamanho, "%s %s", retorno, linha);

	// 	printf(" LOG: LINHA OBTIDA = |%s| em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n", linha[0]);
	// 	if(strlen(linha[0]) > 0)
	// 	{
	// 		strcat(retorno, linha[0]);
	// 	}

	// 	printf(" LOG: Retorno = |%s| até agora em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n", retorno);

	// 	if(retorno == NULL)
	// 	{
	// 		printf(" Warning: Falha ao formatar resultado em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco() vbrjka\n");
	// 		mysql_free_result(resultado);
	// 		resultado = NULL;
	// 		return strdup("ERRO interno, tente novamente");
	// 	}
	// 	if(strlen(retorno) < tamanho - 1)
	// 	{
	// 		strcat(retorno, " ");
	// 	}
	// 	if (!achouAlgumaCoisa)
	// 	{
	// 		achouAlgumaCoisa = true;
	// 	}
	// }

	// if(retorno == NULL)
	// {
	// 	printf(" ERRO: Falha ao obter dados em OperacoesBanco.h obterTop10ProdutosMelhorAvaliadosDoBanco()\n");
	// 	mysql_free_result(resultado);
	// 	resultado = NULL;
	// 	return strdup("ERRO interno, tente novamente");
	// }
	// else
	// {
	// 	mysql_free_result(resultado);
	// 	resultado = NULL;
	// 	if (!achouAlgumaCoisa)
	// 	{
	// 		if (retorno != NULL)
	// 		{
	// 			free(retorno);
	// 			retorno=NULL;
	// 		}
	// 		return strdup("ERRO, nada encontrado");
	// 	}
	// 	return retorno;
	// }

	// return strdup("ERRO interno(desconhecido), tente novamente");
}

char *obterDescricaoProdutoDoBanco(char *idProduto, Usuario *usuario)//DONE
{
	if (usuario->conexao == NULL)
	{
		printf(" Warning: Conexao perdida com o banco de dados em OperacoesBanco.h obterDescricaoProdutoDoBanco() 4e5b11eea\n");
		return strdup("ERRO interno(banco de dados), tente novamente");
	}
	else if(idProduto == NULL)
	{
		printf(" Warning: Id de produto nulo em OperacoesBanco.h obterDescricaoProdutoDoBanco() veqf45\n");
		return strdup("ERRO interno, tente novamente");
	}
	else
	{
		if (produtoVencido(idProduto, usuario))
		{
			printf(" Warning: produto vencido em OperacoesBanco.h obterDescricaoProdutoDoBanco\n");
			return strdup("ERRO: produto vencido");
		}
		char *query = NULL;
		//int tamanho = 55 + TAMANHO_ID_PRODUTO + 1;// Usando TAMANHO_ID_PRODUTO para otimização
		int tamanho = 78;// Usando TAMANHO_ID_PRODUTO para otimização

		query = (char *)malloc(sizeof(char) * tamanho);
		if(query == NULL)
		{
			printf(" Warning: nao foi possível alocar memoria para query em OperacoesBanco.h obterDescricaoProdutoDoBanco()\n");
			return strdup("ERRO interno, tente novamente");
		}

		snprintf(query, tamanho, "SELECT P.descricao FROM produto P WHERE P.idproduto LIKE BINARY \'%s\';", idProduto);

		if(query == NULL)
		{
			printf(" Warning: nao foi possível formatar query em OperacoesBanco.h obterDescricaoProdutoDoBanco() cqhkhbjvr\n");
			return strdup("ERRO interno, tente novamente");
		}

		if(mysql_query(usuario->conexao, query))
		{
			printf(" Warning: falha ao executar query em OperacoesBanco.h obterDescricaoProdutoDoBanco() kjelkzj\n");
			free(query);
			query = NULL;
			return strdup("ERRO interno, tente novamente");
		}

		MYSQL_RES *resultado = mysql_store_result(usuario->conexao);

		if(resultado == NULL)
		{
			printf(" Warning: Consulta não realizada em OperacoesBanco.h obterDescricaoProdutoDoBanco() nao houve usuario->conexao\n");
			free(query);
			query = NULL;
			return strdup("ERRO interno, tente novamente");
		}

		if(mysql_num_fields(resultado) == 0)
		{
			printf(" Warning: Nao houve resultados na pesquisa em OperacoesBanco.h obterDescricaoProdutoDoBanco() bnje\n");
			free(query);
			mysql_free_result(resultado);
			resultado = NULL;
			query = NULL;
			return strdup("ERRO: Nada encontrado");
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
			return strdup("ERRO interno, tente novamente");
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
				return strdup("ERRO interno, tente novamente");
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
				return strdup("ERRO: Nada encontrado");

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
				return strdup("ERRO interno, tente novamente");

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
				return strdup("ERRO interno, tente novamente");

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
	return strdup("ERRO interno(desconhecido), tente novamente");
}

char *obterNomeProdutoDoBanco(char *idProduto, Usuario *usuario)//DONE
{
	if(usuario->conexao == NULL)
	{
		printf(" Warning: Conexao nula com o banco de dados em obterNomeProdutoDoBanco() OperacoesBanco.h \n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(usuario_conectarBanco(usuario))
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return strdup("ERRO interno(banco desconectado), tente novamente");
		}
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" ERRO: usuario não logado detectado em OperacoesBanco.h obterNomeProdutoDoBanco()\n");
		return strdup("ERRO interno(usuario nao conectado)");
	}
	if(idProduto == NULL)
	{
		printf(" Warning: idProduto informado NULO em obterNomeProdutoDoBanco() OperacoesBanco.h\n");
		return strdup("ERRO interno, tente novamente");
	}
	else
	{
		if(produtoVencido(idProduto, usuario))
		{
			printf(" Warning: produto vencido em OperacoesBanco.h obterNomeProdutoDoBanco()\n");
			return strdup("ERRO: Produto vencido");
		}

		char *query = NULL;
		//int tamanho = 57 + TAMANHO_ID_PRODUTO + 1;// Usando TAMANHO_ID_PRODUTO para otimização
		int tamanho = 80;// Usando TAMANHO_ID_PRODUTO para otimização

		query = (char *)malloc(sizeof(char) * tamanho);
		if(query == NULL)
		{
			printf(" Warning: nao foi possível alocar memoria para query em OperacoesBanco.h obterNomeProdutoDoBanco()\n");
			return strdup("ERRO interno, tente novamente");
		}

		snprintf(query, tamanho, "SELECT P.nomeProduto FROM produto P WHERE P.idproduto LIKE BINARY \'%s\';", idProduto);

		if(query == NULL)
		{
			printf(" Warning: nao foi possível formatar query em OperacoesBanco.h obterNomeProdutoDoBanco() cqhkhbjvr\n");
			return strdup("ERRO interno, tente novamente");
		}

		if(mysql_query(usuario->conexao, query))
		{
			printf(" Warning: falha ao executar query em OperacoesBanco.h obterNomeProdutoDoBanco() kjelkzj\n");
			free(query);
			query = NULL;
			return strdup("ERRO interno, tente novamente");
		}

		MYSQL_RES *resultado = mysql_store_result(usuario->conexao);

		if(resultado == NULL)
		{
			printf(" Warning: Consulta não realizada em OperacoesBanco.h obterNomeProdutoDoBanco() nao houve conexao\n");
			free(query);
			query = NULL;
			return strdup("ERRO interno, tente novamente");
		}

		if(mysql_num_fields(resultado) == 0)
		{
			printf(" Warning: Nao houve resultados na pesquisa em OperacoesBanco.h obterNomeProdutoDoBanco() bnje\n");
			free(query);
			mysql_free_result(resultado);
			resultado = NULL;
			query = NULL;
			return strdup("ERRO: nada encontrado");
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
			return strdup("ERRO interno, tente novamente");
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
				return strdup("ERRO: nada encontrado");
			}
			if(strlen(linha[0]) > 1)
			{
				printf(" LOG: Retorno obtido com sucesso em OperacoesBanco.h obterNomeProdutoDoBanco() kjh3as\n");
			}

			// int tamanho = strlen(linha[0]) + 1;
			// retorno = (char *)malloc(sizeof(char) * tamanho);

			// if(retorno == NULL)
			// {

			// 	printf(" Warning: Nao foi possivel alocar memoria para retorno em OperacoesBanco.h obterNomeProdutoDoBanco() sakdjhjkredf\n");
			// 	free(query);
			// 	mysql_free_result(resultado);
			// 	resultado = NULL;
			// 	query = NULL;
			// 	return strdup("ERRO interno, tente novamente");

			// }

			// memset(retorno, '\0', tamanho);

			// strcpy(retorno, linha[0]);
			retorno = strdup(linha[0]);

			if(retorno == NULL)
			{
				printf(" Warning: Nao foi possivel copiar memoria para retorno em OperacoesBanco.h obterNomeProdutoDoBanco() sakdjhjkredf\n");
				free(query);
				mysql_free_result(resultado);
				resultado = NULL;
				query = NULL;
				return strdup("ERRO interno, tente novamente");
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
	return strdup("ERRO interno(desconhecido), tente novamente");
}

char *obterAvaliacaoProdutoDoBanco(char *idProduto, Usuario *usuario)// APP 4 kW * idProduto
{
	printf(" LOG: iniciando obtenção de avaliação em OperacoesBanco.h obterAvaliacaoProdutoDoBanco()\n");
	if(usuario->conexao == NULL)
	{
		printf(" Warning: Conexao nula com o banco de dados em obterAvaliacaoProdutoDoBanco() OperacoesBanco.h \n");
		printf(" \tLOG: Tentando reconexão com banco de dados \n");
		if(usuario_conectarBanco(usuario))
		{
			printf(" \tLOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" \tWarning: Falha ao reconectar-se, encerrando interpretação\n");
			return strdup("ERRO interno(banco), tente novamente");
		}
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" ERRO: usuario não conectado detectado em OperacoesBanco.h obterAvaliacaoProdutoDoBanco()\n");
		return strdup("ERRO interno(usuario não conectado)");
	}
	if(idProduto == NULL)
	{
		printf(" Warning: idProduto informado NULO em obterAvaliacaoProdutoDoBanco() OperacoesBanco.h\n");
		return strdup("ERRO interno, tente novamente");
	}
	else
	{
		if(produtoVencido(idProduto, usuario))
		{
			printf(" Warning: Produto vencido em OperacoesBanco.h obterAvaliacaoProdutoDoBanco()\n");
			return strdup("ERRO: produto vencido");
		}
		char *query = NULL;
		// int tamanho = 79 + 1 + TAMANHO_ID_PRODUTO;
		int tamanho = 111;// Usando TAMANHO_ID_PRODUTO para otimização

		query = (char *)malloc(sizeof(char) * tamanho);
		if(query == NULL)
		{
			printf(" Warning: nao foi possível alocar memoria para query em OperacoesBanco.h obterAvaliacaoProdutoDoBanco()\n");
			return strdup("ERRO interno, tente novamente");
		}

		snprintf(query, tamanho, "SELECT P.avaliacaoPositiva,P.avaliacaoNegativa FROM produto P WHERE P.idproduto LIKE BINARY \'%s\';", idProduto);

		if(query == NULL)
		{
			printf(" Warning: nao foi possível formatar query em OperacoesBanco.h obterAvaliacaoProdutoDoBanco() cqhkhbjvr\n");
			return strdup("ERRO interno, tente novamente");
		}

		if(!executaQuery(usuario, query))
		{
			printf(" Warning: Falha ao executar query em OperacoesBanco.h obterAvaliacaoProdutoDoBanco() qa468fsx2er\n");
			free(query);
			query = NULL;
			return strdup("ERRO interno, tente novamente");
		}
		else
		{
			printf(" LOG: Query executada, continuando para obtenção de dados em OperacoesBanco.h obterAvaliacaoProdutoDoBanco() 49q87b2df\n");
		}

		MYSQL_RES *resultado = mysql_store_result(usuario->conexao);

		if(resultado == NULL)
		{
			printf(" Warning: Consulta não realizada em OperacoesBanco.h obterAvaliacaoProdutoDoBanco() nao houve usuario->conexao\n");
			free(query);
			query = NULL;
			return strdup("ERRO interno, tente novamente");
		}

		if(mysql_num_fields(resultado) == 0)
		{
			printf(" Warning: Nao houve resultados na pesquisa em OperacoesBanco.h obterAvaliacaoProdutoDoBanco() bnje\n");
			free(query);
			mysql_free_result(resultado);
			resultado = NULL;
			query = NULL;
			return strdup("ERRO: nada encontrado");
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
			return strdup("ERRO interno, tente novamente");
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
				return strdup("ERRO: produto sem descricao");
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
				return strdup("ERRO interno, tente novamente");

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
				return strdup("ERRO interno, tente novamente");

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
	return strdup("ERRO interno(desconhecido), tente novamente");

}

char *obterInformacoesBasicasDeProdutoDoBanco(Usuario *usuario, char *idProduto)
{
	if (usuario->conexao == NULL)
	{
		printf(" ERRO: conexao nula em OperacoesBanco.h obterInformacoesBasicasDeProdutoDoBanco()\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(usuario_conectarBanco(usuario))
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return strdup("ERRO interno(falha de conexao com o banco), tente novamente");
		}
	}
	if (idProduto == NULL)
	{
		printf(" Warning: idProduto nulo detectado em OperacoesBanco.h obterInformacoesBasicasDeProdutoDoBanco() dsahjkbr\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (usuario == NULL)
	{
		printf(" Warning: usuario nulo detectado em OperacoesBanco.h obterInformacoesBasicasDeProdutoDoBanco() v98a7ef\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" Warning: usuario nao conectado detectado em OperacoesBanco.h obterInformacoesBasicasDeProdutoDoBanco() e09as8d93\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (produtoVencido(idProduto, usuario))
	{
		printf(" Warning: Produto %s vencido para %s\n", idProduto, usuario_obterLogin(usuario));
		return RETORNO_ERRO_NAO_AUTORIZADO_STR_DINAMICA;
	}

	//int tamanho = 146+10+1;
	int tamanho = 169;
	char *query = (char *)malloc(sizeof(char) * tamanho);
	if (query == NULL)
	{
		printf("\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	snprintf(query, tamanho, "SELECT nomeProduto,descricao,avaliacaoPositiva,avaliacaoNegativa,visualizacoes+visualizacaoanom,tipo,categoria FROM produto P WHERE P.idproduto LIKE BINARY \'%s\';", idProduto);
	if (query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterInformacoesBasicasDeProdutoDoBanco() aporkasd09bv\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	return retornaInformacoesObtidasNaQuery(usuario, query);
}

char *obterInformacoesAvancadasDeProdutoDoBanco(Usuario *usuario, char *idProduto)
{
	if (usuario->conexao == NULL)
	{
		printf(" ERRO: conexao nula em OperacoesBanco.h obterInformacoesAvancadasDeProdutoDoBanco()\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(usuario_conectarBanco(usuario))
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação\n");
			return strdup("ERRO interno(falha de conexao com o banco), tente novamente");
		}
	}
	if (idProduto == NULL)
	{
		printf(" Warning: idProduto nulo detectado em OperacoesBanco.h obterInformacoesAvancadasDeProdutoDoBanco() dsahjkbr\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (usuario == NULL)
	{
		printf(" Warning: usuario nulo detectado em OperacoesBanco.h obterInformacoesAvancadasDeProdutoDoBanco() v98a7ef\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" Warning: usuario nao conectado detectado em OperacoesBanco.h obterInformacoesAvancadasDeProdutoDoBanco() e09as8d93\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (produtoVencido(idProduto, usuario))
	{
		printf(" Warning: Produto %s vencido para %s\n", idProduto, usuario_obterLogin(usuario));
		return RETORNO_ERRO_NAO_AUTORIZADO_STR_DINAMICA;
	}

	//int tamanho = 150+10+1;
	int tamanho = 173;
	char *query = (char *)malloc(sizeof(char) * tamanho);
	if (query == NULL)
	{
		printf("\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	snprintf(query, tamanho, "SELECT nomeProduto,descricao,avaliacaoPositiva,avaliacaoNegativa,visualizacoes,tipo,categoria,duracao,datacriacao FROM produto P WHERE P.idproduto LIKE BINARY \'%s\';", idProduto);
	if (query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterInformacoesAvancadasDeProdutoDoBanco() aporkasd09bv\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	return retornaInformacoesObtidasNaQuery(usuario, query);
}

char *obterInformacoesClienteDoBanco(Usuario *usuario)
{
	if (usuario == NULL)
	{
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	int tamanho = strlen(usuario_obterId(usuario)) + 134 + 1;
	char *query = (char *)malloc(sizeof(char) * tamanho);
	if (query == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h obterInformacoesClienteDoBanco()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	snprintf(query, tamanho, "SELECT C.nome,C.sobrenome,C.sexo,C.telefone,C.datanascimento,C.pontos,C.localizacao_idlocalizacao FROM cliente C WHERE C.idcliente=\'%s\';", usuario_obterId(usuario));
	// printf(" Query = |%s|\n", query);
	// printf(" Tamanho da query |%ld|\n", strlen(query));
	if (query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterInformacoesClienteDoBanco()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	return retornaInformacoesObtidasNaQuery(usuario, query);
}

char *obterInformacoesContratanteDoBanco(Usuario *usuario)
{
	if (usuario == NULL)
	{
		printf(" Warning: Usuario nulo detectado em OperacoesBanco.h obterInformacoesContratanteDoBanco()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	int tamanho = strlen(usuario_obterId(usuario)) + 120 + 1;
	char *query = (char *)malloc(sizeof(char) * tamanho);
	if (query == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h obterInformacoesContratanteDoBanco()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	snprintf(query, tamanho, "SELECT C.nome,C.cnpj,C.plano,C.email,C.telefone,C.localizacao_idlocalizacao FROM contratante C WHERE C.idContratante=\'%s\';", usuario_obterId(usuario));
	if (query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterInformacoesContratanteDoBanco()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	return retornaInformacoesObtidasNaQuery(usuario, query);
}

char *obterUltimos10ProdutosAdicionadosPeloContratanteDoBanco(Usuario *usuario)
{
	if (usuario == NULL)
	{
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	int tamanho = 160 + strlen(usuario_obterId(usuario)) + 1;
	char *query = (char *)malloc(sizeof(char) * tamanho);
	if (query == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h obterUltimos10ProdutosAdicionadosPeloContratanteDoBanco()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	snprintf(query, tamanho, "SELECT P.idproduto FROM produto P JOIN contratante C ON P.contratante_idcontratante=C.idcontratante WHERE C.idcontratante=%s ORDER BY P.datacriacao DESC LIMIT 10;", usuario_obterId(usuario));
	if (query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterUltimos10ProdutosAdicionadosPeloContratanteDoBanco()\n");
	}
	return retornaNIteracoesDaQuery(usuario, query, 10);
}

char *obterUltimos10ProdutosAdicionadosRealidadeAumentadaPeloContratanteDoBanco(Usuario *usuario)
{
	if (usuario == NULL)
	{
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	int tamanho = 174 + strlen(usuario_obterId(usuario)) + 1 + 1;
	char *query = (char *)malloc(sizeof(char) * tamanho);
	if (query == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h obterUltimos10ProdutosAdicionadosRealidadeAumentadaPeloContratanteDoBanco()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	snprintf(query, tamanho, "SELECT P.idproduto FROM produto P JOIN contratante C ON P.contratante_idcontratante=C.idcontratante WHERE C.idcontratante=%s AND P.tipo=\'%s\' ORDER BY P.datacriacao DESC LIMIT 10;", usuario_obterId(usuario), TIPO_REALIDADE_AUMENTADA);
	if (query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterUltimos10ProdutosAdicionadosPeloContratanteDoBanco()\n");
	}
	return retornaNIteracoesDaQuery(usuario, query, 10);
}

char *obterUltimos10ProdutosAdicionadosRealidadeVirtualPeloContratanteDoBanco(Usuario *usuario)
{
	if (usuario == NULL)
	{
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	int tamanho = 174 + strlen(usuario_obterId(usuario)) + 1 + 1;
	char *query = (char *)malloc(sizeof(char) * tamanho);
	if (query == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h obterUltimos10ProdutosAdicionadosRealidadeAumentadaPeloContratanteDoBanco()\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	snprintf(query, tamanho, "SELECT P.idproduto FROM produto P JOIN contratante C ON P.contratante_idcontratante=C.idcontratante WHERE C.idcontratante=%s AND P.tipo=\'%s\' ORDER BY P.datacriacao DESC LIMIT 10;", usuario_obterId(usuario), TIPO_REALIDADE_VIRTUAL);
	if (query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h obterUltimos10ProdutosAdicionadosPeloContratanteDoBanco()\n");
	}
	return retornaNIteracoesDaQuery(usuario, query, 10);
}

/** 
 * @brief  Retorna ao usuario 10 produtos da empresa, definidos em "pagina"
 * @note   libera idContratante e pagina
 * @param  *usuario: 
 * @param  *pagina: 
 * @retval 
 */
char *obter10ProdutosDaEmpresaDoBanco(Usuario *usuario, char *pagina, char *idContratante)
{
	if( usuario == NULL )
	{
		printf(" ERRO: Usuario nulo detectado em OperacoesBanco.h obter10ProdutosDaEmpresaDoBanco() asvdurw\n");
		if (pagina != NULL)
		{
			free(pagina);
			pagina = NULL;
		}
		if (idContratante != NULL)
		{
			free(idContratante);
			idContratante = NULL;
		}
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if( usuario_obterLogin(usuario) == NULL )
	{
		printf(" ERRO: Usuario nao conectado detectado em OperacoesBanco.h obter10ProdutosDaEmpresaDoBanco() asouvnbrjd\n");
		if (pagina != NULL)
		{
			free(pagina);
			pagina = NULL;
		}
		if (idContratante != NULL)
		{
			free(idContratante);
			idContratante = NULL;
		}
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if( pagina == NULL )
	{
		printf(" ERRO: Pagina nula detectada em OperacoesBanco.h obter10ProdutosDaEmpresaDoBanco() 0h28tbn\n");
		if (idContratante != NULL)
		{
			free(idContratante);
			idContratante = NULL;
		}
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if ( !usuario_PermissaoContratante(usuario) )
	{
		printf(" WARNING: Comando permite consultas de produtos antigos de empresas em OperacoesBanco.h obter10ProdutosDaEmpresaDoBanco() eqiugnrehy\n");
		if (idContratante == NULL)
		{
			printf(" Warning: Comando insuficiente detectado em OperacoesBanco.h obter10ProdutosDaEmpresaDoBanco() a616t8j4sda\n");
			free(pagina);
			pagina = NULL;
			return RETORNO_ERRO_COMANDO_INSUFICIENTE_STR_DINAMICA;
		}

		int tamanho = 160 + 2 + strlen(idContratante);
		char * query = (char *)calloc(sizeof(char), tamanho);
		if (query == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para string em OperacoesBanco.h obter10ProdutosDaEmpresaDoBanco() skljnhteuiore50\n");
			free(idContratante);
			idContratante = NULL;
			free(pagina);
			pagina = NULL;
			return RETORNO_ERRO_INTERNO_STR_DINAMICA;
		}

		snprintf(query, tamanho, "SELECT P.idproduto,P.datacriacao FROM produto P JOIN contratante C ON P.contratante_idcontratante=C.idcontratante WHERE C.idcontratante=\'%s\' ORDER BY P.datacriacao", idContratante);
		
		free(idContratante);
		idContratante = NULL;

		if (query == NULL)
		{
			printf(" Warning: Falha ao formatar string em OperacoesBanco.h obter10ProdutosDaEmpresaDoBanco() kasdjghr\n");
			free(pagina);
			pagina = NULL;
			return RETORNO_ERRO_INTERNO_STR_DINAMICA;
		}

		return retornaPaginado(usuario, query, pagina);

	}
	else
	{
		printf(" LOG: Autorização concedida continuando comando em OperacoesBanco.h obter10ProdutosDaEmpresaDoBanco() asdlkfhgrf\n");

		int tamanho = 160 + 2 + strlen(usuario_obterId(usuario));
		char * query = (char *)calloc(sizeof(char), tamanho);
		if (query == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para string em OperacoesBanco.h obter10ProdutosDaEmpresaDoBanco() skljnhteuiore50\n");
			free(pagina);
			pagina = NULL;
			return RETORNO_ERRO_INTERNO_STR_DINAMICA;
		}

		snprintf(query, tamanho, "SELECT P.idproduto,P.datacriacao FROM produto P JOIN contratante C ON P.contratante_idcontratante=C.idcontratante WHERE C.idcontratante=\'%s\' ORDER BY P.datacriacao", usuario_obterId(usuario));
		
		if (query == NULL)
		{
			printf(" Warning: Falha ao formatar string em OperacoesBanco.h obter10ProdutosDaEmpresaDoBanco() kasdjghr\n");
			free(pagina);
			pagina = NULL;
			return RETORNO_ERRO_INTERNO_STR_DINAMICA;
		}

		return retornaPaginado(usuario, query, pagina);
	}
}


/** 
 * @brief  Retorna o ID da empresa responsável pelo produto idProduto
 * @note   Retorna direto ao usuario
 * @param  *usuario: 
 * @param  *idproduto: 
 * @retval int
 */
char *retornaIdDeEmpresaDadoProdutoDoBanco(Usuario *usuario, char *idProduto)
{
	if (!usuarioValido(usuario, "OperacoesBanco.h retornaIdDeEmpresaDadoProdutoDoBanco() sajhjkrh"))
	{
		if (idProduto != NULL)
		{
			free(idProduto);
			idProduto = NULL;
		}
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (idProduto == NULL )
	{
		printf(" Warning: idProduto == NULL em OperacoesBanco.h retornaIdDeEmpresaDadoProdutoDoBanco() askjhbjtdg\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	// int tamanho = 124 + 1 + TAMANHO_ID_PRODUTO;
	int tamanho = 147;
	char *query = (char *)calloc(sizeof(char), tamanho);
	if (query == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para query em OperacoesBanco.h retornaIdDeEmpresaDadoProdutoDoBanco() akejghjkdsf\n");
		free(idProduto);
		idProduto = NULL;
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	snprintf(query, tamanho, "SELECT C.idcontratante FROM contratante C JOIN produto P ON P.contratante_idcontratante=C.idcontratante WHERE P.idproduto LIKE BINARY \'%s\';", idProduto);

	free(idProduto);
	idProduto = NULL;

	if (query == NULL)
	{
		printf(" Warning: Falha ao formatar query em OperacoesBanco.h retornaIdDeEmpresaDadoProdutoDoBanco() askjghrig\n");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	return retornaUnicoRetornoDaQuery(usuario, query);
}

/** 
 * @brief  Gera a query pra lista de desejos e retorna paginado do banco de dados
 * @note   Depende de "retornaPaginado()"
 * @param  *usuario: Usuario a quem a lista de desejos pertenca
 * @param  *pagina: Página que é desejado ser retornada
 * @retval ERRO interno, ou, as informações paginadas da forma correta
 */
char *retornaListaDesejosDoBanco(Usuario *usuario, char *pagina)
{
	const char *localizacao = "OperacoesBanco.h retornaListaDesejosDoBanco()";
	if (!usuarioValido(usuario, localizacao))
	{
		if (pagina != NULL)
		{
			free(pagina);
			pagina = NULL;
		}
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if (pagina == NULL)
	{
		geraLog(LOG, "Usuario nao informou pagina, definindo como \'1\'");
		pagina = strdup("1");
	}

	int tamanho = strlen(usuario_obterId(usuario)) + 1 + 67;
	char *query = (char *)calloc(sizeof(char), tamanho);
	if (query == NULL)
	{
		geraLog(ERRO, "Falha ao alocar memoria para string para query");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}

	snprintf(query, tamanho, "SELECT L.idproduto FROM listaDesejos L WHERE L.cliente_idcliente=\'%s\'", usuario_obterId(usuario));
	
	if (query == NULL)
	{
		geraLog(ERRO, "Falha ao formatar string para query");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	return retornaPaginado(usuario, query, pagina);
}


/** 
 * @brief  Gera a query para obter a quantidade de reclamações para tal produto do banco de dados e retorna ao usuário
 * @note   Retorna direto ao usuario
 * @param  *usuario: Usuario a quem a lista de desejos pertenca
 * @retval ERRO interno, ou, o número de reclamações do produto
 * TODO Completar esse comando
 */
char *obterQuantidadeDeReclamacoesDoProdutoDoBanco(Usuario *usuario, char *idProduto)
{
	// char *query = NULL; //Descomentar depois
	// int tamanho = 0; //Descomentar depois
	if( idProduto == NULL )
	{
		geraLog(ERRO, "id de produto nulo identificado");
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if( !usuarioValido(usuario, "obterQuantidadeDeReclamacoesDoProdutoDoBanco(), OperacoesBanco.h") )
	{
		free( idProduto );
		idProduto = NULL;
		return RETORNO_ERRO_INTERNO_STR_DINAMICA;
	}
	if( !usuario_PermissaoContratante(usuario) || !usuario_PermissaoRoot(usuario) )
	{
		geraLog(WARNING, "Usuario normal tentando acessar dados privados a contratante");
		free( idProduto );
		idProduto = NULL;
		return RETORNO_ERRO_NAO_AUTORIZADO_STR_DINAMICA;
	}

//	tamanho = TAMANHODAQUERY + TAMANHO_ID_PRODUTO + 1;
//
//	snprintf(query, tamanho, "SELECT ", idProduto);
//

	return RETORNO_ERRO_COMANDO_NAO_CONSTRUIDO_STR_DINAMICA;
}

/* ***FIM COMANDOS DE OBTENÇÃO*** */

#endif //__OPERACOES_BANCO__


