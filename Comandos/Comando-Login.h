#pragma once
#include <stdio.h>
#include <mysql/mysql.h>
#include "Comandos.h"
#include "../AdaptadorDeString/AdaptadorDeString.h"
#include "../OperacoesBanco/OperacoesBanco.h"
#include "../OperacoesBanco/OperacoesBanco-FuncoesGenericas.h"
#include "../Usuario.h"

/*bool comandoLogin(Usuario *usuario)
{

}*/

// char *comandoLogin(bool *usuarioAnonimo, Usuario *usuario)
char *comandoLogin(Usuario *usuario)
{
	char *email = NULL;
	char *senha= NULL;
	char *token;
	// int contador = 0;

	token = usuario_getNextToken(usuario);

	if(token != NULL)
	{
		if( token[0] == '1' && token[1] == '\0' )// Otimização
		{
			printf(" LOG: cliente solicitando login (interpretadordecomandos.h) (comandoLogin)\n");
			email = usuario_getNextToken(usuario);
			if(email == NULL)
			{
				printf(" Warning: Email NULO (comandoLogin) (Comando-Login.h)\n");
				return NULL;
			}
			if( stringMaior(email, TAMANHO_LOGIN) )
			{
				printf(" Warning: Email exageradamente grande em Comando-Login.h comandoLogin()\n");
				return NULL;
			}
			
			senha = usuario_getNextToken(usuario);
			if(senha == NULL)
			{
				printf(" Warning: Senha NULA (comandoLogin) (Comando-Login.h)\n");
				return NULL;
			}
			if( stringMaior(senha, TAMANHO_SENHA) )
			{
				printf(" Warning: Email exageradamente grande em Comando-Login.h comandoLogin()\n");
				return NULL;
			}


			printf("\n\n ************COMANDO LOGIN***********\n\n");// TODO **************************************************************************
			printf(" \tEmail : %s\n", email); // Login
			printf(" \tSenha recebida : %s\n", senha);
			printf("\n\n\n\n");//Separando informações

			if( new_Usuario(usuario, email, senha) )//DONE
			{
				printf(" LOG: Estrutura de usuario definida com sucesso, usuario conectado em Comando-Login.h comandoLogin()\n");
				//*usuarioAnonimo = usuario_PermissaoAnonimo(usuario);
			}
			else
			{
				printf(" Warning: Usuario não autorizado em Comando-Login.h comandoLogin()\n");
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
