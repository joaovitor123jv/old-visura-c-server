#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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

#ifndef USUARIO_NIVEL_DE_PERMISSAO_ROOT 
#define USUARIO_NIVEL_DE_PERMISSAO_ROOT 40
#endif

#ifndef USUARIO_NIVEL_DE_PERMISSAO_ELEVADO
#define USUARIO_NIVEL_DE_PERMISSAO_ELEVADO 10
#endif

#ifndef USUARIO_NIVEL_DE_PERMISSAO_ANONIMO
#define USUARIO_NIVEL_DE_PERMISSAO_ANONIMO 5
#endif

#ifndef USUARIO_NIVEL_DE_PERMISSAO_NORMAL
#define USUARIO_NIVEL_DE_PERMISSAO_NORMAL 0
#endif

// Criar biblioteca de criação e manipulacao de usuarios

struct Usuario
{
	char *login;
	int tamanhoLogin;
	char *senha;
	int tamanhoSenha;
	int nivelDePermissao;
};

typedef struct Usuario Usuario;

bool usuarioPrivilegiado(char *email);

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

	if( strcmp(usuario->login, LOGIN_DE_CADASTRO) == 0 )
	{
		usuario->nivelDePermissao = USUARIO_NIVEL_DE_PERMISSAO_ELEVADO;
	}
	else if( strcmp(usuario->login, LOGIN_DO_SITE) == 0 )
	{
		usuario->nivelDePermissao = USUARIO_NIVEL_DE_PERMISSAO_ELEVADO;
	}
	else if( strcmp(usuario->login, LOGIN_USUARIO_ROOT) == 0 )
	{
		usuario->nivelDePermissao = USUARIO_NIVEL_DE_PERMISSAO_ROOT;
	}
	else
	{
		usuario->nivelDePermissao = USUARIO_NIVEL_DE_PERMISSAO_NORMAL;
	}
	return usuario;
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

bool usuario_anonimo(Usuario *usuario)
{
	return false;
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


