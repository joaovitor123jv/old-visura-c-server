estrutura geral de comando:
	NOMEAPLICACAO TIPODECOMANDO Argumentos



PARA FAZER LOGIN
	Login:
		NOMEAPLICACAO 1 login senha
		
	obs: se login não tiver nenhum "@" em toda a string, será identificado automagicamente como usuario anonimo

PARA ADICIONAR COISAS
	Usuario:
		Anonimo:			
			NOMEAPLICACAO 2 1 1 CHAVEPHP 7 login 9 senha
				ou
			NOMEAPLICACAO 2 1 1 CHAVEPHP 9 senha 7 login
		Registrado:
			NOMEAPLICACAO 2 1 2 CHAVEPHP
		
	Visualizacoes:
		NOMEAPLICACAO 2 2 cocacolavc 10
			que é
		NOMEAPLICACAO 2 2 idProduto quantidadeAAdicionar
		
	Cidade:
		NOMEAPLICACAO 2 $ 3125664 L 15350000
			que é
		NOMEAPLICACAO TIPO_ADICIONAR TIPO_ID_CIDADE idCidade TIPO_CEP cep
		
	Localizacao:
		NOMEAPLICACAO 2 
	
	
	
LEGENDA:
	1 = Login
	2 = Adicionar















CHAVE_DE_SEGURANCA_PHP "MIGqAgEAAiEArPXCV6pZjBh+OKNVEM/+eIb5KgGZw24jX2mfu2GzFPcCAwEAAQIgCx8KTgTAGUgB5bZq49fb7tzxlVPJya47G9dtFcNrZ7ECEQDgJPU9398P6I0R5MCV+SeVAhEAxYqPkCR6yJ2YuCUZI903WwIRAMmp9swYo8TYYjP7MBq+9JECECeNYMVZqIP2mhd0xh3aMzUCEBGnaCJnmYNciQfqIT1vmws="

/********************************************************************/



TAMANHO_LOGIN 64
TAMANHO_SENHA 256
TAMANHO_SENHA_PADRAO_USUARIO_ANONIMO 4



REQUISITANDO_LOGIN 1
REQUISITANDO_ATUALIZACAO 2
REQUISITANDO_ESPECIFICO 3
REQUISITANDO_ADICAO 4

OPERACAO_EXECUTADA_COM_SUCESSO 5



/* Constantes de Controle */
COMANDO_LOGIN 1
COMANDO_ADICIONAR 2
COMANDO_ATUALIZAR 3
COMANDO_OBTER 4

TIPO_USUARIO "1"
TIPO_USUARIO_ANONIMO "2"

TIPO_LOGIN "7"
TIPO_SENHA "9"


TIPO_CLIQUE "2" /* Tipo clique é a mesma coisa que tipo Visualizacao */
TIPO_VISUALIZACAO "2" /* Tipo visualização é usado em mais de um lugar com duas interpretações diferentes (como parametro por usuario anonimo ou não) */
TIPO_VISUALIZACAO_ANONIMA 3


TIPO_LOCALIZACAO "l0"

TIPO_NUMERO "n"
TIPO_BAIRRO "b"
TIPO_CEP "L"
TIPO_RUA "R"
TIPO_COMPLEMENTO "M"
TIPO_CIDADE "{"
TIPO_ESTADO "S"
TIPO_PAIS 'pa' /* sim, era pra ser país */

 
SENHA_PADRAO_USUARIO_ANONIMO "nulo"






/* Retornos entre funções */
RETORNO_OK 1
RETORNO_ERRO_DE_PARAMETRO 0
RETORNO_ERRO -1
RETORNO_ERRO_DE_CONEXAO -2
RETORNO_ERRO_FALTA_DE_MEMORIA -3
RETORNO_NULO -4




















