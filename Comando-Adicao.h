#include <stdio.h>
#include <string.h>
#include "Comandos.h"

bool comandoAdicionar(char *email, bool usuarioAnonimo);//Retorna true se adicionado com sucesso
// bool addVisualizacao(char *comando);
bool addVisualizacao(char *email, bool usuarioAnonimo);//Retorna true se adicionado com sucesso
// bool addIndice(char *nomearquivo);

bool addUsuarioAnonimo();//Retorna true se adicionado com sucesso
bool addUsuario(char *emailAnterior);//Retorna true se adicionado com sucesso

bool addContratante();//Retorna true se adicionado com sucesso

bool addProduto();//Retorna true se adicionado com sucesso

bool addCidade();//Retorna true se adicionado com sucesso
bool addLocalizacao();//Retorna true se adicionado com sucesso

bool addInformacoesAUsuario(char *email);//Retorna true se adicionado com sucesso
bool addAvaliacaoAProduto(char *email);//Retorna TRUE se adicionado com sucesso


bool comandoAdicionar(char *email, bool usuarioAnonimo)/* APP 2 */
{
	if(email == NULL)
	{
		printf("Email == NULL (Comando-Adicao.h) comandoAdicionar()\n");
		return false;
	}
	char *token;
	token = strtok(NULL, " ");
	if(token == NULL)
	{
		printf(" ERRO: Comando incorreto (Comando-Adicao.h) comandoAdicionar()\n");
		return false;
	}

	printf(" LOG: TOKEN = |%s| em Comando-Adicao.h comandoAdicionar()\n", token);

	if(strcmp(token, TIPO_VISUALIZACAO) == 0)/* APP 2 2  (Solicitação de adicao de Visualizacao)*/
	{
		printf(" LOG: Solicitando adição de visualizacao de produto (Comando-Adicao.h) comandoAdicionar()\n");
		if(addVisualizacao(email, usuarioAnonimo))
		{
			printf(" LOG: Passou pelo OK Comando-Adicao.h comandoAdicionar()\n");
			return true;
		}
		else
		{
			printf(" LOG: Passou pelo ELSE Comando-Adicao.h comandoAdicionar()\n");
			return false;
		}
	}
	else if(strcmp(token, TIPO_AVALIACAO) == 0)/* APP 2 kW */
	{
		if(addAvaliacaoAProduto(email))
		{
			printf(" LOG: Avaliação cadastrada com sucesso em Comando-Adicao.h comandoAdicionar()\n");
			return true;
		}
		else
		{
			printf(" Warning: Falha ao adicionar avaliação ao produto requisitado em Comando-Adicao.h comandoAdicionar()\n");
			return false;
		}
	}
	else if(strcmp(token, TIPO_USUARIO) == 0)/* APP 2 1 Solicitação de criacao de usuario */
	{
		printf(" LOG: Solicitando adição de usuario ");
		token = strtok(NULL, " ");
		if(token == NULL)
		{
			printf(" Warning: Comando incorreto(2) Comando-Adicao.h comandoAdicionar()\n");
			return false;
		}
		if(strcmp(token, TIPO_USUARIO_ANONIMO) == 0)/* APP 2 1 2 Solicitação de criação de usuário anônimo */
		{
			printf("------------>anônimo Comando-Adicao.h comandoAdicionar()\n");
			token = strtok(NULL, " ");
			if(token == NULL)
			{
				printf(" Warning: Comando incorreto(3) Comando-Adicao.h comandoAdicionar()\n");
				return false;
			}
			if(strcmp(token, CHAVE_DE_SEGURANCA_PHP) == 0)/* APP 2 1 2 asdkjsahdjksahdjksadkj Solicitação de criação de usuário anônimo, informando chave */
			{
				printf(" LOG: Cliente PHP identificado Comando-Adicao.h comandoAdicionar()\n");

				if(addUsuarioAnonimo())
				{
					printf(" LOG: Cliente adicionado com sucesso Comando-Adicao.h comandoAdicionar()\n");
					return true;
				}
				else
				{
					printf(" Warning: Falha ao tentar adicionar usuario\n");
					return false;
				}
				
			}
			else
			{
				printf(" LOG: Não autorizado Comando-Adicao comandoAdicionar()\n");
				return false;
			}
		}
		else if( (strcmp(token, TIPO_USUARIO) == 0) && usuarioAnonimo)/* APP 2 1 1 Solicitação de criação de usuário não-anônimo */
		{
			printf("------------>regular Comando-Adicao.h comandoAdicionar()\n");
			// token = strtok(NULL, " ");
			// if(token == NULL)
			// {
			// 	printf(" Warning: Comando incorreto(4) Comando-Adicao.h comandoAdicionar()\n");
			// 	return false;
			// }

			// Colocar aqui mais tarde suporte a autenticação (TALVEZ)
			if(addUsuario(email))
			{
				printf(" LOG: Adicao executada com sucesso em Comando-Adicao.h comandoAdicionar()\n");
				return true;
			}
			else
			{
				printf(" Warning: Ocorreram erros e o usuario não pôde ser adicionado em Comando-Adicao.h comandoAdicionar()\n");
				return false;
			}
		}
		else
		{
			printf(" LOG: Usuario não é anonimo ou operação desconhecida em Comando-Adicao.h comandoAdicionar()\n");
			return false;
		}
	}
	else if(strcmp(token, TIPO_INFORMACAO) == 0)/* APP 2 & */
	{
		printf(" LOG: Cliente requisita adicao de informação a usuario em Comando-Adicao.h comandoAdicionar()\n");
		if(addInformacoesAUsuario(email))
		{	
			printf(" LOG: Informações adicionadas com sucesso ao banco em Comand-Adicao.h comandoAdicionar()\n");
			return true;
		}
		else
		{
			printf(" Warning: Falha ao adicionar informações a usuario em Comando-Adicao.h comandoAdicionar()\n");
			return false;
		}
	}
	else if(strcmp(token, TIPO_PRODUTO) == 0)/* APP 2 +    -> Solicita criação de produto na base de dados */
	{
		printf(" LOG: Solicitando a adição de Produto em Comando-Adicao.h comandoAdicionar() qqpjah1\n");
		if(addProduto())
		{
			printf(" LOG: Produto adicionado com sucesso em comandoAdicionar() Comando-Adicao.h\n");
			return true;
		}
		else
		{
			printf(" Warning: Não foi possivel adicionar produto ao banco de dados em comandoAdicionar() Comando-Adicao.h\n");
			return false;
		}
	}
	else if(strcmp(token, TIPO_CIDADE) == 0)/* APP 2 { */
	{
		if(addCidade())
		{
			printf(" LOG: Cidade adicionada com sucesso (Comando-Adicao.h) comandoAdicionar()\n");
			return true;
		}
		else
		{
			printf(" Warning: Falha ao adicionar cidade (Comando-Adicao.h) comandoAdicionar()\n");
			return false;
		}
	}
	else if(strcmp(token, TIPO_LOCALIZACAO) == 0)/* APP 2 l0 */
	{
		/* Cliente solicitando adicao de localizacao */
		if(addLocalizacao())
		{
			printf(" LOG: Localizacao adicionada com sucesso Comando-Adicao.h comandoAdicionar()!\n");
			return true;
		}
		else
		{
			printf(" Warning: Não foi possivel adicionar localizacao em Comando-Adicao.h comandoAdicionar()\n");
			return false;
		}
		printf(" Warning: Exceção não manipulável Comando-Adicao.h comandoAdicionar() (TIPO_LOCALIZACAO)\n");
		return false;
	}
	else if(strcmp(token, TIPO_CONTRATANTE) == 0)//APP 2 $C 
	{
		printf(" LOG: Solicitando adição de contratante em Comando-Adicao.h comandoAdicionar()\n");
		if(addContratante())
		{
			printf(" LOG: Contratante adicionada com sucesso em Comando-Adicao.h comandoAdicionar()\n");
			return true;
		}
		else
		{
			printf(" Warning: Não foi possível adicionar contratante em Comando-Adicao.h comandoAdicionar()\n");
			return false;
		}
		printf(" ERRO: Exceção não manipulada em Comando-Adicao.h comandoAdicionar() QNR\n");
		return false;
	}
	else
	{
		printf(" ERRO: TIPO DE ADIÇAO DESCONHECIDA (Comando-Adicao.h) comandoAdicionar()\n");
		return false;
	}
	printf(" ERRO: Algum erro desconhecido ocorreu em Comando-Adicao.h comandoAdicionar()\n");
	return false;
}

