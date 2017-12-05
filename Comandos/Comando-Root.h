#pragma once
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


char *comandoRoot(char *email)
{
	char *token;
	char *retorno;
	token = strtok(NULL, " ");
	if(token == NULL)
	{
		retorno = strdup("Não houve comando: nada a ser executado\n");
		return retorno;
	}
	else if( strcmp(token, "computador") == 0 )
	{
		printf(" COMANDO COMPUTADOR identificado\n");
		token = strtok(NULL, "\0");
		if(token == NULL)
		{
			retorno = strdup("Não houve comando: nada a ser executado\n");
			return retorno;
		}
		else
		{
			char *comandoCompleto = strdup(token);

			printf(" \t\t\t\t\tCOMANDO: |%s|\n", comandoCompleto);
			system(comandoCompleto);
			retorno = strdup("Comando executado com sucesso!");
			free(comandoCompleto);
			comandoCompleto = NULL;
			return retorno;
		}
	}
	else if( strcmp(token, "encerrar") == 0 )
	{
		printf(" \t\t\t\t\tCOMANDO: |interrupcaoForcada|\n");
		interrupcaoForcada(2);
		return strdup("Reiniciando processo");
	}

	retorno = strdup("COMANDO ROOT REQUISITADO\0");
	return retorno;
}

