#ifndef __AdaptadorDeString__
#define __AdaptadorDeString__

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdarg.h>//Pra aceitar multiplos argumentos

#include "../Comandos/Comandos.h"

#ifndef _TESTAR_SEM_FILA_
#include "../Fila/Fila.h"
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

/** 
 * @brief  Transforma caracteres especiais, em expressões que signifiquem esses caracteres propriamente ditos
 * @note   útil para implementação de segurança
 * @param  *original: String original de entrada
 * @retval char * de string formatada
 */
char *padronizarString(char *original);

#endif

/** 
 * @brief  Para uso interno ao Adaptador de string
 * @note   utilizado em funções de conversão, inverte a string
 * @param  s[]: string que será invertida
 * @retval None
 */
void adaptadorDeString_REVERSE(char s[]);


/** 
 * @brief  Converte inteiro para String
 * @note   não retorna string estática, logo, deve ser liberada
 * @param  n: numero que será convertido em string
 * @retval string convertida
 */
char *intToString(int n);


/** 
 * @brief  Retorna true caso a string informada seja menor que o tamanho informado
 * @note   equivalente a if(strlen(string) < tamanho), otimizado
 * @param  *string: string para comparação, não altera
 * @param  tamanho: tamanho para comparação
 * @retval true, false
 */
bool stringMenor(char *string, int tamanho);


/** 
 * @brief  Retorna true caso a string informada seja menor ou igual que o tamanho informado
 * @note   equivalente a if(strlen(string) <= tamanho), otimizado
 * @param  *string: string para comparação, não altera
 * @param  tamanho: tamanho para comparação
 * @retval true, false
 */
bool stringMenorOuIgual(char *string, int tamanho);

/** 
 * @brief  Retorna true caso a string informada seja maior que o tamanho informado
 * @note   equivalente a if(strlen(string) > tamanho), otimizado
 * @param  *string: string para comparação, não altera
 * @param  tamanho: tamanho para comparação
 * @retval true, false
 */
bool stringMaior(char *string, int tamanho);

/** 
 * @brief  Retorna true caso a string informada contenha o tamanho informado
 * @note   equivalente a if(strlen(string) == tamanho), otimizado
 * @param  *string: string para comparação, não altera
 * @param  tamanho: tamanho para comparação
 * @retval true, false
 */
bool stringTamanhoIgual(char *string, int tamanho);

/** 
 * @brief  retorna true, caso tenha identificado mensagem inválida para a aplicação no formato "APP comando"
 * @note   usado para otimização e segurança
 * @param  *mensagem: mensagem que será verificada em busca de falhas
 * @retval true, false
 */
bool mensagemDeEscapeDetectada(const char *mensagem);// Verifica se a mensagem é "APP sair", de forma otimizada


/** 
 * @brief  Função padronizadora para geração de logs
 * @note   Mantendo os logs centralizados, é mais fácil sumir com eles depois
 * @param  tipoLog: WARNING, ERRO, LOG, DEBUG, EXTRA
 * @param  *mensagem: mensagem que deve ser mostrada como log
 * @param  *localizacao: onde a mensagem foi gerada (em que parte do código)
 * @retval None
 */
void geraLog(unsigned int tipoLog, const char *mensagem, const char *localizacao);



// *******************    TOKENIZER   ***********************

/** 
 * @brief  Estrutura para tokenização de strings
 * @note   definição no arquivo .c
 * @retval None
 */
typedef struct Tokenizer Tokenizer;

/** 
 * @brief  Inicia a struct Tokenizer, caso seja alocada estaticamente
 * @note   *tokenizer deve ser liberado depois (tokenizer_destroy()). (inútil por enquanto)
 * @param  *tokenizer: estrutura já inicalizada
 * @param  *comando: comando que será transformado em tokens
 * @param  *delimitador: delimitador da separação dos tokens
 * @retval None
 */
void init_Tokenizer(Tokenizer *tokenizer, const char *comando, const char delimitador);


/** 
 * @brief  Gera um novo Tokenizer, retorna NULL caso falhe
 * @note   tokenizer deve ser liberado depois (delete_Tokenizer(Tokenizer *tokenizer))
 * @param  *comando: Comando que será transformado em tokens
 * @param  delimitador: delimitador da separacao dos tokens
 * @retval NULL, Tokenizer*
 */
Tokenizer *new_Tokenizer(const char *comando, const char delimitador);

/** 
 * @brief  Obtém o pŕoximo token da "lista"
 * @note   não é necessário liberar o retorno dessa função, somente o próprio tokenizer
 * @param  *tokenizer: estrutura gerada por new_Tokenizer ou init_Tokenizer
 * @retval NULL caso não houver novo token ou der erro interno, ponteiro para proximo token caso sucesso
 */
char *tokenizer_getNext(Tokenizer *tokenizer);

/** 
 * @brief  Libera variáveis utilizadas pelo tokenizer e ele mesmo
 * @note   deve ser chamado após a declaracao de new_Tokenizer()
 * @param  *tokenizer: tokenizer criado por new_Tokenizer()
 * @retval None
 */
void delete_Tokenizer(Tokenizer *tokenizer);


// ******************* FIM TOKENIZER  ***********************



#endif //__AdaptadorDeString__
