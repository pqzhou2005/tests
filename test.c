#include"stdafx.h"
#include<iostream>
using namespace std;
#define MAXSIZE 1000
int f[MAXSIZE+1],c[MAXSIZE+1],w[MAXSIZE+1];

int_main(int argc,_TCHAR *argv[])
{
	int N,V;
	
	cin>>N>>V;
	
	int i=1;
	
	for(;i<=N;++i)
	{
		cin>>c[i]>>w[i];
	}
	
	for(i=1;i<=N;++i)
	{
		for(int v=V;v>=c[i];--v)//c[i]可优化为bound,bound=max{V-sumc[i,...n],c[i]}
		{
			f[v]=(f[v]>f[v-c[i]]+w[i])?f[v]:f[v-c[i]]+w[i];
		}
	}
//当i=N时，可以跳出循环单独计算F[V]
//cout<<f[V]<<'\n';
//system("pause");
return0;
}