bool addUsuarioAnonimo()// APP 2 1 2 asdkhasdjkas
{
	printf("\n*********************addUsuarioAnonimo()********************\n");
	char *token = strtok(NULL, " ");
	if(token == NULL)// APP 2 1 2 sakjdhsa 7
	{
		printf(" Warning: Comando \"Insuficiente\" Comando-Adicao.h addUsuarioAnonimo()\n");
		return false;
	}
	if(strcmp(token, TIPO_LOGIN) == 0)// APP 2 1 2 sakdhasjkdn 7
	{
		printf(" LOG: Cliente informando login Comando-Adicao.h addUsuarioAnonimo()\n");
		token = strtok(NULL, " ");//APP 2 1 2 askjdhhh1127868c 7 logindousuario
		if(token == NULL)// APP 2 1 2 askdjsakd 7 logindousuario
		{
			printf(" Warning: Comando insuficiente Comando-Adicao.h addUsuarioAnonimo() (1)\n");
			return false;
		}
		printf(" LOG: Alocando memória para email Comando-Adicao.h addUsuarioAnonimo()\n");
		if(strlen(token) > TAMANHO_LOGIN)
		{
			printf(" LOG: Tamanho de email exageradamente grande Comando-Adicao.h addUsuarioAnonimo()\n");
			return false;
		}
		char *email = malloc(sizeof(char) * (strlen(token) + 1));//Não esquecer do +1 (é o /0)
		if(email == NULL)
		{
			printf(" ERRO: Falha ao alocar memória para email Comando-Adicao.h addUsuarioAnonimo()\n");
			return false;
		}
		printf(" LOG: Memoria alocada, copiando de token para email Comando-Adicao.h addUsuarioAnonimo()\n");
		strcpy(email, token);
		if(strcmp(email, token) != 0)
		{
			printf(" Warning: Falha ao copiar de token para email Comando-Adicao.h addUsuarioAnonimo()\n");
			if(email != NULL)
			{
				free(email);
				email = NULL;
			}
			return false;
		}
		printf(" LOG: String copiada Comando-Adicao.h\n");
		token = strtok(NULL, " ");//APP 2 1 2 asdkjhaskd 7 loginDoUsuario 9
		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente (Comando-Adicao.h) addUsuarioAnonimo() (2)\n");
			free(email);
			email = NULL;
			return false;
		}
		if(strcmp(token, TIPO_SENHA) != 0)//APP 2 1 2 asdhasjdkh 7 logindousuario 9
		{
			printf(" Warning: Comando insuficiente Comando-Adicao.h addUsuarioAnonimo() (3)\n");
			free(email);
			email = NULL;
			return false;
		}
		token = strtok(NULL, "\0");//APP 2 1 2 asdhasjdkh 7 logindousuario 9 senhaInformada
		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente Comando-Adicao.h addUsuarioAnonimo() (4)\n");
			free(email);
			email = NULL;
			return false;
		}
		else//Minimiza erros
		{
			if(strlen(token) != TAMANHO_SENHA_PADRAO_USUARIO_ANONIMO)
			{
				printf(" Warning: Senha incorreta Comando-Adicao.h addUsuarioAnonimo() (1)\n");
				free(email);
				email = NULL;
				return false;
			}
			if(strcmp(token, SENHA_PADRAO_USUARIO_ANONIMO) == 0)//se senha informada for igual à senha padrao de usuario anonimo
			{
				printf(" LOG: alocando memoria para senha Comando-Adicao.h addUsuarioAnonimo()\n");
				char *senha = malloc(sizeof(char) * (strlen(token) + 1));
				if(senha != NULL)
				{
					strcpy(senha, token);
					if(senha != NULL)
					{
						if(strcmp(senha, token) == 0)
						{
							printf(" LOG: Solicitando adicao de usuario à base de dados Comando-Adicao.h addUsuarioAnonimo()\n");
							if(addUsuarioAnonimoAoBanco(email, senha))//SUCESSO usuario criado (manda usuario para adição direta no banco)
							{
								printf(" LOG: Usuario cadastrado com sucesso Comando-Adicao.h addUsuarioAnonimo()\n");
								free(email);
								email = NULL;
								free(senha);
								senha = NULL;
								return true;
							}
							else
							{
								printf(" Warning: Fala ao adicionar usuario ao banco de dados Comando-Adicao.h addUsuarioAnonimo()\n");
								free(email);
								email = NULL;
								free(senha);
								senha = NULL;
								return false;
							}

						}
						else
						{
							printf(" ERRO: Copia de strings falhou, Abortando operação Comando-Adicao.h addUsuarioAnonimo()\n");
							free(email);
							email = NULL;
							free(senha);
							senha = NULL;
							return false;
						}
					}
					else
					{
						printf(" ERRO: não foi possível copiar string de token para senha Comando-Adicao.h addUsuarioAnonimo()\n");
						free(email);
						email= NULL;
						if(senha != NULL)
						{
							free(senha);
							senha = NULL;
						}
						return false;
					}
				}
				else
				{
					printf(" ERRO: Não foi possivel alocar memoria para senha Comando-Adicao.h addUsuarioAnonimo()\n");
					free(email);
					email = NULL;
					return false;
				}
			}
			else
			{
				printf(" Warning: Senha padrao de usuario anonimo nao confirmada Comando-Adicao.h addUsuarioAnonimo()\n");
				printf(" \t\t Abortando! (usuario não será criado)\n");
				free(email);
				email = NULL;
				return false;
			}
			printf(" ERRO: Desconhecido em Comando-Adicao.h addUsuarioAnonimo() (2)\n");
			printf(" \t\tAbortando! (usuario não será criado)\n");
			if(email != NULL)
			{
				free(email);
			}
			email = NULL;
			return false;//TEMP
		}
		if(email != NULL)
		{
			free(email);
			email = NULL;
			printf(" ERRO: Desconhecido em Comando-Adicao.h addUsuarioAnonimo() (1)\n");
			return false;
		}
	}
	else if(strcmp(token, TIPO_SENHA) == 0)// APP 2 1 2 asdjsadklsa 9
	{
		printf(" LOG: Cliente informando senha Comando-Adicao.h addUsuarioAnonimo()\n");
		token = strtok(NULL, " ");
		if(token == NULL)// APP 2 1 2 askdjsakd 9 logindousuario
		{
			printf(" Warning: Comando insuficiente Comando-Adicao.h addUsuarioAnonimo() (1)\n");
			return false;
		}
		printf(" LOG: Alocando memória para senha Comando-Adicao.h addUsuarioAnonimo()\n");
		if(strlen(token) > TAMANHO_SENHA_PADRAO_USUARIO_ANONIMO)
		{
			printf(" LOG: Tamanho de senha exageradamente grande Comando-Adicao.h addUsuarioAnonimo()\n");
			return false;
		}
		char *senha = malloc(sizeof(char) * (strlen(token) + 1));//Não esquecer do +1 (é o /0)
		if(senha == NULL)
		{
			printf(" ERRO: Falha ao alocar memória para senha Comando-Adicao.h addUsuarioAnonimo()\n");
			return false;
		}
		if(strcmp(senha, SENHA_PADRAO_USUARIO_ANONIMO) != 0)
		{
			printf(" Warning: Senha informada é incorreta em Comando-Adicao.h addUsuarioAnonimo() (senha primeiro)\n");
			return false;
		}
		printf(" LOG: Memoria alocada, copiando de token para senha Comando-Adicao.h addUsuarioAnonimo()\n");
		strcpy(senha, token);
		if(strcmp(senha, token) != 0)
		{
			printf(" Warning: Falha ao copiar de token para senha Comando-Adicao.h addUsuarioAnonimo()\n");
			if(senha != NULL)
			{
				free(senha);
				senha = NULL;
			}
			return false;
		}
		printf(" LOG: String copiada Comando-Adicao.h\n");
		token = strtok(NULL, " ");//APP 2 1 2 asdkjhaskd 9 loginDoUsuario 7
		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente (Comando-Adicao.h) addUsuarioAnonimo() (2)\n");
			free(senha);
			senha = NULL;
			return false;
		}
		if(strcmp(token, TIPO_LOGIN) != 0)//APP 2 1 2 asdhasjdkh 9 logindousuario 7
		{
			printf(" Warning: Comando insuficiente Comando-Adicao.h addUsuarioAnonimo() (3)\n");
			free(senha);
			senha = NULL;
			return false;
		}
		token = strtok(NULL, "\0");//APP 2 1 2 asdhasjdkh 9 logindousuario 7 senhaInformada
		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente Comando-Adicao.h addUsuarioAnonimo() (4)\n");
			free(senha);
			senha = NULL;
			return false;
		}
		else//Minimiza erros
		{
			if(strlen(token) > TAMANHO_LOGIN)
			{
				printf(" Warning: Login exageradamente grande em Comando-Adicao.h addUsuarioAnonimo() (1)\n");
				free(senha);
				senha = NULL;
				return false;
			}
			printf(" LOG: alocando memoria para email Comando-Adicao.h addUsuarioAnonimo()\n");
			char *email = malloc(sizeof(char) * (strlen(token) + 1));
			if(email != NULL)
			{
				strcpy(email, token);
				if(email != NULL)
				{
					if(strcmp(senha, token) == 0)
					{
						printf(" LOG: Solicitando adicao de usuario à base de dados Comando-Adicao.h addUsuarioAnonimo()\n");
						if(addUsuarioAnonimoAoBanco(email, senha))//SUCESSO usuario criado (manda usuario para adição direta no banco)
						{
							printf(" LOG: Usuario cadastrado com sucesso Comando-Adicao.h addUsuarioAnonimo()\n");
							free(email);
							email = NULL;
							free(senha);
							senha = NULL;
							return true;
						}
						else
						{
							printf(" Warning: Fala ao adicionar usuario ao banco de dados Comando-Adicao.h addUsuarioAnonimo()\n");
							free(email);
							email = NULL;
							free(senha);
							senha = NULL;
							return false;
						}

					}
					else
					{
						printf(" ERRO: Copia de strings falhou, Abortando operação Comando-Adicao.h addUsuarioAnonimo()\n");
						free(email);
						email = NULL;
						free(senha);
						senha = NULL;
						return false;
					}
				}
				else
				{
					printf(" ERRO: não foi possível copiar string de token para email Comando-Adicao.h addUsuarioAnonimo()\n");
					free(senha);
					senha= NULL;
					if(senha != NULL)
					{
						free(email);
						email = NULL;
					}
					return false;
				}
			}
			else
			{
				printf(" ERRO: Não foi possivel alocar memoria para email Comando-Adicao.h addUsuarioAnonimo()\n");
				free(senha);
				senha = NULL;
				return false;
			}
		printf(" ERRO: Desconhecido em (Comando-Adicao.h) addUsuarioAnonimo()\n");
		return false;//TEMP
		}
	}
	else
	{
		printf(" Warning: TIPO não suportado Comando-Adicao.h addUsuarioAnonimo()\n");
		return false;
	}
}

bool addUsuario(char *emailAnterior)//TODO  APP 2 1 1
{
	if(emailAnterior == NULL)
	{
		printf(" Warning: Email informado é nulo, não será possivel converter usuario anonimo em usuario registrado dessa forma, abortando em Comando-Adicao.h addUsuario()\n");
		return false;
	}

	char *token = NULL;
	token = strtok(NULL, " ");// APP 2 1 1 7            (7 == TIPO_LOGIN)

	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addUsuario() e14d5r2v\n");
		return false;
	}
	else if(strlen(token) > TAMANHO_TIPO)
	{
		printf(" Warning: Comando exageradamente grande em Comando-Adicao.h addUsuario() wekbjjw\n");
		printf(" \t TOKEN = |%s|\n", token);
		return false;
	}
	else if(strcmp(token, TIPO_LOGIN) != 0)
	{
		printf(" Warning: Comando incorreto em Comando-Adicao.h addUsuario() saasdjkh \n");
		return false;
	}

	token = strtok(NULL, " ");// APP 2 1 1 7 emailInformado
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addUsuario() kkqhhasbnv\n");
		return false;
	}
	else if( strlen(token) > TAMANHO_LOGIN)
	{
		printf(" Warning: Comando exageradamente grande em Comando-Adicao.h addUsuario() sakjv31sx0\n");
		return false;
	}

	char *email = NULL;
	email = malloc(sizeof(char) * (strlen(token) + 1));// Esse +1 é o do maldito \0

	if(email == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para email em Comando-Adicao.h addUsuario() sajhbr15550ddsr\n");
		return false;
	}

	strcpy(email, token);
	if(email == NULL)
	{
		printf(" Warning: falha ao copiar de token para email em Comando-Adicao.h addUsuario() asd45r6v1sd0\n");
		return false;
	}

	token = strtok(NULL, " ");// APP 2 1 1 7 emailInformado 9    (9 == TIPO_SENHA)
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addUsuario() chj3wds140g5qaw\n");
		free(email);
		email = NULL;
		return false;
	}
	else if(strlen(token) > TAMANHO_TIPO)
	{
		printf(" Warning: Comando exageradamente grande em Comand-Adicao.h addUsuario()\n");
		free(email);
		email = NULL;
		return false;
	}
	else if(strcmp(token, TIPO_SENHA) != 0)
	{
		printf(" Warning: Comando incorreto em Comand-Adicao.h addUsuario()\n");
		free(email);
		email = NULL;
		return false;
	}

	// AINDA NÃO IMPLEMENTADO DESSA FORMA → APP 2 1 1 7 emailInformado 9 senhaCriptografadaAqui     IT's SHOW TIME ! //TODO
	// ALTERNATIVA EM USO → APP 2 1 1 7 emailInformado 9 senhaCruaInformada
	
	token = strtok(NULL, " ");// APP 2 1 1 7 emailInformado 9 senhaCruaInformada

	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addUsuario() 5a1e55q\n");
		free(email);
		email = NULL;
		return false;
	}
	else if(strlen(token) > TAMANHO_SENHA)
	{
		printf(" Warning: Comando exageradamente grande em Comando-Adicao.h addUsuario() 1q54r2s\n");
		free(email);
		email = NULL;
		return false;
	}

	char *senha = malloc(sizeof(char) * (strlen(token) + 1));// + 1 é do \0

	if(senha == NULL)
	{
		printf(" Warning: Não foi possivel alocar memoria para senha em Comando-Adicao.h addUsuario() q45e8c1aaqeh\n");
		free(email);
		email = NULL;
		return false;
	}

	strcpy(senha, token);

	if(senha == NULL)
	{
		printf(" Warning: Falha ao copiar de token para senha em Comando-Adicao.h addUsuario()  qe848vrw\n");
		free(email);
		email = NULL;
		return false;
	}

	token = strtok(NULL, " ");// APP 2 1 1 7 emailInformado 9 senhaCruaInformada sexoInformado
	if(token == NULL)
	{
		printf(" LOG: Comando de adição de usuario informou somente Login e Senha\n");
		if(addUsuarioAoBanco(emailAnterior, email, senha, NULL, NULL))
		{
			printf(" LOG: Usuario adicionado com sucesso ao banco de dados em Comando-Adicao.h addUsuario() a654q87e\n");
			emailAnterior = NULL;
			emailAnterior = email;
			free(email);
			free(senha);
			senha = NULL;
			email = NULL;
			return true;
		}
		else
		{
			printf(" Warning: falha ao adicionar usuario ao banco de dados em Comand-Adicao.h addUsuario() (sexo não informado)\n");
			free(email);
			free(senha);
			senha = NULL;
			email = NULL;
			return false;
		}
		/* printf(" Warning: Comando insuficiente em Comando-Adicao.h addUsuario() 4q8e1h058\n");
		free(email);
		free(senha);
		senha = NULL;
		email = NULL;
		return false; */
	}

	if(strlen(token) > TAMANHO_SEXO_CLIENTE)
	{
		printf(" Warning: Comando exageradamente grande em Comando-Adicao.h addUsuario() vqe5489wer20\n");
		free(email);
		free(senha);
		senha = NULL;
		email = NULL;
		return false;
	}

	char *sexo = malloc(sizeof(char) * (strlen(token) + 1));
	if(sexo == NULL)
	{
		printf(" Warning: Não foi possivel alocar memoria para sexo em Comando-Adicao.h addUsuario() qef48fsw\n");
		free(email);
		free(senha);
		senha = NULL;
		email = NULL;
		return false;
	}

	strcpy(sexo, token);
	if(sexo == NULL)
	{
		printf(" Warning: Erro ao copiar de token para sexo em Comando-Adicao.h addUsuario()  cqeewf48v92q0\n");
		free(email);
		free(senha);
		senha = NULL;
		email = NULL;
		return false;
	}

	token = strtok(NULL, " ");// APP 2 1 1 7 emailInformado 9 senhaCruaInformada sexoInformado dataNascimentoInformada
	if(token == NULL)
	{
		free(email);
		free(senha);
		free(sexo);
		sexo = NULL;
		senha = NULL;
		email = NULL;
		return false;
	}

	if(strlen(token) > TAMANHO_DATA)
	{
		printf(" Warning: Comando exageradamente grande em Comando-Adicao.h addUsuario() asd15e9q8x\n");
		free(email);
		free(senha);
		free(sexo);
		sexo = NULL;
		senha = NULL;
		email = NULL;
		return false;
	}

	char *dataNascimento = malloc(sizeof(char) * (strlen(token) + 1));
	if(dataNascimento == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para dataNascimento em Comando-Adicao.h addUsuario() eq54v89qc\n");
		free(email);
		free(senha);
		free(sexo);
		sexo = NULL;
		senha = NULL;
		email = NULL;
		return false;
	}

	strcpy(dataNascimento, token);
	if(dataNascimento == NULL)
	{
		printf(" Warning: Falha ao copiar de token para dataNascimento em Comando-Adicao.h addUsuario() jhqiujbcx983\n");
		free(email);
		free(senha);
		free(sexo);
		sexo = NULL;
		senha = NULL;
		email = NULL;
		return false;
	}

	if(addUsuarioAoBanco(emailAnterior, email, senha, sexo, dataNascimento))
	{
		printf(" LOG: Usuario adicionado com sucesso ao banco de dados em Comando-Adicao.h addUsuario()  qbu39sad87\n");
		free(email);
		free(senha);
		free(sexo);
		free(dataNascimento);
		dataNascimento = NULL;
		sexo = NULL;
		senha = NULL;
		email = NULL;
		return true;
	}
	else
	{
		printf(" Warning: Falha ao adicionar usuario ao banco de dados em Comando-Adicao.h addUsuario() qh38v9q2ubhd\n");
		free(email);
		free(senha);
		free(sexo);
		free(dataNascimento);
		dataNascimento = NULL;
		sexo = NULL;
		senha = NULL;
		email = NULL;
		return false;		
	}

	printf(" ERRO: DEU MERDA em Comando-Adicao.h addUsuario()\n");
	return false;
	// return false;  // Teoricamente nunca vai chegar aqui, mas vai que...
}


