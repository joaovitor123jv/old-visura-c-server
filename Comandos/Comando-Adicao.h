#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Comandos.h"
#include "../OperacoesBanco/OperacoesBanco.h"
#include "../Bibliotecas/AdaptadorDeString/AdaptadorDeString.h"


int comandoAdicionar(Usuario *usuario);//Retorna true se adicionado com sucesso

bool addVisualizacao(Usuario *usuario);
//bool addVisualizacao(char *email, bool usuarioAnonimo);//Retorna true se adicionado com sucesso
// bool addIndice(char *nomearquivo);

bool addUsuarioAnonimo(Usuario *usuario);//Retorna true se adicionado com sucesso
bool addUsuario(Usuario *usuario);//Retorna true se adicionado com sucesso

bool addContratante(Usuario *usuario);//Retorna true se adicionado com sucesso

bool addProduto(Usuario *usuario);//Retorna true se adicionado com sucesso

bool addCidade(Usuario *usuario);//Retorna true se adicionado com sucesso
bool addNumeroDeHabitantesACidade(Usuario *usuario);//Retorna true se adicionado com sucesso
bool addLocalizacao(Usuario *usuario);//Retorna true se adicionado com sucesso

bool addInformacoesAUsuario(Usuario *usuario);//Retorna true se adicionado com sucesso
bool addAvaliacaoAProduto(Usuario *usuario);//Retorna TRUE se adicionado com sucesso

bool addFeedbackAProduto(Usuario *usuario);//Retorna TRUE se adicionado com sucesso


int comandoAdicionar(Usuario *usuario)/* APP 2 */
{
	if (usuario == NULL)
	{
		geraLog(ERRO, "Usuario nulo detectado");
		return RETORNO_ERRO_INTERNO;
	}
	geraLog(LOG, "****************COMANDO_ADICAO***************");
	usuario_mostrarDados(usuario);

	char *token = usuario_getNextToken(usuario);
	if(token == NULL)
	{
		geraLog(ERRO, "Comando incorreto detectado");
		return RETORNO_ERRO_INTERNO;
	}

	if(strcmp(token, TIPO_VISUALIZACAO) == 0)/* APP 2 2  (Solicitação de adicao de Visualizacao)*/
	{
		geraLog(LOG, "Solicitando adição de visualizacao de produto");

		if(addVisualizacao(usuario))
		{
			geraLog(LOG, "Visualização adicionada com sucesso");
			return RETORNO_OK;
		}
		else
		{
			geraLog(LOG, "Visualização não foi adicionada");
			return RETORNO_ADICAO_RECUSADA;
		}
	}
	else if(strcmp(token, TIPO_AVALIACAO) == 0)/* APP 2 kW * idProduto avaliacaoProduto */
	{
		if(addAvaliacaoAProduto(usuario))
		{
			geraLog(LOG, "Avaliação cadastrada com sucesso");
			return RETORNO_OK;
		}
		else
		{
			geraLog(WARNING, "Falha ao adicionar avaliação ao produto requisitado");
			return RETORNO_ADICAO_RECUSADA;
		}
	}
	else if(strcmp(token, TIPO_FEEDBACK) == 0)// APP 2 tr * idProduto textoDoFeedBack
	{
		if(addFeedbackAProduto(usuario))
		{
			geraLog(LOG, "Feedback adicionado com sucesso ao banco de dados");
			return RETORNO_OK;
		}
		else
		{
			geraLog(WARNING, "Feedback não foi adicionado ao banco de dados");
			return RETORNO_ADICAO_RECUSADA;
		}
	}
	else if(strcmp(token, TIPO_USUARIO) == 0)/* APP 2 1 Solicitação de criacao de usuario */
	{
		geraLog(LOG, "Solicitando adição de usuario");
		token = usuario_getNextToken(usuario);
		if(token == NULL)
		{
			geraLog(WARNING, "Comando incorreto");
			return RETORNO_COMANDO_INCORRETO;
		}
		if(strcmp(token, TIPO_USUARIO_ANONIMO) == 0)/* APP 2 1 2 Solicitação de criação de usuário anônimo */
		{
			geraLog(LOG, "------------>anônimo");
			token = usuario_getNextToken(usuario);
			if(token == NULL)
			{
				geraLog(WARNING, "Comando incorreto");
				return RETORNO_COMANDO_INCORRETO;
			}
			if(strcmp(token, CHAVE_DE_SEGURANCA_PHP) == 0)/* APP 2 1 2 asdkjsahdjksahdjksadkj Solicitação de criação de usuário anônimo, informando chave */
			{
				geraLog(LOG, "Cliente PHP identificado");

				if(addUsuarioAnonimo(usuario))
				{
					geraLog(LOG, "Cliente adicionado com sucesso");
					return RETORNO_OK;
				}
				else
				{
					geraLog(WARNING, "Falha ao tentar adicionar usuario");
					return RETORNO_ADICAO_RECUSADA;
				}

			}
			else
			{
				geraLog(LOG, "Não autorizado");
				return RETORNO_NAO_AUTORIZADO;
			}
		}
		else if( (strcmp(token, TIPO_USUARIO) == 0) && true)/* APP 2 1 1 Solicitação de criação de usuário não-anônimo && usuarioAnonimo */
		{
			geraLog(LOG, "------------>regular");
			if(addUsuario(usuario))
			{
				geraLog(LOG, "Adicao executada com sucesso");
				return RETORNO_OK;
			}
			else
			{
				geraLog(WARNING, "Ocorreram erros e o usuario não pôde ser adicionado");
				return RETORNO_ADICAO_RECUSADA;
			}
		}
		else
		{
			geraLog(LOG, "Usuario não é anonimo ou operação desconhecida");
			return RETORNO_NAO_AUTORIZADO;
		}
	}
	else if(strcmp(token, TIPO_INFORMACAO) == 0)/* APP 2 & TIPO_DE_INFORMACAO */
	{
		geraLog(LOG, "Cliente requisita adicao de informação a usuario");
		if(addInformacoesAUsuario(usuario))
		{	
			geraLog(LOG, "Informações adicionadas com sucesso ao banco");
			return RETORNO_OK;
		}
		else
		{
			geraLog(WARNING, "Falha ao adicionar informações a usuario");
			return RETORNO_ADICAO_RECUSADA;
		}
	}
	else if(strcmp(token, TIPO_PRODUTO) == 0)/* APP 2 +    -> Solicita criação de produto na base de dados */
	{
		geraLog(LOG, "Solicitando a adição de Produto");
		if(addProduto(usuario))
		{
			geraLog(LOG, "Produto adicionado com sucesso");
			return RETORNO_OK;
		}
		else
		{
			geraLog(WARNING, "Não foi possivel adicionar produto ao banco de dados");
			return RETORNO_ADICAO_RECUSADA;
		}
	}
	else if(strcmp(token, TIPO_CIDADE) == 0)/* APP 2 { */
	{
		if(addCidade(usuario))
		{
			geraLog(LOG, "Cidade adicionada com sucesso");
			return RETORNO_OK;
		}
		else
		{
			geraLog(WARNING, "Falha ao adicionar cidade");
			return RETORNO_ADICAO_RECUSADA;
		}
	}
	else if(strcmp(token, TIPO_LOCALIZACAO) == 0)/* APP 2 l0 */
	{
		/* Cliente solicitando adicao de localizacao */
		if(addLocalizacao(usuario))
		{
			geraLog(LOG, "Localizacao adicionada com sucesso");
			return RETORNO_OK;
		}
		else
		{
			geraLog(LOG, "Não foi possivel adicionar localizacao");
			return RETORNO_ADICAO_RECUSADA;
		}
		geraLog(WARNING, "Exceção não manipulada/manipulável");
		return RETORNO_ERRO_INTERNO;
	}
	else if(strcmp(token, TIPO_CONTRATANTE) == 0)//APP 2 $C 
	{
		geraLog(LOG, "Solicitando adição de contratante");
		if(addContratante(usuario))
		{
			geraLog(LOG, "Contratante adicionada com sucesso");
			return RETORNO_OK;
		}
		else
		{
			geraLog(LOG, "Não foi possível adicionar contratante");
			return RETORNO_ADICAO_RECUSADA;
		}
		geraLog(ERRO, "Exceção não manipulada");
		return RETORNO_ERRO_INTERNO;
	}
	else if(strcmp(token, TIPO_QUANTIDADE_DE_HABITANTES_DA_CIDADE) == 0)// APP 2 qC nomeCidade nomeEstado quantidade
	{
		return addNumeroDeHabitantesACidade(usuario);
	}
	else
	{
		geraLog(ERRO, "TIPO DE ADIÇAO DESCONHECIDA");
		return RETORNO_COMANDO_INCORRETO;
	}
	geraLog(ERRO, "Algum erro desconhecido ocorreu");
	return RETORNO_ERRO_INTERNO;
}

