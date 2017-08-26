#pragma once //Pra compilar só uma vez
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

#ifndef __FILA__
#define __FILA__


typedef struct Fila Fila;

Fila *new_Fila();

bool fila_inserir(Fila *fila, char item);

bool fila_remover(Fila *fila, char *item);
int fila_getQuantidadeDeCaracteres(Fila *fila);

bool free_Fila(Fila *fila);

bool fila_mostraFila(Fila *fila);

#endif
