#include <stdio.h>
#include "Criptografia.h"

int main()
{

	// printf("Iniciando teste de Criptografia\n");
	// unsigned char senha[2048/8] = "Eu te amo Alexia Lui Da Silva !!!!";/* Máximo de 256 caracteres por senha */
	// unsigned char senha[1024] = "Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Silva !!!!Eu te amo Alexia Lui Da Sil";/* Máximo de 256 caracteres por senha */
	unsigned char senha[2048/8] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";/* Máximo de 256 caracteres por senha */
	
	unsigned char *publicKey = getChavePublica();
	unsigned char *privateKey = getChavePrivada();
	/* char publicKey[] = "-----BEGIN PUBLIC KEY-----\n"\
		"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAzSRDGP8DKyXt0FJjr7rj\n"\
		"MskbCCsvzwc4sL9Q0kcFrVF4L1d/aZx3A/+VZMP2fUkbVOlGuC7y8Oh6Gh0OTxh8\n"\
		"ZyupZqDuaWf22wXGXgR+LB5KVcCK7vMldSZO/JnrBz7abRzTQZhZyNNC7lF1DwKl\n"\
		"X0R09YGj+scdhvhe5XnA7eS38Ey7EaSEQC8/5noSt/wTMAmlM7bgLNN/7YPeeqOU\n"\
	 	"++YXI8e/2GREXLmjBwNZQ3c8RrzVmvnJFOfa8M1mHIoKg6rxQHkEK4r5H/2OvMv+\n"\
	 	"2k9lF5UmWUztFSG01BMEMtYxV0HwWNqe8VE59i840RcOrLv09jg+QpmO/jIMw5vz\n"\
	 	"GwIDAQAB\n"\
	 	"-----END PUBLIC KEY-----\n";

	char privateKey[] = "-----BEGIN RSA PRIVATE KEY-----\n"\
		"MIIEowIBAAKCAQEAzSRDGP8DKyXt0FJjr7rjMskbCCsvzwc4sL9Q0kcFrVF4L1d/\n"\
		"aZx3A/+VZMP2fUkbVOlGuC7y8Oh6Gh0OTxh8ZyupZqDuaWf22wXGXgR+LB5KVcCK\n"\
		"7vMldSZO/JnrBz7abRzTQZhZyNNC7lF1DwKlX0R09YGj+scdhvhe5XnA7eS38Ey7\n"\
		"EaSEQC8/5noSt/wTMAmlM7bgLNN/7YPeeqOU++YXI8e/2GREXLmjBwNZQ3c8RrzV\n"\
		"mvnJFOfa8M1mHIoKg6rxQHkEK4r5H/2OvMv+2k9lF5UmWUztFSG01BMEMtYxV0Hw\n"\
		"WNqe8VE59i840RcOrLv09jg+QpmO/jIMw5vzGwIDAQABAoIBAFaa/IjNAHVy6deN\n"\
		"MPC7obeLF5J3SRPKJLDZ1DEyMr9dKbQy6GOZ3tUdTeDgqmtyNcu/QE/54i66tvHX\n"\
		"WnciC0MzN61QO9bcQWduJLUEiI/vqcpSI1WF6UZwCeCyynqgmm5+QOHsg9s49YTP\n"\
		"dvADWfXCXO5Y8PWqolic+tM+d6j2qIrHhDvFUEDCDfdT9HYrXvJ6FQon0a5XCABu\n"\
		"eBinNA0BkWyC3gObLQsVspkma6/QLCkbD/iYqrcXlfJt1WrgIL/T9gQcSml2N8eE\n"\
		"Uy+Z13L91qT1WjRoiTbY/58kNKrpa8ufnvAekWljjzbQ1ef+1yh7R0AEydqkP/jM\n"\
		"U/VkGsECgYEA9ZxEuJsTo+gA7hdpNXZ7NAabFwOSo8YTZwnuoEPLc9l8U7UhNWdm\n"\
		"m7BTHYAxCTdIf0rzDqHb9NhiwScdn2AwYYSLp2yaikqlHx6s/x+fN9924gW7ErpL\n"\
		"HdL+Lq8BfFCQdYWDgeccdIwO6Z+OaBiHUwBGH6LRtUh/0xZfyBh0szkCgYEA1dHB\n"\
		"Ll0pnhOwyuTF66IaDABNzkmiOGvZ6KHw4CRrbHxCUzKneszW3bmpa6rJPMgVwawJ\n"\
		"y4dN1nWRhs0q6S+MhAYS1fwdaD7S9wFQUF02L8kEnPN3aj7wqKmP3gGA0oqv0Soh\n"\
		"OaIQdldMFDY07HiQVUXFceK9rUoPa8TFFeigdPMCgYEAjBVz8KKqYgraNPHvVMnd\n"\
		"3T+DR24HPhekS1mT256vNMUvrwKZDT+Le3LnBOVvkmUQmWGjHY5dyNl+AdKQh60y\n"\
		"CvyxNm2v+v42DXjOjLh5dEGlKabBGx5b54fG59NzEndU3WKoLfgl96dGfiM98stC\n"\
		"q6l1VWflGfNezINvX83rCakCgYAPGWTe9U0/EHznlwgy/5t/RXY5HxIJkfQtbWIp\n"\
		"Mek0HAJDBcy160kxFy9Tnxfe5vJCXvfCG6fvgRXpwwtItEbj+9nQqqEY/3a+YqVd\n"\
		"R3fQIA3nV3oTUqaEApVaRBjoxUptJuB5xWwML1X8B2ftWwY2P1sWE8D5P8WfAqI4\n"\
		"Tdkm6QKBgFKzXPUU9sD53LekL4pDQkoF3ndss0+Pi7U4vQ28pq+FoQUdq7e+jPBr\n"\
		"HlC6UsnNLV1febJzrE3nvc80JM8TtPC9bqEIcZN3Ys29xclncQyX6OHQJmhbeiJz\n"\
		"Nyvo+ZNS7xENtnUFk+066TLQ3H1UskFEZIqiZCy5Mc1KeHmeVUdk\n"\
		"-----END RSA PRIVATE KEY-----\n";*/

	// unsigned char encrypted[4098]={};
	unsigned char encrypted[8192]={};
	// unsigned char decrypted[4098]={};
	unsigned char decrypted[1024]={};
	

	int encrypted_length= public_encrypt(senha, strlen((const char *)senha), publicKey, encrypted);
	if(encrypted_length == -1)
	{
		printf("Erro ao criptografar mensagem\n");
	}

	int decrypted_length = private_decrypt(encrypted,encrypted_length,privateKey, decrypted);

	if(decrypted_length == -1)
	{
		printLastError("Private Decrypt failed ");
		exit(0);
	}

	printf("Decrypted Text =%s\n",decrypted);
	printf("Decrypted Length =%d\n",decrypted_length);

	printf("Crypted Text = %s\n", encrypted );
	printf("Crypted Length = %d\n", encrypted_length );

	printf("LOG: Mensagem decriptografada = |%s|\n", decrypted);
	printf("LOG: Tamanho da mensagem decriptografada = |%d|\n", strlen(decrypted));
	
	return 0;
}