bool addVisualizacao(char *email, bool usuarioAnonimo)/* APP 2 2*/
{
	if(email == NULL)
	{
		printf("Email == NULL (Comando-Adicao.h) addVisualizacao()\n");
		return false;
	}
	printf("*************************COMANDO ADICAO ********************\n");
	
	char* token = strtok(NULL, " ");// APP 2 2 idProduto
	
	if(token == NULL)
	{
		printf(" ERRO: Comando incompleto (1) addVisualizacao() (Comando-Adicao.h)\n");
		return false;
	}
	
	if(strlen(token) != 10)/* APP 2 2 idProduto */
	{
		printf(" Warning: Argumento contém quantidade inválida de caracteres Comando-Adicao.h addVisualizacao()\n");
		return false;
	}
	

	char *id = malloc(sizeof(char) * (strlen(token) + 1));

	if(id == NULL)
	{
		printf(" Warning: ñão foi possivel alocar memoria de ID Comando-Adicao.h addVisualizacao()\n");
		return false;
	}
	
	strcpy(id, token);

	if(id == NULL)
	{
		printf(" ERRO: ao alocar memória de ID (10B) addVisualizacao() (Comando-Adicao.h) \n");
		return false;
	}
	

	if(conexao == NULL)
	{
		// printf("ERRO: Falha ao alocar memória para conexao\n");
		printf(" ERRO: CONEXAO NULA COM O BANCO DE DADOS (Comando-Adicao.h) (addVisualizacao())\n");
		return false;
	}

	// if(!checarIdProduto(conexao, id))
	if(!checarIdProduto(id))
	{
		printf(" ERRO: Produto não existe na base de dados Comando-Adicao.h addVisualizacao()\n");
		// mysql_close(conexao);
		// free(conexao);
		// conexao = NULL;
		free(id);
		id = NULL;
		return false;
	}

	// Calcula a quantidade de cliques informada no quinto argumento
	token = NULL;

	token = strtok(NULL, " ");// APP 2 2 idProduto quantidade
	if(token == NULL)/* APP 2 2 idProduto quantidade */
	{
		printf(" ERRO: Comando incompleto  (2)  addVisualizacao() (Comando-Adicao.h)\n");
		printf("\t\tEncerrando Conexão Comando-Adicao.h addVisualizacao()\n");
		
		// mysql_close(conexao);
		// free(conexao);
		// conexao = NULL;
		free(id);
		id = NULL;
		return false;
	}

	// //Converte quantidade dada em string para quantidade dada em inteiro
	// int quantidade = 0;
	// if(token[0] - '0' < 10 && token[0] - '0' >= 0)
	// {
	// 	quantidade = token[0] - '0';

	// 	// printf(" primeira quantidade → %d\n", quantidade);
	// }
	// else
	// {
	// 	printf(" Warning: Comando incorreto (addVisualizacao) (Comando-Adicao.h)\n");
		
	// 	free(id);
	// 	id = NULL;
	// 	return false;
	// }

	// int i = 1;

	// while(token[i] != '\0')
	// {
	// 	if(token[i] - '0' < 10 && token[i] - '0' >= 0)
	// 	{
	// 		quantidade = quantidade * 10;
	// 		quantidade = (token[i] - '0') + quantidade;
	// 	}
	// 	else
	// 	{
	// 		printf(" ERRO: Comando incorreto (dentro do while) (%d vez)  addVisualizacao() (Comando-Adicao.h)\n", i);
			
	// 		free(id);
	// 		id = NULL;
	// 		return false;
	// 	}
	// 	i++;
	// }
	// //FIM Converte quantidade dada em string para quantidade dada em inteiro

	char *quantidade = strdup(token);

	if(quantidade == NULL)
	{
		printf(" Warning: Não foi possível duplicar token para quantidade em Comando-Adicao.h addVisualizacao()\n");
		free(id);
		id = NULL;
		return false;
	}

	//Quandidade de cliques calculada corretamente

	//CONDIÇÕES PARA GERAR LOG, SOMENTE
	if(!addVisualizacoesAoBanco(id, quantidade, email, usuarioAnonimo))
	{
		printf(" ERRO: Não foi possível atualizar a base de dados addVisualizacao() (Comando-Adicao.h)\n");
		free(id);
		free(quantidade);
		quantidade = NULL;
		id = NULL;
		return false;
	}
	else
	{
		printf(" LOG: Dados atualizados Comando-Adicao.h addVisualizacao()\n");
		free(id);
		free(quantidade);
		quantidade = NULL;
		id = NULL;
		return true;
	}
}

bool addCidade()/* APP 2 { */
{
	char *token = NULL;
	token = strtok(NULL, " ");
	if(token == NULL)/* APP 2 { S */
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addCidade()\n");
		return false;
	}
	if(strlen(token) > 1)
	{
		printf(" Warning: Argumento exageradamente grande em Comando-Adicao.h addCidade()\n");
		return false;
	}
	if(strcmp(token, TIPO_ESTADO) != 0)
	{
		printf(" Warning: Argumento inválido em Comando-Adicao.h addCidade()\n");
		return false;
	}
	token = strtok(NULL, " ");/* APP 2 { nomeDoEstado */
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addCidade()\n");
		return false;
	}
	if(strlen(token) != 2)/* APP 2 { S nomeDoEstado */
	{
		printf(" Warning: Tamanho de estado informado incorreto Comando-Adicao.h addCidade()\n");
		return false;
	}
	char *nomeDoEstado = malloc(sizeof(char) * (strlen(token) + 1));
	if(nomeDoEstado == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para nome do estado em Comando-Adicao.h addCidade()\n");
		return false;
	}
	strcpy(nomeDoEstado, token);

	if(nomeDoEstado == NULL)
	{
		printf(" Warning: Falha ao copiar nome do estado de token para nomeDoEstado em Comando-Adicao.h addCidade()\n");
		return false;
	}
	if(strcmp(nomeDoEstado, token) != 0)
	{
		printf(" Warning: Executada copia incorreta de token para nomeDoEstado em Comando-Adicao.h addCidade()\n");
		return false;
	}
	token = strtok(NULL, " ");/* APP 2 { S nomeDoEstado nomeDaCidade */
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addCidade() (2)\n");
		return false;
	}
	if(strlen(token) > TAMANHO_DO_NOME_DA_CIDADE_COM_MAIOR_NOME_DO_MUNDO)
	{
		printf(" Warning: Tamanho de cidade exageradamente grande em Comando-Adicao.h addCidade()\n");
		return false;
	}

	char *nomeCidade = NULL;
	nomeCidade = malloc(sizeof(char) * (strlen(token) + 1));
	if(nomeCidade == NULL)
	{
		printf(" Warning: falha ao alocar memória para nomeCidade em Comando-Adicao.h addCidade()\n");
		return false;
	}

	printf(" LOG: Copiando de token para nomeCidade em Comando-Adicao.h addCidade()\n");
	strcpy(nomeCidade, token);

	if(nomeCidade == NULL)
	{
		printf(" Warning: Falha ao copiar memória de token para nomeCidade em Comando-Adicao.h addCidade()\n");
		return false;
	}
	if(strcmp(token, nomeCidade) != 0)
	{
		printf(" Warning: Copia incorreta de nomeDeUsuario em Comando-Adicao.h addCidade()\n");
		free(nomeCidade);
		nomeCidade = NULL;
		return false;
	}

	if(!addCidadeAoBanco(nomeDoEstado, nomeCidade))
	{
		printf(" Warning: Falha ao adicionar cidade ao banco de dados Comando-Adicao.h addCidade()\n");
		free(nomeCidade);
		nomeCidade= NULL;
		return false;
	}
	else
	{
		printf(" LOG: Nome de cidade adicionado com sucesso ao banco de dados Comando-Adicao.h addCidade()\n");
		return true;
	}
	printf(" ERRO: Erro Desconhecido em Comando-Adicao.h addCidade()\n");
	return false;
}


