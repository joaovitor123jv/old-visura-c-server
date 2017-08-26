#include <stdio.h>
#include <stdlib.h>
#include "Fila/Fila.h"

int main()
{
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
	char *retorno;

	printf(" Quantidade de caracteres = %d\n", fila_getQuantidadeDeCaracteres(fila));
	fila_remover(fila, retorno);

	printf(" Retorno = %c\n", retorno);
	fila_mostraFila(fila);

	return 0;
}
