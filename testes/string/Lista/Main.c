#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main()
{
	char *teste = malloc(sizeof(char) * (strlen("um teste meu aqui") + 1));
	strcpy(teste, "um teste aqui");
	printf(" teste = %s\n", teste);
	printf(" Tamanho = %d\n", );
	return 0;
}
