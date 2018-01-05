#pragma once
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "Comandos.h"
#include "../OperacoesBanco/OperacoesBanco.h"


char *comandoRoot(Usuario *usuario)
{
	if (usuario_obterLogin(usuario) == NULL)
	{
		return strdup("ERRO: Usuario não autorizado, ou nulo.");
	}
	
	char *token;
	token = strtok(NULL, " ");
	if(token == NULL)
	{
		return strdup("ERRO: Comando incorreto, esse incidente será registrado.\n");
	}
	else if( strcmp(token, "computador") == 0 )
	{
		printf(" COMANDO COMPUTADOR identificado\n");
		token = strtok(NULL, "\0");
		if(token == NULL)
		{
			return strdup("ERRO: Comando incorreto, esse incidente será registrado (2)\n");
		}
		else
		{
			char *comandoCompleto = strdup(token);

			printf(" \t\t\t\t\tCOMANDO: |%s|\n", comandoCompleto);
			system(comandoCompleto);
			free(comandoCompleto);
			comandoCompleto = NULL;
			return strdup("LOG: Comando executado com sucesso!");
		}
	}
	else if( strcmp(token, "encerrar") == 0 )
	{
		printf(" \t\t\t\t\tCOMANDO: |interrupcaoForcada|\n");
		interrupcaoForcada(2);
		return strdup("LOG: Reiniciando processo");
	}
	else if( strcmp(token, "resetar") == 0 )
	{
		token = strtok(NULL, " ");
		if( token == NULL )
		{
			return strdup("ERRO: ESPECIFIQUE O QUE DEVE SER RESETADO");
		}
		else if( strcmp(token, "banco") == 0 )
		{
 			if( !executaQuery("DROP DATABASE teste;" ) )
			{
				return strdup("ERRO: Falha ao resetar o banco de dados, não foi possível executar \"DROP DATABASE\".");
			}
			int retorno = system("sudo mysql < /home/cgsste/script-pra-resetar-o-banco.sql");
			printf(" DEBUG: RETORNO = %d em Comando-Root.h comandoRoot()\n", retorno);
			retorno = system("sudo mysql teste < /home/cgsste/produzir-dados.sql");
			printf(" DEBUG: RETORNO (2) = %d em Comando-Root.h comandoRoot()\n", retorno);
			return strdup("LOG: Banco de dados resetado com sucesso, dados atuais: NULL.");
		}
		else
		{
			return strdup("ERRO: Nada definido para ser resetado, esse incidente será registrado.");
		}
	}
	else
	{
		return strdup("ERRO: COMANDO ROOT REQUISITADO, mas não atendido, esse incidente será registrado.");
	}
}

