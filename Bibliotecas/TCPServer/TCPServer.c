#include "TCPServer.h"


struct TCPServer
{
	int sockfd;//O socket file descriptor
	int port;
	struct sockaddr_in serverAddr;
	int quantidadeDeClientes;
};

bool TCPServer_CONFIGURED = false;

/** 
 * @brief  Retorna o sockfd
 * @note   
 * @retval 
 */
int tcpServer_abreSocket()
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
			printf(" Warning: Falha ao abrir socket detectada em TCPServer.c abreSocket()");
			// printf(" Warning: Falha ao abrir socket detectada em Server.h abreSocket(), tentativa nº %d\n", contador);
		}
		else
		{
			break;
		}
		sleep(tempoEntreTentativas);
	}
	if(sockfd < 0)
	{
		printf(" ERRO: Numero maximo de tentativas para abrir socket detectado, encerrando  %s → %s:%d\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}
	else
	{
		printf(" LOG: Socket configurado, pronto pra estabelecer conexao em %s → %s:%d\n", __FILE__, __FUNCTION__, __LINE__);
		return sockfd;
	}
}

int tcpServer_fazerBind(int sockfd, struct sockaddr *serverAddr)
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
			printf(" Warning: Falha ao fazer bind, em %s → %s:%d, tentativa = %d\n", __FILE__, __FUNCTION__, __LINE__, contador);
		}
		else
		{
			break;
		}
		sleep(tempoEntreTentativas);
	}
	if(bindEstabelecido < 0)
	{
		printf(" ERRO: Fatal, falha ao estabelecer conexão entre aplicação e socket, em %s → %s:%d\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}
	else
	{
		printf(" LOG: Bind estabelecido com sucesso em %s → %s:%d\n", __FILE__, __FUNCTION__, __LINE__);
		return 0;
	}
}


TCPServer *newTCPServer(int port)
{
	if (port <= 0)
	{
		return NULL;
	}
	int i = 0;
	int choque = false;

	if (TCPServer_CONFIGURED == true)
	{
		do
		{
			for(i = 0; TCPServer_CONTROL.numOfPortsUsed;i++)
			{
				if (TCPServer_CONTROL.usedPort[i] == port)
				{
					choque = true;
				}
			}
		} while(choque);

		TCPServer_CONTROL.numOfPortsUsed = TCPServer_CONTROL.numOfPortsUsed + 1;
		TCPServer_CONTROL.usedPort = (int *)realloc(TCPServer_CONTROL.usedPort, TCPServer_CONTROL.numOfPortsUsed * sizeof(int));
		if (TCPServer_CONTROL.usedPort == NULL)
		{
			TCPServer_CONFIGURED = false;
			TCPServer_CONTROL.numOfPortsUsed = 0;
			return NULL;
		}
		TCPServer_CONTROL.usedPort[TCPServer_CONTROL.numOfPortsUsed-1] = port;
		TCPServer_CONFIGURED = true;
	}
	else
	{
		TCPServer_CONTROL.numOfPortsUsed = 1;
		TCPServer_CONTROL.usedPort = (int *)malloc(sizeof(int));
		if (TCPServer_CONTROL.usedPort == NULL)
		{
			return NULL;
		}
		TCPServer_CONTROL.usedPort[0] = port;
	}

	

	TCPServer *server = (TCPServer *)calloc(sizeof(TCPServer), 1);
	if (server == NULL)
	{
		return NULL;
	}
	server->port = port;
	server->sockfd = tcpServer_abreSocket();
	if (server->sockfd <= 0)
	{
		freeTCPServer(server);
		return NULL;
	}
	memset(&server->serverAddr, 0, sizeof(server->serverAddr));


	server->serverAddr.sin_family = AF_INET;
	server->serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	server->serverAddr.sin_port = htons(server->port);

	if(tcpServer_fazerBind(server->sockfd, (struct sockaddr *)&server->serverAddr) < 0 )
	{
		printf(" ERRO: Falha ao fazer bind em %s → %s:%d\n", __FILE__, __FUNCTION__, __LINE__);
		freeTCPServer(server);
		return NULL;
	}

	if(listen(server->sockfd, server->quantidadeDeClientes) < 0)
	{
		printf(" ERRO: Falha ao iniciar listener em %s → %s:%d\n", __FILE__, __FUNCTION__, __LINE__);
		freeTCPServer(server);
		return NULL;
	}

	return server;
}

bool freeTCPServer(TCPServer *server)
{
	if (server == NULL)
	{
		return false;
	}

	if (server->sockfd >= 0)
	{
		close(server->sockfd);
		server->sockfd = 0;
	}
	
	
	bool encontrou = false;
	int i = 0;
	int posicao;
	do
	{
		if (server->port == TCPServer_CONTROL.usedPort[i])
		{
			encontrou = true;
			posicao = i;
		}
		i = i + 1;
	} while(!encontrou);


	for(i = posicao;i < TCPServer_CONTROL.numOfPortsUsed;i++)
	{
		if (i+1 < TCPServer_CONTROL.numOfPortsUsed)
		{
			TCPServer_CONTROL.usedPort[i] = TCPServer_CONTROL.usedPort[i+1];
		}
	}

	TCPServer_CONTROL.usedPort = (int *)realloc(TCPServer_CONTROL.usedPort, sizeof(int) * (TCPServer_CONTROL.numOfPortsUsed - 1));

	TCPServer_CONTROL.numOfPortsUsed = TCPServer_CONTROL.numOfPortsUsed - 1;

	server->port = 0;

	free(server);
	server = NULL;
	return true;
}



bool tcpServer_recebeMensagemDeCliente(char *mensagem, int cliente)
{
	memset(mensagem, '\0', 1024);
	int quantidadeDeBytesLida = read(cliente, mensagem, 1024);	
	if (quantidadeDeBytesLida == 0)
	{	
		return false;
	}
	// printf(" LOG: Mensagem recebida |%s|, em Server.h recebeMensagemDeCliente()\n", mensagem);
	return true;
}

char *tcpServer_receiveMessage(TCPServer *server)
{
	if (server == NULL)
	{
		return NULL;
	}
	if (server->sockfd <= 0)
	{
		return NULL;
	}

	int clientSockFd = 0;
	struct sockaddr_in clienteAddr;
	unsigned int clntLen;
	clntLen = sizeof(clienteAddr);

	clientSockFd = accept(server->sockfd, (struct sockaddr *)&clienteAddr, &clntLen);

	if (clientSockFd < 0)
	{
		return NULL;
	}

	char *mensagem = calloc(sizeof(char), 1024);
	if (mensagem == NULL)
	{
		return NULL;
	}
	tcpServer_recebeMensagemDeCliente(mensagem, clientSockFd);

	close(clientSockFd);

	return mensagem;
}