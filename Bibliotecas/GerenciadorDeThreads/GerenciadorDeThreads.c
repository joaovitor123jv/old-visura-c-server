#ifndef _GERENCIADOR_DE_THREADS_
	#include "GerenciadorDeThreads.h"
#endif
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void *GER_adicionaThread(void *arg, GerenciadorDeThreads *this)
{
	if( this == NULL )
	{
		printf(" askdjhsajkdhaskjd\n");
		return NULL;
	}
	printf(" THREAD ADICIONADA E EXECUTANDO\n");
}

GerenciadorDeThreads *initGerenciadorDeThreads()
{
	GerenciadorDeThreads *gerenciador;
	gerenciador = malloc(sizeof(GerenciadorDeThreads));
	if( gerenciador == NULL )
	{
		printf(" Warning: Falha ao inicializar gerenciador de threads em GerenciadorDeThreads.c initGerenciadorDeThreads()\n");
		return NULL;
	}
	gerenciador->numeroDeThreads = 0;
	gerenciador->rodarEmBackground = GER_adicionaThread;

	return gerenciador;
}


