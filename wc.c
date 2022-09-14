#include <stdio.h>
#include <errno.h>
#include <sys/param.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void readl(int fd, char *path)
{
    long count = 1;
    char *buf = malloc(1024);
    int n_read = 0;

    while ((n_read = read(fd, buf, 1024)) != 0)
    {
        for (int i = 0; i < n_read; i++)
        {
            if (buf[i] == '\n')
                count = count + 1;
        }
    }
    printf("%ld %s \n", count, path);
    close(fd);
}
void readc(int fd, char *path)
{
    long count = 1;
    char *buf = malloc(1024);
    int n_read = 0;

    while ((n_read = read(fd, buf, 1024)) != 0)
    {
        count = count + n_read;
    }
    printf("%ld %s \n", count, path);
    close(fd);
}
void readw(int fd, char *path)
{
    long count = 1;
    char *buf = malloc(1024);
    int n_read = 0;
    long wcount=0;
    while ((n_read = read(fd, buf, 1024)) != 0)
    {
        for(int i=0;i<n_read;i++)
        {
            if(buf[i]!=' ')wcount=wcount+1;
            if(buf[i]==' ' && wcount!=0){
                count=count+1;
                wcount=0;
            }
        }
    }
    printf("%ld %s \n", count, path);
    close(fd);
}
void readlwc(int fd,char* path)
{
    printf("no of lines \n");
    readl(fd,path);
    fd = open(path, O_RDONLY);
    printf("no of words \n");
    readw(fd,path);
    fd = open(path, O_RDONLY);
    printf("no of characters \n");
    readc(fd,path);
}
int main(int argc, char *argv[])
{
    int l_f = 0;
    int c_f = 0;
    int w_f = 0;
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-l") == 0)
            l_f = 1;
        else if (strcmp(argv[i], "-c") == 0)
            c_f = 1;
        else if (strcmp(argv[i], "-w") == 0)
            w_f = 1;
    }
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            int fd = open(argv[i], O_RDONLY);
            if(l_f==1)
            readl(fd, argv[i]);
            else if(c_f==1)
            readc(fd,argv[i]);
            else if(w_f==1)
            readw(fd,argv[i]);
            else readlwc(fd,argv[i]);
        }
    }
}