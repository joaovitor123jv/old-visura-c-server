#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#ifndef bool
#define bool char
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

typedef struct
{
	ListElement *start;
	ListElement *end;
	int sizeOfList;
}List;

typedef struct
{
	char element;
	ListElement *next;
	ListElement *before;
}ListElement;


List *newList();

bool freeList(List *lista);

bool insertCharOnEnd(List *lista, char elementoAInserir);
bool insertCharOnStart(List *lista, char elementoAInserir);

bool insertCharOnPosition(List *lista, int posicao, char elementoAInserir);

char getEnd(List *lista, bool *controlVariable);//controlVariable can be NULL
char getStart(List *lista, bool *controlVariable);//controlVariable can be NULL

bool removeEnd(List *lista);
bool removeStart(List *lista);


