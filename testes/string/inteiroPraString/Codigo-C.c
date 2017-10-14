#include<stdio.h>

void inteiroParaString(int n, char s[])
{
    int i=0, sign;
    if ((sign = n) < 0)  /* record sign */
    {
        n = -n;          /* make n positive */
    }
    do 
    {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
    {
        s[i++] = '-';
    }
    s[i] = '\0';
    adaptadorDeString_REVERSE(s);
}


void adaptadorDeString_REVERSE(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}




int main()
{
	int a = 12*12;
	char *retorno = malloc(sizeof(char) * 10);
	inteiroParaString(a, retorno);
	printf(" %d == %s ?\n ", a, retorno);
	return 0;
}
