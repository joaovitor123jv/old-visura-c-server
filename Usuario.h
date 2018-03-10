#ifndef __USUARIO_VISURA__
	#define __USUARIO_VISURA__


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Comandos/Comandos.h"
#include "AdaptadorDeString/AdaptadorDeString.h"
//#include "OperacoesBanco/OperacoesBanco.h"
#include <mysql/mysql.h>


#ifndef CONEXAO_COM_BANCO_DE_DADOS_DEFINIDA
	#define CONEXAO_COM_BANCO_DE_DADOS_DEFINIDA
	MYSQL *conexao;
#endif

char *obterRetornoUnicoDaQuery(char *query);

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef bool
#define bool char
#endif

#ifndef USUARIO_NIVEL_DE_PERMISSAO_NULL
#define USUARIO_NIVEL_DE_PERMISSAO_NULL -10
#endif

#ifndef _USUARIO_NIVEL_DE_PERMISSAO_NAO_OBTIDO_
#define _USUARIO_NIVEL_DE_PERMISSAO_NAO_OBTIDO_ -1
#endif

#ifndef USUARIO_NIVEL_DE_PERMISSAO_NORMAL
#define USUARIO_NIVEL_DE_PERMISSAO_NORMAL 0
#endif

#ifndef USUARIO_NIVEL_DE_PERMISSAO_ANONIMO
#define USUARIO_NIVEL_DE_PERMISSAO_ANONIMO 5
#endif

#ifndef USUARIO_NIVEL_DE_PERMISSAO_CONTRATANTE
#define USUARIO_NIVEL_DE_PERMISSAO_CONTRATANTE 9
#endif

#ifndef USUARIO_NIVEL_DE_PERMISSAO_ELEVADO
#define USUARIO_NIVEL_DE_PERMISSAO_ELEVADO 10
#endif

#ifndef USUARIO_NIVEL_DE_PERMISSAO_ROOT 
#define USUARIO_NIVEL_DE_PERMISSAO_ROOT 40
#endif

#ifndef USUARIO_NIVEL_DE_PERMISSAO_CADASTRO
#define USUARIO_NIVEL_DE_PERMISSAO_CADASTRO 77
#endif


bool queryRetornaConteudo(char *);
bool conectarBanco();

// Criar biblioteca de criação e manipulacao de usuarios

struct Usuario
{
	char *login;
	int tamanhoLogin;
	char *senha;
	int tamanhoSenha;
	int nivelDePermissao;
	char *id;
	Tokenizer *tokenizer;
};

typedef struct Usuario Usuario;

bool usuarioPrivilegiado(char *email);
int usuario_checarLogin(const char *email, const char *senha, Usuario *usuario);//Retorna o nível de permissao do usuario

void init_Usuario(Usuario *usuario)
{
	usuario->login = NULL;
	usuario->tamanhoLogin = 0;
	usuario->senha = NULL;
	usuario->tamanhoSenha = 0;
	usuario->id = NULL;
	usuario->nivelDePermissao = _USUARIO_NIVEL_DE_PERMISSAO_NAO_OBTIDO_;
	usuario->tokenizer = NULL;
}

bool new_Usuario(Usuario *usuario, const char *login, const char *senha)
{
	if (usuario == NULL)
	{
		printf(" Warning: Usuario precisa ser inicializado antes de chegar aqui, em Usuario.h new_Usuario()\n");
		return false;
	}
	if( login == NULL )
	{
		printf(" Warning: Login nulo identificado em Usuario.h new_Usuario()\n");
		return false;
	}
	if( senha == NULL )
	{
		printf(" Warning: Senha nula identificada em Usuario.h new_Usuario()\n");
		return false;
	}

	printf(" LOG: Iniciando criacao de usuario em Usuario.h new_Usuario()\n");

	int nivelDePermissao = usuario_checarLogin(login, senha, usuario);
	if (nivelDePermissao == _USUARIO_NIVEL_DE_PERMISSAO_NAO_OBTIDO_)
	{
		return false;
	}
	else if (nivelDePermissao == USUARIO_NIVEL_DE_PERMISSAO_NULL)
	{
		return false;
	}

	usuario->tamanhoSenha = strlen(senha);
	usuario->tamanhoLogin = strlen(login);


	usuario->login = strdup(login);
	if(usuario->login == NULL)
	{
		printf(" Warning: Falha ao duplicar login em Usuario.h new_Usuario()\n");
		return false;
	}
	usuario->senha = strdup(senha);
	if(usuario->senha == NULL)
	{
		printf(" Warning: Falha ao duplicar senha em Usuario.h new_Usuario()\n");
		free(usuario->login);
		usuario->login = NULL;
		return false;
	}

	if (nivelDePermissao == USUARIO_NIVEL_DE_PERMISSAO_NORMAL)
	{
		if( strcmp(usuario->login, LOGIN_USUARIO_ROOT) == 0 )
		{
			usuario->nivelDePermissao = USUARIO_NIVEL_DE_PERMISSAO_ROOT;
		}
		else if( strcmp(usuario->login, LOGIN_DO_SITE) == 0 )
		{
			usuario->nivelDePermissao = USUARIO_NIVEL_DE_PERMISSAO_ELEVADO;
		}
		else if( strcmp(usuario->login, LOGIN_DE_CADASTRO) == 0 )
		{
			usuario->nivelDePermissao = USUARIO_NIVEL_DE_PERMISSAO_CADASTRO;
		}
		else
		{
			int contador;

			for(contador  = 0; contador < usuario->tamanhoLogin; contador++)/* Checa se email é válido */
			{
				if(usuario->login[contador] == '@')
				{
					usuario->nivelDePermissao = USUARIO_NIVEL_DE_PERMISSAO_NORMAL;
					return true;
				}
			}

			usuario->nivelDePermissao = USUARIO_NIVEL_DE_PERMISSAO_ANONIMO;
		}
	}
	else 
	{
		usuario->nivelDePermissao = nivelDePermissao;
	}
	// usuario->tokenizer = NULL;

	return true;
}

