#include <stdio.h>

#define MAX_SIZE 12

int select(int data[]);
int bubble(int data[]);
int insert(int data[]);

int swap(int x,int y,int data[]);

int main(int args,char **argv)
{
	int data_S[MAX_SIZE]={1,5,6,5,2,12,3,4,87,2,3,9};
	int data_B[MAX_SIZE]={1,5,6,5,2,12,3,4,87,2,3,9};
	int data_I[MAX_SIZE]={1,5,6,5,2,12,3,4,87,2,3,9};
	
	select(data_S);

	bubble(data_B);
	
	insert(data_I);

}

int bubble(int data[])
{
	int i;
	int j;
	
	printf("bubble start................\n");

	for(i=0;i<MAX_SIZE;i++)
	{
		for(j=0;j<MAX_SIZE-i-1;j++)
		{
			if(data[j]>data[j+1])
			{
				swap(j,j+1,data);
			}
		}

		print_all(data);
	}

	printf("bubble end................\n");

} 

int select(int data[])
{
	int i;
	int j;
	int min;

	printf("select start................\n");

	for(i=0;i<MAX_SIZE;i++)
	{
		min = i;
		for(j=i+1;j<MAX_SIZE;j++)
		{
			if(data[j]<data[min])
			{
				min = j;	
			}	
		}

		if(min!=i)
		{
			swap(min,i,data);
		}

		print_all(data);
	}
	
	printf("select end................\n");
}

int insert(int data[])
{
	int i;
	int j;
	int tmp;

	printf("insert start................\n");
	
	for(i=1;i<MAX_SIZE;i++)
	{
		tmp = data[i];
		for(j=i-1;j>=0;j--)
		{
			if(tmp<data[j])
			{
				data[j+1]=data[j];
			}
			else
			{
				break;
			}
			
		}	
		data[j+1] = tmp;
		
		print_all(data);
	}

	printf("insert end................\n");
}

int shell(int data[],int size)
{
	int i;
	int j;

	for($i=)
}

int print_all(int data[])
{
	int i;
	for(i=0;i<MAX_SIZE;i++)
	{
		printf("%d ",data[i]);
	}
	printf("\n");
}

int swap(int x,int y,int data[])
{
	int tmp;
	tmp = data[x];
	data[x] = data[y];
	data[y] = tmp;
}
