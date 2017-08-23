#include<stdio.h>
#include<mysql/mysql.h>

#define HOST "127.0.0.1"
#define USER "interface"
#define PASS "senha"
#define DB "teste"

int main()
{
	MYSQL conexao;
	MYSQL_RES *resposta;
	MYSQL_ROW linhas;
	MYSQL_FIELD *campos;

//	char query[] = "SELECT C.idcidade FROM cidade C JOIN estado E ON E.idestado=C.estado_idestado WHERE E.nome=\'SP\' AND C.nome=\'Auriflama\';";
//	char query[] = "SELECT C.idcidade FROM cidade C JOIN estado E ON E.idestado=C.estado_idestado WHERE E.nome=\'SP\';";
//	char query[] = "SELECT C.idcidade, E.nome FROM cidade C JOIN estado E ON E.idestado=C.estado_idestado WHERE E.nome=\'SP\';";
	char query[] = "SELECT idproduto FROM produto;";

	int contador;

	mysql_init(&conexao);
	if(mysql_real_connect(&conexao, HOST, USER, PASS, DB, 0, NULL, 0))
	{

		printf(" Conectado com sucesso\n");
		if(mysql_query(&conexao, query))
		{
			printf(" Deu pau\n");
			return 0;
		}
		else
		{

			resposta = mysql_store_result(&conexao);// Recebe consulta
			if(resposta == NULL)
			{
				printf(" Não houve consulta\n");
				return 0;
			}
			else
			{
//				campos = mysql_fetch_fields(resposta);
//				printf("campos[0].name →  %s\n", (campos[0]).name);

				while((linhas=mysql_fetch_row(resposta)) != NULL)
				{
					printf("linhas[0] → %s\n", linhas[0]);
					printf(" \n");
				}
				mysql_free_result(resposta);//Libera variavel

			}
			mysql_close(&conexao);///Libera conexao

		}
	}
	else
	{
		printf(" Falha ao conectar-se ao banco de dados\n");
		if(mysql_errno(&conexao))
		{
			printf(" Erro %d:\t%s\n", mysql_errno(&conexao), mysql_error(&conexao));
		}
		return 0;
	}
}

