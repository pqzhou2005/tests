#include<stdio.h>

int main(int args,char **argv)
{
	int data[4] = {3,4,5,4};
	int size = 7;
	int i=0;
	int sum = 1;

	while(1)
	{
		if(sum>size) break;
		sum = sum*2;
		i++;
	}

	printf("%d ",i);
}
