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


bool comandoRemover(char *email, bool usuarioAnonimo)/* APP 5 algumaCoisa */
{
	if( email == NULL )
	{
		printf(" Warning: email nulo detectado em Comando-Remover.h comandoRemover()\n");
		return false;
	}
	printf(" LOG: Solicitando remocao de algo em Comando-Remover.h comandoRemover()\n");
	return false;
}
