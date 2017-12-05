#include<stdio.h>

#include "GerenciadorDeThreads.h"

int main()
{
	printf(" INISNDISANDIAS\n");
	GerenciadorDeThreads *gerenciador;
	gerenciador = initGerenciadorDeThreads();
	gerenciador->rodarEmBackground(NULL);
	return 0;
}