bool usuarioValido(Usuario *usuario, const char *localizacao)
{
	if (usuario == NULL)
	{
		geraLog(WARNING, "Usuario nulo detectado", localizacao);
		return false;
	}
	if (usuario->login == NULL	)
	{
		geraLog(WARNING, "Usuario nao conectado detectado", localizacao);
		return false;
	}
	geraLog(LOG, "Usuario validado", localizacao);
	return true;
}

bool usuarioPrivilegiado(char *email)
{
	if(email == NULL)
	{
		return false;
	}
	if(strcmp(email, LOGIN_DE_CADASTRO) == 0)
	{
		return true;
	}
	if(strcmp(email, LOGIN_DO_SITE) == 0)
	{
		return true;
	}
	return false;
}

bool usuarioRoot(char *email)
{
	if( email == NULL )
	{
		return false;
	}
	else if( strcmp(email, LOGIN_USUARIO_ROOT) == 0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Retorna true, se usuario for usuario Normal.
bool usuario_PermissaoPrivilegiada(Usuario *usuario)
{
	if( usuario == NULL )
	{
		printf(" Warning: usuario == NULL detectado em Usuario.h usuario_PermissaoPrivilegiada()\n");
		return false;
	}
	if( usuario->nivelDePermissao == USUARIO_NIVEL_DE_PERMISSAO_NORMAL )
	{
		return false;
	}
	else
	{
		return true;
	}
}

//Retorna true, se usuario tiver permissão ROOT
bool usuario_PermissaoRoot(Usuario *usuario)
{
	if( usuario == NULL )
	{
		printf(" Warning: usuario == NULL em Usuario.h usuario_PermissaoRoot()\n");
		return false;
	}
	if( usuario->nivelDePermissao == USUARIO_NIVEL_DE_PERMISSAO_ROOT )
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Retorna true se usuario for Contratante
bool usuario_PermissaoContratante(Usuario *usuario)
{
	if (usuario == NULL)
	{
		return false;
	}
	else
	{
		if (usuario->nivelDePermissao == USUARIO_NIVEL_DE_PERMISSAO_CONTRATANTE)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

//Retorna TRUE, se usuário for Anônimo
bool usuario_PermissaoAnonimo(Usuario *usuario)
{
	if (usuario == NULL)
	{
		printf(" Warning: Usuario == NULL em Usuario.h usuario_PermissaoAnonimo()\n");
		return false;
	}
	else
	{
		if (usuario->nivelDePermissao == USUARIO_NIVEL_DE_PERMISSAO_ANONIMO)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

//Retorna TRUE, se o usuario tiver permissao de acesso normal ou anonimo
bool usuario_PermissaoCliente(Usuario *usuario)
{
	if (usuario == NULL)
	{
		return false;
	}
	if (usuario->nivelDePermissao == USUARIO_NIVEL_DE_PERMISSAO_NORMAL || usuario->nivelDePermissao == USUARIO_NIVEL_DE_PERMISSAO_ANONIMO)
	{
		return true;
	}
	return false;
}

//Retorna um char * contendo o e-mail (login) do usuario, ou NULL caso não tenha sido criado
char *usuario_obterLogin(Usuario *usuario)
{
	if( usuario == NULL )
	{
		printf(" Warning: Usuario nula detectado em Usuario.h usuario_obterLogin()\n");
		return NULL;
	}
	return usuario->login;
}


//retorna um char * contendo a senha do usuario, ou NULL caso não tenha sido criado.
char *usuario_obterSenha(Usuario *usuario)
{
	if( usuario == NULL )
	{
		printf(" Warning: Usuario nula detectado em Usuario.h usuario_obterSenha()\n");
		return NULL;
	}
	return usuario->senha;
}

int usuario_obterTamanhoSenha(Usuario *usuario)
{
	if( usuario == NULL )
	{
		printf(" Warning: Usuario nula detectado em Usuario.h usuario_obterTamanhoSenha()\n");
		return -1;
	}
	return usuario->tamanhoSenha;
}

int usuario_obterTamanhoLogin(Usuario *usuario)
{
	if( usuario == NULL )
	{
		printf(" Warning: Usuario nula detectado em Usuario.h usuario_obterTamanhoLogin()\n");
		return -1;
	}
	return usuario->tamanhoLogin;
}

char *usuario_obterId(Usuario *usuario)
{
	if (usuario == NULL)
	{
		return NULL;
	}
	return usuario->id;
}

Tokenizer *usuario_getTokenizer(Usuario *usuario)
{
	if (usuario == NULL)
	{
		geraLog(WARNING, "Usuario nulo detectado", "Usuario.h usuario_getTokenizer()");
		return NULL;
	}
	if (usuario->tokenizer == NULL)
	{
		geraLog(WARNING, "Usuario nao possui tokenizer", "Usuario.h usuario_getTokenizer()");
		return NULL;
	}
	return usuario->tokenizer;
}

char *usuario_getNextToken(Usuario *usuario)
{
	if (usuario == NULL)
	{
		geraLog(WARNING, "Usuario nulo detectado", "Usuario.h usuario_getNextToken()");
		return NULL;
	}
	if (usuario->tokenizer == NULL)
	{
		geraLog(WARNING, "Usuario nao possui tokenizer", "Usuario.h usuario_getNextToken()");
		return NULL;
	}
	return tokenizer_getNext(usuario->tokenizer);
}

void usuario_setTokenizer(Usuario *usuario, const char *comando)
{
	if (usuario == NULL)
	{
		geraLog(WARNING, "Usuario nulo detectado", "Usuario.h usuario_setTokenizer()");
		return;
	}
	else
	{
		if (usuario->tokenizer != NULL)
		{
			delete_Tokenizer(usuario->tokenizer);
			usuario->tokenizer = NULL;
			
		}
		usuario->tokenizer = new_Tokenizer(comando, ' ');
		if (usuario->tokenizer == NULL)
		{
			geraLog( ERRO, "Falha ao gerar tokenizer para usuario", "Usuario.h usuario_setTokenizer()");
			return;
		}
		else
		{
			geraLog( LOG, "usuario agora possui Tokenizer", "Usuario.h usuario_setTokenizer()");
			return;
		}
	}
}

void usuario_mostrarDados(Usuario *usuario)
{
	if(usuario == NULL)
	{
		printf(" Warning: Usuario nulo detectado em Usuario.h usuario_mostrarDados() sakdhsa\n");
		return;
	}
	printf(" LOG: *************************DADOS DE USUARIO********************* Usuario.h usuario_mostrarDados()\n");
	printf(" LOG:              Email: 	|%s|\n", usuario->login);
	printf(" LOG:              ID:		|%s|\n", usuario->id);
	printf(" LOG:              Senha:	|%s|\n", usuario->senha);
	printf(" LOG: Nível de permissão:	|%d|\n", usuario->nivelDePermissao);
}

bool usuario_atualizarLogin(Usuario *usuario, char *login)
{
	if (usuario == NULL)
	{
		printf(" Warning: Usuario nulo detectado em Usuario.h usuario_atualizarLogin()\n");
		return false;
	}
	if (usuario->login != NULL)
	{
		free(usuario->login);
		usuario->login = NULL;
	}
	usuario->login = strdup(login);
	if (usuario->login == NULL)
	{
		printf(" Warning: Falha ao atualizar login de usuario, string não duplicada em Usuario.h usuario_atualizarLogin()\n");
		return false;
	}
	usuario->tamanhoLogin = strlen(usuario->login);
	return true;
}


bool delete_Usuario(Usuario *usuario)
{
	if( usuario == NULL )
	{
		printf(" Warning: Usuario == nulo detectado em Usuario.h delete_Usuario()\n");
		return false;
	}
	if (usuario->login != NULL)
	{
		free(usuario->login);
		usuario->login = NULL;
	}
	if (usuario->senha != NULL)
	{
		free(usuario->senha);
		usuario->senha = NULL;
	}
	if (usuario->id != NULL)
	{
		free(usuario->id);
		usuario->id = NULL;
	}
	if (usuario->tokenizer != NULL)
	{
		delete_Tokenizer(usuario->tokenizer);
		usuario->tokenizer = NULL;
	}
	// free(usuario);
	// usuario = NULL;
	return true;
}


bool reset_Usuario(Usuario *usuario)
{
	if (usuario == NULL)
	{
		printf(" Warning: Usuario nulo detectado em Usuario.h reset_Usuario()\n");
		return false;
	}
	if (usuario->login != NULL)
	{
		free(usuario->login);
		usuario->login = NULL;
		usuario->tamanhoLogin = 0;
	}
	if (usuario->senha != NULL)
	{
		free(usuario->senha);
		usuario->senha = NULL;
		usuario->tamanhoSenha = 0;
	}
	usuario->nivelDePermissao = _USUARIO_NIVEL_DE_PERMISSAO_NAO_OBTIDO_;
	return true;
}

int usuario_checarLogin(const char *email, const char *senha, Usuario *usuario)// RETORNA Nível de permissão do usuario
{
	if(conexao == NULL)
	{
		printf("ERRO DE CONEXÃO (Usuario.h) (usuario_checarLogin)\n");
		printf(" LOG: Tentando reconexão com banco de dados \n");
		if(conectarBanco())
		{
			printf(" LOG: Reconectado com sucesso, continuando interpretação em Usuario.h usuario_checarLogin()\n");
		}
		else
		{
			printf(" Warning: Falha ao reconectar-se, encerrando interpretação em Usuario.h usuario_checarLogin()\n");
			return _USUARIO_NIVEL_DE_PERMISSAO_NAO_OBTIDO_;
		}
	}
	if (usuario->id != NULL)
	{
		free(usuario->id);
		usuario->id = NULL;
	}
	if(email == NULL)
	{
		printf(" ERRO: Não há login para checar (email == NULL) em Usuario.h usuario_checarLogin()\n");
		return _USUARIO_NIVEL_DE_PERMISSAO_NAO_OBTIDO_;
	}
	if(senha == NULL)
	{
		printf(" ERRO: Senha não foi informada em Usuario.h usuario_checarLogin() asbiv3888wq2\n");
		return _USUARIO_NIVEL_DE_PERMISSAO_NAO_OBTIDO_;
	}
	
	char *query = NULL;
	//size_t tamanho = strlen(email) + strlen(senha) + 66 + 1;
	size_t tamanho = strlen(email) + strlen(senha) + 67;
	
	
	query = (char *)malloc(sizeof(char) * tamanho);
	if(query == NULL)
	{
		printf(" ERRO: não foi possível alocar memória para a query em Usuario.h usuario_checarLogin()\n");
		return _USUARIO_NIVEL_DE_PERMISSAO_NAO_OBTIDO_;
	}
	
	snprintf(query, sizeof(char) * tamanho, "SELECT C.idcliente from cliente C WHERE C.email=\'%s\' AND C.senha=\'%s\';", email, senha);
	
	
	if(query == NULL)
	{
		printf(" ERRO: não foi possível alocar memória para a query (2) em Usuario.h usuario_checarLogin()\n");
		return _USUARIO_NIVEL_DE_PERMISSAO_NAO_OBTIDO_;
	}
	
	usuario->id = obterRetornoUnicoDaQuery(query);

	if(usuario->id != NULL)
	{
		printf(" LOG: Foi encontrado um ID para cliente que satisfaça as seguintes comparações: em Usuario.h usuario_checarLogin()\n");
		printf(" \t\tcliente.email = |%s|\n", (const char *)email);
		printf(" \t\tcliente.senha = |%s|\n", (const char *)senha);
		query = NULL;
		return USUARIO_NIVEL_DE_PERMISSAO_NORMAL;
	}
	else
	{
		//tamanho = 74 + strlen(email) + strlen(senha) + 1;
		usuario->id = NULL;
		tamanho = 75 + strlen(email) + strlen(senha);
		query = (char *)malloc(sizeof(char) * tamanho);
		if (query == NULL)
		{
			return _USUARIO_NIVEL_DE_PERMISSAO_NAO_OBTIDO_;
		}
		snprintf(query, tamanho, "SELECT C.idcontratante FROM contratante C WHERE C.email=\'%s\' AND C.senha=\'%s\';", email, senha);
		if (query == NULL)
		{
			printf(" ERRO: Falha ao checar login de contratante em Usuario.h usuario_checarLogin()\n");
			return _USUARIO_NIVEL_DE_PERMISSAO_NAO_OBTIDO_;
		}

		usuario->id = obterRetornoUnicoDaQuery(query);
		if (usuario->id != NULL)
		{
			printf(" LOG: Foi encontrado um ID para contratante que satisfaça as seguintes comparações: em Usuario.h usuario_checarLogin()\n");
			query = NULL;
			return USUARIO_NIVEL_DE_PERMISSAO_CONTRATANTE;
		}
		else
		{
			query = NULL;
			usuario->id = NULL;
			return USUARIO_NIVEL_DE_PERMISSAO_NULL;
		}
	}
}





#endif //__USUARIO_VISURA__
