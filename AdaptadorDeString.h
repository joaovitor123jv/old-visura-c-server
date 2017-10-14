#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "Fila/Fila.h"


#ifndef bool
#define bool char
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

char *padronizarString(char *original)
{
	char *resposta = NULL;
	if(original == NULL)
	{
		return NULL;
	}
	else
	{
		Fila *fila = new_Fila();
		if(fila == NULL)
		{
			printf(" Warning: Falha ao padronizar string em AdaptadorDeString.h padronizarString(char *resposta char *original)\n");
			return NULL;
		}
		else
		{
			int i = 0;
			while(original[i] != '\0')
			{
				if(original[i] == '\'')/* Aspas simples */
				{
					fila_inserir(fila, '\\');
					fila_inserir(fila, '\'');
				}
				else if(original[i] == '\"')/* Aspas duplas */
				{
					fila_inserir(fila, '\\');
					fila_inserir(fila, '\"');
				}
				else if(original[i] == '\\')/* Barra invertida */
				{
					fila_inserir(fila, '\\');
					fila_inserir(fila, '\\');
				}
				else if(original[i] == '\t')/* HARD Tab */
				{
					fila_inserir(fila, '\\');
					fila_inserir(fila, 't');
				}
				else if(original[i] == '\r')/* Retorno de carro */
				{
					fila_inserir(fila, '\\');
					fila_inserir(fila, 'r');
				}
				else if(original[i] == '\n')/* Quebra de linha */
				{
					fila_inserir(fila, '\\');
					fila_inserir(fila, 'n');
				}
				else if(original[i] == '\a')/* BEEP */
				{
					fila_inserir(fila, '\\');
					fila_inserir(fila, 'a');
				}
				else
				{
					fila_inserir(fila, original[i]);
				}
				i = i + 1;
			}
//			fila_mostraFila(fila);
			int tamanho = fila_getQuantidadeDeCaracteres(fila);
			resposta = (char *)malloc(sizeof(char) * (tamanho + 1));/* +1 por causa d \0 */
			for(i = 0; i < tamanho; i++)
			{
				fila_remover(fila, &resposta[i]);
//				fila_mostraFila(fila);
			}
//			i = i + 1;
//			resposta[i] = '\0';
			free_Fila(fila);
/*			printf(" resposta = |%s| \n", resposta);
			printf(" Endereco de resposta = %ld\n", &resposta);
			printf(" Endereco de valor de resposta = %ld\n", &(*resposta));*/
			return resposta;
		}
	}
}

void adaptadorDeString_REVERSE(char s[])
{
    int i, j;
    char c;

	for (i = 0, j = strlen(s)-1; i<j; i++, j--) 
	{
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

char *intToString(int n)
{
	int i=0, sign;
	char *s = (char *)malloc(sizeof(char) * 12);
    if ((sign = n) < 0)  /* record sign */
    {
        n = -n;          /* make n positive */
    }
    do 
    {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
    {
        s[i++] = '-';
    }
    s[i] = '\0';
	adaptadorDeString_REVERSE(s);
	return s;
}