bool addLocalizacao()/* APP 2 l0 */
{
	char *token = NULL;
	token = strtok(NULL, " ");/* APP 2 l0 $ */
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addLocalizacao() (1)\n");
		return false;
	}
	if(strcmp(token, TIPO_ID_CIDADE) != 0)
	{
		printf(" Warning: Argumento inválido em Comando-Adicao.h addLocalizacao()\n");
		return false;
	}

	token = strtok(NULL, " ");// APP 2 l0 $ idCidade
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addLocalizacao() (2)\n");
		return false;
	}
	if(strlen(token) > TAMANHO_DE_INTEIRO_EM_BANCO_DE_DADOS)
	{
		printf(" Warning: Argumento exageradamente grande em Comando-Adicao.h addLocalizacao()\n");
		return false;
	}

	char *idCidade = NULL;
	idCidade = malloc(sizeof(char) * (strlen(token) + 1));
	if(idCidade == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para idCidade em Comando-Adicao.h addLocalizacao()\n");
		return false;
	}
	strcpy(idCidade, token);
	if(idCidade == NULL)
	{
		printf(" Warning: Falha ao copiar token para idCidade Comando-Adicao.h addLocalizacao()\n");
		return false;
	}
	if(strcmp(idCidade, token) != 0)
	{
		printf(" Warning: Falha em copiar strings de token para idCidade Comando-Adicao.h addLocalizacao()\n");
		free(idCidade);
		idCidade = NULL;
		return false;
	}
	token = strtok(NULL, " ");// APP 2 l0 $ idCidade L
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addLocalizacao() (3)\n");
		free(idCidade);
		idCidade = NULL;
		return false;
	}
	if(strcmp(token, TIPO_CEP) != 0)
	{
		printf(" Warning: CEP Não informado (que é obrigatorio) em Comando-Adicao.h addLocalizacao()\n");
		free(idCidade);
		idCidade = NULL;
		return false;
	}

	token = strtok(NULL, " ");// APP 2 l0 $ idCidade L CEP
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addLocalizacao() (4)\n");
		free(idCidade);
		idCidade = NULL;
		return false;
	}
	if(strlen(token) != 8)
	{
		printf(" Warning: Argumento incorreto (precisa ter 8 de tamanho, o CEP Comando-Adicao.h addLocalizacao()\n");
		free(idCidade);
		idCidade = NULL;
		return false;
	}

	char *cep = malloc(sizeof(char) * (strlen(token + 1)));
	if(cep == NULL)
	{
		printf(" Warning: não foi possível alocar memoria para CEP em Comando-Adicao.h addLocalizacao()\n");
		free(idCidade);
		idCidade = NULL;
		return false;
	}

	strcpy(cep, token);
	if(cep == NULL)
	{
		printf(" Warning: Falha ao copiar de token para CEP em Comando-Adicao.h addLocalizacao()\n");
		free(idCidade);
		idCidade = NULL;
		return false;
	}
	if(strcmp(token, cep) != 0)
	{
		printf(" Warning: Copia incorreta de token para CEP em Comando-Adicao.h addLocalizacao()\n");
		free(idCidade);
		idCidade = NULL;
		free(cep);
		cep = NULL;
		return false;
	}

	token = strtok(NULL, " ");// APP 2 l0 $ idCidade L CEP R      ou     APP 2 l0 $ idCidade L CEP RN
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addLocalizacao() (5)\n");
		free(idCidade);
		idCidade = NULL;
		free(cep);
		cep = NULL;
		return false;
	}
	if(strcmp(token, TIPO_RUA) == 0)
	{
		printf(" LOG: Usuario informou adicao de rua em Comando-Adicao.h addLocalizacao()\n");
		//TODO
	}
	else if(strcmp(token, TIPO_RUA_NULO) == 0)// APP 2 l0 $ idCidade L CEP RN
	{
		printf(" LOG: Usuario não informou adicao de rua, prosseguindo com interpretacao\n");
		token = strtok(NULL, " ");// APP 2 l0 $ idCidade L CEP RN n      OU    APP 2 l0 $ idCidade L CEP RN nN
		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente em Comando-Adicao.h addLocalizacao() (6)\n");
			free(idCidade);
			idCidade = NULL;
			free(cep);
			cep = NULL;
			return false;
		}
		if(strcmp(token, TIPO_NUMERO) == 0)// APP 2 l0 $ idCidade L CEP RN n
		{
			printf(" LOG: Usuario informou Numero em Comando-Adicao.h addLocalizacao()\n");
			token = strtok(NULL, " ");// APP 2 l0 $ idCidade L CEP Rn n numero
			if(token == NULL)
			{
				printf(" Warning: token == NULL em Comando-Adicao.h addLocalizacao()\n");
				free(cep);
				free(idCidade);
				cep = NULL;
				idCidade = NULL;
				return false;
			}
			if(strlen(token) > TAMANHO_NUMERO)
			{
				printf(" Warning: Tamanho informado é exageradamente grande em Comando-Adicao.h addLocalizacao()\n");
				free(idCidade);
				free(cep);
				cep = NULL;
				idCidade = NULL;
				return false;
			}
			char *numero = malloc(sizeof(char) * (strlen(token) + 1));
			if(numero == NULL)
			{
				printf(" Warning: Impossível alocar memoria para numero em Comando-Adicao.h addLocalizacao()\n");
				free(cep);
				free(idCidade);
				idCidade = NULL;
				cep = NULL;
				return false;
			}
			strcpy(numero, token);
			if(strcmp(numero, token) != 0)
			{
				printf(" Warning: Falha ao copiar de token para numero em Comando-Adicao.h addLocalizacao() KKAHSJJJHANNE\n");
				free(idCidade);
				free(cep);
				cep = NULL;
				idCidade = NULL;
				if(numero != NULL)
				{
					free(numero);
					numero = NULL;
				}
				return false;
			}
			token = strtok(NULL, " ");// APP 2 l0 $ idCidade L CEP Rn n numero b     OU   APP 2 l0 $ idCidade L CEP Rn n numero bN
			if(token == NULL)
			{
				printf(" Warning: Comando insuficiente em Comando-Adicao.h addLocalizacao() JJJANNNAH\n");
				free(cep);
				free(numero);
				free(idCidade);
				numero = NULL;
				idCidade = NULL;
				cep = NULL;
				return false;
			}
			if(strcmp(token, TIPO_BAIRRO) == 0)// APP 2 l0 $ idCidade L CEP Rn n numero b
			{
				//TODO
				return false; // porque não está pronto ainda
			}
			else if(strcmp(token, TIPO_BAIRRO_NULO) == 0)// APP 2 l0 $ idCidade L CEP Rn n numero bN
			{
				token = strtok(NULL, " ");// APP 2 l0 $ idCidade L CEP Rn n numero algumaCoisa
				if(token == NULL)
				{
					printf(" Warning: Comando insuficiente em Comando-Adicao.h addLocalizacao() KKKKKK\n");
					free(cep);
					free(numero);
					free(idCidade);
					cep = NULL;
					numero = NULL;
					idCidade = NULL;
					return false;
				}
				if(strlen(token) > TAMANHO_TIPO)
				{
					printf(" Warning: Tamanho de comando exageradamente grande em Comando-Adicao.h addLocalizacao() JANNEJH\n");
					free(idCidade);
					free(cep);
					free(numero);
					cep = NULL;
					numero = NULL;
					idCidade = NULL;
					return false;
				}
				if(strcmp(token, TIPO_COMPLEMENTO) == 0)// APP 2 l0 $ idCidade L CEP Rn n numero M
				{
					printf(" LOG: Cliente informando complemento em Comando-Adicao.h addLocalizacao() JNNANKJHE\n");
					token = strtok(NULL, " ");// APP 2 l0 $ idCidade L CEP Rn n numero M complementoInformado
					if (token == NULL)
					{
						printf(" Warning: Comando insuficiente em Comando-Adicao.h addLocalizacao() JJNNNNABB\n");
						free(idCidade);
						free(cep);
						free(numero);
						numero = NULL;
						cep = NULL;
						idCidade = NULL;
						return false;
					}
					if(strlen(token) > TAMANHO_COMPLEMENTO)
					{
						printf(" Warning: Tamanho de complemento informado exageradamente grande em Comando-Adicao.h addLocalizacao() NJNEBQ\n");
						free(idCidade);
						free(cep);
						free(numero);
						numero = NULL;
						cep = NULL;
						idCidade = NULL;
						return false;
					}
					char *complemento = malloc(sizeof(char) * (strlen(token) + 1));
					if(complemento == NULL)
					{
						printf(" Warning: Não foi possível alocar memoria para complemento em Comando-Adicao.h addLocalizacao() NNNQQ\n");
						free(idCidade);
						free(cep);
						free(numero);
						numero = NULL;
						cep = NULL;
						idCidade = NULL;
						return false;
					}
					strcpy(complemento, token);
					if(complemento == NULL)
					{
						printf(" Warning: Falha ao copiar de token para complemento em Comando-Adicao.h addLocalizacao() HJHQN\n");
						free(idCidade);
						free(numero);
						free(cep);
						cep = NULL;
						numero = NULL;
						idCidade = NULL;
						return false;
					}
					if(strcmp(complemento, token) != 0)
					{
						printf(" Warning: Erro em copia de token para complemento em Comando-Adicao.h addLocalizacao() QNSK\n");
						free(idCidade);
						free(cep);
						free(numero);
						free(complemento);
						complemento = NULL;
						cep = NULL;
						numero = NULL;
						idCidade = NULL;
						return false;
					}

					if(addLocalizacaoAoBanco(idCidade, cep, NULL, NULL, numero, complemento))
					{
						printf(" LOG: Localizacao adicionada com sucesso ao banco de dados em Comando-Adicao.h addLocalizacao() GGGQH\n");
						free(cep);
						free(idCidade);
						free(numero);
						free(complemento);
						complemento = NULL;
						cep = NULL;
						numero = NULL;
						idCidade = NULL;
						return true;
					}
					else
					{
						printf(" Warning: Não foi possível adicionar localizacao ao banco de dados em Comando-Adicao.h addLocalizacao() HQWEK\n");
						free(idCidade);
						free(cep);
						free(complemento);
						free(numero);
						numero = NULL;
						cep = NULL;
						complemento = NULL;
						idCidade = NULL;
						return false;
					}
					printf(" ERRO FATAL: erro indentificável em Comando-Adicao.h addLocalizacao() QWEJGC\n");
					if(cep != NULL)
					{
						free(cep);
						cep = NULL;
					}
					if(complemento != NULL)
					{
						free(complemento);
						complemento = NULL;
					}
					if(numero != NULL)
					{
						free(numero);
						numero = NULL;
					}
					if(idCidade != NULL)
					{
						free(numero);
						numero = NULL;
					}
					return false;
				}
				else if(strcmp(token, TIPO_COMPLEMENTO_NULO) == 0)//  APP 2 l0 $ idCidade L CEP RN n numero MN
				{
					printf(" LOG: Cliente não informou complemento em Comando-Adicao.h addLocalizacao() lNNJANJEH\n");
					if(addLocalizacaoAoBanco(idCidade, cep, NULL, NULL, numero, NULL))
					{
						printf(" LOG: Localizacao adicionada com sucesso ao banco de dados em Comando-Adicao.h addLocalizacao()  HHJHAJEN\n");
						free(idCidade);
						free(cep);
						free(numero);
						numero = NULL;
						cep = NULL;
						idCidade = NULL;
						return true;
					}
					else
					{
						printf(" Warning: falha ao adicionar localizacao ao banco de dados em Comando-Adicao.h addLocalizacao() NNNMAH\n");
						free(cep);
						free(idCidade);
						free(numero);
						numero = NULL;
						idCidade = NULL;
						cep = NULL;
						return false;
					}
				}
				else
				{
					printf(" ERRO: Comando incorreto informado em Comando-Adicao.h addLocalizacao()\n");
					free(cep);
					free(idCidade);
					free(numero);
					numero = NULL;
					idCidade = NULL;
					cep = NULL;
					return false;
				}
				printf(" ERRO FATAL: identificável em Comando-Adicao.h addLocalizacao()\n");
				return false;
			}
			else
			{
				printf(" Warning: Comando incorreto em Comando-Adicao.h addLocalizacao() KKAHHJNENEN\n");
				free(idCidade);
				free(cep);
				free(numero);
				numero = NULL;
				cep = NULL;
				idCidade = NULL;
				return false;
			}
		}
		else if(strcmp(token, TIPO_NUMERO_NULO) == 0)
		{
			printf(" LOG: Usuario não informou Numero em Comando-Adicao.h addLocalizacao()\n");
			token = strtok(NULL, " ");// APP 2 l0 $ idCidade L CEP RN nN b      OU    APP 2 l0 $ idCidade L CEP RN nN bN
			if(token == NULL)
			{
				printf(" Warning: Comando insuficiente em Comando-Adicao.h addLocalizacao() (7)\n");
				free(idCidade);
				idCidade = NULL;
				free(cep);
				cep = NULL;
				return false;
			}
			if(strcmp(token, TIPO_BAIRRO) == 0)// APP 2 l0 $ idCidade L CEP RN nN b
			{
				printf(" LOG: Usuario informou bairro em Comando-Adicao.h addLocalizacao()\n");

				token = strtok(NULL, " ");// APP 2 l0 $ idCidade L CEP RN nN b nomeDoBairro
				if(token == NULL)
				{
					printf(" Warning: Comando insuficiente em Comando-Adicao.h addLocalizacao() (10)\n");
					free(idCidade);
					idCidade = NULL;
					free(cep);
					cep= NULL;
					return false;
				}
				if(strlen(token) > TAMANHO_BAIRRO)
				{
					printf(" Warning: Tamanho de bairro exageradamente grande em Comando-Adicao.h addLocalizacao()\n");
					free(idCidade);
					free(cep);
					cep = NULL;
					idCidade = NULL;
					return false;
				}
				char *bairro = malloc(sizeof(char) * (strlen(token) + 1));
				if(bairro == NULL)
				{
					printf(" Warning: Falha ao alocar memoria para bairro Comando-Adicao.a addLocalizacao()\n");
					free(idCidade);
					free(cep);
					cep = NULL;
					idCidade = NULL;
					return false;
				}
				strcpy(bairro, token);
				if(bairro == NULL)
				{
					printf(" Warning: Falha ao copiar de token para bairro em Comando-Adicao.h addLocalizacao()\n");
					free(idCidade);
					free(cep);
					cep = NULL;
					idCidade = NULL;
					return false;
				}
				token = strtok(NULL, " ");//APP 2 l0 $ idCidade L CEP RN nN b nomeDoBairro M      ou     APP 2 l0 $ idCidade L CEP RN nN b nomeDoBairro MN
				if(token == NULL)
				{
					printf(" Warning: Comando insuficiente em Comando-Adicao.h addLocalizacao() (11)\n");
					free(idCidade);
					free(cep);
					free(bairro);
					bairro = NULL;
					cep = NULL;
					idCidade = NULL;
					return false;
				}
				if(strcmp(token, TIPO_COMPLEMENTO) == 0)//APP 2 l0 $ idCidade L CEP RN nN b nomeDoBairro M
				{
					token = strtok(NULL, " ");//APP 2 l0 $ idCidade L CEP Rn nN b nomeDoBairro M complementoInformado
					if(token == NULL)
					{
						printf(" Warning: Comando insuficiente em Comando-Adicao.h addLocalizacao() (12)\n");
						free(idCidade);
						free(cep);
						free(bairro);
						bairro = NULL;
						idCidade = NULL;
						cep = NULL;
						return false;
					}
					if(strlen(token) > TAMANHO_COMPLEMENTO)
					{
						printf(" Warning: Tamanho de complemento exageradamente grande em Comando-Adicao.h addLocalizacao() (lasdkhjksahjk)\n");
						free(idCidade);
						free(cep);
						free(bairro);
						bairro = NULL;
						cep = NULL;
						idCidade = NULL;
						return false;
					}
					char *complemento = malloc(sizeof(char) * (strlen(token) + 1));
					if(complemento == NULL)
					{
						printf(" Warning: Falha ao alocar memoria para complemento em Comando-Adicao.h addLocalizacao() (NNJNakjnsj*798f1hghsj)\n");
						free(idCidade);
						free(cep);
						free(bairro);
						bairro = NULL;
						cep = NULL;
						idCidade = NULL;
						return false;
					}
					strcpy(complemento, token);
					if(complemento == NULL)
					{
						printf(" Warning: Falha ao copiar de token para complemento em Comando-Adicao.h addLocalizacao() (asjkdjq33386c76c8a)\n");
						free(idCidade);
						free(cep);
						free(bairro);
						idCidade = NULL;
						cep = NULL;
						bairro = NULL;
						return false;
					}

					if(addLocalizacaoAoBanco(idCidade, cep, bairro, NULL, NULL, complemento))
					{
						printf(" LOG: Sucesso ao adicionar localizacao ao banco de dados em Comando-Adicao.h addLocalizacao() (askjdhhh1127868c)\n");
						free(bairro);
						free(idCidade);
						free(cep);
						free(complemento);
						complemento = NULL;
						cep = NULL;
						bairro = NULL;
						idCidade = NULL;
						return true;
					}
					else
					{
						printf(" Warning: Falha ao adicionar localizacao ao banco de dados em Comando-Adicao.h addLocalizacao() (kjslllakk)\n");
						free(idCidade);
						free(cep);
						free(bairro);
						free(complemento);
						complemento = NULL;
						bairro = NULL;
						cep = NULL;
						idCidade = NULL;
						return false;
					}
				}
				else if(strcmp(token, TIPO_COMPLEMENTO_NULO) == 0)// APP 2 l0 $ idCidade L CEP RN nN b nomeDoBairro MN
				{
					if(addLocalizacaoAoBanco(idCidade, cep, bairro, NULL, NULL, NULL))
					{
						printf(" LOG: Localizacao adicionada com sucesso em Comando-Adicao.h addLocalizacao() (LanjKjhajHjenKJAhsudiuyYYahhekNNkahbcirouqb*78111867&)\n");
						free(idCidade);
						free(cep);
						free(bairro);
						bairro = NULL;
						cep = NULL;
						idCidade = NULL;
						return true;
					}
					else
					{
						printf(" Warning: Falha ao adicionar localizacao ao banco de dados em Comando-Adicao.h addLocalizacao() (vbwhdjg93776s6s67213jirs8vsg2jbb9s8)\n");
						free(idCidade);
						free(cep);
						free(bairro);
						bairro = NULL;
						cep = NULL;
						idCidade = NULL;
						return false;
					}
				}
				else
				{
					printf(" Warning: Comando incorreto em Comando-Adicao.h addLocalizacao() (5)\n");
					free(idCidade);
					free(cep);
					free(bairro);
					bairro = NULL;
					cep = NULL;
					idCidade = NULL;
					return false;
				}
			}
			else if(strcmp(token, TIPO_BAIRRO_NULO) == 0)// APP 2 l0 $ idCidade L CEP RN nN bN
			{
				printf(" LOG: Usuario não informou Bairro em Comando-Adicao.h addLocalizacao()\n");
				token = strtok(NULL, " ");// APP 2 l0 $ idCidade L CEP RN nN bN M     OU    APP 2 l0 $ idCidade L CEP RN nN bN MN
				if(token == NULL)
				{
					printf(" Warning: Comando insuficiente em Comando-Adicao.h addLocalizacao() (8)\n");
					free(idCidade);
					idCidade = NULL;
					free(cep);
					cep = NULL;
					return false;
				}
				if(strcmp(token, TIPO_COMPLEMENTO) == 0)// APP 2 l0 $ idCidade L CEP RN nN bN M
				{
					printf(" LOG: Usuario informou complemento em Comando-Adicao.h addLocalizacao()\n");

					token = strtok(NULL, " ");
					if(token == NULL)
					{
						printf(" Warning: Comando insuficiente em Comando-Adicao.h addLocalizacao() (9)\n");
						free(idCidade);
						idCidade = NULL;
						free(cep);
						cep = NULL;
						return false;
					}
					char *complemento = malloc(sizeof(char) * (strlen(token) + 1));
					if(complemento == NULL)
					{
						printf(" Warning: Falha ao alocar memoria para complemento em Comando-Adicao.h addLocalizacao()\n");
						free(idCidade);
						free(cep);
						idCidade = NULL;
						cep = NULL;
						return false;
					}
					strcpy(complemento, token);

					if(addLocalizacaoAoBanco(idCidade, cep, NULL, NULL, NULL, complemento))
					{
						printf(" LOG: Localizacao adicionada com sucesso ao banco de dados em Comando-Adicao.h addLocalizacao() (2)\n");
						free(idCidade);
						free(complemento);
						free(cep);
						cep = NULL;
						complemento = NULL;
						idCidade = NULL;
						return true;
					}
					else
					{
						printf(" Warning: Não foi possivel adicionar Localizacao ao banco de dados em Comando-Adicao.h addLocalizacao()\n");
						free(idCidade);
						free(complemento);
						free(cep);
						cep = NULL;
						complemento = NULL;
						idCidade = NULL;
						return false;
					}
					printf(" Warning: Exceção não manipulada em Comando-Adicao.h addLocalizacao() (LAKJSNJHJAK)\n");
					return false;
				}
				else if(strcmp(token, TIPO_COMPLEMENTO_NULO) == 0)// APP 2 l0 $ idCidade L CEP RN nN bN MN
				{
					printf(" LOG: Usuario não informou complemento em Comando-Adicao.h addLocalizacao()\n");
					if(addLocalizacaoAoBanco(idCidade, cep, NULL, NULL, NULL, NULL))
					{
						printf(" LOG: Adicao de localizacao efetuada com sucesso em Comando-Adicao.h addLocalizacao()\n");
						free(idCidade);
						idCidade = NULL;
						free(cep);
						cep = NULL;
						return true;
					}
					else
					{
						printf(" Warning: Falha ao adicionar Localizacao ao banco de dados em Comando-Adicao.h addLocalizacao()\n");
						free(idCidade);
						idCidade = NULL;
						free(cep);
						cep = NULL;
						return false;
					}
				}
				else
				{
					printf(" Warning: Comando incorreto em Comando-Adicao.h addLocalizacao() (4)\n");
					free(idCidade);
					idCidade = NULL;
					free(cep);
					cep = NULL;
					return false;
				}
			}
			else
			{
				printf(" Warning: Comando incorreto em Comando-Adicao.h addLocalizacao() (3)\n");
				free(idCidade);
				idCidade = NULL;
				free(cep);
				cep = NULL;
				return false;
			}
		}
		else
		{
			printf(" Warning: Comando incorreto em Comando-Adicao.h addLocalizacao() (2)\n");
			free(idCidade);
			idCidade = NULL;
			free(cep);
			cep = NULL;
			return false;
		}
	}
	else
	{
		printf(" Warning: Comando incorreto em Comando-Adicao.h addLocalizacao() (1)\n");
		free(idCidade);
		idCidade = NULL;
		free(cep);
		cep = NULL;
		return false;
	}
	printf(" ERRO: Exceção não manipulada em Comando-Adicao.h addLocalizacao() \n");
	free(idCidade);
	idCidade = NULL;
	free(cep);
	cep = NULL;
	return false;
}


