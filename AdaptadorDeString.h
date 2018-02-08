#ifndef __AdaptadorDeString__
#define __AdaptadorDeString__

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdarg.h>//Pra aceitar multiplos argumentos

#ifndef _TESTAR_SEM_FILA_
#include "Fila/Fila.h"
#endif

#ifdef __COM_CRIPTOGRAFIA__
	#include "Criptografia/Criptografia.h"
#endif

#ifndef bool
#define bool char
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef _TESTAR_SEM_FILA_
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
			int tamanho = fila_getQuantidadeDeCaracteres(fila);
			resposta = (char *)malloc(sizeof(char) * (tamanho + 1));/* +1 por causa d \0 */
			for(i = 0; i < tamanho; i++)
			{
				fila_remover(fila, &resposta[i]);
			}
			free_Fila(fila);
			return resposta;
		}
	}
}
#endif

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
        s[i++] = n % 10 + '0';   
    } 
	while ((n /= 10) > 0);     
    if (sign < 0)
    {
        s[i++] = '-';
    }
    s[i] = '\0';
	adaptadorDeString_REVERSE(s);
	return s;
}

bool stringMenor(char *string, int tamanho)
{
	if( string == NULL )
	{
		return false;
	}
	if( tamanho <= 0 )
	{
		return false;
	}
	int i;
	for( i = 0; i<tamanho; i++ )
	{
		if( string[i] == '\0' )
		{
			return true;
		}
	}
	return false;
}

bool stringMenorOuIgual(char *string, int tamanho)
{
	if( string == NULL )
	{
		return false;
	}
	if( tamanho <= 0 )
	{
		return false;
	}
	int i;
	for( i = 0; i<=tamanho; i++ )
	{
		if( string[i] == '\0' )
		{
			return true;
		}
	}
	return false;
}

bool stringMaior(char *string, int tamanho)
{
	if( string == NULL )
	{
		return false;
	}
	if( tamanho <= 0 )
	{
		return false;
	}
	int i;
	for( i = 0; i<=tamanho; i++ )
	{
		if( string[i] == '\0' )
		{
			return false;
		}
	}
	return true;
}

bool stringTamanhoIgual(char *string, int tamanho)
{
	if( string == NULL )
	{
		return false;
	}
	if( tamanho <= 0 )
	{
		return false;
	}
	int i;
	for( i = 0; i<=tamanho; i++ )
	{
		if( string[i] == '\0' )
		{
			if( i == tamanho )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}



// Depois que isso estiver pronto, padronizar pra não usar aqui, colocar em códigos separados
void liberar(void *var1, ...)
{
	if (var1 == NULL)
	{
		return;
	}

	va_list variaveis;
	char *temp;
	va_start(variaveis, var1);

	while ((temp = va_arg(variaveis, char *)))  
	{
		if (temp != NULL)
		{
			free(temp);
			temp = NULL;	
		}
	}

  va_end(variaveis);
  return;
}

bool mensagemDeEscapeDetectada(const char *mensagem)// Verifica se a mensagem é "APP sair", de forma otimizada
{
	//	strcmp(mensagem, "APP sair");
	if(mensagem == NULL)
	{
		return true;
	}
	else if(mensagem[0] != 'A')
	{
		return true;
	}
	else if( mensagem[1] != 'P' )
	{
		return true;
	}
	else if( mensagem[2] != 'P' )
	{
		return true;
	}
	else if(mensagem[3] != ' ')
	{
		return true;
	}
	else if(mensagem[4] == 's')
	{
		if( mensagem[5] == 'a' )
		{
			if( mensagem[6] == 'i' )
			{
				if( mensagem[7] == 'r' )
				{
					return true;
				}
			}
		}
	}
	return false;
}


#endif //__AdaptadorDeString__