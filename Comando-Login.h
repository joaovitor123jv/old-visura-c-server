#include <stdio.h>
#include <mysql/mysql.h>
#include "Comandos.h"
#include "OperacoesBanco.h"

char *comandoLogin(char *comando, bool *usuarioAnonimo)/* APP 1 login senha */
{
	char *token = NULL;
	char *senha= NULL;
	token = strtok(NULL, " ");
	if(comando == NULL)
	{
		printf("ERRO FATAL (interpretadorDeComandos.h) (comandoLogin)\n");
		exit(1);
	}
	if(token != NULL)
	{
		if(strcmp(token, "1") == 0)/* Cliente solicitou Login (APP 1) */
		{
			printf("CLIENTE SOLICITANDO LOGIN (interpretadorDeComandos.h) (comandoLogin)\n");
			token = strtok(NULL, " ");
			// token = "email";
			if(token == NULL)
			{
				printf(" Token NULO (comandoLogin) (Comando-Login.h)\n");
				return NULL;
			}
			senha = strtok(NULL, "\0");
			if(senha == NULL)
			{
				printf(" Senha NULA (comandoLogin) (Comando-Login.h)\n");
				return NULL;
			}


			printf("\n\n ************COMANDO LOGIN***********\n\n");// TODO **************************************************************************
			printf(" \tComando recebido: %s\n", comando);
			printf(" \tProximo token (Login): %s\n", token); // Login
			printf(" \tTamanho do login: %ld\n", strlen(token));
			printf(" \tSenha recebida : %s\n", senha);
			printf(" \tTamanho da senha recebida: %ld\n", strlen(senha));
			printf("\n\n\n\n");//Separando informações

			if(strlen(token) <= 0 || strlen(token) > 64)
			{
				printf(" Login muito grande ou muito pequeno(Comando-Login.h) (comandoLogin())\n");
				return NULL;
			}

			// if(strlen(senha) != 256)
			// {
			// 	printf("Tamanho incorreto para senha criptografada (Comando-Login.h) (comandoLogin())\n");
			// 	return false;
			// }

			int contador = 0;
			bool emailValido = false;

			for(contador  = 0; contador < strlen(token); contador++)/* Checa se email é válido */
			{
				if(token[contador] == '@')
				{
					emailValido = true;
					break;
				}
			}

			if(!emailValido)
			{
				*usuarioAnonimo = true;
				// return NULL;
			}
			else
			{
				*usuarioAnonimo = false;
				// *usuarioAnonimo = true;
			}

			// MYSQL *conexao = malloc(sizeof(MYSQL));

			if(conexao == NULL)
			{
				// printf(" Falha ao tentar alocar memória para conexao (Comando-Login.h) comandoLogin()\n");
				printf("Conexão com o banco de dados é NULA (Comando-Login.h) (comandoLogin())\n");
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

			// if(!conectarBanco(conexao))
			// {
			// 	printf("Falha ao tentar se conectar ao banco de dados (comandoLogin) (Comando-Login.h)\n");
			// 	return NULL;
			// }
			// printf(" Retorno com sucesso para Comando-Login\n");


			// if(!checarLogin(conexao, token))
			if(!checarLogin(token))
			{
				printf("Erro ao conectar (Comando-Login.h) (comandoLogin)\n");
				printf("\tFalha ao checar login\n");
				printf("\t\tEncerrando Conexão\n");
				
				// mysql_close(conexao);
				// free(conexao);
				// conexao = NULL;
				return NULL;
			}


			// printf("\t\tEncerrando Conexão\n");
			
			// mysql_close(conexao);
			// free(conexao);
			// conexao = NULL;

			printf("CONEXÃO ENCERRADA\n");
			printf(" TOKEN = %s\n", token);
			return token;
		}
		else
		{
			printf("CLIENTE PRECISA SOLICITAR LOGIN PARA REALIZAR QUALQUER OPERACAO\n");
			return NULL;
		}
	}
	else
	{
		printf("POUCOS ARGUMENTOS PARA CONTINUAR (interpretadorDeComandos.h) (comandoLogin)\n");
		return NULL;
	}
	printf("ERRO INDECIFRAVEL (interpretadorDeComandos.h) (comandoLogin)\n");
	return NULL;// APAGAR DEPOIS
	exit(1);
}
