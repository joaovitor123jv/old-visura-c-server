#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>



#define TAMANHO_DA_RESPOSTA 1024
#define IP_DESTINO "127.0.0.1"
#define PORTA 7420

int main(int argc, char* argv[])
{
	int socket_desc; //Socket Description (descrição do socket)
	struct sockaddr_in server; //Struct que contém os dados do servidor o qual desejo acessar
	int x = 0;
	int y = 0;
	
	
	
	char message[1024];//Mensagem Enviada
	char server_reply[TAMANHO_DA_RESPOSTA];//Resposta do servidor
	
	printf(" Preparando cliente\n");
	
	
	
	//Conexão a servidor remoto
	socket_desc = socket(PF_INET, SOCK_STREAM, 0);	//AF_INET == usando IPV4
	
	printf(" Socket criado\n");
	
	//Cria o Socket
	if(socket_desc == -1)
	{
		printf("Falha ao criar socket\n");
		return -1;
	}
	
	
	//Configurações
	printf(" Configurando Servidor\n");
	
	server.sin_addr.s_addr = inet_addr(IP_DESTINO);//Endereço IP desse server
	server.sin_family = AF_INET;//Usando IPV4
	server.sin_port = htons( PORTA );//Define a porta que vai chamar
	
	
	printf(" conectando ao Servidor\n");
	if( connect( socket_desc, (const struct sockaddr *)&server, sizeof(server) ) < 0 )
	{
		printf("Erro de conexão\n");
		return 1;
	}
	else
	{
		printf("Conectado !\n");
	}
	
	
	while(1)
	{
		printf(" Digite a mensagem que deseja enviar ao Servidor\n");
		scanf("%[^\n]s", message);
		setbuf(stdin, NULL);
		
		if(strcmp(message, "exit") == 0)
		{
			break;
		}
		if(send(socket_desc, message, (sizeof(char)* (strlen(message) )) , 0) < 0) //Envia dados ao servidor
		{
			printf("Erro: Não foi possível enviar a mensagem ao servidor");
			return -2;
		}
		else
		{
			printf("\tDados enviados !\n");
		}
		
		printf(" \t\tESPERANDO POR RESPOSTA\n");
		memset(server_reply, '\0', strlen(server_reply));
		
		if(recv(socket_desc, server_reply, TAMANHO_DA_RESPOSTA, 0)<0) //Busca a resposta do servidor
		{
			printf("\tFalha ao receber dados\n");
		}
		else
		{
			printf("\n\t\aResposta recebida!!!\n\n\n");
			printf("\n%s\n", server_reply);
		}
	}
	
	
	//SAIR
	strcpy(message, "APP sair\0");
	if(send(socket_desc, message, strlen(message), 0) < 0) //Envia dados ao servidor
	{
		printf("Erro: Não foi possível enviar a mensagem ao servidor");
		return -2;
	}
	else
	{
		printf("\tDados enviados !\n");
	}
	
	memset(server_reply, '\0', strlen(server_reply));
	if(recv(socket_desc, server_reply, TAMANHO_DA_RESPOSTA, 0)<0) //Busca a resposta do servidor
	{
		printf("\tFalha ao receber dados\n");
	}
	else
	{
		printf("\n\t\aResposta recebida!!!\n\n\n");
		printf("\n%s\n", server_reply);
	}
	close(socket_desc);//Fecha o Socket
	
	
	return 0;
}


