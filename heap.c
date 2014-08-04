#include <stdio.h>
#include <stdlib.h>
void HeapSort(int num[],int size);
void BuildHeap(int num[] ,int size);
void PercolateDown(int num[] , int index,int size);
void PrintHeap(const char* strMsg,int array[],int nLength);
void Swap(int num[] , int v, int u);
int main(int argc, char *argv[])
{
  int data[17]={3,7,9,2,7,9,5,3,34,5,6,34,55,44,22,14,678};
  HeapSort(data,17);
   
  return 0;
}
void HeapSort(int num[] ,int size)
{
    int i;
    int iLength=size;
     
    PrintHeap("Befor Sort:",num,iLength);
     
    BuildHeap(num,size);// 建立小顶堆  
     
    for (i = iLength - 1; i >= 1; i--) {  
        Swap(num, 0, i);// 交换  
        size--;// 每交换一次让规模减少一次  
        PercolateDown(num, 0,size);// 将新的首元素下滤操作
        PrintHeap("Sort Heap:",num,iLength); 
    }
}
// 建堆方法，只需线性时间建好  
void BuildHeap(int num[] ,int size) {
    int i;
    for (i = size / 2 - 1; i >= 0; i--) {// 对前一半的节点（解释为“从最后一个非叶子节点开始，将每个父节点都调整为最小堆”更合理一些）  
        PercolateDown(num, i,size);// 进行下滤操作
        PrintHeap("Build heap:",num,size);
    }  
}
     
// 对该数进行下滤操作，直到该数比左右节点都小就停止下滤  
void PercolateDown(int num[] , int index,int size) {  
    int min;// 设置最小指向下标  
    while (index * 2 + 1<size) {// 如果该数有左节点，则假设左节点最小  
        min = index * 2 + 1;// 获取左节点的下标  
        if (index * 2 + 2<size) {// 如果该数还有右节点  
            if (num[min] > num[index * 2 + 2]) {// 就和左节点分出最小者  
                min = index * 2 + 2;// 此时右节点更小，则更新min的指向下标  
            }  
        }  
        // 此时进行该数和最小者进行比较，  
        if (num[index] <= num[min]) {// 如果index最小，  
            break;// 停止下滤操作  
        } else {  
            Swap(num, index, min);// 交换两个数，让大数往下沉  
            index = min;// 更新index的指向  
        }  
    }// while  
}
     
// 给定数组交换两个数的位置  
void Swap(int num[] , int v, int u) { 
    int temp = num[v];  
    num[v] = num[u];  
    num[u] = temp;  
}  
void PrintHeap(const char* strMsg,int array[],int nLength)
{
     int i;
     printf("%s",strMsg);
     for(i=0;i<nLength;i++)
     {
        printf("%d ",array[i]);
     }
     printf("\n");
}
