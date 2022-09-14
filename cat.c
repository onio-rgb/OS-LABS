#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
int main(int argc,char * argv[])
{
	char buf[1024];
	int fd;

	if(argc==1)
	{
		printf("Invalid Usage For help -- man cat");
	}
	for(int i=0;i<argc;i++)
	{
		if(argv[i][0]!='.')
		{	 
			printf("%s \n",argv[i]);
			if((fd=open(argv[i],O_RDONLY)))
			{
				ssize_t n_read;
				while((n_read=read(fd,buf,1024)))
				{
					for(int j=0;j<n_read;j++)
						printf("%c",buf[j]);
				}
			}
		}
		printf("\n");
	}
}