bool addUsuarioAnonimo(Usuario *usuario)// APP 2 1 2 asdkhasdjkas
{
	printf("\n*********************addUsuarioAnonimo()********************\n");
	char *token = usuario_getNextToken(usuario);
	if(token == NULL)// APP 2 1 2 sakjdhsa 7
	{
		printf(" Warning: Comando \"Insuficiente\" Comando-Adicao.h addUsuarioAnonimo()\n");
		return false;
	}
	if (stringMaior(token, TAMANHO_TIPO))
	{
		printf(" Warning: Comando exageradamente grande (parametro tipo) detectado em Comando-Adicao.h addUsuarioAnonimo()\n");
		return false;
	}
	if(strcmp(token, TIPO_LOGIN) == 0)// APP 2 1 2 sakdhasjkdn 7
	{
		printf(" LOG: Cliente informando login Comando-Adicao.h addUsuarioAnonimo()\n");
		//APP 2 1 2 askjdhhh1127868c 7 logindousuario
		token = usuario_getNextToken(usuario);
		if(token == NULL)// APP 2 1 2 askdjsakd 7 logindousuario
		{
			printf(" Warning: Comando insuficiente Comando-Adicao.h addUsuarioAnonimo() (1)\n");
			return false;
		}
		printf(" LOG: Alocando memória para email Comando-Adicao.h addUsuarioAnonimo()\n");
		if (stringMaior(token, TAMANHO_LOGIN))
		{
			printf(" LOG: Tamanho de email exageradamente grande Comando-Adicao.h addUsuarioAnonimo()\n");
			return false;
		}
		char *email = strdup(token);
		if(email == NULL)
		{
			printf(" Warning: Falha ao copiar de token para email Comando-Adicao.h addUsuarioAnonimo()\n");
			return false;
		}
		//APP 2 1 2 asdkjhaskd 7 loginDoUsuario 9
		token = usuario_getNextToken(usuario);
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
		//APP 2 1 2 asdhasjdkh 7 logindousuario 9 senhaInformada
		token = usuario_getNextToken(usuario);
		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente Comando-Adicao.h addUsuarioAnonimo() (4)\n");
			free(email);
			email = NULL;
			return false;
		}
		else//Minimiza erros
		{
			if ( ! stringTamanhoIgual(token, TAMANHO_SENHA_PADRAO_USUARIO_ANONIMO))
			{
				printf(" Warning: Senha incorreta Comando-Adicao.h addUsuarioAnonimo() (1)\n");
				free( email );
				email = NULL;
				return false;
			}
			if(strcmp(token, SENHA_PADRAO_USUARIO_ANONIMO) == 0)//se senha informada for igual à senha padrao de usuario anonimo
			{
				printf(" LOG: alocando memoria para senha Comando-Adicao.h addUsuarioAnonimo()\n");
				char *senha = strdup(token);
				if(senha != NULL)
				{
					printf(" LOG: Solicitando adicao de usuario à base de dados Comando-Adicao.h addUsuarioAnonimo()\n");
					if(addUsuarioAnonimoAoBanco(usuario, email))//SUCESSO usuario criado (manda usuario para adição direta no banco)
					{
						printf(" LOG: Usuario cadastrado com sucesso Comando-Adicao.h addUsuarioAnonimo()\n");
						free( email );
						free( senha );
						email = NULL;
						senha = NULL;
						return true;
					}
					else
					{
						printf(" Warning: Fala ao adicionar usuario ao banco de dados Comando-Adicao.h addUsuarioAnonimo()\n");
						free( email );
						free( senha );
						email = NULL;
						senha = NULL;
						return false;
					}
				}
				else
				{
					printf(" ERRO: Não foi possivel alocar memoria para senha Comando-Adicao.h addUsuarioAnonimo()\n");
					free( email );
					email = NULL;
					return false;
				}
			}
			else
			{
				printf(" Warning: Senha padrao de usuario anonimo nao confirmada Comando-Adicao.h addUsuarioAnonimo()\n");
				printf(" \t\t Abortando! (usuario não será criado)\n");
				free( email );
				email = NULL;
				return false;
			}
			printf(" ERRO: Desconhecido em Comando-Adicao.h addUsuarioAnonimo() (2)\n");
			printf(" \t\tAbortando! (usuario não será criado)\n");
			free( email );
			email = NULL;
			return false;//TEMP
		}
		free( email );
		email = NULL;
		printf(" ERRO: Desconhecido em Comando-Adicao.h addUsuarioAnonimo() (1)\n");
		return false;
	}
	else
	{
		printf(" Warning: TIPO não suportado Comando-Adicao.h addUsuarioAnonimo()\n");
		return false;
	}
}

/** 
 * @brief  Obtém os dados da query recebida e redireciona-os para a função que adiciona os dados no banco
 * @note   exemplo de uso:
 * 				APP 2 1 1 7 login 9 senha
 * 				APP 2 1 1 7 login 9 senha sexo datanascimento
 * @param  *usuario: o usuário que está solicitando a adição de um outro usuário
 * @retval true caso seja bem sucedido, false caso contrário
 */
bool addUsuario(Usuario *usuario)//TODO  APP 2 1 1      (done)
{
	if (usuario == NULL)
	{
		geraLog(WARNING, "Usuario nulo detectado");
		return false;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		geraLog(WARNING, "Usuario não conectado detectado");
		return false;
	}

	char *token = NULL;
	// APP 2 1 1 7            (7 == TIPO_LOGIN)
	token = usuario_getNextToken(usuario);

	if(token == NULL)
	{
		geraLog(WARNING, "Comando insuficiente detectado");
		return false;
	}
	else if(strlen(token) > TAMANHO_TIPO)
	{
		geraLog(WARNING, "Comando exageradamente grande detectado");
		printf(" \t TOKEN = |%s|\n", token);
		return false;
	}
	else if(strcmp(token, TIPO_LOGIN) != 0)
	{
		geraLog(WARNING, "Comando incorreto detectado");
		return false;
	}

	// APP 2 1 1 7 emailInformado
	token = usuario_getNextToken(usuario);
	if(token == NULL)
	{
		geraLog(WARNING, "Comando insuficiente detectado");
		return false;
	}
	else if (stringMaior(token, TAMANHO_LOGIN))
	{
		//printf(" Warning: Comando exageradamente grande em Comando-Adicao.h addUsuario() sakjv31sx0\n");
		geraLog(WARNING, "Comando exageradamente grande detectado");
		return false;
	}

	char *email = strdup(token);

	if(email == NULL)
	{
		geraLog(ERRO, "Falha ao duplicar token (email)");
		return false;
	}

	// APP 2 1 1 7 emailInformado 9    (9 == TIPO_SENHA)
	token = usuario_getNextToken(usuario);
	if(token == NULL)
	{
		geraLog(WARNING, "Comando insuficiente (esperando TIPO_SENHA)");
		free( email );
		email = NULL;
		return false;
	}
	else if (stringMaior(token, TAMANHO_TIPO))
	{
		geraLog(WARNING, "Comando exageradamente grande detectado (esperando tipo senha)");
		free( email );
		email = NULL;
		return false;
	}
	else if(strcmp(token, TIPO_SENHA) != 0)
	{
		geraLog(WARNING, "Comando incorreto detectado (esperando tipo senha = 9)");
		free( email );
		email = NULL;
		return false;
	}

	// AINDA NÃO IMPLEMENTADO DESSA FORMA → APP 2 1 1 7 emailInformado 9 senhaCriptografadaAqui     IT's SHOW TIME ! //TODO
	// ALTERNATIVA EM USO → APP 2 1 1 7 emailInformado 9 senhaCruaInformada

	// APP 2 1 1 7 emailInformado 9 senhaCruaInformada
	token = usuario_getNextToken(usuario);

	if(token == NULL)
	{
		geraLog(WARNING, "Comando insuficiente detectado");
		free( email );
		email = NULL;
		return false;
	}
	else if(strlen(token) > TAMANHO_SENHA)
	{
		geraLog(WARNING, "Comando exageradamente grande detectado");
		free( email );
		email = NULL;
		return false;
	}

	char *senha = strdup(token);

	if(senha == NULL)
	{
		geraLog(WARNING, "Falha ao duplicar string (senha)");
		free( email );
		email = NULL;
		return false;
	}

	// APP 2 1 1 7 emailInformado 9 senhaCruaInformada sexoInformado]
	token = usuario_getNextToken(usuario);
	if(token == NULL)
	{
		geraLog(LOG, "Comando de adição de usuario informou somente login e senha");
		if(addUsuarioAoBanco(usuario, email, senha, NULL, NULL))
		{
			geraLog(LOG, "Usuario adicionado com sucesso ao banco de dados");
			usuario_atualizarLogin(usuario, email);
			free( email );
			free( senha );
			email = NULL;
			senha = NULL;
			return true;
		}
		else
		{
			geraLog(WARNING, "Falha ao adicionar usuario ao banco");
			free( email );
			free( senha );
			email = NULL;
			senha = NULL;
			return false;
		}
	}

	if(strlen(token) > TAMANHO_SEXO_CLIENTE)
	{
		geraLog(WARNING, "Comando exageradamente grande detectado (esperando SEXO CLIENTE)");
		free( email );
		free( senha );
		email = NULL;
		senha = NULL;
		return false;
	}

	char *sexo = strdup(token);

	// APP 2 1 1 7 emailInformado 9 senhaCruaInformada sexoInformado dataNascimentoInformada
	token = usuario_getNextToken(usuario);
	if(token == NULL)
	{
		geraLog(LOG, "Comando insuficiente detectado");
		free( email );
		free( senha );
		free( sexo );
		email = NULL;
		senha = NULL;
		senha = NULL;
		return false;
	}

	if(strlen(token) > TAMANHO_DATA)
	{
		geraLog(WARNING, "Comando exageradamente grande detectado");
		free( email );
		free( senha );
		free( sexo );
		email = NULL;
		senha = NULL;
		senha = NULL;
		return false;
	}

	char * dataNascimento = strdup(token);
	if(dataNascimento == NULL)
	{
		geraLog(WARNING, "Falha ao duplicar token para dataNascimento");
		free( email );
		free( senha );
		free( sexo );
		email = NULL;
		senha = NULL;
		senha = NULL;
		return false;
	}

	if(addUsuarioAoBanco(usuario, email, senha, sexo, dataNascimento))
	{
		geraLog(LOG, "Usuario adicionado com sucesso ao banco");
		usuario_atualizarLogin(usuario, email);

		geraLog(LOG, "Executando script RUBY para enviar email de confirmacao WEB");

		int tamanho = usuario_obterTamanhoLogin(usuario) + TAMANHO_CONFIRMADOR_DE_EMAIL_RUBY + 6 + 1;
		char *comando = (char *)malloc(sizeof(char) * tamanho);

		if(comando == NULL)
		{
			geraLog(WARNING, "Falha ao alocar memoria para comando");
			/* Mesmo que não envie confirmação, o usuario foi adicionado *//* TODO: fazer verificação de usuarios não autenticados */
		}
		else
		{
			snprintf(comando, tamanho, "ruby %s %s", CONFIRMADOR_DE_EMAIL_RUBY, email);
			if(comando == NULL)
			{
				printf(" Warning: Falha ao formatar comando em Comando-Adicao.h addUsuario() ahqoif9a3k)\n");
				geraLog(WARNING, "Falha ao formatar comando");
			}
			else
			{
				geraLog(LOG, "Inicializando script RUBY");
				printf(" \t Comando sendo executado: |%s|\n", comando);
				if ( system(comando) == 0)
				{
					geraLog(LOG, "Não ocorreu nenhum erro ao executar a chamada de sistema");
				}
				geraLog(LOG, "Finalizada execucao de script RUBY (NAO USANDO WRAPPER)");
				free( comando );
				comando = NULL;
			}
		}
		free( email );
		free( senha );
		free( sexo );
		free( dataNascimento );
		email = NULL;
		senha = NULL;
		senha = NULL;
		dataNascimento = NULL;
		return true;
	}
	else
	{
		geraLog(WARNING, "Falha ao adicionar usuario ao banco");
		free( email );
		free( senha );
		free( sexo );
		free( dataNascimento );
		email = NULL;
		senha = NULL;
		senha = NULL;
		dataNascimento = NULL;
		return false;		
	}

	geraLog(ERRO, "Deu ruim, erro desconhecido e teoricamente impossivel");
	return false;
}


