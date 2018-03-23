#include "AdaptadorDeString.h"
#include "../Comandos/Comandos.h"


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

//#define free(p) { free(p); p=NULL; }
/* 
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
		} 
	} 
	va_end(variaveis); 
} */

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



void geraLog_internal(unsigned int tipoLog, const char *mensagem, const char *file, const char *function, const int linha)
{
	#ifndef DISABLE_LOGS
		if (mensagem == NULL)
		{
			return;
		}
		
		switch(tipoLog)
		{
			case WARNING:
				fprintf(stdout, " WARNING: %s em %s na função %s: linha: %d\n", mensagem, file, function, linha);
				break;
			case ERRO:
				fprintf(stdout, " ERRO: %s em %s na função %s: linha: %d\n", mensagem, file, function, linha);
				break;
			case LOG:
				fprintf(stdout, " LOG: %s em %s na função %s: linha: %d\n", mensagem, file, function, linha);
				break;
			case DEBUG:
				fprintf(stdout, " DEBUG: %s em %s na função %s: linha: %d\n", mensagem, file, function, linha);
				break;
			case EXTRA:
				fprintf(stdout, " EXTRA: %s em %s na função %s: linha: %d\n", mensagem, file, function, linha);
				break;
			default:
				fprintf(stdout, " ERRO: INDEFINICAO DE LOG EM AdaptadorDeString.h geraLog(unsigned int tipoLog, const char *mensagem, const char * localizacao);\n");
				break;
		}
	#else
		return;
	#endif // DISABLE_LOGS

}



void tokenizer_private_reverteAlteracoesPorDelimitador(Tokenizer *tokenizer);

/** 
 * @brief  Retorna o endereco de uma página (sem as delimitações de servidor)
 * @note   A URL Deve estar no formato    algumsite.com
 * 			Ou seja, não deve ter o "http://" antes de "algumsite.com"
 * @param  *pageURL: 
 * @retval 
 */
char *obterEnderecoDadoURL(char *pageURL)
{
	static const char *localizacao = "AdaptadorDeString.h obterEnderecoDadoURL(char *pageURL)";
	if (pageURL == NULL)
	{
		geraLog(WARNING, "pageURL nulo");
		return NULL;
	}
	char *endereco = NULL;

	Tokenizer *tokenizer = new_Tokenizer(pageURL, '/');
	endereco = strdup(tokenizer_getNext(tokenizer));
	delete_Tokenizer(tokenizer);
	
	return endereco;
}

/** 
 * @brief  Retorna o endereco de uma página (sem as delimitações de servidor)
 * @note   A URL Deve estar no formato    algumsite.com
 * 			Ou seja, não deve ter o "http://" antes de "algumsite.com"
 * @param  *pageURL: 
 * @retval 
 */
char *obterPaginaDadoURL(char *pageURL)
{
	if (pageURL == NULL)
	{
		geraLog(WARNING, "pageURL nulo");
		return NULL;
	}
	int i;

	for(i = 0;i < strlen(pageURL);i++)
	{
		if(pageURL[i] == '/')
		{
			break;
		}
	}

	if (i >= strlen(pageURL))
	{
		return "/";
	}
	else
	{
		return &pageURL[i];
	}
	
}



// ***************************************TOKENIZER  - BEGIN****************************************************

struct Tokenizer
{
	char *comando;
	int inicio;
	int fim;
	int atual;
	char delimitador;
};



void tokenizer_private_substituiDelimitadorPorFimDeString(Tokenizer *tokenizer)
{
	int i = 0;
	//for(i = 0; i<tokenizer->fim; tokenizer++) Lembrete de macacagem
	for(i = 0; i<tokenizer->fim; i++)
	{
		if (tokenizer->comando[i] == tokenizer->delimitador)
		{
			tokenizer->comando[i] = '\0';
		}
	}
	return;
}

void tokenizer_private_reverteAlteracoesPorDelimitador(Tokenizer *tokenizer)
{
	int i = 0;
	for(i = 0; i<tokenizer->fim; tokenizer++)
	{
		if (tokenizer->comando[i] == '\0')
		{
			tokenizer->comando[i] = tokenizer->delimitador;
		}
	}
	return;
}


void init_Tokenizer(Tokenizer *tokenizer, const char *comando, const char delimitador)
{
	if (tokenizer == NULL)
	{
		geraLog(WARNING, "nenhum tokenizer foi passado");
		return;
	}
	if (comando == NULL)
	{
		geraLog(WARNING, "Comando não pode ser NULL");
		return;
	}

	if (stringMaior((char *)comando, BUFFER_CLIENTE))
	{
		geraLog(ERRO, "Comando exageradamente grande detectado");
		return;
	}

	tokenizer->comando = strdup(comando);
	if (tokenizer->comando == NULL)
	{
		geraLog(ERRO, "Falha ao duplicar comando");
		return;
	}

	tokenizer->inicio = 0;
	tokenizer->fim = strlen(comando);
	
	tokenizer->atual = 0;
	tokenizer->delimitador = delimitador;

	tokenizer_private_substituiDelimitadorPorFimDeString(tokenizer);
}


Tokenizer *new_Tokenizer(const char *comando, const char delimitador)
{
	if (comando == NULL)
	{
		geraLog(WARNING, "Comando não pode ser NULL");
		return NULL;
	}

	if (stringMaior((char *)comando, BUFFER_CLIENTE))
	{
		geraLog(ERRO, "Comando exageradamente grande detectado");
		return NULL;
	}

	Tokenizer *tokenizer = (Tokenizer *)malloc(sizeof(Tokenizer));

	tokenizer->comando = strdup(comando);
	if (tokenizer->comando == NULL)
	{
		geraLog(ERRO, "Falha ao duplicar comando");
		return NULL;
	}

	tokenizer->inicio = 0;
	tokenizer->fim = strlen(comando);
	
	tokenizer->atual = 0;
	tokenizer->delimitador = delimitador;
	tokenizer_private_substituiDelimitadorPorFimDeString(tokenizer);
	return tokenizer;
}

char *tokenizer_getNext(Tokenizer *tokenizer)
{
	char *localizacao = "AdaptadorDeString.h tokenizer_getNext()";
	if (tokenizer == NULL)
	{
		geraLog(WARNING, "Tokenizer nulo passado como parâmetro");
	}
	if (tokenizer->comando == NULL)
	{
		geraLog(WARNING, "Tokenizer com comando nulo identificado");		
	}
	
	// for(i = tokenizer->atual ; i < tokenizer->fim && pare ; i++)
	// {
		
	// }
	char *retorno = &tokenizer->comando[tokenizer->atual];
	int valor = strlen(&tokenizer->comando[tokenizer->atual]) + tokenizer->atual + 1;//+1 pra pular o \0
	if( valor > tokenizer->fim+1)
	{
		geraLog(LOG, "Tokenizer alcançou seu limite...");
		return NULL;
	}
	else
	{
		tokenizer->atual = valor;
		return retorno;
	}
}

void delete_Tokenizer(Tokenizer *tokenizer)
{
	char *localizacao = "AdaptadorDeString.h delete_Tokenizer(Tokenizer *tokenizer)";
	if (tokenizer == NULL)
	{
		geraLog(WARNING, "Tokenizer nulo detectado");
	}
	if (tokenizer->comando != NULL)
	{
		free(tokenizer->comando);
		tokenizer->comando = NULL;
	}
	free(tokenizer);
	tokenizer = NULL;
}


// ***************************************TOKENIZER  - END****************************************************