#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifndef ITEM_LISTA
#define ITEM_LISTA int
#endif

#ifndef bool
#define bool char
#endif

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif


typedef struct
{
	ITEM_LISTA item;
}Lista;
