#include <string.h>
#include <unistd.h>

#define BUFFER_CLIENTE 1024//(aceitou 1024*1024 (1MB))

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





int abreSocket()
{
	int numeroDeTentativas = 10;
	int contador = 0;
	int sockfd = -1;
	int tempoEntreTentativas = 1;
	for(contador = 0; contador<numeroDeTentativas || sockfd < 0; contador++)
	{
		sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if( sockfd < 0)
		{
			printf(" Warning: Falha ao abrir socket detectada em Server.h abreSocket(), tentativa nº %d\n", contador);
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

	printf(" sockfd = %d\n", sockfd);

	for(contador = 0; contador<numeroDeTentativas && bindEstabelecido < 0; contador++)
	{
		bindEstabelecido = bind(sockfd, serverAddr, sizeof(struct sockaddr));
		if(bindEstabelecido<0)
		{
			printf(" Warning: Falha ao fazer bind, em Server.h fazerBind(), tentativa nº %d\n", contador);
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
		printf(" Falha ao iniciar listener\n");
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
	
	memset(bufferCliente, '\0', BUFFER_CLIENTE);
	memset(mensagemAnterior, '\0', BUFFER_CLIENTE);

	printf(" Aguardando por mensagens\n");
	while(true)
	{
		if(bufferCliente[0] != '\0')// tentativa de otimização
		{
			strcpy(mensagemAnterior, bufferCliente);
			memset(bufferCliente, '\0', BUFFER_CLIENTE);
			if(mensagem != NULL)
			{
				mensagem = NULL;
			}
		}


		read(sockEntrada, bufferCliente, sizeof(bufferCliente));
		printf(" RECEBIDO: |%s|\n", bufferCliente);


		//OTIMIZAR AQUI !!!!
		if(strcmp(bufferCliente, "APP sair") != 0)/* Se cliente não pedir pra sair */
		{
			if(strcmp(bufferCliente, "") == 0)
			{
				SAIR_DA_THREAD:
				interpretando = false;
				printf(" LOG: Cliente não escreveu nada, então saiu, em Server.h Servidor()\n");
				close(sockEntrada);
				liberaMemoriaTalvezUtilizada(email);
				pthread_exit( (void *) 0);
			}
			else
			{
				printf(" LOG: Cliente não saiu\n");
				if(mensagemAnterior != NULL)
				{
					if(strcmp(bufferCliente,mensagemAnterior) == 0)
					{
						printf("\t\tMensagens Repetidas, Saindo\n");
						goto SAIR_DA_THREAD;//CUIDADO AQUI
					}
				}

				printf(" LOG: tAguardando liberacao para interpretacao\n");
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
						mensagem = obterDados(email);
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
