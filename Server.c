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




int main()
{
	system("clear");
	int x =0;
	printf(" Iniciando servidor\n");

	printf(" Configurando servidor.");
	int sockfd = configuraServidor();
	printf("\r Configurando servidor..................... OK");

	printf(" Aguardando conexão\n");

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
			exit(1);
		}

		if(pthread_create(&thread, NULL, Servidor, (void *)&clienteSockfd) != 0)
		{
			printf(" Falha ao tentar criar Thread\n");
			printf(" Provavel limite de threads simultâneas atingido\n");
			exit(1);
		}
		x++;
		printf("\t\t\t\t\t\t\t\t\t\t\t\t Cliente %d aceito\n", x);
		pthread_detach(thread);/* Procurar o que essa função faz depois */
	}
	exit(0);
}

