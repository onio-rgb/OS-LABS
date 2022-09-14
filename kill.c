#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    char *signo = "9";
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
            signo = argv[i]+1;
    }
    long err = 0;
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            err = syscall(SYS_kill, atoi(argv[i]),atoi(signo));
            int e = errno;
            if (err != -1)
                printf("Prcess terminated : %s \n", argv[i]);
            else
                printf("%d \n", e);
        }
    }
}