#include<string.h>

int chtoint(char *p)
{
	int i,n,sum;
	sum = 0;
	n = strlen(p);

	for(i=0;i<n;i++)
	{
		sum = sum*10 + p[i]-'0';
	}
	return sum;
}
