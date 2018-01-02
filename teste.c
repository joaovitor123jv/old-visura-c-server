#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#ifndef bool
#define bool char
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

/*
 *   @author João Vitor Antoniassi Segantin
 *     @github https://github.com/joaovitor123jv
 *     
 */

char *completaString1(char *string)
{
//	printf(" Endereço da string 1= %ld\n", &string);
//	string = "Teste";
	*string = strdup("Teste");

//	printf(" Endereço da string 2= %ld\n", &string);
	printf(" STRING RECEBIDA = %s\n", string);
	string[1] = 'j';
	printf(" STRING MODIFICADA = %s\n", string);
	return NULL;
}


int main(void)
{
	printf(" Iniciando programa\n");
//	char *string = NULL;
//	printf(" Endereço da string 1= %ld\n", &string);
//	char *tst;
//	tst = string;
//	*tst = "TESTE";
//	printf(" %s   %s\n", tst, string);

//	completaString1(string);
//	printf(" Endereço da string 2= %ld\n", &string);

//	if( string == NULL )
//	{
//		printf(" ASKJdHKJSAHDJKSALHDJKSALHDJKLASDHJK\n");
//	}

//	printf(" String = %s\n", string);
//	char *i = strdup("teste");
	char *i = NULL;
//	printf(" i = %s\n", i);
	char *j = NULL;
	j = i;

	completaString1(j);

	printf(" i = %s, j = %s\n", i, j);
}
