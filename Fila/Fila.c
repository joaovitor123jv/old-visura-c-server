#include <stdio.h>
#include <stdlib.h>

#ifndef bool
#define bool char
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif



typedef struct Elemento_Fila
{
	char item;
	struct Elemento_Fila *proximo;
}Elemento_Fila;

struct Fila
{
	struct Elemento_Fila *inicio;
	struct Elemento_Fila *fim;
	int quantidadeDeCaracteres;
};


struct Fila *new_Fila()
{
	struct Fila *fila = malloc(sizeof(struct Fila));
	if(fila == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para fila em Fila.c new_Fila() 1244800\n");
		return NULL;
	}
	fila->inicio = NULL;
	fila->fim = NULL;
	fila->quantidadeDeCaracteres = 0;
	return fila;
}

bool free_Fila(struct Fila *fila)
{
	if(fila == NULL)
	{
		printf(" Warning: ponteiro nulo em Fila.c free_Fila() hqknbcc\n");
		return false;
	}

	if(fila->quantidadeDeCaracteres == 0)
	{
		printf(" LOG: Nada a resolver, simplesmente liberando fila em Fila.c free_Fila()\n");
		free(fila);
		fila = NULL;
		return true;
	}
	else
	{
		struct Elemento_Fila *elementoAux;

		while(fila->inicio != NULL)
		{
			printf(" LOG: Limpou ! em Fila.c free_Fila()\n");
			elementoAux = fila->inicio->proximo;
			free(fila->inicio);
			fila->inicio = NULL;
			fila->inicio = elementoAux;
		}
		free(fila);
		fila = NULL;

		return true;
	}
}


bool fila_inserir(struct Fila *fila, char item)//Insere item no fim da fila
{
	if(fila == NULL)
	{
		printf(" Warning: ponteiro de fila nulo em Fila.c fila_inserir() \n");
		return false;
	}

	if(fila->inicio == NULL)
	{
		Elemento_Fila *elementoAux = malloc(sizeof(Elemento_Fila));
		if(elementoAux == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para primeiro elemento da fila em Fila.c fila_inserir()\n");
			return false;
		}
		else
		{
			fila->inicio = elementoAux;
			fila->fim = elementoAux;
			elementoAux->item = item;
			elementoAux->proximo = NULL;
			fila->quantidadeDeCaracteres = fila->quantidadeDeCaracteres + 1;
			return false;
		}
	}
	else if(fila->inicio == fila->fim)
	{
		fila->fim = malloc(sizeof(Elemento_Fila));
		if(fila->fim == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para fim de fila em Fila.c fila_inserir\n");
			fila->fim = fila->inicio;
			return false;
		}
		else
		{
			fila->fim->item = item;
			fila->fim->proximo = NULL;
			fila->inicio->proximo = fila->fim;
			fila->quantidadeDeCaracteres = fila->quantidadeDeCaracteres + 1;
			return true;
		}
	}
	else
	{
		Elemento_Fila *elementoAux = malloc(sizeof(Elemento_Fila));
		Elemento_Fila *anterior;
		if(elementoAux == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para elemento auxiliar em Fila.c fila_inserir n3iosin22\n");
			return false;
		}
		else
		{
			elementoAux->item = item;
			elementoAux->proximo = NULL;
			anterior = fila->fim;
			anterior->proximo = elementoAux;
			fila->fim = elementoAux;
			fila->quantidadeDeCaracteres = fila->quantidadeDeCaracteres + 1;
			return true;
		}
	}
}

bool fila_remover(struct Fila *fila, char *retorno)// Funcionando !
{
	if(fila == NULL)
	{
//		retorno = NULL;
		return false;
	}
	else if(fila->inicio == fila->fim)
	{
		*retorno = fila->inicio->item;
		free(fila->inicio);
		fila->inicio = NULL;//Fila inicio e fila fim são a mesma coisa
		fila->fim = NULL;
		return true;
	}
	else
	{
		if(fila->quantidadeDeCaracteres > 1)
		{
			Elemento_Fila *elementoAux;
			elementoAux = fila->inicio->proximo;
//			retorno = NULL;
			*retorno = fila->inicio->item;
			free(fila->inicio);
			fila->inicio = elementoAux;
			fila->quantidadeDeCaracteres = fila->quantidadeDeCaracteres - 1;
			return true;
		}
		else if(fila->quantidadeDeCaracteres == 1)
		{
			printf(" Warning: 2 caracteres na fila !!!!!!!!!\n");
//			retorno = NULL;
			*retorno = fila->inicio->item;
			free(fila->inicio);
			fila->inicio = fila->fim;
			fila->quantidadeDeCaracteres = fila->quantidadeDeCaracteres - 1;
			return true;
		}
		else
		{
			printf(" Warning: Chegou num lugar que não era pra chegar em Fila.c fila_remover()\n");
			return false;
		}
	}
}


bool fila_mostraFila(struct Fila *fila)
{
	Elemento_Fila *elementoAux;
	int contador = 0;
	if(fila == NULL)
	{
		printf(" Warning: argumento invalido, fila == NULL em Fila.c fila_mostraFila()\n");
		return false;
	}
	else if(fila->inicio == NULL)
	{
		printf(" Warning: Fila vazia, inválida ou corrompida detectada em Fila.c fila_mostraFila()\n");
		return false;
	}
	else
	{
		elementoAux = fila->inicio;
		printf("*** INICIO DA FILA ***\n");
		while(elementoAux != NULL)
		{
			printf(" Elemento %d -> |%c|\n", contador, elementoAux->item);
			contador++;
			elementoAux = elementoAux->proximo;
		}
		printf(" Total de caracteres = %d\n", fila->quantidadeDeCaracteres);
		printf("***  FIM DA FILA ***\n");
		return true;
	}
	return false;
}

int fila_getQuantidadeDeCaracteres(struct Fila *fila)
{
	if(fila == NULL)
	{
		printf(" Warning: Ponteiro invalido em Fila.c fila_getQuantidadeDeCaracteres()\n");
		return false;
	}
	return fila->quantidadeDeCaracteres;
}
