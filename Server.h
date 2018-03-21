#include <string.h>
#include <unistd.h>
#include<signal.h>

// #include "GerenciadorDeThreads.h"

#include "Comandos/interpretadorDeComandos.h"


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


bool enviaMensagemParaCliente(const char *mensagem, void *cliente)
{
	#ifdef __COM_CRIPTOGRAFIA__
		// char *encrypted = (unsigned char *)malloc(sizeof(unsigned char) * 4098);
		// int encrypted_length= private_encrypt(mensagem,strlen((const char *)mensagem),getChavePublica(),encrypted);
		// if(encrypted_length == -1)
		// {
		// 	printf(" ERRO:Erro ao criptografar mensagem em Server.h recebeMensagemDeCliente()\n");
		// }
		printf(" ERRO: COMANDO ESSENCIAL INCOMPLETO em Server.h enviaMensagemParaCliente()\n");
		return false;
	#else
		write( *(int *)cliente, mensagem, strlen(mensagem) + 1 );
		printf(" ************** ****************** *******************\n");
		printf(" LOG: Mensagem enviada |%s|, em Server.h enviaMensagemParaCliente()\n", mensagem);
		return true;
	#endif
}

bool recebeMensagemDeCliente(char *mensagem, void *cliente)
{
	#ifdef __COM_CRIPTOGRAFIA__
		printf(" ERRO: COMANDO ESSENCIAL INCOMPLETO em Server.h recebeMensagemDeCliente()\n");
		return false;
	#else
		memset(mensagem, '\0', BUFFER_CLIENTE);
		read(*(int*)cliente, mensagem, BUFFER_CLIENTE);	
		printf(" LOG: Mensagem recebida |%s|, em Server.h recebeMensagemDeCliente()\n", mensagem);
		return true;
	#endif
}



