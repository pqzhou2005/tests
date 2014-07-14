#include <stdio.h>
int http_log(char *msg)
{
	char *path = "http.log";
	
	FILE *fp;
	fp = fopen(path,"aw");
	
	int i=0;
	while(msg[i]!='\0')
	{
		fputc(msg[i],fp);
		i++;	
	}
	fputc('\n',fp);
	fclose(fp);
	return 0;
}
