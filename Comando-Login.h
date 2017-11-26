#include <stdio.h>
#include <mysql/mysql.h>
#include "Comandos.h"
#include "OperacoesBanco.h"

//char *comandoLogin(char *comando, bool *usuarioAnonimo)/* APP 1 login senha */
char *comandoLogin(bool *usuarioAnonimo)
{
	char *email = NULL;
	char *senha= NULL;
	char *token;
	int contador = 0;

	token = strtok(NULL, " ");

	if(token != NULL)
	{
//		if(strcmp(token, COMANDO_LOGIN) == 0)/* Cliente solicitou Login (APP 1) */
		if( token[0] == '1' && token[1] == '\0' )// Otimização
		{
			printf(" LOG: cliente solicitando login (interpretadordecomandos.h) (comandoLogin)\n");
			email = strtok(NULL, " ");
			if(email == NULL)
			{
				printf(" Warning: Email NULO (comandoLogin) (Comando-Login.h)\n");
				return NULL;
			}
			if( strlen(email) > TAMANHO_LOGIN )
			{
				printf(" Warning: Email exageradamente grande em Comando-Login.h comandoLogin()\n");
				return NULL;
			}
			
			senha = strtok(NULL, "\0");
			if(senha == NULL)
			{
				printf(" Warning: Senha NULA (comandoLogin) (Comando-Login.h)\n");
				return NULL;
			}
			if( strlen(senha) > TAMANHO_SENHA )
			{
				printf(" Warning: Email exageradamente grande em Comando-Login.h comandoLogin()\n");
				return NULL;
			}


			printf("\n\n ************COMANDO LOGIN***********\n\n");// TODO **************************************************************************
			printf(" \tEmail : %s\n", email); // Login
			printf(" \tSenha recebida : %s\n", senha);
			printf("\n\n\n\n");//Separando informações

			*usuarioAnonimo = false;

			for(contador  = 0; contador < strlen(token); contador++)/* Checa se email é válido */
			{
				if(token[contador] == '@')
				{
					*usuarioAnonimo = true;
					break;
				}
			}


			if(!checarLogin(email, senha))// APP 1 login senha
			{
				printf(" Warning: Erro ao conectar ou de autenticação em (Comando-Login.h) (comandoLogin)\n");
				printf("\tFalha ao checar login\n");
				return NULL;
			}

			return email;
		}
		else
		{
			printf(" Warning: cliente precisa solicitar login para realizar qualquer operacao Comando-Login.h comandoLogin()\n");
			return NULL;
		}
	}
	else
	{
		printf(" Warning; poucos argumentos para continuar (Comando-Login].h) (comandoLogin)\n");
		return NULL;
	}
	printf(" Warning: erro indecifravel (Comando-Login.h) (comandoLogin)\n");
	return NULL;
}
