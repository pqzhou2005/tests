#include<stdio.h>

int build_heap(int data[],int size);

int heap_order(int data[],int index,int size);

int swap(int data[],int source_index,int target_index);

int print_heap(int data[],int size);

int main()
{
	//int data[] = {4,5,3,1,9,8,6};
	int data[] = {3,7,9,2,7,9,5,3,34,5,6,34,55,44,22,14,678};
	int size = 17;
	
	print_heap(data,size);
	
	build_heap(data,size);
	
	int length = size;
	int i;

	for(i=length-1;i>0;i--)
	{
		//if(data[0]<data[i])
		{
			swap(data,i,0);
			size--;
			print_heap(data,length);
			heap_order(data,0,size);
			print_heap(data,length);
		}
	}
}

int build_heap(int data[],int size)
{
	int i;
	for(i=size/2-1;i>=0;i--)
	{
		heap_order(data,i,size);
		print_heap(data,size);
	}
}
/*
int heap_order(int data[],int index,int size)
{
	int left_index;
	int right_index;
	int min;

	while(1)
	{
		left_index = index*2+1;
		right_index = index*2+2;
	
		if(left_index<size)
		{
			min = left_index;
			if(right_index<size)
			{
				if(data[right_index]<data[min])
				{
					min = right_index;
				}
			}
		}
		else
		{
			break;
		}

		if(data[index]>data[min])
		{
			swap(data,min,index);
			index = min;
			printf("min:%d\n",min);
		}
		else
		{
			break;
		}
	}
}
*/
int heap_order(int data[],int index,int size)
{
	int left_index;
	int right_index;
	int min;

	while(1)
	{
		min = index;
		left_index = index*2+1;
		right_index = index*2+2;
	
		if(left_index<size)
		{
			if(data[left_index]<data[min])
			{
				min = left_index;
			}

			if(right_index<size)
			{
				if(data[right_index]<data[min])
				{
					min = right_index;
				}
			}
		}
		else
		{
			break;
		}

		if(min!=index)
		{
			swap(data,min,index);
			index=min;
			printf("min:%d\n",min);
		}
		else
		{
			break;
		}
	}

}

int swap(int data[],int source_index,int target_index)
{
	int tmp;
	tmp = data[source_index];	
	data[source_index] = data[target_index];
	data[target_index] = tmp;
}

int print_heap(int data[],int size)
{
	int i;
	for(i=0;i<size;i++)
        {
                printf("%d ",data[i]);
        }
	printf("\n");
}
