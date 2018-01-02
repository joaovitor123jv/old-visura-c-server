#include <string.h>
#include <unistd.h>

#define BUFFER_CLIENTE 1024//(aceitou 1024*1024 (1MB))

// #include "GerenciadorDeThreads.h"

#include "Comandos/interpretadorDeComandos.h"

#define PORTA 7420

#define QTD_CLIENTE 5 /* Quantidade de clientes máxima aceita */
#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef bool
#define bool char
#endif

int abreSocket()
{
	int numeroDeTentativas = 10;
	int contador = 0;
	int sockfd = -1;
	int tempoEntreTentativas = 1;
	for(contador = 0; contador<numeroDeTentativas; contador++)
	{
		sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if( sockfd < 0)
		{
			printf(" Warning: Falha ao abrir socket detectada em Server.h abreSocket(), tentativa nº %d\n", contador);
		}
		else
		{
			break;
		}
		sleep(tempoEntreTentativas);
	}
	if(sockfd < 0)
	{
		printf(" ERRO: Numero maximo de tentativas para abrir socket detectado, encerrando\n");
		return -1;
	}
	else
	{
		printf(" LOG: Socket configurado, pronto pra estabelecer conexao em Server.h abreSocket()\n");
		return sockfd;
	}
}

int fazerBind(int sockfd, struct sockaddr *serverAddr)
{
	int numeroDeTentativas = 100;
	int tempoEntreTentativas = 1;
	int contador;
	int bindEstabelecido = -7;

	for(contador = 0; contador<numeroDeTentativas; contador++)
	{
		bindEstabelecido = bind(sockfd, serverAddr, sizeof(struct sockaddr));
		if(bindEstabelecido<0)
		{
			printf(" Warning: Falha ao fazer bind, em Server.h fazerBind(), tentativa nº %d\n", contador);
		}
		else
		{
			break;
		}
		sleep(tempoEntreTentativas);
	}
	if(bindEstabelecido < 0)
	{
		printf(" ERRO: Fatal, falha ao estabelecer conexão entre aplicação e socket, em Server.h fazerBind()\n");
		return -1;
	}
	else
	{
		printf(" LOG: Bind estabelecido com sucesso em Server.h fazerBind()\n");
		return 0;
	}
}


int configuraServidor()
{
	int sockfd = -1;//Inicializado com -1 por causa do for
	int contador = 0;
	int numeroDeTentativas = 99;
	int tempoEntreTentativas = 1;
	bool conectadoAoBanco = false;

	struct sockaddr_in serverAddr;

	if((sockfd = abreSocket()) < 0)
	{
		printf(" ERRO: Fatal, impossível abrir socket, em Server.h configuraServidor()\n");
		return ERRO;
	}

	/* Configuração de variáveis, pra fazer bind */
	memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(PORTA);
	/* FIM - Configuração de variáveis, pra fazer bind */

	if(fazerBind(sockfd, (struct sockaddr *)&serverAddr) < 0)
	{
		printf(" ERRO: Falha ao fazer bind em Server.h configuraServidor()\n");
		return ERRO;
	}

	/* Fica na escuta de até 5 clientes */
	if(listen(sockfd, QTD_CLIENTE) < 0)
	{
		printf(" ERRO: Falha ao iniciar listener em Server.h configuraServidor()\n");
		return ERRO;
	}

	for(contador = 0; contador<numeroDeTentativas; contador++)
	{
		if(!conectarBanco())
		{
			printf(" Warning: Falha ao estabelecer conexao com o banco de dados, tentatica nº %d em Server.h configuraServidor()\n", contador);
		}
		else
		{
			printf(" LOG: Conectado com sucesso ao banco de dados em Server.h configuraServidor()\n");
			conectadoAoBanco = true;
			break;
		}
		sleep(tempoEntreTentativas);
	}

	if(conectadoAoBanco)
	{
		return sockfd;
	}
	else
	{
		return ERRO;
	}
}


void liberaMemoriaTalvezUtilizada(char *email)
{
	if(email != NULL)
	{
		free(email);
		email = NULL;
	}
	desconectarBanco();
}


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


bool enviaMensagemParaCliente(const char *mensagem, void *cliente)
{
	write( *(int *)cliente, mensagem, strlen(mensagem) + 1 );
	printf(" LOG: Mensagem enviada |%s|, em Server.h enviaMensagemParaCliente()\n", mensagem);
	return true;
}

