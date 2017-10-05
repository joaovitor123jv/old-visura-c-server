#include <string.h>
#include <unistd.h>//sleep

#define BUFFER_CLIENTE 1024//ANTERIOR 256 (aceitou 1024*1024 (1MB))

#include "interpretadorDeComandos.h"


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






int configuraServidor()
{
	printf(".");
	int sockfd;
	printf(".");
	int contador = 0;
	printf(".");
	struct sockaddr_in serverAddr;
	printf(".");


	if((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		printf(" Erro ao tentar criar socket \n");
		contador++;

		exit(1);
	}
	printf(".");
	/* Zera a estrutura */
	memset(&serverAddr, 0, sizeof(serverAddr));
	printf(".");

	serverAddr.sin_family = AF_INET;
	printf(".");
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	printf(".");
	serverAddr.sin_port = htons(PORTA);
	printf(".");
	int contAux = 1;

TENTAR_DENOVO:
	if( bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0 )
	{
		printf("\r Configurando servidor.......");
		fflush(stdout);
		while(contAux < contador)
		{
			fflush(stdout);
			printf(".");
			contAux++;
		}
		contAux = 0;
		printf("Erro ao fazer BIND (%d)", contador+1);
		if(contador < 9)
		{
			contador++;
			//printf("Tentando novamente\n");
			sleep(10);
			goto TENTAR_DENOVO;
		}
		printf("\r Não foi possível iniciar o servidor, (ERRO AO FAZER BIND) Encerrando\n");
		exit(1);
	}
	printf(".");

	/* Fica na escuta de até 5 clientes */
	if(listen(sockfd, QTD_CLIENTE) < 0)
	{
		printf(" Falha ao iniciar listener\n");
		exit(1);
	}
	printf(".");
	
	printf(".");
	if(conectarBanco())
	{
		printf(".");
	}
	else
	{
		printf("Erro ao conectar-se ao banco de dados\n");
		exit(1);
	}

	return sockfd;
}


void liberaMemoriaTalvezUtilizada(char *email)
{
	if(email != NULL)
	{
		free(email);
		email = NULL;
	}
}


void *Servidor(void *arg)
{
	char bufferCliente[BUFFER_CLIENTE];
	bool autorizado = false;
	char *email = NULL;
	int sockEntrada = *(int*)arg;
	char *mensagem = NULL;
	char mensagemAnterior[BUFFER_CLIENTE];
	int resultado;
	bool usuarioAnonimo = true;
	// int auxImediata = 0;
	//
	
	memset(bufferCliente, '\0', BUFFER_CLIENTE);
	memset(mensagemAnterior, '\0', BUFFER_CLIENTE);

	printf(" Aguardando por mensagens\n");
	while(true)
	{
		//if(strlen(bufferCliente) > 0)
		if(bufferCliente[0] != '\0')// tentativa de otimização
		{
			strcpy(mensagemAnterior, bufferCliente);
			memset(bufferCliente, '\0', BUFFER_CLIENTE);
			if(mensagem != NULL)
			{
				// printf("PASSO 1\n");
				// auxImediata = strlen(mensagem);
				// printf("PASSO 2\n");
				// memset(mensagem, '\0', auxImediata);
				// auxImediata = 0;
				// free(mensagem);
				mensagem = NULL;
			}
			printf("\t\t*************BUFFER_CLIENTE Limpo****************\n\n");
			// printf("COMECO DE TUDO →→→→→→→→→→→→→→→→→→→→→→→→→→→→→→  EMAIL = %s\n", email);
		}

		//OTIMIZAR AQUI !!!!

		read(sockEntrada, bufferCliente, sizeof(bufferCliente));
		printf(" RECEBIDO: |%s|\n", bufferCliente);
		if(strcmp(bufferCliente, "APP sair") != 0)/* Se cliente não pedir pra sair */
		{
			if(strcmp(bufferCliente, "") == 0)
			{
				SAIR_DA_THREAD:
				interpretando = false;
				printf(" Cliente não escreveu nada, então saiu\n");
				close(sockEntrada);
				liberaMemoriaTalvezUtilizada(email);
				pthread_exit( (void *) 0);
			}
			else
			{
				printf("Cliente não saiu\n");
				if(mensagemAnterior != NULL)
				{
					if(strcmp(bufferCliente,mensagemAnterior) == 0)
					{
						printf("\t\tMensagens Repetidas, Saindo\n");
						goto SAIR_DA_THREAD;//CUIDADO AQUI
					}
				}

				printf(" \tAguardando liberacao para interpretacao\n");
				while(interpretando)
				{
					pthread_yield();/* Causa um warning, mas nada demais */
				}//Aguarda, liberando cpu para outros trabalhos (senão fica fazendo coisa a toa até ser interrompida pelo SO)
				printf("\n\n \t*********************Inicio de Interpretação *************\n");


				if(email == NULL)
				{
					// printf("Email == NULL (Server.h)\n");
					email = interpretaComando(bufferCliente, &autorizado, &resultado, email, &usuarioAnonimo);
					if(email != NULL)
					{
						char *aux = email;
						aux = email;
						email = malloc(sizeof(char) * (strlen(aux) + 1));
						strcpy(email, aux);
					}
					else
					{
						printf("Warning: Email resetado !!!!!!\n");
						printf("\t\t\tOU\n");
						printf("\tUsuário não autorizado\n");
					}
				}
				else
				{
					interpretaComando(bufferCliente, &autorizado, &resultado, email, &usuarioAnonimo);
				}



				switch(resultado)
				{
					case ERRO:
						// interpretando = false;
						printf("Warning: Erro na execução do comando\n");
						mensagem = NULL;
						mensagem="ERRO, Desconectando\0";
						printf("Log: Mensagem a enviar = |%s|\n", mensagem);
						write(*(int *)arg, mensagem, strlen(mensagem) +1);
						close(sockEntrada);
						liberaMemoriaTalvezUtilizada(email);
						pthread_exit( (void *) 0);
						// exit(1);
						break;

					case OK:
						// interpretando = false;
						printf("Log: Comando bem-sucedido (Server Thread)\n");
						// mensagem = calloc(strlen("Comando bem-sucedido testanto o teste do teste do teste da testa que não sei se da certo\0"), sizeof(char));
						// strcpy(mensagem, "Comando bem-sucedido testanto o teste do teste do teste da testa que não sei se da certo\0");
						mensagem = NULL;
						mensagem = "OK\0";
						printf("Log: Mensagem a enviar = |%s|\n", mensagem);
						write(*(int *)arg, mensagem, strlen(mensagem) + 1);
						// exit(1);
						break;

					case REQUISITANDO_LOGIN:/* Cliente NÃO AUTORIZADO OK */
						interpretando = false;
						printf(" Cliente Requisitou Login, mas Não foi atendido (Server Thread)\n");
						// mensagem = calloc(strlen("Não autorizado\0"), sizeof(char));
						// strcpy(mensagem, "Não autorizado\0");
						mensagem = NULL;
						mensagem = "Não autorizado\0";
						write( *(int *)arg, mensagem, strlen(mensagem) +1);
						close(sockEntrada);
						liberaMemoriaTalvezUtilizada(email);
						pthread_exit( (void *) 0);
						break;

					case REQUISITANDO_ADICAO:/* OK */
						// interpretando = false;
						printf(" Cliente requisitou adicao\n");
						// mensagem = calloc(strlen("Adição requisitada mas não atendida\0"), sizeof(char));
						// strcpy(mensagem, "Adição requisitada mas não atendida\0");
						mensagem = NULL;
						mensagem = "Adição recusada\0";
						write( *(int *)arg, mensagem, strlen(mensagem) +1);
						break;
						
					case REQUISITANDO_OBTENCAO:
						mensagem = obterDados();
						interpretando = false;
						bool precisaLiberar = true;
						if(mensagem == NULL)
						{
							precisaLiberar = false;
							mensagem = RETORNO_ERRO_CHAR;
						}
						write( *(int *)arg, mensagem, strlen(mensagem) +1);
						if(precisaLiberar)
						{
							free(mensagem);
							mensagem = NULL;
						}
						break;

					default:
						// interpretando = false;
						printf(" Não foi possivel interpretar comando (Server.h) (Servidor()) Resultado == %d\n", resultado );
						// mensagem = calloc(strlen("ERRO: Comando não indentificado\0"), sizeof(char));
						mensagem = NULL;
						mensagem="Comando incorreto, Desconectando\0";
						// strcpy(mensagem, "ERRO: Comando não indentificado\0");
						printf(" Mensagem a enviar = |%s|\n", mensagem);
						write(*(int *)arg, mensagem, strlen(mensagem) +1);
						close(sockEntrada);
						liberaMemoriaTalvezUtilizada(email);
						pthread_exit( (void *) 0);
						// exit(1);
						break;
				}
			}
		}
		else
		{
			// interpretando = false;
			printf(" Cliente saiu\n");
			memset(mensagemAnterior, '\0', strlen(mensagemAnterior));
			// mensagem = calloc((strlen("Conexao encerrada") + 1), sizeof(char));
			// strcpy(mensagem, "Conex encerrada\0");
			mensagem = NULL;
			mensagem = "Conexao encerrada\0";
			write(*(int *)arg, mensagem, strlen(mensagem) + 1);
			close(sockEntrada);
			liberaMemoriaTalvezUtilizada(email);
			// free(mensagem);
			mensagem = NULL;
			pthread_exit( (void *) 0);
		}
	}
}
