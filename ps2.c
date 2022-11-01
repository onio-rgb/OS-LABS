#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>   
int isDigit(char c[])
{
    for (int i = 0; i < strlen(c); i++)
    {
        if (c[i] < 48 || c[i] > 57)
            return 0;
    }
    return 1;
}
int main(int argc, char *argv[])
{
    DIR *dirp = opendir("/proc/");
    char path[300];
    struct dirent *cur ;
    while ((cur = readdir(dirp)) != NULL)
    {
        if (isDigit(cur->d_name))
        {
            sprintf(path, "/proc/%s/comm", cur->d_name);
            sprintf(path,"/proc/%s/fd/0",cur->d_name);
            int fd=open(path,O_RDONLY);
            char *tty;
            if(fd==-1)return 1;
            tty=ttyname(fd);
            if(tty)
            printf("%s\n",tty);
             fd = open(path, O_RDONLY);
            
            char name[30];
            int n_read = read(fd, name, 30);
            
            printf("%s\t%s\t", cur->d_name   ,name);
            
            else printf("?\n");
            
        }
    }
}