bool addContratante()// APP 2 $C 
{
	printf("\t********************ADICAO DE CONTRATANTE***************************\n");
	char *token;
	token = strtok(NULL, " ");// APP 2 $C CHAVE_DE_SEGURANCA_PHP
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addContratante() NJQWEKJHIUYQBVJHIAHJSD\n");
		return false;
	}
	if(strlen(token) != TAMANHO_CHAVE_DE_SEGURANCA_PHP)
	{
		printf(" Warning: Chave incorreta em Comando-Adicao.h addContratante() (1)\n");
		return false;
	}
	if(strcmp(token, CHAVE_DE_SEGURANCA_PHP) != 0)
	{
		printf(" Warning: Chave incorreta em Comando-Adicao.h addContratante() (2)\n");
		return false;
	}
	// A partir daqui o usuário está autorizado (chave aprovada e usuario conectado)
	token = strtok(NULL, " ");// APP 2 $C CHAVE_DE_SEGURANCA_PHP #
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addContratante() nnqksjhr\n");
		return false;
	}
	if(strlen(token) > TAMANHO_TIPO)
	{
		printf(" Warning: Token obtido exageradamente grande em Comando-Adicao.h addContratante() qjwehka\n");
		return false;
	}
	if(strcmp(token, TIPO_NOME) != 0)
	{
		printf(" Warning: Comando incorreto em Comando-Adicao.h addContratante() asdkjqb\n");
		return false;
	}
	token = strtok(NULL, " ");// APP 2 $C CHAVE_DE_SEGURANCA_PHP # nome
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addContratante() qkrba\n");
		return false;
	}
	if(strlen(token) > TAMANHO_NOME)
	{
		printf(" Warning: Tamanho de token recebido exageradamente grande em Comando-Adicao.h addContratante() q1khaiu\n");
		return false;
	}
	char *nome;
	nome = malloc(sizeof(char) * (strlen(token) +1));
	if(nome == NULL)
	{
		printf(" Warning: Falha ao tentar alocar memoria para nome de contratante em Comando-Adicao.h addContratante() qaskb\n");
		return false;
	}
	strcpy(nome, token);
	if(strcmp(nome, token) != 0)
	{
		printf(" Warning: Copia incorreta em Comando-Adicao.h addContratante() qkjbxw\n");
		printf(" \tNome = |%s|\n", nome);
		printf(" \tToken = |%s|\n", token);
		free(nome);
		nome = NULL;
		return false;
	}

	token = strtok(NULL, " ");// APP 2 $C CHAVE_DE_SEGURANCA_PHP # nome cnpj
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addContratante() nqkjhiruha\n");
		free(nome);
		nome = NULL;
		return false;
	}
	if(strlen(token) != TAMANHO_CNPJ)
	{
		printf(" Warning: Tamanho incorreto de token de cnpj em Comando-Adicao.h addContratante()\n");
		free(nome);
		nome = NULL;
		return false;
	}
	char *cnpj;
	cnpj = malloc(sizeof(char) * (TAMANHO_CNPJ + 1));
	if(cnpj == NULL)
	{
		printf(" Warning: falha ao alocar memoria para cnpj em Comando-Adicao.h addContratante()\n");
		free(nome);
		nome = NULL;
		return false;
	}
	strcpy(cnpj, token);
	if(cnpj == NULL)
	{
		printf(" Warning: erro ao copiar cnpj em Comando-Adicao.h addContratante() qjhkc\n");
		free(nome);
		nome = NULL;
		return false;
	}
	if(strcmp(cnpj, token) != 0)
	{
		printf(" Warning: Copia incorreta de cnpj em Comando-Adicao.h addContratante() qkbqo\n");
		free(cnpj);
		free(nome);
		nome = NULL;
		cnpj = NULL;
		return false;
	}
	token = strtok(NULL, " ");// APP 2 $C CHAVE_DE_SEGURANCA_PHP # nome cnpj plano
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addContratante() qkjhbvq\n");
		free(cnpj);
		free(nome);
		nome = NULL;
		cnpj = NULL;
		return false;
	}
	if(strlen(token) != TAMANHO_PLANO)
	{
		printf(" Warning: Comando incorreto em Comando-Adicao.h addContratante() qkbvqox\n");
		free(cnpj);
		free(nome);
		nome = NULL;
		cnpj = NULL;
		return false;
	}
	char *plano;
	plano = malloc(sizeof(char) * (strlen(token) + 1));
	if(plano == NULL)
	{
		printf(" Warning: falha ao alocar memoria para plano em Comando-Adicao.h addContratante() qkbhve\n");
		free(cnpj);
		free(nome);
		nome = NULL;
		cnpj = NULL;
		return false;
	}
	strcpy(plano, token);
	if(plano == NULL)
	{
		printf(" Warning: falha ao copiar de token para plano em Comando-Adicao.h addContratante() qkbccw\n");
		free(cnpj);
		free(nome);
		nome = NULL;
		cnpj = NULL;
		return false;
	}
	if(strcmp(plano, token) != 0)
	{
		printf(" Warning: Copia incorreta de token para plano em Comando-Adicao.h addContratante() qkbveq\n");
		free(cnpj);
		free(nome);
		free(plano);
		plano = NULL;
		cnpj = NULL;
		nome = NULL;
		return false;
	}

	token = strtok(NULL, " ");// APP 2 $C CHAVE_DE_SEGURANCA_PHP # nome cnpj plano email
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addContratante() bcqkj\n");
		free(cnpj);
		free(nome);
		free(plano);
		plano = NULL;
		nome = NULL;
		cnpj = NULL;
		return false;
	}

	if(strlen(token) > TAMANHO_LOGIN)
	{
		printf(" Warning: token recebido é exageradamente grande em Comando-Adicao.h addContratante() kb1ius\n");
		free(cnpj);
		free(nome);
		free(plano);
		plano = NULL;
		nome = NULL;
		cnpj = NULL;
		return false;
	}

	char *email;
	email = malloc(sizeof(char) * (strlen(token) + 1));

	if (email == NULL)
	{
		printf(" Warning: não foi possível alocar memoria para email em Comando-Adicao.h addContratante() kqbceo\n");
		free(nome);
		free(cnpj);
		free(plano);
		plano = NULL;
		cnpj = NULL;
		nome = NULL;
		return false;
	}

	strcpy(email, token);

	if(email == NULL)
	{
		printf(" Warning: Cópia mal-sucedida em Comando-Adicao.h addContratante() qkbveas\n");
		free(nome);
		free(plano);
		free(cnpj);
		cnpj = NULL;
		plano = NULL;
		nome = NULL;
		return false;
	}

	if(strcmp(email, token) != 0)
	{
		printf(" Warning: Copia incorreta em Comando-Adicao.h addContratante() qkbeja\n");
		free(cnpj);
		free(nome);
		free(plano);
		free(email);
		email = NULL;
		plano = NULL;
		nome = NULL;
		cnpj = NULL;
		return false;
	}

	token = strtok(NULL, " ");// APP 2 $C CHAVE_DE_SEGURANCA_PHP # nome cnpj plano email algumaCoisa

	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.a addContratante() qbvkeja\n");
		free(cnpj);
		free(plano);
		free(nome);
		free(email);
		email = NULL;
		nome = NULL;
		plano = NULL;
		cnpj = NULL;
		return false;
	}

	if(strlen(token) > TAMANHO_TIPO)
	{
		printf(" Warning: token exageradamente grande recebido em Comando-Adicao.h addContratante() qvboa72\n");
		free(cnpj);
		free(nome);
		free(email);
		free(plano);
		plano = NULL;
		nome = NULL;
		email = NULL;
		cnpj = NULL;
		return false;
	}

	if(strcmp(token, TIPO_TELEFONE) == 0)// APP 2 $C CHAVE_DE_SEGURANCA_PHP # nome cnpj plano email >
	{
		printf(" LOG: Cliente informando telefone em Comando-Adicao.h addContratante() qkejbv\n");
		//TODO Adicionar suporte a senha (chave de seguranca)
		
		token = strtok(NULL, " ");// APP 2 $C CHAVE_DE_SEGURANCA_PHP # nome cnpj plano email > telefone
		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente em addContratante() Comando-Adicao.h qqqokasdn\n");
			free(nome);
			free(email);
			free(cnpj);
			free(plano);
			plano = NULL;
			email = NULL;
			cnpj = NULL;
			nome = NULL;
			return false;
		}

		if(strlen(token) > TAMANHO_TELEFONE)
		{
			printf(" Warning: Telefone informado é exageradamente grande em addContratante() Comando-Adicao.h\n");
			free(nome);
			free(email);
			free(cnpj);
			free(plano);
			plano = NULL;
			cnpj = NULL;
			email = NULL;
			nome = NULL;
			return false;
		}

		char *telefone = NULL;
		telefone = malloc(sizeof(char) * (strlen(token) + 1));
		if(telefone == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para telefone em addContratante() Comando-Adicao.h\n");
			free(nome);
			free(email);
			free(cnpj);
			free(plano);
			plano = NULL;
			cnpj = NULL;
			email = NULL;
			nome = NULL;
			return false;
		}

		strcpy(telefone, token);
		if(telefone == NULL)
		{
			printf(" Warning: Não foi possível copiar de token para telefone em addContratante() Comando-Adicao.h sakjdhsa\n");
			free(email);
			free(plano);
			free(nome);
			free(cnpj);
			cnpj = NULL;
			nome = NULL;
			plano = NULL;
			email = NULL;
			return false;
		}

		token = strtok(NULL, " ");// APP 2 $C CHAVE_DE_SEGURANCA_PHP # nome cnpj plano email > telefone idCidadeInformado
		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente em addContratante() Comando-Adicao.h mankchru\n");
			free(plano);
			free(email);
			free(nome);
			free(cnpj);
			free(telefone);
			telefone = NULL;
			cnpj = NULL;
			nome = NULL;
			email = NULL;
			plano = NULL;
			return false;
		}

		if(strlen(token) > TAMANHO_CHAVE_PRIMARIA)
		{
			printf(" Warning: Chave informada para idLocalizacao possui tamanho exageradamente grande em addContratante() Comando-Adicao.h cqbwjk\n");
			free(cnpj);
			free(email);
			free(nome);
			free(plano);
			free(telefone);
			telefone = NULL;
			plano = NULL;
			nome = NULL;
			email = NULL;
			cnpj = NULL;
			return false;
		}

		char *idLocalizacao = malloc(sizeof(char) * (strlen(token) + 1));
		if(idLocalizacao == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para idLocalizacao em addContratante() Comando-Adicao.h cqbjkcqjk\n");
			free(cnpj);
			free(plano);
			free(nome);
			free(email);
			free(telefone);
			telefone = NULL;
			email = NULL;
			nome = NULL;
			cnpj = NULL;
			plano = NULL;
			return false;
		}

		strcpy(idLocalizacao, token);
		if(idLocalizacao == NULL)
		{
			printf(" Warning: Não foi possivel copiar string de token para idLocalizacao em addContratante() Comando-Adicao.h 5wkb8d\n");
			free(cnpj);
			free(plano);
			free(nome);
			free(email);
			free(telefone);
			telefone = NULL;
			email = NULL;
			nome = NULL;
			plano = NULL;
			cnpj = NULL;
			return false;
		}

		if(strcmp(idLocalizacao, token) != 0)
		{
			printf(" Warning: Copia incorreta de token para idLocalizacao em addContratante() Comando-Adicao.h hwq \n");
			free(cnpj);
			free(plano);
			free(nome);
			free(email);
			free(telefone);
			telefone = NULL;
			email = NULL;
			nome = NULL;
			plano = NULL;
			cnpj = NULL;
			return false;
		}

		if(addContratanteAoBanco(nome, cnpj, plano, email, telefone, idLocalizacao))
		{
			printf(" LOG: Contratante adicionado com sucesso ao banco de dados em addContratante() Comando-Adicao.h qkcnsjbwodcv\n");
			free(telefone);
			free(nome);
			free(email);
			free(plano);
			free(cnpj);
			cnpj = NULL;
			plano = NULL;
			email = NULL;
			nome = NULL;
			telefone = NULL;
			return true;
		}
		else
		{
			printf(" Warning: Falha ao tentar adicionar contratante ao banco de dados em addContratante() Comando-Adicao.h dsahsajkdhsajkdhjksadh\n");
			free(nome);
			free(email);
			free(plano);
			free(cnpj);
			free(telefone);
			telefone = NULL;
			cnpj = NULL;
			plano = NULL;
			email = NULL;
			nome = NULL;
			return false;
		}

	}
	else if(strcmp(token, TIPO_TELEFONE_NULO) == 0)// APP 2 $C CHAVE_DE_SEGURANCA_PHP # nome cnpj plano email >N
	{
		printf(" LOG: Cliente não informou telefone em Comando-Adicao.h addContratante() qkbveasfve\n");
		/*
		// SENHA TODO
		token = strtok(NULL, " ");
		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente em Comando-Adicao.h addContratante() bkjqj\n");
			free(cnpj);
			free(nome);
			free(email);
			free(plano);
			plano = NULL;
			email = NULL;
			cnpj = NULL;
			nome = NULL;
			return false;
		}
		*/

		token = strtok(NULL, " ");// APP 2 $C CHAVE_DE_SEGURANCA_PHP # nome cnpj plano email >N idCidadeInformado
		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente em addContratante() Comando-Adicao.h 13qjcxd \n");
			free(email);
			free(nome);
			free(cnpj);
			free(plano);
			plano = NULL;
			cnpj = NULL;
			nome = NULL;
			email = NULL;
			return false;
		}

		if(strlen(token) > TAMANHO_CHAVE_PRIMARIA)
		{
			printf(" Warning: idLocalizacao informado contem tamanho exageradamente grande em addContratante() Comando-Adicao.h sadnjksadh\n");
			free(email);
			free(nome);
			free(cnpj);
			free(plano);
			plano = NULL;
			cnpj = NULL;
			nome = NULL;
			email = NULL;
			return false;
		}

		char *idLocalizacao;
		idLocalizacao = NULL;
		idLocalizacao = malloc(sizeof(char) * (strlen(token) + 1));
		
		if(idLocalizacao == NULL)
		{
			printf(" Warning: Falha ao alocar memoria para idLocalizacao em addContratante() Comando-Adicao.h sahdjkds \n");
			free(email);
			free(nome);
			free(cnpj);
			free(plano);
			plano = NULL;
			cnpj = NULL;
			nome = NULL;
			email = NULL;
			return false;
		}

		strcpy(idLocalizacao, token);

		if(idLocalizacao == NULL)
		{
			printf(" Warning: Falha ao copiar de token para idLocalizacao em addContratante() Comando-Adicao.h sadhjksad \n");
			free( cnpj);
			free(nome);
			free(email);
			free(plano);
			plano = NULL;
			email = NULL;
			nome = NULL;
			cnpj = NULL;
			return false;
		}

		if(strcmp(token, idLocalizacao) != 0)
		{
			printf(" Warning: Copia incorreta de token para idLocalizacao em addContratante() Comando-Adicao.h 54g8j\n");
			free(email);
			free(cnpj);
			free(nome);
			free(plano);
			free(idLocalizacao);
			idLocalizacao = NULL;
			plano = NULL;
			nome = NULL;
			cnpj = NULL;
			email = NULL;
			return false;
		}

		if(addContratanteAoBanco(nome, cnpj, plano, email, NULL, idLocalizacao))
		{
			printf(" LOG: Contratante adicionada com sucesso ao banco de dados em addContratante() Comando-Adicao.h\n");
			free(nome);
			free(email);
			free(cnpj);
			free(plano);
			free(idLocalizacao);
			idLocalizacao = NULL;
			plano = NULL;
			nome = NULL;
			cnpj = NULL;
			email = NULL;
			return true;
		}
		else
		{
			printf(" Warning: Falha ao adicionar contratante ao banco de dados em addContratante() Comando-Adicao.h\n");
			free(nome);
			free(email);
			free(cnpj);
			free(plano);
			plano = NULL;
			nome = NULL;
			cnpj = NULL;
			email = NULL;
			return false;
		}
		
	}
	else
	{
		printf(" Warning: Comando incorreto em Comando-Adicao.h addContratante() qibeuisdf\n");
		free(plano);
		free(nome);
		free(cnpj);
		free(email);
		email = NULL;
		cnpj = NULL;
		nome = NULL;
		plano = NULL;
		return false;
	}

	free(plano);
	free(nome);
	free(cnpj);
	free(email);
	email = NULL;
	cnpj = NULL;
	nome = NULL;
	plano = NULL;

	printf(" ERRO: Exceção não manipulada em Comando-Adicao.h addContratante() NQJWREGAIUWYYTOBQ\n");
	return false;
}


