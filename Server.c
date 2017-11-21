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

bool inicializaMonitorDeBancoDeDados();


int main(void)
{
	system("clear");
	int x =0;
	printf(" LOG: Iniciando servidor em Server.c main()\n");

	printf(" LOG: Configurando servidor.");
	int sockfd = configuraServidor();
	printf("\r LOG: Configurando servidor............................ OK em Server.c main()\n");

	printf(" LOG: Inicializando monitor de banco de dados (ruby) ");
	if(inicializaMonitorDeBancoDeDados())
	{
		printf(" OK em Server.c main()\n");
	}
	else
	{
		printf(" ERRO ao inicalizar monitor de Base de Dados em Server.c main()\n");
	}

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


void limpaBuffers(void)// Funfou !!!
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

void *threadMonitoraDoBancoDeDados(void *args)
{
	if(args != NULL)
	{
		printf(" Warning: DEU PAU na thread de monitorar o banco de dados kkk\n");
		pthread_exit( NULL );
	}
	else
	{
		printf(" Inicializar o ruby aqui\n");
		system(COMANDO_MONITOR_DO_BANCO_DE_DADOS_RUBY);
		pthread_exit( NULL );
	}
	printf(" Vai que.... threadMonitoraDoBancoDeDados()\n");
	pthread_exit( NULL );
}

bool inicializaMonitorDeBancoDeDados()/* Abre uma thread e executa o monitor do banco de dados */
{
	pthread_t identificadorDaThread;
	if(pthread_create(&identificadorDaThread, NULL, threadMonitoraDoBancoDeDados, NULL))
	{
		printf(" Warning: Falha ao criar thread para monitorar o banco de dados em inicializaMonitorDeBancoDeDados() Server.c\n");
		return false;
	}
	else
	{
		printf(" LOG: Thread criada para monitorar o banco de dados em inicializaMonitorDeBancoDeDados() Server.c\n");
		pthread_detach(identificadorDaThread);
		return true;
	}
	return false;
}
