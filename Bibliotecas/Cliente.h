#include <stdio.h>
#include <string.h>//strlen...
#include <unistd.h>//Close  (para fechar o socket depois de abrir..... é....)
#include <sys/socket.h>//AF_INET....
#include <arpa/inet.h>//send, recv...
#include <netdb.h>//hostent (Para descobrir o ip de um server)
#include "../AdaptadorDeString/AdaptadorDeString.h"
#include "../Comandos/Comandos.h"

// #include <curl/curl.h>



/** 
 * @brief  Conecta-se ao endereço IP especificado, na porta 80, e então retorna o descritor do arquivo
 * @note   o sockFD, explicado mais especificamente no connectToAddress()
 * @param  *enderecoIP: endereço IP a ser conectado na porta 80 (Padrão WEB)
 * @retval int sockFD
 */
int connectToHTTPServer(char *enderecoIP)
{
	// static const char *localizacao = "Cliente.h connectToHTTPServer(char *enderecoIP)";
	int socketFileDescriptor;
	struct sockaddr_in server;// Estrutura que contém os dados do servidor que eu desejo acessar

	//Conexão a servidor remoto
	geraLog(LOG, "Criando socket");
	socketFileDescriptor = socket(PF_INET, SOCK_STREAM, 0);	//PF_INET == usando IPV4
	
	if (socketFileDescriptor == -1)
	{
		geraLog(ERRO, "Falha ao criar socket");
		return 0;
	}
	else
	{
		geraLog(LOG, "socket criado");
	}
	
	geraLog(LOG, "Configurando servidor");
	server.sin_family = AF_INET; // IPV4
	server.sin_addr.s_addr = inet_addr(enderecoIP);
	
	server.sin_port = htons(PORTA_PADRAO_WEB);
	geraLog(LOG, "Servidor configurado");

	
	geraLog(LOG, "Conectando ao servidor");
	// if( connect( socketFileDescriptor, (const struct sockaddr *)&server, sizeof(server) ) < 0 )// estabelece conexão
	if (connect(socketFileDescriptor, (struct sockaddr *) &server, sizeof (server)) < 0)
	{
		geraLog(ERRO, "Falha ao estabelecer conexão com o servidor");
		return 0;
	}
	else
	{
		geraLog(LOG, "Conectado com sucesso ao servidor WEB");
	}

	return socketFileDescriptor;
}



/** 
 * @brief  Conecta à URL especificada, e então retorna um FD (File Descriptor) (da tabela descritora de arquivos abertos por processo) para a conexão como um inteiro.
 * @note   Pensa que ele retorna um ponteiro pra conexão, mas isso é um inteiro
 * @param  *pageURL: a URL a ser aberta a conexão
 * @retval 
 */
int connectToAddress(char *pageURL)
{
	// static const char *localizacao = "Cliente.h connectToAddress(char *pageURL)";// Localização (utilizada pra logs)
	int socketFd;// Socket File Descriptor
	
	char enderecoIP[15];// O endereço IP da pageURL recebida (descoberto aqui, por sinal)
	struct hostent *enderecosPossiveis;//Os possíveis endereços que um hostname pode obter	
	struct in_addr **listaDeEnderecos;// Lista de endereços que respondem pela pageURL especificada
	int i;//contador,normal


	if (pageURL == NULL)
	{
		geraLog(ERRO, "URL Nula detectada");
		return 0;
	}

	if ( (enderecosPossiveis = gethostbyname( pageURL )) == NULL)
	{
		//Falha ao obter endereço
		geraLog(ERRO, "Falha ao obter endereco especificado");
		return 0;
	}
	//Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only

	listaDeEnderecos = (struct in_addr **) enderecosPossiveis->h_addr_list;
	for(i = 0; listaDeEnderecos[i] != NULL; i++)
	{
		//retorna a primeira ocorrência;
		strcpy(enderecoIP , inet_ntoa(*listaDeEnderecos[i]) );
	}
	geraLog(LOG, "Endereço resolvido");
	printf("\t|%s| resolvido para : |%s|\n" , pageURL , enderecoIP);

	socketFd = connectToHTTPServer(enderecoIP);
	
	if (socketFd == 0)
	{
		geraLog(ERRO, "Falha ao conectar-se ao servidor HTTP");
		return 0;
	}

	geraLog(LOG, "Conectado com sucesso ao servidor WEB");
	return socketFd;
}


/** 
 * @brief  Envia uma solicitação HTTP ao servidor já conectado
 * @note   o socket precisa estar conectado antes
 * @param  socketFileDescriptor: socket aberto e conectado previamente
 * @param  *page: Página a ser aberta
 * @retval bool (enviou ou não)
 */