bool addProduto()//DONE
{
	char *token = NULL;
	token = strtok(NULL, " ");//APP 2 + CHAVE_DE_SEGURANCA_PHP
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em addProduto() Comando-Adicao.h CHAVE: kqcbjdui45\n");
		return false;
	}

	if(strlen(token) != TAMANHO_CHAVE_DE_SEGURANCA_PHP)
	{
		printf(" Warning: Chave de segurança informada incorreta em addProduto() Comando-Adicao.h cjqkkgjrwv \n");
		return false;
	}
	
	if(strcmp(token, CHAVE_DE_SEGURANCA_PHP) != 0)
	{
		printf(" Warning: Chave de segurança informada possui tamanho correto, mas não bate em addProduto() Comando-Adicao.h bkqvej\n");
		return false;
	}

	token = strtok(NULL, " ");// APP 2 + CHAVE_DE_SEGURANCA_PHP ;
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em addProduto() Comando-Adicao.h 1b35 \n");
		return false;
	}

	if(strlen(token) > TAMANHO_TIPO)
	{
		printf(" Warning: Tipo informado em token exageradamente grande em addProduto() Comando-Adicao.h qk5j\n");
		return false;
	}

	if(strcmp(token, TIPO_ID_CONTRATANTE) != 0)
	{
		printf(" Warning: Tipo informado incorreto em addProduto() Comando-Adicao.h vkbn\n");
		return false;
	}

	token = strtok(NULL, " ");// APP 2 * CHAVE_DE_SEGURANCA_PHP ; idContratante
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em addProduto() Comando-Adicao.h 1b34 \n");
		return false;
	}

	if(strlen(token) > TAMANHO_CHAVE_PRIMARIA)
	{
		printf(" Warning: Tamanho de chave primaria informada exageradamente grande em addProduto() Comando-Adicao.h 47dbj\n");
		return false;
	}

	char *idContratante = NULL;
	idContratante = malloc(sizeof(char) * (strlen(token) + 1));

	//Checando id e copia
	if(idContratante == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para idContratante em addProduto() Comando-Adicao.h ekbna\n");
		return false;
	}

	strcpy(idContratante, token);

	if(idContratante == NULL)
	{
		printf(" Warning: falha ao copiar de token para idContratante em addProduto() Comando-Adicao.h b2i5\n");
		return false;
	}
	else if(strlen(idContratante) > TAMANHO_CHAVE_PRIMARIA)
	{
		printf(" Warning: Copia de token para idContratante incorreto em addProduto() Comando-Adicao.h 3124w8\n");
		free(idContratante);
		idContratante = NULL;
		return false;
	}

	if(strcmp(token, idContratante) != 0)
	{
		printf(" Warning: Copia incorreta de variaveis em addProduto() Comando-Adicao.h bvk3\n");
		free(idContratante);
		idContratante = NULL;
		return false;
	}


	token = strtok(NULL, " ");// APP 2 * CHAVE_DE_SEGURANCA_PHP ; idContratante idProduto

	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em addProduto() Comando-Adicao.h cbne4kj\n");
		free(idContratante);
		idContratante = NULL;
		return false;
	}

	if(strlen(token) != TAMANHO_ID_PRODUTO)
	{
		printf(" Warning: Comando informado é exageradamente grande em addProduto() Comando-Adicao.h bk5j2h\n");
		free(idContratante);
		idContratante = NULL;
		return false;
	}

	char *idProduto = NULL;
	idProduto = malloc(sizeof(char) * (strlen(token) + 1));

	if(idProduto == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para idProduto em addProduto() Comando-Adicao.h kwj4\n");
		free(idContratante);
		idContratante = NULL;
		return false;
	}

	strcpy(idProduto, token);

	if(idProduto == NULL)
	{
		printf(" Warning: Falha ao copiar de token para idProduto em addProduto() Comando-Adicao.h 324tg\n");
		free(idContratante);
		idContratante = NULL;
		return false;
	}
	
	//ID PRODUTO SEM CHECAGEM DE SEGURANÇA, POSSIVEL "ERRO" PROPOSITAL
	

	//TODO
	token = strtok(NULL, " ");// APP 2 * CHAVE_DE_SEGURANCA_PHP ; idContratante idProduto duracao

	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em addProduto() Comando-Adicao.h 3jh6\n");
		free(idContratante);
		free(idProduto);
		idProduto = NULL;
		idContratante = NULL;
		return false;
	}

	if(strlen(token) > TAMANHO_DURACAO)
	{
		printf(" Warning: Comando informado exageradamente grande em addProduto() Comando-Adicao.h ehjik\n");
		free(idProduto);
		free(idContratante);
		idContratante = NULL;
		idProduto = NULL;
		return false;
	}

	char *duracao = NULL;
	duracao = malloc(sizeof(char) * (strlen(token) + 1));

	if(duracao == NULL)
	{
		printf(" Warning: Não foi possível alocar memoria para duracao em addProduto() Comando-Adicao.h vbjk\n");
		free(idProduto);
		free(idContratante);
		idContratante = NULL;
		idProduto = NULL;
		return false;
	}
	
	strcpy(duracao, token);
	if(duracao == NULL)
	{
		printf(" Warning: Não foi possivel copiar de token para duracao em addProduto() Comando-Adicao.h sadkjh4\n");
		free(idContratante);
		free(idProduto);
		idProduto = NULL;
		idContratante = NULL;
		return false;
	}

	
	token = strtok(NULL, " ");// APP 2 + CHAVE_DE_SEGURANCA_PHP ; idContratante idProduto duracao nomeProduto
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em addProduto() Comando-Adicao.h\n");
		free(idProduto);
		free(idContratante);
		free(duracao);
		duracao = NULL;
		idContratante = NULL;
		idProduto = NULL;
		return false;
	}

	if(strlen(token) > TAMANHO_NOME)
	{
		printf(" Warning: Tamanho de token informado é exageradamente grande em addProduto() Comando-Adicao.h 5jhk2g\n");
		free(idProduto);
		free(duracao);
		free(idContratante);
		idContratante = NULL;
		duracao = NULL;
		idProduto = NULL;
		return false;
	}

	char *nomeProduto = NULL;
	nomeProduto = malloc(sizeof(char) * (strlen(token) + 1));

	if(nomeProduto == NULL)
	{
		printf(" Warning: não foi possível alocar memoria para nome de produto em addProduto() Comando-Adicao.h 672bc98\n");
		free(idProduto);
		free(idContratante);
		free(duracao);
		duracao = NULL;
		idContratante = NULL;
		idProduto = NULL;
		return false;
	}

	strcpy(nomeProduto, token);

	if(nomeProduto == NULL)
	{
		printf(" Warning: Falha ao copiar de token para nomeProduto em addProduto() Comando-Adicao.h jkehqk57df\n");
		free(idProduto);
		free(idContratante);
		free(duracao);
		duracao = NULL;
		idContratante = NULL;
		idProduto = NULL;
		return false;
	}

	token = strtok(NULL, " ");
	if(token == NULL)
	{
		if(addProdutoAoBanco(idContratante, idProduto, duracao, nomeProduto, NULL))//TODO Adicionar descrição de produto
		{
			printf(" LOG: Produto adicionado com sucesso ao banco de dados em addProdutoAoBanco() Comando-Adicao.h\n");
			free(duracao);
			free(idProduto);
			free(idContratante);
			idContratante = NULL;
			idProduto = NULL;
			duracao = NULL;
			return true;
		}
		else
		{
			printf(" Warning: Não foi possivel adicionar produto ao banco de dadoe em addProduto() Comando-Adicao.h bcejqk\n");
			free(duracao);
			free(idProduto);
			free(idContratante);
			idContratante = NULL;
			idProduto = NULL;
			duracao = NULL;
			return false;
		}
	}
	else
	{
		// Se o usuario informar descricao de produto
		if(strlen(token) > TAMANHO_DESCRICAO_PRODUTO)
		{
			printf(" Warning: Tamanho de descricao informada é exageradamente grande para produto em Comando-Adicao.h addProduto() q45w687vw\n");
			free(idProduto);
			free(idContratante);
			free(duracao);
			duracao = NULL;
			idContratante = NULL;
			idProduto = NULL;
			return false;
		}
		char *descricao = NULL;
		descricao = malloc(sizeof(char) * (strlen(token) + 1));
		if(descricao == NULL)
		{
			printf(" Warning: Não foi possível alocar memoria para descricao em Comando-Adicao.h addProduto() 13121215454q\n");
			free(duracao);
			free(idProduto);
			free(idContratante);
			idContratante = NULL;
			idProduto = NULL;
			duracao = NULL;
			return false;
		}
		strcpy(descricao, token);

		if(descricao == NULL)
		{
			printf(" Warning: Falha ao copiar de token para descricao em Comando-Adicao.h addProduto() q4r84t32saw\n");
			free(duracao);
			free(idProduto);
			free(idContratante);
			idContratante = NULL;
			idProduto = NULL;
			duracao = NULL;
			return false;
		}

		if(addProdutoAoBanco(idContratante, idProduto, duracao, nomeProduto, descricao))//TODO Adicionar descrição de produto
		{
			printf(" LOG: Produto adicionado com sucesso ao banco de dados em addProdutoAoBanco() Comando-Adicao.h\n");
			free(duracao);
			free(idProduto);
			free(idContratante);
			free(descricao);
			descricao = NULL;
			idContratante = NULL;
			idProduto = NULL;
			duracao = NULL;
			return true;
		}
		else
		{
			printf(" Warning: Não foi possivel adicionar produto ao banco de dadoe em addProduto() Comando-Adicao.h bcejqk\n");
			free(duracao);
			free(idProduto);
			free(idContratante);
			free(descricao);
			descricao = NULL;
			idContratante = NULL;
			idProduto = NULL;
			duracao = NULL;
			return false;
		}

		if(descricao != NULL)
		{
			free(descricao);
			descricao = NULL;
			return false;
		}
	}

	//NAO CHECANDO COPIA DE DURACAO, POSSIVEL ERRO

	printf(" Warning: Erro desconhecido em addProduto() Comando-Adicao.h chagsjkchsajk\n");

	if(idProduto != NULL)
	{
		free(idProduto);
		idProduto = NULL;
	}

	if(duracao != NULL)
	{
		free(duracao);
		duracao = NULL;
	}

	if(idContratante != NULL)
	{
		free(idContratante);
		idContratante = NULL;
	}

	return false;
}

