// char *email = "NULO";
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include "Server.h"

void limpaBuffers(void) __attribute__ ((destructor));

int main(void)
{
	system("clear");
	int x =0;
	printf(" LOG: Iniciando servidor em Server.c main()\n");

	printf(" LOG: Configurando servidor.");
	int sockfd = configuraServidor();
	printf("\r LOG: Configurando servidor............................ OK em Server.c main()");

	printf(" LOG: Aguardando conexão em Server.c main()\n");

	while(true)
	{
		int clienteSockfd;
		struct sockaddr_in clienteAddr;
		unsigned int clntLen;

		clntLen = sizeof(clienteAddr);

		pthread_t thread;

		if((clienteSockfd = accept(sockfd, (struct sockaddr *)&clienteAddr, &clntLen)) < 0)
		{
			printf(" Erro ao aceitar cliente\n");
			return 0;
		}

		if(pthread_create(&thread, NULL, Servidor, (void *)&clienteSockfd) != 0)
		{
			printf(" Falha ao tentar criar Thread\n");
			printf(" Provavel limite de threads simultâneas atingido\n");
			return 0;
		}
		x++;
		printf(" LOG:\t\t\t\t\t\t\t\t\t\t\t\t Cliente %d aceito em Server.c main(void)\n", x);
		pthread_detach(thread);/* Procurar o que essa função faz depois */
	}
	return 0;
}


void limpaBuffers(void)// Nunca foi chamada, mas vai que...
{
	printf(" LOG: Liberando memoria residual utilizada em Server.h limpaBuffers() \n");
	if(conexao != NULL)
	{
		printf(" LOG: Liberando conexão com o banco de dados em Server.c limpaBuffers()\n");
		mysql_close(conexao);
		mysql_thread_end();
		free(conexao);
		conexao = NULL;
		printf(" LOG: Conexão com o banco de dados liberada em Server.c limpaBuffers()\n");
	}
	printf(" LOG: Buffers Limpos com sucesso em Server.c void limpaBuffers(void) __attribute__ ((destructor));\n");
}