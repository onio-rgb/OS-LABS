#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include<fcntl.h>
int a_f = 0;
void processDir(char *path)
{
    DIR *dirp;
    if (!(dirp = opendir(path)))
    {
        printf("Cant access current directory");
        return;
    }

    struct dirent *cur_dirent = malloc(sizeof(struct dirent));
    while ((cur_dirent = readdir(dirp)))
    {
        if (a_f == 0 && cur_dirent->d_name[0] == '.')
            continue;
        printf("%s \t", cur_dirent->d_name);
    }
    printf("\n");
}

void processDirl(char *path)
{
    DIR *dirp;
    if (!(dirp = opendir(path)))
    {
        printf("Cant access current directory");
        return;
    }

    struct dirent *cur_dirent = malloc(sizeof(struct dirent));
    struct stat* buf=malloc(sizeof(struct stat));
    while ((cur_dirent = readdir(dirp)))
    {
        if (a_f == 0 && cur_dirent->d_name[0] == '.')
            continue;
        // printf("%s",cur_dirent->d_name);
        int fd=open(cur_dirent->d_name,O_RDONLY);
        if(fstat(fd,buf)!=0)return;
        mode_t perm = buf->st_mode;
        printf("%c",((perm & S_IRUSR) ? ('r') : ('-')));
        printf("%c",((perm & S_IWUSR) ? 'w' : '-'));
        printf("%c",((perm & S_IXUSR) ? 'x' : '-'));
        printf("%c",((perm & S_IRGRP) ? 'r' : '-'));
        printf("%c",((perm & S_IWGRP) ? 'w' : '-'));
        printf("%c",((perm & S_IXGRP) ? 'x' : '-'));
        printf("%c",((perm & S_IROTH) ? 'r' : '-'));
        printf("%c",((perm & S_IWOTH) ? 'w' : '-'));
        printf("%c \t",((perm & S_IXOTH) ? 'x' : '-'));
        printf("%ld \t",buf->st_nlink);
        printf("%s \t",(buf->st_gid==0)?"root":"");
        printf("%s \t",(buf->st_uid==0)?"root":"");
        printf("%ld \t",buf->st_size);
        printf("%ld \t", buf->st_ctim.tv_sec);
        printf("%s",cur_dirent->d_name);
        
	printf("\n");
    }
 
}

void processDirR(char *path)
{
    DIR *dirp;
    if (!(dirp = opendir(path)))
    {
        printf("Cant access current directory");
	    return;
    }

    struct dirent *cur_dirent = malloc(sizeof(struct dirent));
    printf("%s \n \n",path);
    while ((cur_dirent = readdir(dirp)))
    {
        if (a_f == 0 && cur_dirent->d_name[0] == '.')
            continue;

        printf("%s \n",cur_dirent->d_name);
    }
    printf("\n");
    dirp = opendir(path);
    cur_dirent = malloc(sizeof(struct dirent));
    while ((cur_dirent = readdir(dirp)))
    {
        if (a_f == 0 && cur_dirent->d_name[0] == '.')
            continue;

        if((cur_dirent->d_type & DT_DIR))
        {
            char* new_path=malloc(256);
            strcat(new_path,path);
            strcat(new_path,"/");
            strcat(new_path,cur_dirent->d_name);
            processDirR(new_path);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    int l_f = 0;
    int R_f = 0;
    int i;
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-l") == 0)
            l_f = 1;
        else if (strcmp(argv[i], "-a") == 0)
            a_f = 1;
        else if (strcmp(argv[i], "-R") == 0)
            R_f = 1;
    }

    if (l_f == 1)
        processDirl((argv[argc - 1][0] != '-' && argv[argc - 1][0] != '.') ? (argv[argc - 1]) : ("."));
    else if (a_f == 1)
        processDir((argv[argc - 1][0] != '-' && argv[argc - 1][0] != '.') ? (argv[argc - 1]) : ("."));
    else if (R_f == 1)
        processDirR((argv[argc - 1][0] != '-' && argv[argc - 1][0] != '.') ? (argv[argc - 1]) : ("."));
    else
        processDir((argv[argc - 1][0] != '-' && argv[argc - 1][0] != '.') ? (argv[argc - 1]) : ("."));
}