bool addInformacoesAUsuario(char *email)// DOING
{
	if(email == NULL)
	{
		printf(" Warning: email não informado em Comando-Adicao.h addInformacoesAUsuario()\n");
		return false;
	}
	
	char *token = strtok(NULL, " ");//APP 2 & tipoInformacao

	if (token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addInformacoesAUsuario() qkjrbbc\n");
		return false;
	}

	if (strlen(token) > TAMANHO_TIPO)
	{
		printf(" Warning: Comando exageradamente grande em Comando-Adicao.h addInformacoesAUsuario() qjkhbht\n");
		return false;
	}

	if (strcmp(token, TIPO_NOME) == 0)// APP 2 & # nomeInformado
	{
		printf(" LOG: cliente solicitando adição de informação: NOME em Comando-Adicao.h addInformacoesAUsuario()\n");
		token = strtok(NULL, " ");
		if (token == NULL)
		{
			printf(" Warning: Comando insuficiente em Comando-Adicao.h addInformacoesAUsuario() 654q898fgf\n");
			return false;
		}
		if(strlen(token) > TAMANHO_NOME)
		{
			printf(" Warning: Comando exageradamente grande em Comando-Adicao.h addInformacoesAUsuario() cjhkjhjekqaweg\n");
			return false;
		}

		char *nome = strdup(token);
		if(nome == NULL)
		{
			printf(" Warning: Falha ao duplicar string em Comando-Adicao.h addInformacoesAUsuario() qkujhjfge\n");
			return false;
		}
		else
		{
			if(addNomeDeUsuarioAoBanco(email, nome))
			{
				printf(" LOG: Informação de nome adicionada com sucesso ao banco de dados em Comando-Adicao.h addInformacoesAUsuario() 546q\n");
				free(nome);
				nome = NULL;
				return true;
			}
			else
			{
				printf(" Warning: Falha ao adicionar nome de usuario ao banco de dados em Comando-Adicao.h addInformacoesAUsuario() qkjhkebr\n");
				free(nome);
				nome = NULL;
				return false;
			}
		}
		return false;	
	}
	else if(strcmp(token, TIPO_SOBRENOME) == 0)// APP 2 & Y sobrenomeInformado
	{
		printf(" LOG: cliente solicitando adição de informação: SOBRENOME em Comando-Adicao.h addInformacoesAUsuario()\n");
		token = strtok(NULL, " ");
		if (token == NULL)
		{
			printf(" Warning: Comando insuficiente em Comando-Adicao.h addInformacoesAUsuario() 654q898fgf\n");
			return false;
		}
		if(strlen(token) > TAMANHO_SOBRENOME)
		{
			printf(" Warning: Comando exageradamente grande em Comando-Adicao.h addInformacoesAUsuario() cjhkjhjekqaweg\n");
			return false;
		}

		char *sobrenome = strdup(token);
		if(sobrenome == NULL)
		{
			printf(" Warning: Falha ao duplicar string em Comando-Adicao.h addInformacoesAUsuario() qkujhjfge\n");
			return false;
		}
		else
		{
			if(addSobrenomeDeUsuarioAoBanco(email, sobrenome))
			{
				printf(" LOG: Informação de nome adicionada com sucesso ao banco de dados em Comando-Adicao.h addInformacoesAUsuario() 546q\n");
				free(sobrenome);
				sobrenome = NULL;
				return true;
			}
			else
			{
				printf(" Warning: Falha ao adicionar nome de usuario ao banco de dados em Comando-Adicao.h addInformacoesAUsuario() qkjhkebr\n");
				free(sobrenome);
				sobrenome = NULL;
				return false;
			}
		}
		return false;	
	}
	else if(strcmp(token, TIPO_SEXO) == 0)// APP 2 & SQ sexoInformado
	{
		printf(" LOG: cliente solicitando adição de informação: SEXO em Comando-Adicao.h addInformacoesAUsuario()\n");
		token = strtok(NULL, " ");
		if (token == NULL)
		{
			printf(" Warning: Comando insuficiente em Comando-Adicao.h addInformacoesAUsuario() 654q898fgf\n");
			return false;
		}
		if(strlen(token) > TAMANHO_SEXO_CLIENTE)
		{
			printf(" Warning: Comando exageradamente grande em Comando-Adicao.h addInformacoesAUsuario() cjhkjhjekqaweg\n");
			return false;
		}

		char *sexo = strdup(token);
		if(sexo == NULL)
		{
			printf(" Warning: Falha ao duplicar string em Comando-Adicao.h addInformacoesAUsuario() qkujhjfge\n");
			return false;
		}
		else
		{
			if(addSexoDeUsuarioAoBanco(email, sexo))
			{
				printf(" LOG: Informação de nome adicionada com sucesso ao banco de dados em Comando-Adicao.h addInformacoesAUsuario() 546q\n");
				free(sexo);
				sexo = NULL;
				return true;
			}
			else
			{
				printf(" Warning: Falha ao adicionar nome de usuario ao banco de dados em Comando-Adicao.h addInformacoesAUsuario() qkjhkebr\n");
				free(sexo);
				sexo = NULL;
				return false;
			}
		}
		return false;	
	}
	else if(strcmp(token, TIPO_DATA) == 0)// APP 2 & T dataInformada
	{
		printf(" LOG: Cliente solicitando adição de data de nascimento a usuario em Comando-Adicao.h addInformacoesAUsuario()\n");
		token = strtok(NULL, " ");
		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente em Comando-Adicao.h addInformacoesAUsuario() 65q48t1y0bsd.\n");
			return false;
		}
		char *dataNascimento = strdup(token);
		if(dataNascimento == NULL)
		{
			printf(" Warning: Falha ao duplicar token para dataNascimento em Comando-Adicao.h addInformacoesAUsuario()\n");
			return false;
		}
		else
		{
			if(addDataNascimentoDeUsuarioAoBanco(email, dataNascimento))
			{
				printf(" LOG: Data de nascimento adicionada com sucesso em Comando-Adicao.h addInformacoesAUsuario()\n");
				free(dataNascimento);
				dataNascimento = NULL;
				return true;
			}
			else
			{
				printf(" Warning: Falha ao adicionar data de nascimento de usuario ao banco de dados em Comano-Adicao.h addInformacoesAUsuario()\n");
				free(dataNascimento);
				dataNascimento = NULL;
				return false;
			}
		}
	}
	else
	{
		printf(" Warning: Tipo de informação não manipulada em Comando-Adicao.h addInformacoesAUsuario() q548g\n");
		return false;
	}

}

