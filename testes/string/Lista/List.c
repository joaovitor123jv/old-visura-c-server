#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"List.h"

List* newList()
{
	List *lista = malloc(sizeof(List));
	ListElement->start = NULL;
	ListElement->end = NULL;
}

bool freeList(List *lista)
{
	while(lista->start != NULL)
	{
		removeEnd(List *lista);
	}
	free(lista);
	lista = NULL;
}

bool insertCharOnEnd(List *lista, char elementoAInserir)
{
	if(lista == NULL)
	{
		return false;
	}

	if(lista->start == NULL && lista->end == NULL)
	{
		lista->start = malloc(sizeof(ListElement));
		if(lista->start == NULL)
		{
			return false;
		}
		lista->start->element = elementoAInserir;
		lista->start->before = NULL;
		lista->end = malloc(sizeof(ListElement));
		if(lista->end == NULL)
		{
			free(lista->start);
			lista->start = NULL;
			return false;
		}
		lista->end->element = elementoAInserir;
		lista->end->after = NULL;
		lista->end->before = lista->start;
		lista->start->after = lista->end;
		return true;
	}
	else
	{
		ListElement *elemento;
		elemento = lista->end;
		lista->end = malloc(sizeof(ListElement));
		if(lista->end == NULL)
		{
			lista->end = elemento;
			return false;
		}
		elemento->next = lista->end;
		lista->end->element = elementoAInserir;
		lista->end->next = NULL;
		lista->end->before = elemento;
		return true;
	}
}

bool insertCharOnStart(List *lista, char elementoAInserir)
{
	if(lista == NULL)
	{
		return false;
	}

	if(lista->start == NULL && lista->end == NULL)
	{
		lista->start = malloc(sizeof(ListElement));
		if(lista->start == NULL)
		{
			return false;
		}
		lista->start->element = elementoAInserir;
		lista->start->before = NULL;
		lista->end = malloc(sizeof(ListElement));
		if(lista->end == NULL)
		{
			free(lista->start);
			lista->start = NULL;
			return false;
		}
		lista->end->element = elementoAInserir;
		lista->end->after = NULL;
		lista->end->before = lista->start;
		lista->start->after = lista->end;
		return true;
	}
	else
	{
		ListElement *elemento = NULL;
		elemento = lista->start;
		lista->start = malloc(sizeof(ListElement));
		if(lista->start == NULL)
		{
			lista->start = elemento;
			return false;
		}
		else
		{
			lista->start->element = elementoAInserir;
			lista->start->next = elemento;
			lista->start->before = NULL;
			elemento->before = lista->start;
			return true;
		}
	}
}

bool insertCharOnPosition(List *lista, int posicao, char elementoAInserir);//TODO

char getEnd(List *lista, bool *controlVariable)//controlVariable can be NULL
{
	return lista->end->element;
}

char getStart(List *lista, bool *controlVariable)//controlVariable can be NULL
{
	if(lista == NULL)
	{
		*controlVariable = false;
		return false;
	}
	if(lista->end != NULL)
	{
		*controlVariable = true;
		return lista->start->element;
	}
	else
	{
		*controlVariable = false;
	}
	return false;
}

char getCharOnPosition(List *lista, int posicao)
{
//TODO
return false;
}

bool removeEnd(List *lista);
bool removeStart(List *lista);


