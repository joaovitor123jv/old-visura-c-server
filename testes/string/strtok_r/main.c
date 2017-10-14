#include<stdio.h>
#include<string.h>
 
int main()
{
/*    char str[] = "Geeks for Geeks";
    char *token;
    char *rest = str;
 
    while ((token = strtok_r(rest, " ", &rest)))
        printf("%s\n", token);

	printf(" STR = |%s|\n", str);
 
    return(0);*/

	char *str = "Testando 1 2 3 4 5 teste 6 7 testando !\nt\tA F";

	char *token;
	while((token = strtok_r(str, " ", &str)))
	{
		printf(" TOKEN = |%s|\n", token);
		printf(" STR = |%s|\n", str);
		printf("\n\n\n");
	}

	return 0;
}