void *Servidor(void *cliente)
{
	char bufferCliente[BUFFER_CLIENTE];
	bool autorizado = false;
	char *email = NULL;
	char *mensagem = NULL;
	int resultado;
	bool usuarioAnonimo = true;
	Usuario usuario;
	
	printf(" LOG: Aguardando por mensagens\n");
	while(true)
	{
		memset(bufferCliente, '\0', BUFFER_CLIENTE);
		if(mensagem != NULL)
		{
			mensagem = NULL;
		}

		read(*(int*)cliente, bufferCliente, sizeof(bufferCliente));
		printf(" RECEBIDO: |%s|\n", bufferCliente);

		void sairDaThread(bool liberaInterpretacao)
		{
			if(liberaInterpretacao)
			{
				interpretando = false;
			}
			printf(" LOG: Encerrando thread de Servidor em Server.h Servidor()->sairDaThread()\n");
			close(*(int *)cliente);
			liberaMemoriaTalvezUtilizada(email);
			pthread_exit( (void *) 0 );
		}

		/* Se a mensagem recebida não for incompatível com nenhum comando */
		if( !mensagemDeEscapeDetectada(bufferCliente) )
		{
			printf(" LOG: Mensagem recebida, Aguardando liberacao para interpretacao\n");
			while(interpretando)
			{
				pthread_yield();/* Causa um warning, mas nada demais */
			}//Aguarda, liberando cpu para outros trabalhos (senão fica fazendo coisa a toa até ser interrompida pelo SO)

			printf("\n\n \t*********************Inicio de Interpretação *************\n");

			if(email == NULL)
			{
				email = interpretaComando(bufferCliente, &autorizado, &resultado, email, &usuarioAnonimo, &usuario);

				usuario_mostrarDados(&usuario);

				//printf(" DEBUG: email era NULO em Server.h Servidor()\n");	
				//printf(" ***********usuario->login = %s em Server.h Servidor()\n", usuario_obterLogin(&usuario));
				
				if(email != NULL)
				{
					email = strdup(email);
				}
				else
				{
					printf("\t LOG :Usuário não autorizado em Server.h Servidor()\n");
				}
			}
			else
			{
				interpretaComando(bufferCliente, &autorizado, &resultado, email, &usuarioAnonimo, &usuario);
			}

			switch(resultado)
			{
				case ERRO:
					printf("Warning: Erro na execução do comando\n");
					enviaMensagemParaCliente("Erro, desconectando (calma, estou arrumando kkk)\0", cliente);
					sairDaThread(false);
					break;

				case OK:
					printf("Log: Comando bem-sucedido (Server Thread)\n");
					enviaMensagemParaCliente("OK\nainda conectado\0", cliente);
					break;

				case REQUISITANDO_LOGIN:/* Cliente NÃO AUTORIZADO OK */
					printf(" Cliente Requisitou Login, mas Não foi atendido (Server Thread)\n");
					enviaMensagemParaCliente("Não autorizado, desconectando\0", cliente);
					sairDaThread(true);
					break;

				case REQUISITANDO_ADICAO:/* OK */
					printf(" Cliente requisitou adicao\n");
					enviaMensagemParaCliente("Adicao recusada, desconectando\0", cliente);
					sairDaThread(false);
					break;

				case REQUISITANDO_OBTENCAO:
					mensagem = obterDados(email);
					interpretando = false;
					bool precisaLiberar = true;
					if(mensagem == NULL)
					{
						precisaLiberar = false;
						mensagem = RETORNO_ERRO_CHAR;
					}
					write( *(int *)cliente, mensagem, strlen(mensagem) +1);
					if(precisaLiberar)
					{
						if(strcmp(mensagem, RETORNO_NOT_FOUND) == 0)
						{
							mensagem = NULL;
						}
						else
						{
							free(mensagem);
							mensagem = NULL;
						}
					}
					break;

				case REQUISITANDO_REMOCAO:
					printf(" Warning: Falha ao concluir comando de remocao em Server.h Servidor()\n");
					enviaMensagemParaCliente("Falha ao executar comando de remocao\nainda conectado\0", cliente);
					break;

				case REQUISITANDO_ROOT:
					if( usuario_PermissaoRoot(&usuario) )
					{
						 mensagem = comandoRoot(&usuario);
						 interpretando = false;// NÃO REMOVER !!!
						 enviaMensagemParaCliente(mensagem, cliente);
						 free(mensagem);
						 mensagem = NULL;
					}
					else
					{
						enviaMensagemParaCliente("Não Autorizado, desconectando\0", cliente);
					}
					break;

				default:
					printf(" Não foi possivel interpretar comando (Server.h) (Servidor()) Resultado == %d\n", resultado );
					enviaMensagemParaCliente("Comando incorreto, desconectando\0", cliente);
					sairDaThread(false);
					break;
			}
		}
		else
		{
			printf(" LOG: Mensagem de escape detectada em Server.h Servidor()\n");
			enviaMensagemParaCliente("Voce pediu pra sair... Conexao encerrada\0", cliente);
			sairDaThread(false);
		}
	}
}
