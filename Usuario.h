#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Comando-Login.h"

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef bool
#define bool char
#endif

#ifndef _USUARIO_NIVEL_DE_PERMISSAO_NAO_OBTIDO_
#define _USUARIO_NIVEL_DE_PERMISSAO_NAO_OBTIDO_ -1
#endif


// Criar biblioteca de criação e manipulacao de usuarios

typedef struct Usuario
{
	char *login;
	int tamanhoLogin;
	char *senha;
	int tamanhoSenha;
	int nivelDePermissao;
};

Usuario *new_Usuario(const char *login, const char *senha)
{
	if( login == NULL )
	{
		printf(" Warning: Login nulo identificado em Usuario.h new_Usuario()\n");
		return NULL;
	}
	if( senha == NULL )
	{
		printf(" Warning: Senha nula identificada em Usuario.h new_Usuario()\n");
		return NULL;
	}

	Usuario *usuario;
	printf(" LOG: Iniciando criacao de usuario em Usuario.h new_Usuario()\n");
	usuario = malloc(sizeof(Usuario));

	usuario->tamanhoSenha = strlen(senha);
	usuario->tamanhoLogin = strlen(login);


	usuario->login = strdup(login);
	if(usuario->login == NULL)
	{
		free(usuario);
		usuario = NULL;
		return NULL;
	}
	usuario->senha = strdup(senha);
	if(usuario->senha == NULL)
	{
		printf(" Warning: Falha ao duplicar senha em Usuario.h new_Usuario()\n");
		free(usuario->login);
		usuario->login = NULL;
		free(usuario);
		usuario = NULL;
		return NULL;
	}
	usuario->nivelDePermissao = _USUARIO_NIVEL_DE_PERMISSAO_NAO_OBTIDO_;
	return usuario;
}

char *usuario_obterLogin(Usuario *usuario)
{
	if( usuario == NULL )
	{
		printf(" Warning: Usuario nula detectado em Usuario.h usuario_obterLogin()\n");
		return NULL;
	}
	return usuario->login;
}

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
		return NULL;
	}
	return usuario->tamanhoSenha;
}

int usuario_obterTamanhoLogin(Usuario *usuario)
{
	if( usuario == NULL )
	{
		printf(" Warning: Usuario nula detectado em Usuario.h usuario_obterTamanhoLogin()\n");
		return NULL;
	}
	return usuario->tamanhoLogin;
}

void usuario_mostrarDados(Usuario *usuario)
{
	if(usuario == NULL)
	{
		printf(" Warning: Usuario nulo detectado em Usuario.h usuario_mostrarDados() sakdhsa\n");
		return;
	}
	printf(" LOG: *************************DADOS DE USUARIO********************* Usuario.h usuario_mostrarDados()\n");
	printf(" LOG:              Email: |%s|\n", usuario->login);
	printf(" LOG:              Senha: |%s|\n", usuario->senha);
	printf(" LOG: Nível de permissão: |%d|\n", usuario->nivelDePermissao);
}


bool delete_Usuario(Usuario *usuario)
{
	if( usuario == NULL )
	{
		printf(" Warning: Usuario == nulo detectado em Usuario.h delete_Usuario()\n");
		return false;
	}
	free(usuario->login);
	free(usuario->senha);
	usuario->login = NULL;
	usuario->senha = NULL;
	free(usuario);
	usuario = NULL;
	return true;
}


