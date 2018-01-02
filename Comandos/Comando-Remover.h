#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "Comandos.h"

#ifndef bool
#define bool char
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

/*
	@author João Vitor Antoniassi Segantin
	@github https://github.com/joaovitor123jv
*/


bool comandoRemover(Usuario *usuario)/* APP 5 algumaCoisa */
{
	if( usuario == NULL )
	{
		printf(" Warning: usuario nulo detectado em Comando-Remover.h comandoRemover()\n");
		return false;
	}
	if (usuario_PermissaoAnonimo(usuario))
	{
		printf(" Warning: Usuario anonimo não pode excluir sua conta em Comando-Remover.h comandoRemover()\n");
		return false;
	}
	printf(" LOG: Solicitando remocao de algo em Comando-Remover.h comandoRemover()\n");
	char *token = strtok(NULL, " ");
	if( token == NULL )
	{
		printf(" Warning: Comando insuficiente em Comando-Remover.h comandoRemover()\n");
		return false;
	}
	return false;
}