//bool addVisualizacao(char *email, bool usuarioAnonimo)/* APP 2 2 idProduto quantidade*/
bool addVisualizacao(Usuario *usuario)
{
	char *token;
	if( !usuarioValido(usuario, "Comando-Adicao.h addVisualizacao()") )
	{
		return false;
	}

	if(!conexaoAtiva())
	{
		geraLog(ERRO, "Conexao nula com o banco de dados");
		return false;
	}

	geraLog(LOG,"*************************COMANDO ADICAO*************************" );

	// APP 2 2 idProduto
	token = usuario_getNextToken(usuario);

	if(token == NULL)
	{
		geraLog(ERRO, "Comando insuficiente (esperado idProduto)");
		return false;
	}

	if ( ! stringTamanhoIgual(token, TAMANHO_ID_PRODUTO))// APP 2 2 idProduto
	{
		geraLog(WARNING, "Argumento contém quantidade invalida de caracteres (esperando 10)");
		return false;
	}

	char *id = strdup(token);

	if(id == NULL)
	{
		geraLog(ERRO, "Falha ao duplicar idProduto informado");
		return false;
	}

	if(!checarIdProduto(usuario, id))
	{
		geraLog(ERRO, "Produto não existe na base de dados");
		free( id );
		id = NULL;
		return false;
	}

	// APP 2 2 idProduto quantidade
	token = usuario_getNextToken(usuario);
	if(token == NULL)/* APP 2 2 idProduto quantidade */
	{
		geraLog(ERRO, "Comando incompleto (esperado quantidade)");
		free( id );
		id = NULL;
		return false;
	}

	char *quantidade = strdup(token);

	if(quantidade == NULL)
	{
		geraLog(WARNING, "Falha ao duplicar token para quantidade");
		free( id );
		id = NULL;
		return false;
	}

	//CONDIÇÕES PARA GERAR LOG, SOMENTE
	if(!addVisualizacoesAoBanco(id, quantidade, usuario))
	{
		geraLog(ERRO, "Falha ao adicionar visualização à base de dados");
		free( id );
		free( quantidade );
		id = NULL;
		quantidade = NULL;
		return false;
	}
	else
	{
		geraLog(LOG, "Dados atualizados no banco");
		free( id );
		free( quantidade );
		id = NULL;
		quantidade = NULL;
		return true;
	}
}

