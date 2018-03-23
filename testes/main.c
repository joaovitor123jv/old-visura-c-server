#include<stdio.h>
#include<stdlib.h>
#include "../AdaptadorDeString/AdaptadorDeString.h"
#include "../Comandos/Comandos.h"

#include "../Cliente/Cliente.h"


/** 
 * @brief  Obtem a página (html puro) solicitada
 * @note   cria uma conexão breve
 * @param  *pageURL: URL da página a ser obtido o html
 * @retval a página inteira nume string dinâmica
 */
// char *getHTTPPage(char *pageURL);

/** 
 * @brief  Função para transformar essa parte de código em um teste stand-alone
 * @note   teste
 * @param  argc: contador de argumentos
 * @param  *argv[]: argumentos em si, sendo argv[0] o nome do executavel
 * @retval porrrrra nenhuma
 */

int main(int argc, char *argv[])
{
	static const char *localizacao = "main.c main(int argc, char *argv[])";
	if (argc == 1)
	{
		geraLog(WARNING, "Esse programa não pode ser executado sem um parâmetro correspondente");
		return -1;
	}

	char *htmlDaPagina = getHTTPPage(argv[1]);

	printf(" HTML::::: \n\n\n\n\n→|%s|", htmlDaPagina);
	return 0;
}
