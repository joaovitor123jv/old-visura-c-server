#pragma once
#include<stdio.h>
#include<string.h>

#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<pthread.h>
#include<unistd.h>


#ifndef bool
#define bool char
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif




struct TCPServer_Control
{
	int *usedPort;
	int numOfPortsUsed;
};

struct TCPServer_Control TCPServer_CONTROL;


typedef struct TCPServer TCPServer;

/** 
 * @brief  Cria um novo servidor TCP na porta especificada
 * @note   deve ser liberado. Aguarda até poder criar o servidor.
 * @param  port: a porta que o servidor deve escutar
 * @retval NULL caso falhe, TCPServer * caso sucesso
 */
TCPServer *newTCPServer(int port);

/** 
 * @brief  Libera o servidor, destruindo-o e liberando a porta para a criação de um novo servidor
 * @note   geralmente é o último comando utilizado quando trabalhando com um servidor
 * @param  *server: O ponteiro obtido na criação do TCPServer
 * @retval true caso sucesso, false caso falhe
 */
bool freeTCPServer(TCPServer *server);

/** 
 * @brief  Aguarda até receber uma mensagem, e retorna a mensagem recebida como ponteiro de caracter
 * @note   necessario liberar ponteiro de caracter
 * @param  *server: O ponteiro obtido na criação do TCPServer
 * @retval 
 */
char *tcpServer_receiveMessage(TCPServer *server);
