
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<ctype.h>
int main(int argc, char *argv[])
{
    DIR *dir;
    dir = opendir("/proc");
    struct dirent *cur_dirent = malloc(sizeof(struct dirent));
    while ((cur_dirent = readdir(dir)))
    {
        if (!isdigit(cur_dirent->d_name[0]))
            continue;
        char *path = malloc(1024);
        path = "/proc";
        char * pid_path=malloc(4);
        strcat(path, cur_dirent->d_name);
        strcat(path, "/stat");
        FILE *file = fopen(path, O_RDONLY);
        int pid;
        char *com;
        char state;
        int ppid;
        int pgrp;
        int sess;
        int tty;
        fscanf(file, "%d%s%c%d%d%d%d", &pid, com, &state, &ppid, &pgrp, &sess, &tty);
        if (state!='R')
        continue;
        printf("%d\t pts/%d %s",pid,tty,com);
    }
}