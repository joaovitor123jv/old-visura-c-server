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


/*
	COMO GERAR A CHAVE RSA:
		Crie uma chave privada com o seguinte comando:
			openssl genrsa -out private.pem 8192
		Crie uma chave publica, passando a privada criada como argumento com o seguinte comando:
			openssl rsa -in private.pem -outform PEM -pubout -out public.pem

	Observações: Como o tamanho de um comando pode ser de até 1024 caracteres, a chave PRECISA, ser de tamanho 8192
		O tamanho da chave é dado em bits
		Lembrando que 1B = 8b, então (8192/8 == 1024) (1024B criptografados, com uma chave de 8192b)
*/


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
// #define padding RSA_PKCS1_PADDING //Inutiliza 10 Bytes
#define padding RSA_NO_PADDING // Utiliza todos os Bytes, mas, precisa de todos os bytes de entrada


#define PUBLIC_RSA true
#define PRIVATE_RSA false

int public_encrypt(unsigned char * data, int data_len, unsigned char * key, unsigned char *encrypted)
{
	RSA * rsa = createRSA(key , PUBLIC_RSA);
	int result = RSA_public_encrypt(data_len, data, encrypted, rsa, padding);// Chama função da biblioteca do OpenSSL
	return result;
}


int private_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
{
	RSA * rsa = createRSA(key, PRIVATE_RSA);
	int  result = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,padding);// Chama função da biblioteca do OpenSSL
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
	char * err = (char *)malloc(130);
	ERR_load_crypto_strings();
	ERR_error_string(ERR_get_error(), err);
	printf("*****************(Criptografia.h) (printLastError)\n");
	printf("%s ERROR: %s\n",msg, err);
	free(err);
}


unsigned char *getChavePublica()
{
	/*
	return (unsigned char *)"-----BEGIN PUBLIC KEY-----\n"\
							"MIIEIjANBgkqhkiG9w0BAQEFAAOCBA8AMIIECgKCBAEAwvmpP5N0cHbVOT+iRUYg\n"\
							"jysPLijNnY/yaxBsInPimumVTv9nEOma3PQtr7fJApFufC0gH42Cvvf2vTFZLoq6\n"\
							"HoognABHL7xvnRNf+MkEIDkvXb9SDjR3f0HwNt04PXUdZdQC5uBxFK6AXikwPH3W\n"\
							"cR6aAc93Zu2EOWbxqPntcl2rJqd/eD36r7NDdiklb+JvRPNIcSSRR/zIdohQh2x9\n"\
							"n3fLCQGgL/Bsp0lLHluUNniIRJYLZ9ws2JbWVBEgheeiE4Tr/QdxGMV0r2ioST3e\n"\
							"usGllQzoFqh1mk/DCv+fKR3zSoWonarQbgGBcscgkw3HKGNJ+kjpnFwlRfpK8rgR\n"\
							"xw2jYfeAfJIrWmtMTzwBU/D2RPQpFFvAO22D2ebqWEOqvAfK36qrDq+xaNEroV2b\n"\
							"rJ7TYjHPtMWFaypSr0BSjCHeg6Ppl3oWxuYm+O+GDAMydvsWz8U4L0rTCV8faacI\n"\
							"epnL9RbaLxO6+X5535hMPnIZdmZHzHQH9L7X7GxN6QrwhQKjyg0oroSMUh6oCbkA\n"\
							"0uN5hitqlukP1ruQSIRmV2c4pMvYDe4MADHIW7u/Y5ip0uWdiVWhO6/JCE/Y9Kve\n"\
							"CfbPhRiz75cy1GMc3ezjbVWzbCP/V1Gmp/LY6iEGFvV2a3MCroz1R2V7DibJSZEO\n"\
							"B6lmva0WOXhnWzcLxfBeqCWDKOYnq2UJ4rkgnPPSJPfJ3eyUX9aYcnR74nlQxOGB\n"\
							"cfucwYfeafCu+Ta+W2NN3Mpnh6cSW37fX7R3h37Gir98hlh/w9TGulRAf9U71kA9\n"\
							"h0kG1bFmQMaqXy1H9H2GQxRcOMAb57Dw8rHSgSGVmAYQ7T9ldSkiKn1qA5jI6y3L\n"\
							"R/+a5Gvx+xZThdGpjhIILQ0PU4MO3205Igg82dKNylWK3SHCrgS3myMivEehqwit\n"\
							"iMD5mWRkUhUkxtpduL/xKub58ekbHSJB8iMHg0TXHbGpK8Qf42Isfdhxw7pVrlOM\n"\
							"L+mOYGCfIgX5Mrn4guGIR/jzptDplLSm8Plhcw54GoUlOjzye/sQPcG6wPBvNOX/\n"\
							"3JIcNUKcObzkZwvRBouK/yHky+REE3RKf+CNxxVVuuIlorpFOVvqbOXA01PsNBsw\n"\
							"il2fSZxX2jvjL3ncPWpS5+5fDhLAUUFIBRQdFq5TDhb+5+ad4oOAwn7hF2UsaAF7\n"\
							"XMANpQZwU+2/FlNNW2MEp9xW/iw76o62ih3kUMCT79ysHFUpgNnrq7coaoMtjQLN\n"\
							"93wLe2LPJHP/pFRV/R6LOoOXPdTc2wqJJ7MgbLEOepitDybXqK0IH7BzGlmUSDFz\n"\
							"no2tBH9xvWvJNO0BTOA/iQcnWE9DAU9QqZLk0kD4pTpxZo0/jnGuSR136jcGn7VB\n"\
							"/wIDAQAB\n"\
							"-----END PUBLIC KEY-----\n";*/
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
/*	return (unsigned char *)"-----BEGIN RSA PRIVATE KEY-----\n"\
							"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\n"\
							"-----END RSA PRIVATE KEY-----\n";*/
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

#endif // CRIPTOGRAFIA