#ifndef CRIPTOGRAFIA
#define CRIPTOGRAFIA

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef bool
#define bool char
#endif


RSA *createRSA(unsigned char * key,bool public)
{
	RSA *rsa= NULL;
	BIO *keybio ;
	keybio = BIO_new_mem_buf(key, -1);
	if (keybio==NULL)
	{
		printf( "Failed to create key BIO");
		return 0;
	}
	if(public)
	{
		rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
	}
	else
	{
		rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
	}

	return rsa;
}


RSA *createRSAWithFilename(char *filename, bool public)
{
	if(filename == NULL)
	{
		printf(" Falha ao tentar ler arquivo (NULL) (Criptografia.h) (createRSAWithFilename)\n");
		return NULL;
	}
	FILE * fp = fopen(filename,"rb");

	if(fp == NULL)
	{
		printf("Unable to open file %s \n",filename);
		return NULL;    
	}
	RSA *rsa= RSA_new() ;

	if(public)
	{
		rsa = PEM_read_RSA_PUBKEY(fp, &rsa,NULL, NULL);
	}
	else
	{
		rsa = PEM_read_RSAPrivateKey(fp, &rsa,NULL, NULL);
	}

	return rsa;
}



/********************************DEFINICAO DE TIPO DE CRIPTOGRAFIA (RSA)******************/
#define padding RSA_PKCS1_PADDING

int public_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
{
	RSA * rsa = createRSA(key,1);
	int result = RSA_public_encrypt(data_len,data,encrypted,rsa,padding);
	return result;
}


int private_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
{
	RSA * rsa = createRSA(key,0);
	int  result = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,padding);
	return result;
}

int private_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
{
	RSA * rsa = createRSA(key,0);
	int result = RSA_private_encrypt(data_len,data,encrypted,rsa,padding);
	return result;
}
int public_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
{
	RSA * rsa = createRSA(key,1);
	int  result = RSA_public_decrypt(data_len,enc_data,decrypted,rsa,padding);
	return result;
}


void printLastError(char *msg)
{
	char * err = malloc(130);;
	ERR_load_crypto_strings();
	ERR_error_string(ERR_get_error(), err);
	printf("*****************(Criptografia.h) (printLastError)\n");
	printf("%s ERROR: %s\n",msg, err);
	free(err);
}


unsigned char *getChavePublica()
{
	return (unsigned char *)"-----BEGIN PUBLIC KEY-----\n"\
		"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAzSRDGP8DKyXt0FJjr7rj\n"\
		"MskbCCsvzwc4sL9Q0kcFrVF4L1d/aZx3A/+VZMP2fUkbVOlGuC7y8Oh6Gh0OTxh8\n"\
		"ZyupZqDuaWf22wXGXgR+LB5KVcCK7vMldSZO/JnrBz7abRzTQZhZyNNC7lF1DwKl\n"\
		"X0R09YGj+scdhvhe5XnA7eS38Ey7EaSEQC8/5noSt/wTMAmlM7bgLNN/7YPeeqOU\n"\
		"++YXI8e/2GREXLmjBwNZQ3c8RrzVmvnJFOfa8M1mHIoKg6rxQHkEK4r5H/2OvMv+\n"\
		"2k9lF5UmWUztFSG01BMEMtYxV0HwWNqe8VE59i840RcOrLv09jg+QpmO/jIMw5vz\n"\
		"GwIDAQAB\n"\
		"-----END PUBLIC KEY-----\n";
}

unsigned char* getChavePrivada()
{
	return (unsigned char *)"-----BEGIN RSA PRIVATE KEY-----\n"\
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
		"-----END RSA PRIVATE KEY-----\n";
}

#endif