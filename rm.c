#include<stdio.h>
#include<unistd.h>
#include<string.h>
int main(int argc,char * argv[])
{
	int i_f=0;
	for(int i=0;i<argc;i++)
	{
		if(strcmp(argv[i],"-i")==0)
			i_f=1;
	}
	for(int i=0;i<argc;i++)
	{
		if(argv[i][0]!='-' && argv[i][0]!='.')
		{
			if(i_f==1)
			{
				printf("do yu want to delete %s Y/N : ",argv[i]);
				char c;
				scanf(" %c",&c);
				if(c=='N') break;
			}
			unlink(argv[i]);
		}
	}
}