bool addCidade(Usuario *usuario)/* APP 2 { */
{
	char *token = NULL;
	/* APP 2 { nomeDoEstado */
	token = usuario_getNextToken(usuario);
	if(token == NULL)
	{
		geraLog(WARNING, "Comando insuficiente detectado");
		return false;
	}
	if(strlen(token) != 2)/* APP 2 { nomeDoEstado */
	{
		geraLog(WARNING, "Tamanho de estado incorreto");
		return false;
	}
	char *nomeDoEstado = (char *)malloc(sizeof(char) * (strlen(token) + 1));
	if(nomeDoEstado == NULL)
	{
		printf(" Warning: Falha ao alocar memoria para nome do estado em Comando-Adicao.h addCidade()\n");
		geraLog(WARNING, "Falha ao alocar memoria para nome do estado");
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
	/* APP 2 { nomeDoEstado nomeDaCidade */
	token = usuario_getNextToken(usuario);
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
	nomeCidade = (char *)malloc(sizeof(char) * (strlen(token) + 1));
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
		free( nomeCidade );
		nomeCidade = NULL;
		return false;
	}

	if(!addCidadeAoBanco(usuario, nomeDoEstado, nomeCidade))
	{
		printf(" Warning: Falha ao adicionar cidade ao banco de dados Comando-Adicao.h addCidade()\n");
		free( nomeCidade );
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


bool addLocalizacao(Usuario *usuario)/* APP 2 l0 */
{
	char *token = usuario_getNextToken(usuario);// APP 2 l0 idCidade
	if (token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addLocalizacao() eiuhbase\n");
		return false;
	}
	if (strlen(token) > TAMANHO_CHAVE_PRIMARIA)
	{
		printf(" Warning: Comando exageradamente grande em Comando-Adicao.h addLocalizacao() asb89as3\n");
		return false;
	}
	char *idCidade = strdup(token);
	if (idCidade == NULL)
	{
		printf(" Warning: falha ao copiar de token para idCidade em Comando-Adicao.h addLocalizacao() qibuwey5\n");
		return false;
	}
	// APP 2 l0 idCidade cep
	token = usuario_getNextToken(usuario);
	if (token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addLocalizacao() viubrusdh6wer5\n");
		free( idCidade );
		idCidade = NULL;
		return false;
	}
	if (strlen(token) != TAMANHO_CEP)
	{
		printf(" Warning: Cliente passou algo que não é o CEP em Comando-Adicao.h addLocalizacao() dsajhbveasd\n");
		free( idCidade );
		idCidade = NULL;
		return false;
	}
	char *cep = strdup(token);
	if (cep == NULL)
	{
		printf(" Warning: Falha ao duplicar cep para token em Comando-Adicao.h addLocalizacao() ivubudg\n");
		free( idCidade );
		idCidade = NULL;
		return false;
	}

	// APP 2 l0 idCidade cep bairro
	token = usuario_getNextToken(usuario);
	if (token == NULL)// Informando idCidade e CEP
	{
		printf(" LOG: Cliente só informou idCidade e CEP em Comando-Adicao.h addLocalizacao() aiuhgu tsdf\n");
		if (addLocalizacaoAoBanco(usuario, idCidade, cep, NULL, NULL, NULL, NULL))
		{
			printf(" LOG: localizacao adicionada ao banco de dados com sucesso em Comando-Adicao.h addLocalizacao() ekjhgbdsf\n");
			free( idCidade );
			free( cep );
			idCidade = NULL;
			cep = NULL;
			return true;
		}
		else
		{
			printf(" Warning: Ocorreu ao menos um erro ao tentar adicionar localizacao ao banco de dados em Comando-Adicao.h addLocalizacao() akjghjvksdf\n");
			free( idCidade );
			free( cep );
			idCidade = NULL;
			cep = NULL;
			return false;
		}
	}
	else
	{
		if (strlen(token) > TAMANHO_BAIRRO)
		{
			printf(" Warning: Tamanho de bairro informado é exageradamente grande em Comando-Adicao.h addLocalizacao() dsakjburiasd\n");
			free( idCidade );
			free( cep );
			idCidade = NULL;
			cep = NULL;
			return false;
		}
		char *bairro = strdup(token);
		if (bairro == NULL)
		{
			printf(" Warning: Falha ao duplicar bairro em Comando-Adicao.h addLocalizacao() askjhbv rusidfsd\n");
			free( idCidade );
			free( cep );
			idCidade = NULL;
			cep = NULL;
			return false;
		}

		// APP 2 l0 idCidade cep bairro rua
		token = usuario_getNextToken(usuario);
		if (token == NULL)// Informando idCidade, CEP e bairro
		{
			printf(" LOG: Cliente informou idCidade cep e bairro em Comando-Adicao.h addLocalizacao() ivubursfth\n");
			if (addLocalizacaoAoBanco(usuario, idCidade, cep, bairro, NULL, NULL, NULL))
			{
				printf(" LOG: localizacao adicionada ao banco de dados com sucesso em Comando-Adicao.h addLocalizacao() ekjhgbdsf\n");
				free( idCidade );
				free( cep );
				free( bairro );
				idCidade = NULL;
				cep = NULL;
				bairro = NULL;
				return true;
			}
			else
			{
				printf(" Warning: Falha ao adicionar localizacao ao banco em Comando-Adicao.h addLocalizacao() adkjhgbhjirbasd\n");
				free( idCidade );
				free( cep );
				free( bairro );
				idCidade = NULL;
				cep = NULL;
				bairro = NULL;
				return false;
			}
		}
		else
		{
			if (strlen(token) > TAMANHO_RUA)
			{
				printf(" Warning: Tamanho da rua informada é exageradamente grande em Comando-Adicao.h addLocalizacao() askjbvrkasd\n");
				free( idCidade );
				free( cep );
				free( bairro );
				idCidade = NULL;
				cep = NULL;
				bairro = NULL;
				return false;
			}
			char *rua = strdup(token);
			if (rua == NULL)
			{
				printf(" Warning: Falha ao duplicar rua em Comando-Adicao.h addLocalizacao() sakjvbehsads\n");
				free( idCidade );
				free( cep );
				free( bairro );
				idCidade = NULL;
				cep = NULL;
				bairro = NULL;
				return false;
			}

			// APP 2 l0 idCidade cep bairro rua numero
			token = usuario_getNextToken(usuario);
			if (token == NULL)// Se informar idCidade, cep, bairro e rua
			{
				printf(" LOG: Cliente informou idCidade cep, bairro e rua em Comando-Adicao.h addLocalizacao() askbvjiuisdfg\n");
				if (addLocalizacaoAoBanco(usuario, idCidade, cep, bairro, rua, NULL, NULL))
				{
					printf(" LOG: Localizacao adicionada ao banco de dados com sucesso em Comando-Adicao.h addLocalizacao() sauvgbuirvsdf\n");
					free( idCidade );
					free( cep );
					free( bairro );
					free( rua );
					idCidade = NULL;
					cep = NULL;
					bairro = NULL;
					rua = NULL;
					return true;
				}
				else
				{
					printf(" Warning: Falha ao adicionar localizacao em Comando-Adicao.h addLocalizacao() ewuinuisf\n");
					free( idCidade );
					free( cep );
					free( bairro );
					free( rua );
					idCidade = NULL;
					cep = NULL;
					bairro = NULL;
					rua = NULL;
					return false;
				}
			}
			else
			{
				if (strlen(token) > TAMANHO_NUMERO)
				{
					printf(" Warning: Comando exageradamente grande em Comando-Adicao.h addLocalizacao() askjbvruisdgjh589sad\n");
					free( idCidade );
					free( cep );
					free( bairro );
					free( rua );
					idCidade = NULL;
					cep = NULL;
					bairro = NULL;
					rua = NULL;
					return false;
				}
				char *numero = strdup(token);
				if (numero == NULL)
				{
					printf(" Warning: Falha ao duplicar numero em Comando-Adicao.h addLocalizacao() aksjb uiasdgr\n");
					free( idCidade );
					free( cep );
					free( bairro );
					free( rua );
					idCidade = NULL;
					cep = NULL;
					bairro = NULL;
					rua = NULL;
					return false;
				}

				// APP 2 l0 idCidade cep bairro rua numero complemento
				token = usuario_getNextToken(usuario);
				if (token == NULL)// Se informar idCidade, cep, bairro, rua e numero
				{
					printf(" LOG: Cliente informou idCidade, cep, bairro, rua e numero em Comando-Adicao.h addLocalizacao() kjubrui9dfjt\n");
					if (addLocalizacaoAoBanco(usuario, idCidade, cep, bairro, rua, numero, NULL))
					{
						printf(" LOG: Localizacao adicionada com sucesso ao banco de dados em Comando-Adicao.h addLocalizacao() askjbvruyiasxfrgb\n");
						free( idCidade );
						free( cep );
						free( bairro );
						free( rua );
						free( numero );
						idCidade = NULL;
						cep = NULL;
						bairro = NULL;
						rua = NULL;
						numero = NULL;
						return true;
					}
					else
					{
						printf(" Warning:Falha ao adicionar localizacao em Comando-Adicao.h addLocalizacao() askjbvruyiasxfrgb\n");
						free( idCidade );
						free( cep );
						free( bairro );
						free( rua );
						free( numero );
						idCidade = NULL;
						cep = NULL;
						bairro = NULL;
						rua = NULL;
						numero = NULL;
						return false;
					}
				}
				else// Se informar idCidade, cep, bairro, rua, numero e complemento
				{
					printf(" LOG: Cliente informando todos os dados em Comando-Adicao.h addLocalizacao() dkjghbuiuisdf\n");
					if (strlen(token) > TAMANHO_COMPLEMENTO)
					{
						printf(" Warning: Comando exageradamente grande em Comando-Adicao.h addLocalizacao() askjvnruisdbtb165 \n");
						free( idCidade );
						free( cep );
						free( bairro );
						free( rua );
						free( numero );
						idCidade = NULL;
						cep = NULL;
						bairro = NULL;
						rua = NULL;
						numero = NULL;
						return false;
					}
					char *complemento = strdup(token);
					if (complemento == NULL)
					{
						printf(" Warning: Falha ao duplicar complemento em Comando-Adicao.h addLocalizacao() asjhvjdkeh892df\n");
						free( idCidade );
						free( cep );
						free( bairro );
						free( rua );
						free( numero );
						idCidade = NULL;
						cep = NULL;
						bairro = NULL;
						rua = NULL;
						numero = NULL;
						return false;
					}

					if (addLocalizacaoAoBanco(usuario, idCidade, cep, bairro, rua, numero, complemento))
					{
						printf(" LOG: Localizacao adicionada com sucesso ao banco de dados em Comando-Adicao.h addLocalizacao() askjbvru8sd784sdf\n");
						free( idCidade );
						free( cep );
						free( bairro );
						free( rua );
						free( numero );
						free( complemento );
						idCidade = NULL;
						cep = NULL;
						bairro = NULL;
						rua = NULL;
						numero = NULL;
						complemento = NULL;
						return true;
					}
					else
					{
						printf(" Warning: Falha ao adicionar localizacao ao banco de dados em Comando-Adicao.h addLocalizacao() asbvuei872df\n");
						free( idCidade );
						free( cep );
						free( bairro );
						free( rua );
						free( numero );
						free( complemento );
						idCidade = NULL;
						cep = NULL;
						bairro = NULL;
						rua = NULL;
						numero = NULL;
						complemento = NULL;
						return false;
					}
				}
			}
		}
	}
}


bool addContratante(Usuario *usuario)// APP 2C $C CHAVE_DE_SEGURANCA_PHP nome cnpj plano email senha idLocalizacao telefone
{
	printf("\t********************ADICAO DE CONTRATANTE***************************\n");
	char *token;
	// APP 2 $C CHAVE_DE_SEGURANCA_PHP
	token = usuario_getNextToken(usuario);
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

	// APP 2 $C CHAVE_DE_SEGURANCA_PHP nome
	token = usuario_getNextToken(usuario);
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
	//nome = malloc(sizeof(char) * (strlen(token) +1));
	nome = strdup(token);
	if( nome == NULL )
	{
		printf(" Warning: Falha ao duplicar de token para nome em Comando-Adicao.h addContratante() sahdjkasvbeu\n");
		return false;
	}

	// APP 2 $C CHAVE_DE_SEGURANCA_PHP nome cnpj
	token = usuario_getNextToken(usuario);
	if( token == NULL )
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addContratante() abvria093sd\n");
		free( nome );
		nome = NULL;
		return false;
	}

	char *cnpj;
	//cnpj = malloc(sizeof(char) * (TAMANHO_CNPJ + 1));
	cnpj = strdup(token);
	if(cnpj == NULL)
	{
		printf(" Warning: erro ao copiar cnpj em Comando-Adicao.h addContratante() qjhkc\n");
		free( nome );
		nome = NULL;
		return false;
	}

	// APP 2 $C CHAVE_DE_SEGURANCA_PHP nome cnpj plano
	token = usuario_getNextToken(usuario);
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addContratante() qkjhbvq\n");
		free( cnpj );
		free( nome );
		nome = NULL;
		cnpj = NULL;
		return false;
	}
	if(strlen(token) > TAMANHO_PLANO)
	{
		printf(" Warning: Comando incorreto em Comando-Adicao.h addContratante() qkbvqox\n");
		free( cnpj );
		free( nome );
		nome = NULL;
		cnpj = NULL;
		return false;
	}
	char *plano;
	//plano = malloc(sizeof(char) * (strlen(token) + 1));
	plano = strdup(token);
	if(plano == NULL)
	{
		printf(" Warning: falha ao copiar de token para plano em Comando-Adicao.h addContratante() qkbccw\n");
		free(cnpj);
		free(nome);
		nome = NULL;
		cnpj = NULL;
		return false;
	}

	// APP 2 $C CHAVE_DE_SEGURANCA_PHP nome cnpj plano email
	token = usuario_getNextToken(usuario);
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addContratante() bcqkj\n");
		free(cnpj);
		free(nome);
		free(plano);
		cnpj = NULL;
		nome = NULL;
		plano = NULL;
		return false;
	}

	if(strlen(token) > TAMANHO_LOGIN)
	{
		printf(" Warning: token recebido é exageradamente grande em Comando-Adicao.h addContratante() kb1ius\n");
		free(cnpj);
		free(nome);
		free(plano);
		cnpj = NULL;
		nome = NULL;
		plano = NULL;
		return false;
	}

	char *email;
	//email = malloc(sizeof(char) * (strlen(token) + 1));
	email = strdup(token);

	if(email == NULL)
	{
		printf(" Warning: Cópia mal-sucedida em Comando-Adicao.h addContratante() qkbveas\n");
		free(nome);
		free(plano);
		free(cnpj);
		nome = NULL;
		plano = NULL;
		cnpj = NULL;
		return false;
	}

	// APP 2 $C CHAVE_DE_SEGURANCA_PHP nome cnpj plano email senha
	token = usuario_getNextToken(usuario);

	if(token == NULL)
	{
		printf(" Warning: Cliente não informando senha em Comando-Adicao.h addContratante() bvkauqofs\n");
		free(cnpj);
		free(plano);
		free(nome);
		free(email);
		cnpj = NULL;
		plano = NULL;
		nome = NULL;
		email = NULL;
		return false;
	}
	if (strlen(token) > TAMANHO_SENHA)
	{
		printf(" Warning: senha informada tem tamanho exageradamente grande em Comando-Adicao.h addContratante() akjvebuiasdwn");
		free(cnpj);
		free(plano);
		free(nome);
		free(email);
		cnpj = NULL;
		plano = NULL;
		nome = NULL;
		email = NULL;
		return false;
	}

	char *senha = strdup(token);
	if(senha == NULL)
	{
		printf(" Warning: Falha ao duplicar senha em Comando-Adicao.h addContratante() qoifoaisdj\n");
		free(cnpj);
		free(plano);
		free(nome);
		free(email);
		cnpj = NULL;
		plano = NULL;
		nome = NULL;
		email = NULL;
		return false;
	}

	// APP 2C $C CHAVE_DE_SEGURANCA_PHP nome cnpj plano email senha idLocalizacao
	token = usuario_getNextToken(usuario);
	if(token == NULL)
	{
		printf(" Warning: cliente não informou localizacao em Comando-Adicao.h addContratante() sakjcbjrkbjkasd\n");
		free(cnpj);
		free(plano);
		free(nome);
		free(senha);
		free(email);
		cnpj = NULL;
		plano = NULL;
		nome = NULL;
		senha = NULL;
		email = NULL;
		return false;
	}

	if (strlen(token) > 11)// 11 é o tamanho de IDs de "int" no banco de dados
	{
		printf(" Warning: ID de Localizacao recebido é exageradamente grande em Comando-Adicao.h addContratante() asdfbjvkrlasd\n");
		free(cnpj);
		free(plano);
		free(nome);
		free(senha);
		free(email);
		cnpj = NULL;
		plano = NULL;
		nome = NULL;
		senha = NULL;
		email = NULL;
		return false;
	}

	char *idLocalizacao = strdup(token);

	if( idLocalizacao == NULL )
	{
		printf(" Warning: Falha ao duplicar ID de Localizacao em Comando-Adicao.h addContratante() sajkdhjkrenubasd\n");
		free(cnpj);
		free(plano);
		free(nome);
		free(senha);
		free(email);
		email = NULL;
		nome = NULL;
		senha = NULL;
		plano = NULL;
		cnpj = NULL;
		return false;
	}

	// APP 2C $C CHAVE_DE_SEGURANCA_PHP nome cnpj plano email senha idLocalizacao telefone
	token = usuario_getNextToken(usuario);

	if(token == NULL)
	{
		printf(" Warning: Cliente não informando telefone em Comando-Adicao.a addContratante() qbvkeja\n");
		free(cnpj);
		free(plano);
		free(nome);
		free(senha);
		free(email);
		free( idLocalizacao );
		idLocalizacao = NULL;
		email = NULL;
		nome = NULL;
		senha = NULL;
		plano = NULL;
		cnpj = NULL;
		return false;
	}
	else
	{
		if(addContratanteAoBanco(usuario, nome, cnpj, plano, email, NULL, idLocalizacao, senha))// Se der tudo certo ao tentar adicionar contratante ao banco de dados
		{
			printf(" LOG: Contratante adicionado com sucesso ao banco de dados em addContratante() Comando-Adicao.h qasdjiwuygh48sd987\n");
			free(cnpj);
			free(nome);
			free(email);
			free(plano);
			free(senha);
			free( idLocalizacao );
			idLocalizacao = NULL;
			senha = NULL;
			plano = NULL;
			email = NULL;
			nome = NULL;
			cnpj = NULL;
			return true;
		}
		else
		{
			printf(" Warning: Falha ao adicionar contratante ao banco de dados em Comando-Adicao.h addContratante() asbnveioasd\n");
			free(cnpj);
			free(plano);
			free(nome);
			free(senha);
			free(email);
			free( idLocalizacao );
			idLocalizacao = NULL;
			email = NULL;
			nome = NULL;
			senha = NULL;
			plano = NULL;
			cnpj = NULL;
			return false;
		}
	}

	if(strlen(token) > TAMANHO_TELEFONE)
	{
		printf(" Warning: token exageradamente grande recebido em Comando-Adicao.h addContratante() qvboa72\n");
		free(cnpj);
		free(nome);
		free(email);
		free(plano);
		free(senha);
		free( idLocalizacao );
		idLocalizacao = NULL;
		senha = NULL;
		plano = NULL;
		nome = NULL;
		email = NULL;
		cnpj = NULL;
		return false;
	}

	char *telefone = strdup(token);

	if(telefone == NULL)
	{
		printf(" Warning: Falha ao duplicar telefone em Comando-Adicao.h addContratante() anv4ias0\n");
		free(cnpj);
		free(nome);
		free(email);
		free(plano);
		free(senha);
		free( idLocalizacao );
		idLocalizacao = NULL;
		senha = NULL;
		plano = NULL;
		email = NULL;
		nome = NULL;
		cnpj = NULL;
		return false;
	}

	if(addContratanteAoBanco(usuario, nome, cnpj, plano, email, telefone, idLocalizacao, senha))// Se der tudo certo ao tentar adicionar contratante ao banco de dados
	{
		printf(" LOG: Contratante adicionado com sucesso ao banco de dados em addContratante() Comando-Adicao.h qkcnsjbwodcv\n");
		free(cnpj);
		free(nome);
		free(email);
		free(plano);
		free(senha);
		free( idLocalizacao );
		free( telefone );
		telefone = NULL;
		idLocalizacao = NULL;
		senha = NULL;
		plano = NULL;
		email = NULL;
		nome = NULL;
		cnpj = NULL;
		return true;
	}
	else
	{
		printf(" Warning: Falha ao tentar adicionar contratante ao banco de dados em addContratante() Comando-Adicao.h dsahsajkdhsajkdhjksadh\n");
		free(cnpj);
		free(nome);
		free(email);
		free(plano);
		free(senha);
		free( idLocalizacao );
		free( telefone );
		telefone = NULL;
		idLocalizacao = NULL;
		senha = NULL;
		plano = NULL;
		email = NULL;
		nome = NULL;
		cnpj = NULL;
		return false;
	}

	//Caso dê um erro TOTALMENTE imprevisível

	if( cnpj != NULL )
	{
		free( cnpj );
		cnpj = NULL;
	}

	if( telefone != NULL )
	{
		free( telefone );
		telefone = NULL;
	}

	if( idLocalizacao != NULL )
	{
		free( idLocalizacao );
		idLocalizacao = NULL;
	}
	if( senha != NULL )
	{
		free(senha);
		senha = NULL;
	}
	if( plano != NULL )
	{
		free(plano);
		plano = NULL;
	}
	if( nome != NULL )
	{
		free(nome);
		nome = NULL;
	}
	if( email != NULL )
	{
		free(email);
		email = NULL;
	}

	printf(" ERRO: Exceção não manipulada em Comando-Adicao.h addContratante() NQJWREGAIUWYYTOBQ\n");
	return false;
}

