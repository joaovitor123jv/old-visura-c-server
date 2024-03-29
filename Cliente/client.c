#include <stdio.h>//printf ....
#include <string.h>//strlen...
#include <unistd.h>//Close  (para fechar o socket depois de abrir..... é....)
#include <sys/socket.h>//AF_INET....
#include <arpa/inet.h>//send, recv...
#include <netdb.h>//hostent (Para descobrir o ip de um server)
#include "Criptografia.h"


#define TAMANHO_DA_RESPOSTA 1024
//#define IP_DESTINO "192.168.1.13"
#define IP_DESTINO "127.0.0.1"
//#define IP_DESTINO "104.198.132.185" //GOOGLE CLOUD IP
//#define PORTA 80
#define PORTA 7420

int main(int argc, char* argv[])
{
	int socket_desc; //Socket Description (descrição do socket)
	struct sockaddr_in server; //Struct que contém os dados do servidor o qual desejo acessar
	int x = 0;
	int y = 0;



	char* message;//Mensagem Enviada
	char server_reply[TAMANHO_DA_RESPOSTA];//Resposta do servidor

	//Opcionais
	/*
	   char *hostname = "projetopropaganda.ddns.net";//hostname do qual queremos descobrir o ip
	   char ip[15];//Endereço Ip final
	   struct hostent *he;//Os possíveis endereços que um hostname pode obter
	   struct in_addr **addr_list;//Lista dos endereços disponíveis para o hostname
	   int i;//contador
	   if ( (he = gethostbyname( hostname ) ) == NULL)
	   {
			//gethostbyname failed
			printf("gethostbyname ERROR\n");
			return 1;
		}
		//Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
		addr_list = (struct in_addr **) he->h_addr_list;
		for(i = 0; addr_list[i] != NULL; i++)
		{
			//Return the first one;
			strcpy(ip , inet_ntoa(*addr_list[i]) );
		}
		printf("%s resolved to : %s\n" , hostname , ip);
		//Fim Opcionais
	 */



	
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


	message = "APP 1 interface@servidor senatauri";//Login
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



//message = "APP 2 2 cocacomnos 101\0";//Adiciona 512 visualizacoes ao produto cocacomnos
//message = "APP 4 MIGqAgEAAiEArPXCV6pZjBh+OKNVEM/+eIb5KgGZw24jX2mfu2GzFPcCAwEAAQIgCx8KTgTAGUgB5bZq49fb7tzxlVPJya47G9dtFcNrZ7ECEQDgJPU9398P6I0R5MCV+SeVAhEAxYqPkCR6yJ2YuCUZI903WwIRAMmp9swYo8TYYjP7MBq+9JECECeNYMVZqIP2mhd0xh3aMzUCEBGnaCJnmYNciQfqIT1vmws= $ { Auriflama SP\0";//Obtem ID da cidade Auriflama, no estado de SP
//message = "APP 4 J\0";// Obtem ultimos 10 produtos adicionados
//message = "APP 4 Q * cocacolavc";// Requisita obtenção de descrição de produto para o produto de ID "cocacolavc", retorna ERRO, quando não encontra
//message = "APP 4 1. cocacolavc";// Requisita obtenção de nome de produto para o produto de ID "cocacolavc", retorna ERRO, quando não encontra



//message = "APP 2 1 2 MIGqAgEAAiEArPXCV6pZjBh+OKNVEM/+eIb5KgGZw24jX2mfu2GzFPcCAwEAAQIgCx8KTgTAGUgB5bZq49fb7tzxlVPJya47G9dtFcNrZ7ECEQDgJPU9398P6I0R5MCV+SeVAhEAxYqPkCR6yJ2YuCUZI903WwIRAMmp9swYo8TYYjP7MBq+9JECECeNYMVZqIP2mhd0xh3aMzUCEBGnaCJnmYNciQfqIT1vmws= 7 joaovitor 9 nulo\0";// adiciona usuario anonimo ao banco
//message = "APP 2 l0 $ 10 L 12654267 RN nN b nomeDoBairro M Cx$Postal$11\0";// adiciona localizacao ao banco
//
//message = "APP 2 $C MIGqAgEAAiEArPXCV6pZjBh+OKNVEM/+eIb5KgGZw24jX2mfu2GzFPcCAwEAAQIgCx8KTgTAGUgB5bZq49fb7tzxlVPJya47G9dtFcNrZ7ECEQDgJPU9398P6I0R5MCV+SeVAhEAxYqPkCR6yJ2YuCUZI903WwIRAMmp9swYo8TYYjP7MBq+9JECECeNYMVZqIP2mhd0xh3aMzUCEBGnaCJnmYNciQfqIT1vmws= # Adicionado_Pela_Interface 12346578954120 10 teste-da-coca-cola@gmail.com > 21445122540 7\0";//Adiciona um contratante ao banco 
message = "APP 2 + MIGqAgEAAiEArPXCV6pZjBh+OKNVEM/+eIb5KgGZw24jX2mfu2GzFPcCAwEAAQIgCx8KTgTAGUgB5bZq49fb7tzxlVPJya47G9dtFcNrZ7ECEQDgJPU9398P6I0R5MCV+SeVAhEAxYqPkCR6yJ2YuCUZI903WwIRAMmp9swYo8TYYjP7MBq+9JECECeNYMVZqIP2mhd0xh3aMzUCEBGnaCJnmYNciQfqIT1vmws= ; 2 apqiieujah 10 Olha-que-nome-reideicoulou-queu-eu-coulouqueui Olha_A-Descricao_Aqui!!!\0";// Adiciona produt ao banco
printf(" MENSAGEM ENVIADA AO SERVER: |||%s|||\n",message);

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


//SAIR
	message = "APP sair";
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



	// int socket_desc; //Socket Description (descrição do socket)

	// socket_desc = socket(PF_INET, SOCK_STREAM, 0);	//AF_INET == usando IPV4
	// //SOCK_STREAM == Use TCP (Protocolo orientado a TCP)
	// //IPPROTO_IP ou 0 == Use protocolo IP


	// //Cria o Socket
	// if(socket_desc == -1)
	// {
	// 	printf("Falha ao criar socket\n");
	// 	return -1;
	// }

// }
	return 0;
}