bool sendHTTPRequest(int socketFileDescriptor, char *page)
{
	if (socketFileDescriptor == 0)
	{
		return false;
	}

	static const char *header = "GET  HTTP/1.0\r\n\r\n";


	int tamanho = strlen(page) + 1 + strlen(header);
	char *requisicao = (char *)calloc(sizeof(char), tamanho);
	if (requisicao == NULL)
	{
		geraLog(WARNING, "Falha ao alocar memoria para string");
		return false;
	}
	// https:///brasil/sp/monte-aprazivel/panorama

	snprintf(requisicao, tamanho, "GET %s HTTP/1.0\r\n\r\n", page);
	if (requisicao == NULL)
	{
		geraLog(WARNING, "Falha ao formatar string pra ser enviada");
		return false;
	}
	geraLog(DEBUG, "Requisição formatada");
	printf("\t|%s|\n", requisicao);

	int quantidadeEscrita = write(socketFileDescriptor, requisicao, tamanho);

	free(requisicao);
	requisicao = NULL;
	geraLog(LOG, "Quantidade de dados escrita");
	printf("\tEsperado = %d\n", tamanho);
	printf("\tEscrito = %d\n", quantidadeEscrita);

	if (tamanho == quantidadeEscrita)
	{
		geraLog(LOG, "Dados escritos com sucesso, necessário captar resposta");
		return true;
	}
	else
	{
		geraLog(ERRO, "Pode ser que dados corrompidos tenham sido enviados ao servidor");
		return false;
	}
}


/** 
 * @brief  Recebe uma quantidade de bytes do servidor
 * @note   retorna o conteúdo recebido alocando memoria pra isso
 * @param  fd: File Descriptor (o socket (conexão))
 * @param  quantidadeDeBytes: quantidade de bytes a ser lida
 * @retval o conteúdo
 */
char *receiveFromServer(int fd, int quantidadeDeBytes)
{
	static const char *localizacao = "Cliente.h receiveFromServer(int fd, int quantidadeDeBytes)";
	if (quantidadeDeBytes <= 0)
	{
		geraLog(ERRO, "Quantidade de bytes menor ou igual a zerro");
		return NULL;
	}
	if (fd <= 0)
	{
		geraLog(ERRO, "Descritor de arquivos menor ou igual a zerro");
		return NULL;
	}
	char *buffer = (char *)calloc(sizeof(char), quantidadeDeBytes);
	if (buffer == NULL)
	{
		geraLog(ERRO, "Falha ao alocar memoria para buffer");
		return NULL;
	}
	int quantidadeLida = read(fd, buffer, quantidadeDeBytes);

	return buffer;
}


/** 
 * @brief  Abre uma conexão com a URL estabelecida, obtem a página em HTML, fecha a conexão e retorna a página
 * @note   pode dar trabalho
 * @param  *pageURL: A URL da página solicitada
 * @retval a página inteira
 */
char *getHTTPPage(char *pageURL)
{
	static const char *localizacao = "Cliente.h getHTTPPage(char *pageURL)";
	if (pageURL == NULL)
	{
		geraLog(ERRO, "URL Nula detectada");
		return NULL;
	}

	char *enderecoURL = obterEnderecoDadoURL(pageURL);

	int conexao = connectToAddress(enderecoURL);

	free(enderecoURL);
	enderecoURL = NULL;

	if (conexao == 0)
	{
		geraLog(ERRO, "Não foi possível abrir a conexao com a pagina");
		return NULL;
	}
	geraLog(LOG, "Conexão efetuada com sucesso");


	printf("   DEBUG: endereço completo = |%s|\n", pageURL);

	char *enderecoPaginaHtml = obterPaginaDadoURL(pageURL);

	geraLog(DEBUG, "Endereço da página HTML é");
	printf(" \t |%s|\n", enderecoPaginaHtml);


	if (sendHTTPRequest(conexao , enderecoPaginaHtml))
	{
		geraLog(LOG, "Deu BOM, requisicao HTTP enviada");
	}
	else
	{
		geraLog(ERRO, "Deu ruim, falha ao enviar requisição http");
		return NULL;
	}

	char *recebido = NULL;
	int tamanho = 1;
	char *retorno = (char *)calloc(sizeof(char), tamanho);

	while(true)
	{
		recebido = receiveFromServer(conexao, 1024);
		printf("Recebido = |||%s|||\n", recebido);
		tamanho += strlen(recebido);
		retorno = (char *)realloc(retorno, tamanho);
		if (retorno == NULL)
		{
			geraLog(ERRO, "Falha ao realocar memoria para retorno");
			return NULL;
		}
		strcat(retorno, recebido);

		if (strlen(recebido) <= 0)
		{
			free(recebido);
			recebido = NULL;
			break;
		}
		free(recebido);
		recebido = NULL;
		
	}

	return retorno;

}