void *Servidor(void *cliente)
{
	char bufferCliente[BUFFER_CLIENTE];
	bool autorizado = false;
	char *mensagem = NULL;
	int resultado;
	static const char *localizacao = "Server.h Servidor(void *cliente)";
	Usuario usuario;
	init_Usuario(&usuario);

	void sairDaThread()
	{
		printf(" LOG: Encerrando thread de Servidor em Server.h Servidor()->sairDaThread()\n");
		// reset_Usuario(&usuario);
		delete_Usuario(&usuario);
		if (cliente != NULL)
		{
			close(*(int *)cliente);	
			cliente = (void *)NULL;
		}
		pthread_exit( (void *) 0 );
	}

	void interrupcaoForcadaEmThread(int sinal)
	{
		printf(" Sinal pego na Thread = |%d| ← \n", sinal);
		sairDaThread();
	}
	signal(SIGINT, interrupcaoForcadaEmThread);
	
	printf(" LOG: Aguardando por mensagens\n");
	while(true)
	{
		recebeMensagemDeCliente(bufferCliente, cliente);

		/* Se a mensagem recebida não for incompatível com nenhum comando */
		if( !mensagemDeEscapeDetectada(bufferCliente) )
		{
			printf(" LOG: Mensagem recebida, Aguardando liberacao para interpretacao\n");

			printf("\n\n \t*********************Inicio de Interpretação *************\n");

			interpretaComando(bufferCliente, &autorizado, &resultado, &usuario);

			switch(resultado)
			{
				case ERRO:
					printf("Warning: Erro na execução do comando\n");
					enviaMensagemParaCliente("ERRO, desconectando\0", cliente);
					sairDaThread();
					break;

				case OK:
					printf("Log: Comando bem-sucedido (Server Thread)\n");
					enviaMensagemParaCliente("OK\0", cliente);
					break;

				case REQUISITANDO_LOGIN:/* Cliente NÃO AUTORIZADO OK */
					printf(" Cliente Requisitou Login, mas Não foi atendido (Server Thread)\n");
					enviaMensagemParaCliente("ERRO: Não autorizado, desconectando\0", cliente);
					sairDaThread();
					break;

				case REQUISITANDO_ATUALIZACAO:
					geraLog(LOG, "Cliente solicitando comando de atualização", localizacao);
					mensagem = comandoAtualizar(&usuario);
					enviaMensagemParaCliente(mensagem, cliente);
					free(mensagem);
					mensagem = NULL;
					break;

				// case REQUISITANDO_ADICAO:/* OK */
				// 	printf(" Cliente requisitou adicao\n");
				// 	enviaMensagemParaCliente("ERRO: Adicao recusada, desconectando\0", cliente);
				// 	sairDaThread(false);
				// 	break;
				case RETORNO_ERRO_INTERNO:/* se ocorrer algum erro de memória, por exemplo */
					printf(" Warning: Erro interno identificado em Server.h Servidor() qt658f7tgf34\n");
					enviaMensagemParaCliente(RETORNO_ERRO_INTERNO_STR, cliente);
					sairDaThread();
					break;
				
				case RETORNO_COMANDO_INCORRETO:/* Se algum parâmetro estiver errado */
					printf(" Warning: Operação falhou, parâmetros incorretos detectados. em Server.h Servidor() 20ty9uwe\n");
					enviaMensagemParaCliente( RETORNO_COMANDO_INCORRETO_STR , cliente);
					sairDaThread();
					break;
				
				case RETORNO_NAO_AUTORIZADO:
					printf(" Warning: Não autorizado a executar essa operação em Server.h Servidor() q0r59hnthrueqgf\n");
					enviaMensagemParaCliente(RETORNO_ERRO_NAO_AUTORIZADO_STR, cliente);
					sairDaThread();
					break;

				case LOGIN_NAO_AUTORIZADO:
					printf(" Warning: Falha ao logar em Server.h Servidor()\n");
					enviaMensagemParaCliente("ERRO: usuario ou senha incorretos, desconectando.", cliente);
					sairDaThread();
					break;

				case ERRO_DE_EXECUCAO:
					printf(" Warning: Falha ao executar comando em Server.h Servidor()\n");
					enviaMensagemParaCliente("ERRO: Falha na execucao do comando, desconectando.", cliente);
					sairDaThread();
					break;

				case REQUISITANDO_OBTENCAO:
					mensagem = comandoObter(&usuario);
					enviaMensagemParaCliente(mensagem, cliente);
					free(mensagem);
					mensagem = NULL;
					break;

				case REQUISITANDO_REMOCAO:
					printf(" Warning: Falha ao concluir comando de remocao em Server.h Servidor()\n");
					// enviaMensagemParaCliente("Remoção Negada\nainda conectado\0", cliente);
					char *tete = RETORNO_ERRO_COMANDO_NAO_CONSTRUIDO_STR_DINAMICA;
					enviaMensagemParaCliente( tete, cliente);
					free(tete);
					tete = NULL;
					break;

				case RETORNO_ADICAO_RECUSADA:
					geraLog(LOG, "Adicao recusada detectada", "Server.h Servidor()");
					enviaMensagemParaCliente("ERRO: Adição recusada", cliente);
					break;

				case REQUISITANDO_ROOT:
					if( usuario_PermissaoRoot(&usuario) )
					{
						 mensagem = comandoRoot(&usuario);
						 enviaMensagemParaCliente(mensagem, cliente);
						 free(mensagem);
						 mensagem = NULL;
					}
					else
					{
						enviaMensagemParaCliente("ERRO: Você nn pode fazer isso, muahahahaha \\o/\0", cliente);
						sairDaThread();
					}
					break;

				default:
					printf(" Não foi possivel interpretar comando (Server.h) (Servidor()) Resultado == %d\n", resultado );
					enviaMensagemParaCliente("ERRO: Comando não compreendido, desconectando\0", cliente);
					sairDaThread();
					break;
			}
		}
		else
		{
			printf(" LOG: Mensagem de escape detectada em Server.h Servidor()\n");
			enviaMensagemParaCliente("ERRO: Voce pediu pra sair... Conexao encerrada\0", cliente);
			sairDaThread();
		}
	}
}