bool addAvaliacaoAProduto(char *email)
{
	if (email == NULL)
	{
		printf(" Warning: Email nulo em Comando-Adicao.h addAvaliacaoAProduto()\n");
		return false;
	}
	char *token = strtok(NULL, " ");// APP 2 kW * idProduto

	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addAvaliacaoAProduto() q654t88y77q9f2\n");
		return false;
	}
	if (token[1] != '\0' && token[2] != '\0')
	{
		printf(" Warning: Comando exageradamente grande detectado em Comando-Adicao.h addAvaliacaoAProduto() 46487871121a\n");
		return false;
	}
	if (strcmp(token, TIPO_ID_PRODUTO) != 0)
	{
		printf(" Warning: Comando incorreto detectado em Comando-Adicao.h addAvaliacaoAProduto() 41q54r8bw\n");
		return false;
	}

	token = strtok(NULL, " ");// APP 2 kW * idProduto
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addAvaliacaoAProduto() qkjh4uhuasd\n");
		return false;
	}
	if(strlen(token) == TAMANHO_ID_PRODUTO)
	{
		printf(" Warning: Argumento invalido detectado em Comando-Adicao.h addAvaliacaoAProduto() qkjhewjkvaw\n");
		return false;
	}
	char *idProduto = strdup(token);
	if(idProduto == NULL)
	{
		printf(" Warning: Falha ao duplicar token para idProduto em Comando-Adicao.h addAvaliacaoAProduto() qjwkhjkbve\n");
		return false;
	}

	token = strtok(NULL, " ");
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addAvaliacaoAProduto() jklsadve\n");
		free(idProduto);
		idProduto = NULL;
		return false;
	}
	if (token[1] != '\0')
	{
		printf(" Warning: Comando incorreto em Comando-Adicao.h addAvaliacaoAProduto() ckjergbry\n");
		free(idProduto);
		idProduto = NULL;
		return false;
	}
	if(token[0] != '0' || token[0] != '1')//OTIMIZANDO !!!
	{
		printf(" Warning: Comando incorreto em (argumento invalido) Comando-Adicao.h addAvaliacaoAProduto().\n");
		free(idProduto);
		idProduto = NULL;
		return false;
	}

	char *avaliacao = malloc(sizeof(char) * 2);// o caracter e o \0
	if (avaliacao == NULL)
	{
		printf(" Warning: Falha a alocar memoria para avaliacao em Comando-Adicao.h addAvaliacaoAProduto() c3qw45f89yt87j5sd\n");
		free(idProduto);
		idProduto = NULL;
		return false;
	}
	avaliacao[0] = token[0];
	avaliacao[1] = token[1];

	if(addAvaliacaoAProdutoAoBanco(email, idProduto, avaliacao))
	{
		printf(" LOG: Avaliacao adicionada ao banco com sucesso em Comando-Adicao.h addAvaliacaoAProduto() 46q5w4vb\n");
		free(idProduto);
		idProduto = NULL;
		return true;
	}
	else
	{
		printf(" Warning: Falha ao adicionar avaliação ao banco de dados em Comando-Adicao.h addAvaliacaoAProduto() kjqhjkhve\n");
		free(idProduto);
		idProduto = NULL;
		return false;
	}
}