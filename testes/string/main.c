#include <stdio.h>
#include <stdlib.h>
#include "Fila/Fila.h"
#include "AdaptadorDeString.h"

int main()
{
	char *retorno;
	char original[] = { "Eu\testou\ntestando\nisso\\aqui\'porque\"sim\"\'\ncontra\r\aminha\tvontade\nsoqn"};

	printf(" ****** original = \n|%s|\n", original);
	padronizarString(retorno, original);
//	printf(" ****** modificada = \n|%s|\n", retorno);
	return 0;





	/* 
	Fila *fila;
	fila = new_Fila();

	if(fila == NULL)
	{
		printf(" DEU PAU !\n");
	}

	fila_inserir(fila, 'c');
	fila_mostraFila(fila);

	printf(" QUantidade de caracteres = %d\n", fila_getQuantidadeDeCaracteres(fila));

	fila_inserir(fila, 'o');
	fila_inserir(fila, 'c');
	fila_inserir(fila, 'a');
	fila_inserir(fila, ' ');
	fila_inserir(fila, 'c');
	fila_inserir(fila, 'o');
	fila_inserir(fila, 'l');
	fila_inserir(fila, 'a');
	fila_inserir(fila, '!');
	fila_mostraFila(fila);

	printf("\n\n\n");
	char retorno = 'F';

	printf(" Quantidade de caracteres = %d\n", fila_getQuantidadeDeCaracteres(fila));
	fila_remover(fila, &retorno);

	printf(" Retorno = %c\n", retorno);
	fila_mostraFila(fila);
	printf(" Quantidade de caracteres = %d\n", fila_getQuantidadeDeCaracteres(fila));



	printf("\n\n\n");
	fila_remover(fila, &retorno);
	fila_mostraFila(fila);
	printf(" Quantidade de caracteres = %d\n", fila_getQuantidadeDeCaracteres(fila));
	printf(" Retorno = %c\n", retorno);

	free_Fila(fila);

	return 0;
	*/
}
