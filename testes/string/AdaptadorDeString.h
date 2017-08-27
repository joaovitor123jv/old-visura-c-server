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

bool padronizarString(char *resposta, char *original)
{
	if(original == NULL)
	{
		resposta = NULL;
		return false;
	}
	else
	{
		Fila *fila = new_Fila();
		if(fila == NULL)
		{
			printf(" Warning: Falha ao padronizar string em AdaptadorDeString.h padronizarString(char *resposta char *original)\n");
			resposta = NULL;
			return false;
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
			fila_mostraFila(fila);
			int tamanho = fila_getQuantidadeDeCaracteres(fila);
			resposta = malloc(sizeof(char) * (tamanho + 1));/* +1 por causa d \0 */
			for(i = 0; i < tamanho-1; i++)
			{
				printf(" \t\tTentando remover\n");
				fila_remover(fila, &resposta[i]);
				fila_mostraFila(fila);
			}
			free_Fila(fila);
			printf(" resposta = |%s| \n", resposta);
			return true;
		}
	}
}