char *obterIdContratanteDoBancoPorUsuario(Usuario *usuario);

bool addProduto(Usuario *usuario)//DONE   "APP 2 + "
{
	if (usuario == NULL)
	{
		printf(" Warning: Usuario nulo detectado em Comando-Adicao.h addProduto()\n");
		return false;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" Warning: Usuario não conectado detectado em Comando-Adicao.h addProduto()\n");
		return false;
	}
	char *descricao = NULL;
	char *tipoProduto = NULL;
	char *nomeProduto = NULL;
	char *duracao = NULL;
	char *idProduto = NULL;
	char *idContratante = NULL;
	char *token = NULL;
	char *categoria = NULL;// Acabar de implementar

	if (usuario_PermissaoContratante(usuario))
	{
		// APP 2 + idProduto
		token = usuario_getNextToken(usuario);
		if (token == NULL)
		{
			printf(" Warning: Comando insuficiente em Comando-Adicao.h addProduto() asvnr89asd9\n");
			return false;
		}
		if( !stringTamanhoIgual(token, TAMANHO_ID_PRODUTO) )
		{
			printf(" Warning: Tamanho de id de produto insuficiente em Comando-Adicao.h addProduto() dnioriasd\n");
			return false;
		}
		idProduto = strdup(token);
		if (idProduto == NULL)
		{
			printf(" ERRO: Falha ao duplicar string de idProduto em Comando-Adicao.h addProduto() asibb8r9asd\n");
			return false;
		}
		// APP 2 + idProduto duracao
		token = usuario_getNextToken(usuario);
		if (token == NULL)
		{
			printf(" Warning: Comando insuficiente detectado em Comando-Adicao.h addProduto() aoi8hbv89a78sd\n");
			free( idProduto );
			idProduto = NULL;
			return false;
		}
		if (stringMaior(token, TAMANHO_DURACAO))
		{
			printf(" Warning: Comando exageradamente grande detectado em Comando-Adicao.h addProduto() ashbvr98as7d3\n");
			free( idProduto );
			idProduto = NULL;
			return false;
		}
		duracao = strdup(token);
		if (duracao == NULL)
		{
			printf(" Warning: Falha ao duplicar duracao em Comando-Adicao.h addProduto()\n");
			free( idProduto );
			idProduto = NULL;
			return false;
		}
		// APP 2 + idProduto duracao nomeProduto
		token = usuario_getNextToken(usuario);
		if (token == NULL)
		{
			printf(" Warning: Comando insuficiente em Comando-Adicao.h addProduto() sahvuroiuyasd\n");
			free( idProduto );
			free( duracao );
			idProduto = NULL;
			duracao = NULL;
			return false;
		}
		if (stringMaior(token, TAMANHO_NOME_PRODUTO))
		{
			printf(" Warning: Tamanho de nome de produto exageradamente grande em Comando-Adicao.h addProduto() akbh7r98a7sd\n");
			free( idProduto );
			free( duracao );
			idProduto = NULL;
			duracao = NULL;
			return false;
		}
		nomeProduto = strdup(token);
		if (nomeProduto == NULL)
		{
			printf(" Warning: falha ao duplicar nome de produto em Comando-Adicao.h addProduto() ahvr89a7sr\n");
			free( idProduto );
			free( duracao );
			idProduto = NULL;
			duracao = NULL;
			return false;
		}
		// APP 2 + idProduto duracao nomeProduto tipoProduto
		token = usuario_getNextToken(usuario);
		if (token == NULL)
		{
			printf(" Warning: Comando insuficiente detectado em Comando-Adicao.h addProduto() averuiuasy\n");
			free( idProduto );
			free( duracao );
			free( nomeProduto );
			idProduto = NULL;
			duracao = NULL;
			nomeProduto = NULL;
			return false;
		}
		if (stringMaior(token, TAMANHO_TIPO_PRODUTO))
		{
			printf(" Warning: Tamanho do tipo de produto é exageradamente grande em Comando-Adicao.h addProduto() a09878dar\n");
			free( idProduto );
			free( duracao );
			free( nomeProduto );
			idProduto = NULL;
			duracao = NULL;
			nomeProduto = NULL;
			return false;
		}
		tipoProduto = strdup(token);
		if (tipoProduto == NULL)
		{
			printf(" Warning: Falha ao duplicar tipo de produto em Comando-Adicao.h addProduto() asioubv879asd4\n");
			free( idProduto );
			free( duracao );
			free( nomeProduto );
			idProduto = NULL;
			duracao = NULL;
			nomeProduto = NULL;
			return false;
		}
		idContratante = strdup(usuario_obterId(usuario));// Estava liberando o id o usuario direto... o que é bizarro... mas ok
		if (idContratante == NULL)
		{
			printf(" Warning: Falha ao obter id de contratante do banco de dados em Comando-Adicao.h addProduto() askbgvr8u9asud\n");
			free( idProduto );
			free( duracao );
			free( nomeProduto );
			free( tipoProduto );
			idProduto = NULL;
			duracao = NULL;
			nomeProduto = NULL;
			tipoProduto = NULL;
			return false;
		}
		// APP 2 + idProduto duracao nomeProduto tipoProduto categoriaProduto
		token = usuario_getNextToken(usuario);
		if (token == NULL)// Se o cliente não informar categoria de produto
		{
			printf(" LOG: Usuario nao informou Categoria de produto em Comando-Adicao.h addProduto()\n");
			if ( addProdutoAoBanco(usuario, idContratante, idProduto, duracao, nomeProduto, NULL, tipoProduto, NULL) )
			{
				printf(" LOG: Produto adicionado ao banco de dados com sucesso em Comando-Adicao.h addProduto() asdjhfsf\n");
				free( idProduto );
				free( duracao );
				free( nomeProduto );
				free( tipoProduto );
				free( idContratante );
				idProduto = NULL;
				duracao = NULL;
				nomeProduto = NULL;
				tipoProduto = NULL;
				idContratante = NULL;
				return true;
			}
			else
			{
				printf(" Warning: Falha ao adicionar produto ao banco de dados em Comando-Adicao.h addProduto() askjhsdsa\n");
				free( idProduto );
				free( duracao );
				free( nomeProduto );
				free( tipoProduto );
				free( idContratante );
				idProduto = NULL;
				duracao = NULL;
				nomeProduto = NULL;
				tipoProduto = NULL;
				idContratante = NULL;
				return false;
			}

		}
		else
		{
			printf(" LOG: Cliente informando categoria de produto em Comando-Adicao.h addProduto() sakdjhsajkr\n");
			if (stringMaior(token, TAMANHO_CATEGORIA))
			{
				printf(" Warning: Categoria de tamanho exageradamente grande em Comando-Adicao.h addProduto() askjhguias\n");
				free( idProduto );
				free( duracao );
				free( nomeProduto );
				free( tipoProduto );
				free( idContratante );
				idProduto = NULL;
				duracao = NULL;
				nomeProduto = NULL;
				tipoProduto = NULL;
				idContratante = NULL;
				return false;
			}
			categoria = strdup(token);
			if (categoria == NULL)
			{
				printf(" Warning: Falha ao duplicar categoria em Comando-Adicao.h addProduto() sajobrasd\n");
				free( idProduto );
				free( duracao );
				free( nomeProduto );
				free( tipoProduto );
				free( idContratante );
				idProduto = NULL;
				duracao = NULL;
				nomeProduto = NULL;
				tipoProduto = NULL;
				idContratante = NULL;
				return false;
			}
			
			token = usuario_getNextToken(usuario);
			if (token == NULL)
			{
				printf(" LOG: Cliente não está informando descricao de ṕroduto em Comando-Adicao.h addProduto() sakveuaisd\n");
				if (addProdutoAoBanco(usuario, idContratante, idProduto, duracao, nomeProduto, NULL, tipoProduto, categoria))
				{
					printf(" LOG: Produto adicionado ao banco de dados com sucesso em Comando-Adicao.h addProduto() asvber98as7d3\n");
					free( idProduto );
					free( duracao );
					free( nomeProduto );
					free( tipoProduto );
					free( idContratante );
					free( categoria );
					idProduto = NULL;
					duracao = NULL;
					nomeProduto = NULL;
					tipoProduto = NULL;
					idContratante = NULL;
					categoria = NULL;
					return true;
				}
				else
				{
					printf(" Warning: Falha ao adicionar produto ao banco de dados em Comando-Adicao.h addProduto() saouyr8usd\n");
					free( idProduto );
					free( duracao );
					free( nomeProduto );
					free( tipoProduto );
					free( idContratante );
					free( categoria );
					idProduto = NULL;
					duracao = NULL;
					nomeProduto = NULL;
					tipoProduto = NULL;
					idContratante = NULL;
					categoria = NULL;
					return false;
				}
			}
			else
			{
				if (stringMaior(token, TAMANHO_DESCRICAO_PRODUTO))
				{
					printf(" Warning: Descricao exageradamente grande em Comando-Adicao.h addProduto() askjdhjkr\n");
					free( idProduto );
					free( duracao );
					free( nomeProduto );
					free( tipoProduto );
					free( idContratante );
					free( categoria );
					idProduto = NULL;
					duracao = NULL;
					nomeProduto = NULL;
					tipoProduto = NULL;
					idContratante = NULL;
					categoria = NULL;
					return false;
				}
				descricao = strdup(token);
				if (descricao == NULL)
				{
					printf(" Warning: Falha ao duplicar descricao em Comando-Adicao.h addProduto() sakjr0asd\n");
					free( idProduto );
					free( duracao );
					free( nomeProduto );
					free( tipoProduto );
					free( idContratante );
					free( categoria );
					idProduto = NULL;
					duracao = NULL;
					nomeProduto = NULL;
					tipoProduto = NULL;
					idContratante = NULL;
					categoria = NULL;
					return false;
				}
				if (addProdutoAoBanco(usuario, idContratante, idProduto, duracao, nomeProduto, descricao, tipoProduto, categoria))
				{
					printf(" LOG: Produto adicionado ao banco de dados com sucesso em Comando-Adicao.h addProduto() asiurbhuiasd\n");
					free( idProduto );
					free( duracao );
					free( nomeProduto );
					free( tipoProduto );
					free( idContratante );
					free( categoria );
					free( descricao );
					idProduto = NULL;
					duracao = NULL;
					nomeProduto = NULL;
					tipoProduto = NULL;
					idContratante = NULL;
					categoria = NULL;
					descricao = NULL;
					return true;
				}
				else
				{
					printf(" Warning: Falha ao adicionar produto ao banco de dados em Comando-Adicao.h addProduto() eoasb7890sad\n");
					free( idProduto );
					free( duracao );
					free( nomeProduto );
					free( tipoProduto );
					free( idContratante );
					free( categoria );
					free( descricao );
					idProduto = NULL;
					duracao = NULL;
					nomeProduto = NULL;
					tipoProduto = NULL;
					idContratante = NULL;
					categoria = NULL;
					descricao = NULL;
					return true;
				}
			}
			return false;
		}
		return false;

	}
	else if (usuario_PermissaoRoot(usuario))	//TODO: Implementar suporte à adicao de categoria a produto quando logado como ROOT
	{

		//APP 2 + CHAVE_DE_SEGURANCA_PHP
		token = usuario_getNextToken(usuario);
		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente em addProduto() Comando-Adicao.h CHAVE: kqcbjdui45\n");
			return false;
		}

		// if(strlen(token) != TAMANHO_CHAVE_DE_SEGURANCA_PHP)
		if ( ! stringTamanhoIgual(token, TAMANHO_CHAVE_DE_SEGURANCA_PHP))
		{
			printf(" Warning: Chave de segurança informada incorreta em addProduto() Comando-Adicao.h cjqkkgjrwv \n");
			return false;
		}

		if(strcmp(token, CHAVE_DE_SEGURANCA_PHP) != 0)
		{
			printf(" Warning: Chave de segurança informada possui tamanho correto, mas não bate em addProduto() Comando-Adicao.h bkqvej\n");
			return false;
		}

		// APP 2 + CHAVE_DE_SEGURANCA_PHP idContratante
		token = usuario_getNextToken(usuario);
		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente em addProduto() Comando-Adicao.h 1b34 \n");
			return false;
		}

		// if(strlen(token) > TAMANHO_CHAVE_PRIMARIA)
		if (stringMaior(token, TAMANHO_CHAVE_PRIMARIA))
		{
			printf(" Warning: Tamanho de chave primaria informada exageradamente grande em addProduto() Comando-Adicao.h 47dbj\n");
			return false;
		}

		idContratante = strdup(token);

		if(idContratante == NULL)
		{
			printf(" Warning: falha ao copiar de token para idContratante em addProduto() Comando-Adicao.h b2i5\n");
			return false;
		}

		// APP 2 * CHAVE_DE_SEGURANCA_PHP idContratante idProduto
		token = usuario_getNextToken(usuario);

		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente em addProduto() Comando-Adicao.h cbne4kj\n");
			free( idContratante );
			idContratante = NULL;
			return false;
		}

		// if(strlen(token) != TAMANHO_ID_PRODUTO)
		if ( ! stringTamanhoIgual(token, TAMANHO_ID_PRODUTO) )
		{
			printf(" Warning: Comando informado é exageradamente grande em addProduto() Comando-Adicao.h bk5j2h\n");
			free( idContratante );
			idContratante = NULL;
			return false;
		}

		idProduto = strdup(token);

		if( idProduto == NULL )
		{
			printf(" Warning: Faha ao duplicar string de token para idProduto em Comando-Adicao.h addProduto() djasdhjkvbka\n");
			free( idContratante );
			idContratante = NULL;
			return false;
		}

		// APP 2 * CHAVE_DE_SEGURANCA_PHP idContratante idProduto duracao
		token = usuario_getNextToken(usuario);

		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente em addProduto() Comando-Adicao.h 3jh6\n");
			free( idContratante );
			free( idProduto );
			idContratante = NULL;
			idProduto = NULL;
			return false;
		}

		// if(strlen(token) > TAMANHO_DURACAO)
		if (stringMaior(token, TAMANHO_DURACAO))
		{
			printf(" Warning: Comando informado exageradamente grande em addProduto() Comando-Adicao.h ehjik\n");
			free(idContratante);
			free(idProduto);
			idContratante = NULL;
			idProduto = NULL;
			return false;
		}

		duracao = strdup(token);

		if(duracao == NULL)
		{
			printf(" Warning: Não foi possivel copiar de token para duracao em addProduto() Comando-Adicao.h sadkjh4\n");
			free( idContratante );
			free( idProduto );
			idContratante = NULL;
			idProduto = NULL;
			return false;
		}


		// APP 2 + CHAVE_DE_SEGURANCA_PHP idContratante idProduto duracao nomeProduto
		token = usuario_getNextToken(usuario);
		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente em addProduto() Comando-Adicao.h\n");
			free( idContratante );
			free( idProduto );
			free( duracao );
			idContratante = NULL;
			idProduto = NULL;
			duracao = NULL;
			return false;
		}

		// if(strlen(token) > TAMANHO_NOME)
		if (stringMaior(token, TAMANHO_NOME))
		{
			printf(" Warning: Tamanho de token informado é exageradamente grande em addProduto() Comando-Adicao.h 5jhk2g\n");
			free( idContratante );
			free( idProduto );
			free( duracao );
			idContratante = NULL;
			idProduto = NULL;
			duracao = NULL;
			return false;
		}

		nomeProduto = strdup(token);

		if(nomeProduto == NULL)
		{
			printf(" Warning: Falha ao copiar de token para nomeProduto em addProduto() Comando-Adicao.h jkehqk57df\n");
			free( idContratante );
			free( idProduto );
			free( duracao );
			idContratante = NULL;
			idProduto = NULL;
			duracao = NULL;
			return false;
		}

		// APP 2 + CHAVE_DE_SEGURANCA_PHP idContratante idProduto duracao nomeProduto tipoProduto
		token = usuario_getNextToken(usuario);
		if (token == NULL)
		{
			printf(" Warning: Tipo de produto não especificado em Comando-Adicao.h addProduto() askbvu8ra8sd7\n");
			free( idContratante );
			free( idProduto );
			free( duracao );
			free( nomeProduto );
			idContratante = NULL;
			idProduto = NULL;
			duracao = NULL;
			nomeProduto = NULL;
			return false;
		}
		// if (strlen(token) != TAMANHO_TIPO_PRODUTO)
		if (stringMaior(token, TAMANHO_TIPO_PRODUTO))
		{
			printf(" Warning: Tamanho do tipo do produto está exageradamente grande em Comando-Adicao.h addProduto() virb89s7d8hfs\n");
			free( idContratante );
			free( idProduto );
			free( duracao );
			free( nomeProduto );
			idContratante = NULL;
			idProduto = NULL;
			duracao = NULL;
			nomeProduto = NULL;
			return false;
		}

		tipoProduto = strdup(token);
		if (tipoProduto == NULL)
		{
			printf(" Warning: Falha ao duplicar tipo de produto em Comando-Adicao.h addProduto() ewiugh89da7dadfsfd\n");
			free( idContratante );
			free( idProduto );
			free( duracao );
			free( nomeProduto );
			idContratante = NULL;
			idProduto = NULL;
			duracao = NULL;
			nomeProduto = NULL;
			return false;
		}

		// APP 2 + CHAVE_DE_SEGURANCA_PHP idContratante idProduto duracao nomeProduto tipoProduto descricaoProduto
		token = usuario_getNextToken(usuario);
		if(token == NULL)
		{
			if(addProdutoAoBanco(usuario, idContratante, idProduto, duracao, nomeProduto, NULL, tipoProduto, NULL))//TODO Adicionar descrição de produto
			{
				printf(" LOG: Produto adicionado com sucesso ao banco de dados em addProdutoAoBanco() Comando-Adicao.h\n");
				free( idContratante );
				free( idProduto );
				free( duracao );
				free( nomeProduto );
				free( tipoProduto );
				idContratante = NULL;
				idProduto = NULL;
				duracao = NULL;
				nomeProduto = NULL;
				tipoProduto = NULL;
				return true;
			}
			else
			{
				printf(" Warning: Não foi possivel adicionar produto ao banco de dadoe em addProduto() Comando-Adicao.h bcejqk\n");
				free( idContratante );
				free( idProduto );
				free( duracao );
				free( nomeProduto );
				free( tipoProduto );
				idContratante = NULL;
				idProduto = NULL;
				duracao = NULL;
				nomeProduto = NULL;
				tipoProduto = NULL;
				return false;
			}
		}
		else
		{
			// Se o usuario informar descricao do produto
			if (stringMaior(token, TAMANHO_DESCRICAO_PRODUTO))
			{
				printf(" Warning: Tamanho de descricao informada é exageradamente grande para produto em Comando-Adicao.h addProduto() q45w687vw\n");
				free( idContratante );
				free( idProduto );
				free( duracao );
				free( nomeProduto );
				free( tipoProduto );
				idContratante = NULL;
				idProduto = NULL;
				duracao = NULL;
				nomeProduto = NULL;
				tipoProduto = NULL;
				return false;
			}

			descricao = strdup(token);

			if(descricao == NULL)
			{
				printf(" Warning: Falha ao copiar de token para descricao em Comando-Adicao.h addProduto() q4r84t32saw\n");
				free( idContratante );
				free( idProduto );
				free( duracao );
				free( nomeProduto );
				free( tipoProduto );
				idContratante = NULL;
				idProduto = NULL;
				duracao = NULL;
				nomeProduto = NULL;
				tipoProduto = NULL;
				return false;
			}

			if(addProdutoAoBanco(usuario, idContratante, idProduto, duracao, nomeProduto, descricao, tipoProduto, NULL))
			{
				printf(" LOG: Produto adicionado com sucesso ao banco de dados em addProdutoAoBanco() Comando-Adicao.h\n");
				free( idContratante );
				free( idProduto );
				free( duracao );
				free( nomeProduto );
				free( tipoProduto );
				free( descricao );
				idContratante = NULL;
				idProduto = NULL;
				duracao = NULL;
				nomeProduto = NULL;
				tipoProduto = NULL;
				descricao = NULL;
				return true;
			}
			else
			{
				printf(" Warning: Não foi possivel adicionar produto ao banco de dadoe em addProduto() Comando-Adicao.h bcejqk\n");
				free( idContratante );
				free( idProduto );
				free( duracao );
				free( nomeProduto );
				free( tipoProduto );
				free( descricao );
				idContratante = NULL;
				idProduto = NULL;
				duracao = NULL;
				nomeProduto = NULL;
				tipoProduto = NULL;
				descricao = NULL;
				return false;
			}
		}
	}
	else
	{
		printf(" Warning: Usuario não possui privilegios suficientes para executar esse comando em Comando-Adicao.h addProduto()\n");
		return false;
	}

	printf(" Warning: Erro desconhecido em addProduto() Comando-Adicao.h chagsjkchsajk\n");

	free( idContratante );
	free( idProduto );
	free( duracao );
	free( nomeProduto );
	free( tipoProduto );
	free( descricao );
	idContratante = NULL;
	idProduto = NULL;
	duracao = NULL;
	nomeProduto = NULL;
	tipoProduto = NULL;
	descricao = NULL;

	return false;
}

