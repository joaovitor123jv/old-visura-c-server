#include <string.h>
#include <unistd.h>
#include<signal.h>

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
	Usuario usuario;
	init_Usuario(&usuario);

	void sairDaThread(bool liberaInterpretacao)
	{
		if(liberaInterpretacao)
		{
			interpretando = false;
		}
		printf(" LOG: Encerrando thread de Servidor em Server.h Servidor()->sairDaThread()\n");
		// reset_Usuario(&usuario);
		delete_Usuario(&usuario);
		if (cliente != NULL)
		{
			close(*(int *)cliente);	
		}
		cliente = NULL;
		pthread_exit( (void *) 0 );
	}

	void interrupcaoForcadaEmThread(int sinal)
	{
		printf(" Sinal pego na Thread = |%d| ← \n", sinal);
		sairDaThread(false);
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
			while(interpretando)
			{
				pthread_yield();
			}//Aguarda, liberando cpu para outros trabalhos (senão fica fazendo coisa a toa até ser interrompida pelo SO)

			printf("\n\n \t*********************Inicio de Interpretação *************\n");

			interpretaComando(bufferCliente, &autorizado, &resultado, &usuario);

			switch(resultado)
			{
				case ERRO:
					printf("Warning: Erro na execução do comando\n");
					enviaMensagemParaCliente("ERRO, desconectando\0", cliente);
					sairDaThread(false);
					break;

				case OK:
					printf("Log: Comando bem-sucedido (Server Thread)\n");
					enviaMensagemParaCliente("OK\0", cliente);
					break;

				case REQUISITANDO_LOGIN:/* Cliente NÃO AUTORIZADO OK */
					printf(" Cliente Requisitou Login, mas Não foi atendido (Server Thread)\n");
					enviaMensagemParaCliente("ERRO: Não autorizado, desconectando\0", cliente);
					sairDaThread(true);
					break;

				case REQUISITANDO_ADICAO:/* OK */
					printf(" Cliente requisitou adicao\n");
					enviaMensagemParaCliente("ERRO: Adicao recusada, desconectando\0", cliente);
					sairDaThread(false);
					break;

				case LOGIN_NAO_AUTORIZADO:
					printf(" Warning: Falha ao logar em Server.h Servidor()\n");
					enviaMensagemParaCliente("ERRO: usuario ou senha incorretos, desconectando.", cliente);
					sairDaThread(false);
					break;

				case ERRO_DE_EXECUCAO:
					printf(" Warning: Falha ao executar comando em Server.h Servidor()\n");
					enviaMensagemParaCliente("ERRO: Falha na execucao do comando, desconectando.", cliente);
					sairDaThread(false);
					break;

				case REQUISITANDO_OBTENCAO:
					mensagem = comandoObter(&usuario);
					interpretando = false;
//					mensagem = RETORNO_ERRO_INTERNO_STR_DINAMICA;
//					write( *(int *)cliente, mensagem, strlen(mensagem) +1);
					enviaMensagemParaCliente(mensagem, cliente);
//					liberar(mensagem);
					free(mensagem);
					break;

				case REQUISITANDO_REMOCAO:
					printf(" Warning: Falha ao concluir comando de remocao em Server.h Servidor()\n");
					enviaMensagemParaCliente("Remoção Negada\nainda conectado\0", cliente);
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
						enviaMensagemParaCliente("Você nn pode fazer isso, muahahahaha \\o/\0", cliente);
						sairDaThread(true);
					}
					break;

				default:
					printf(" Não foi possivel interpretar comando (Server.h) (Servidor()) Resultado == %d\n", resultado );
					enviaMensagemParaCliente("Comando não compreendido, desconectando\0", cliente);
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
