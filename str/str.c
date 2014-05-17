#include<string.h>
#include<stdio.h>

int cstrcpy(char *s,char *t,int m,int length)
{
	int sn,tn,i,j;
	tn = strlen(t);
	if(m+length>tn)
	{
		return -1;
	}

	j=m;
	for(i=0;i<length;i++)
	{
		s[i]=t[j];
		j++;	
	}

	return 0;
}


