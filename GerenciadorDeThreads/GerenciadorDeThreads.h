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

typedef struct 
{
	int numeroDeThreads;
	void *(*rodarEmBackground)(void *);
	GerenciadorDeThreads *this;
	void *(threadMonitoradora)(void *);
}GerenciadorDeThreads;

GerenciadorDeThreads *initGerenciadorDeThreads();
