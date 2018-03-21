#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[])
{
	int retorno = system("ruby teste.rb um dois tres");
	printf(" retorno: %d\n", retorno);
	return 0;
}
