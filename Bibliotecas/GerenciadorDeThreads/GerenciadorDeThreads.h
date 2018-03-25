#ifndef _GERENCIADOR_DE_THREADS_
#define _GERENCIADOR_DE_THREADS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef bool
#define bool char
#endif

typedef struct GerenciadorDeThreads
{
	int numeroDeThreads;
	void *(*rodarEmBackground)(void *, struct GerenciadorDeThreads *);
	struct GerenciadorDeThreads *this;
//	void *(threadMonitoradora)(void *);
}GerenciadorDeThreads;

GerenciadorDeThreads *initGerenciadorDeThreads();

bool freeGerenciadorDeThreads(GerenciadorDeThreads *gerenciador);

#endif /* _GERENCIADOR_DE_THREADS_ */
