#include <stdio.h>
#include <stdlib.h>
#include "Fila/Fila.h"
#include "AdaptadorDeString.h"

int main()
{
	char *retorno = NULL;
	char original[] = { "Eu\testou\ntestando\nisso\\aqui\'porque\"sim\"\'\ncontra\r\aminha\tvontade\nsoqn"};

	printf(" ****** original = \n|%s|\n", original);
	retorno = padronizarString(original);
	if(retorno != NULL)
	{
		printf(" DEU CERTO PORRA!\n");
		printf(" Retorno = |%s|\n", retorno);
	}
	else
	{
		printf(" Que caralhos ta acontecendo aqui ?\n");
	}
	return 0;

}