bool addInformacoesAUsuario(Usuario *usuario)// DOING FOREVER
{
	if(usuario == NULL)
	{
		printf(" Warning: usuario não informado em Comando-Adicao.h addInformacoesAUsuario()\n");
		return false;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" Warning: usuario não conectado detectado em Comando-Adicao.h addInformacoesAUsuario()\n");
		return false;
	}

	char *token = usuario_getNextToken(usuario);//APP 2 & tipoInformacao

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

	if (strcmp(token, TIPO_LISTA_DESEJOS) == 0)// APP 2 & LD idProduto		//Adiciona produto à lista de desejos
	{
		printf(" LOG: cliente solicitando adicao de informação à sua lista de desejos em Comando-Adicao.h addInformacoesAUsuario()\n");
		if (!usuario_PermissaoCliente(usuario))
		{
			printf(" LOG: Usuario conectado não é cliente comum, portanto, nao pode ter lista de desejos em Comando-Adicao.h addInformacoesAUsuario() ao8ry8sad\n");
			return false;
		}
		
		token = usuario_getNextToken(usuario);
		if (token == NULL)
		{
			printf(" Warning: Comando insuficiente em Comando-Adicao.h addInformacoesAUsuario() asiuvrhu9as87ras\n");
			return false;
		}
		if (!stringTamanhoIgual(token, TAMANHO_ID_PRODUTO))
		{
			printf(" Warning: idProduto de tamanho incorreto informado em Comando-Adicao.h addInformacoesAUsuario() aiurey7898asd32\n");
			return false;
		}
		char *idProduto = strdup(token);
		return addProdutoAListaDeDesejosDoClienteAoBanco(usuario, idProduto);
	}
	else if (strcmp(token, TIPO_NOME) == 0)// APP 2 & # nomeInformado	//Adiciona ou substitui o nome do usuario pelo nome informado
	{
		printf(" LOG: cliente solicitando adição de informação: NOME em Comando-Adicao.h addInformacoesAUsuario()\n");
		
		token = usuario_getNextToken(usuario);
		if (token == NULL)
		{
			printf(" Warning: Comando insuficiente em Comando-Adicao.h addInformacoesAUsuario() 654q898fgf\n");
			return false;
		}
		if (stringMaior(token, TAMANHO_NOME))
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
			if(addNomeDeUsuarioAoBanco(usuario, nome))
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
	else if(strcmp(token, TIPO_PONTO) == 0)// APP 2 & Dj quantidadeDePontos		//Adiciona numero de pontos ao usuario
	{
		
		token = usuario_getNextToken(usuario);
		if(token == NULL)
		{
			printf(" Warning: Comando insuficiente detectado em Comando-Adicao.h addInformacoesAUsuario() sabruioxjk:w\n");
			return false;
		}
		if(strlen(token) > TAMANHO_DE_INTEIRO_EM_BANCO_DE_DADOS)
		{
			printf(" Warning: Comando exageradamente grande em Comando-Adicao.h addInformacoesAUsuario() aibvruiuouge\n");
			return false;
		}
		char *quantidade = strdup(token);
		if(quantidade == NULL)
		{
			printf(" Warning: falha ao duplicar token para quantidade em Comando-Adicao.h addInformacoesAUsuario() eiuabruioaisdr\n");
			return false;
		}
		else
		{
			if(addPontosDeUsuarioAoBanco(usuario, quantidade))
			{
				printf(" LOG: Pontos adicionados com sucesso a usuario em Comando-Adicao.h addInformacoesAUsuario() cnqiunasd\n");
				free(quantidade);
				quantidade = NULL;
				return true;
			}
			else
			{
				printf(" Warning: Falha ao adicionar pontos ao banco de dados detectada em Comando-Adicao.h addInformacoesAUsuario() qi44h1898dn\n");
				free(quantidade);
				quantidade = NULL;
				return false;
			}
		}

	}
	else if(strcmp(token, TIPO_SOBRENOME) == 0)// APP 2 & Y sobrenomeInformado		//Adiciona ou substitui o sobrenome do usuario pelo sobrenome informado
	{
		printf(" LOG: cliente solicitando adição de informação: SOBRENOME em Comando-Adicao.h addInformacoesAUsuario()\n");
		
		token = usuario_getNextToken(usuario);
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
			if(addSobrenomeDeUsuarioAoBanco(usuario, sobrenome))
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
	else if(strcmp(token, TIPO_SEXO) == 0)// APP 2 & SQ sexoInformado		//Adiciona ou substitui o sexo do usuario pelo sexo informado
	{
		printf(" LOG: cliente solicitando adição de informação: SEXO em Comando-Adicao.h addInformacoesAUsuario()\n");
		
		token = usuario_getNextToken(usuario);
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
			if(addSexoDeUsuarioAoBanco(usuario, sexo))
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
	else if(strcmp(token, TIPO_DATA) == 0)// APP 2 & T dataInformada		//Adiciona ou substitui a data de nascimento do usuario pela data informada
	{
		printf(" LOG: Cliente solicitando adição de data de nascimento a usuario em Comando-Adicao.h addInformacoesAUsuario()\n");
		
		token = usuario_getNextToken(usuario);
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
			if(addDataNascimentoDeUsuarioAoBanco(usuario, dataNascimento))
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


/* A avaliação do usuario é dada por um caracter:
   1  → para avaliação positiva
   0  → para avaliacao negativa
   2  → para avaliação neutra
 */
bool addAvaliacaoAProduto(Usuario *usuario)//DONE       APP 2 kW * idProduto avaliacaoDoUsuario
{
	if (usuario == NULL)
	{
		printf(" Warning: Usuario nulo em Comando-Adicao.h addAvaliacaoAProduto()\n");
		return false;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" Warning: Usuario não conectado detectado em Comando-Adicao.h addAvaliacaoAProduto()\n");
		return false;
	}
	char *token = usuario_getNextToken(usuario);// APP 2 kW idProduto

	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addAvaliacaoAProduto() qkjh4uhuasd\n");
		return false;
	}
	if(strlen(token) != TAMANHO_ID_PRODUTO)
	{
		printf(" Warning: Argumento invalido (%s) detectado em Comando-Adicao.h addAvaliacaoAProduto() qkjhewjkvaw\n", token);
		return false;
	}
	char *idProduto = strdup(token);
	if(idProduto == NULL)
	{
		printf(" Warning: Falha ao duplicar token para idProduto em Comando-Adicao.h addAvaliacaoAProduto() qjwkhjkbve\n");
		return false;
	}

	// APP 2 kW idProduto avaliacaoDoUsuario
	token = usuario_getNextToken(usuario);
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
	if(!(token[0] == '0' || token[0] == '1' || token[0] == '2') || token[1]!= '\0')//OTIMIZANDO !!!
	{
		printf(" Warning: Comando incorreto em (argumento invalido) Comando-Adicao.h addAvaliacaoAProduto().\n");
		free(idProduto);
		idProduto = NULL;
		return false;
	}

	char *avaliacao = (char *)malloc(sizeof(char) * 2);// o caracter e o \0
	if (avaliacao == NULL)
	{
		printf(" Warning: Falha a alocar memoria para avaliacao em Comando-Adicao.h addAvaliacaoAProduto() c3qw45f89yt87j5sd\n");
		free(idProduto);
		idProduto = NULL;
		return false;
	}
	avaliacao[0] = token[0];
	avaliacao[1] = token[1];

	if(addAvaliacaoAProdutoAoBanco(usuario, idProduto, avaliacao))
	{
		printf(" LOG: Avaliacao adicionada ao banco com sucesso em Comando-Adicao.h addAvaliacaoAProduto() 46q5w4vb\n");
		free(idProduto);
		free(avaliacao);
		avaliacao = NULL;
		idProduto = NULL;
		return true;
	}
	else
	{
		printf(" Warning: Falha ao adicionar avaliação ao banco de dados em Comando-Adicao.h addAvaliacaoAProduto() kjqhjkhve\n");
		free(idProduto);
		free(avaliacao);
		avaliacao = NULL;
		idProduto = NULL;
		return false;
	}
}

bool addFeedbackAProduto(Usuario *usuario)// APP 2 tr * idProduto tituloDoFeedback textoDoFeedBack
{
	// if(email == NULL)
	// {
	// 	printf(" Warning: email == NULL em addFeedbackAProduto() Comando-Adicao.h qq654nn");
	// 	return false;
	// }
	if (usuario == NULL)
	{
		printf(" Warning: Usuario nulo detectado em Comando-Adicao.h addFeedbackAProduto()\n");
		return false;
	}
	if (usuario_obterLogin(usuario) == NULL)
	{
		printf(" Warning: Usuario nao conectado detectado em Comando-Adicao.h addFeedbackAProduto()\n");
		return false;
	}
	char *token = usuario_getNextToken(usuario);// APP 2 tr idProduto

	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em addFeedbackAProduto() Comando-Adicao.h asdkjbvd\n");
		return false;
	}
	if(strlen(token) != TAMANHO_ID_PRODUTO)
	{
		printf(" Warning: Comando incorreto detectado em addFeedBackAProduto() Comando-Adicao.h asfbjrdf\n");
		return false;
	}
	char *idProduto = strdup(token);
	if(idProduto == NULL)
	{
		printf(" Warning: Não foi possível duplicar token para idProduto em addFeedbackAProduto() Comando-Adicao.h asfdhjkr\n");
		return false;
	}

	// APP 2 tr idProduto tituloFeedBack
	token = usuario_getNextToken(usuario);
	if(token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addFeedbackAProduto() sv489s51hrd\n");
		free(idProduto);
		idProduto = NULL;
		return false;
	}
	if(strlen(token) > TAMANHO_TITULO_FEEDBACK)
	{
		printf(" Warning: Comando exageradamente grande detectado em addFeedbackAProduto() Comando-Adicao.h sahjkr\n");
		free(idProduto);
		idProduto = NULL;
		return false;
	}
	char *titulo = strdup(token);
	if(titulo == NULL)
	{
		printf(" Warning: Falha ao duplicar token para titulo em addFeedbackAProduto() Comando-Adicao,h asgjhrs\n");
		free(idProduto);
		idProduto = NULL;
		return false;
	}
	// APP 2 tr * idproduto tituloFeedBack conteudoFeedback
	token = usuario_getNextToken(usuario);
	if (token == NULL)
	{
		printf(" Warning: Comando insuficiente em Comando-Adicao.h addFeedBackAProduto() shgjvrsd\n");
		free(idProduto);
		free(titulo);
		titulo = NULL;
		idProduto = NULL;
		return false;
	}
	if (strlen(token) > TAMANHO_CONTEUDO_FEEDBACK)
	{
		printf(" Warning: Comando exageradamente grande detectado em Comando-Adicao.h addFeedbackAProduto() gjhrh4aq32j0d\n");
		free(idProduto);
		free(titulo);
		titulo = NULL;
		idProduto = NULL;
		return false;
	}
	char *conteudo = strdup(token);
	if(conteudo == NULL)
	{
		printf(" Warning: Falha ao duplicar conteudo de token para conteudo em Comando-Adicao.h addFeedBackAProduto() askhjgv5rd0f15\n");
		free(idProduto);
		free(titulo);
		titulo = NULL;
		idProduto = NULL;
		return false;
	}

	if (addFeedBackDeProdutoAoBanco(usuario, idProduto, titulo, conteudo))
	{
		printf(" LOG: Feedback adicionado ao banco de dados com sucesso em Comando-Adicao.h addFeedbackAProduto() vhjkfdg\n");
		free(idProduto);
		free(titulo);
		free(conteudo);
		conteudo = NULL;
		idProduto = NULL;
		titulo = NULL;
		return true;
	}
	else
	{
		printf(" Warning: Feedback não foi adicionado ao banco de dados em Comando-Adicao.h addFeedBackAProduto() asjkghjkvbrds\n");
		free(idProduto);
		free(titulo);
		free(conteudo);
		conteudo = NULL;
		idProduto = NULL;
		titulo = NULL;
		return false;
	}
	printf(" ERRO: erro desconhecido detectado em Comando-Adicao.h addFeedBackAProduto() ashjgvrsafrh41586210bvds\n");
	return false;
}

/** 
 * @brief  Altera a quantidade de habitantes do banco de dados, pra quantidade informada pelo usuario
 * @note   Pode ser acessada somente pelo ROOT
 * @call   APP 2 qC nomeCidade nomeEstado quantidade
 * @param  *usuario: usado pra checar se é ROOT ou não
 * @retval true caso tudo ocorra bem, false caso contrário
 */
bool addNumeroDeHabitantesACidade(Usuario *usuario)
{
	const char *localizacao = "Comando-Adicao.h addNumeroDeHabitantesACidade()";
	if (!usuarioValido(usuario, localizacao))
	{
		return false;
	}
	if (usuario_PermissaoRoot(usuario))
	{
		char *token = usuario_getNextToken(usuario);
		if (token == NULL)
		{
			geraLog(WARNING, "Comando insuficiente, cidade não informada");
			return false;
		}
		if (stringMaior(token, TAMANHO_DO_NOME_DA_CIDADE_COM_MAIOR_NOME_DO_MUNDO))
		{
			geraLog(WARNING, "Cidade informada possui nome exageradamente grande")	;
			return false;
		}
		char *nomeCidade = strdup(token);
		if (nomeCidade == NULL)
		{
			geraLog(ERRO, "Falha ao duplicar nome de cidade");
			return false;
		}
		token = usuario_getNextToken(usuario);
		if (token == NULL)
		{
			geraLog(WARNING, "Cliente nao informou estado");
			free(nomeCidade);
			nomeCidade = NULL;
			return false;
		}
		if (!stringTamanhoIgual(token, TAMANHO_ESTADO))
		{
			geraLog(WARNING, "Parametro incorreto identificado");
			free(nomeCidade);
			nomeCidade = NULL;
			return false;
		}
		char *nomeEstado = strdup(token);
		if (nomeEstado == NULL)
		{
			geraLog(ERRO, "Falha ao duplicar nome de estado");
			free(nomeCidade);
			nomeCidade = NULL;
			return false;
		}

		token = usuario_getNextToken(usuario);
		if (token == NULL)
		{
			geraLog(WARNING, "Numero de habitantes não foi informado");
			free(nomeCidade);
			nomeCidade = NULL;
			free(nomeEstado);
			nomeEstado = NULL;						
			return false;
		}
		char *numeroDeHabitantes = strdup(token);
		if (numeroDeHabitantes == NULL)
		{
			geraLog(ERRO, "Falha ao duplicar quantidade de habitantes");
			free(nomeCidade);
			nomeCidade = NULL;
			free(nomeEstado);
			nomeEstado = NULL;
			return false;
		}

		return addNumeroDeHabitantesACidadeAoBanco(usuario, nomeCidade, nomeEstado, numeroDeHabitantes);
	}
	else
	{
		geraLog(WARNING, "Usuario que não é root tentou acessar esse comando");
		return false;
	}
}
