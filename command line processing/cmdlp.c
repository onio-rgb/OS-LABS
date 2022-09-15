#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
int main()
{
	printf("$");
	char *cmd = malloc(1024);
	scanf("%[^\n]%*c", cmd);
	char *word = malloc(20);
	char args[20][50];
	int argc = 0;
	int n_read = 0;
	int cpy_stdout;
	int cpy_stdin;
	while (sscanf(cmd, "%s%n", word, &n_read) != EOF)
	{
		strcpy(args[argc], word);
		argc = argc + 1;
		cmd = cmd + n_read + 1;
	}
	int i_buff = 0;
	char buff[20][50];
	int back_f = 0;
	for (int i = 0; i < argc; i++)
	{

		if (strcmp(args[i], ">") == 0)
		{
			cpy_stdout = dup(1);
			int outfd = open(args[i + 1], O_WRONLY | O_CREAT);
			dup2(outfd, 1);
			break;
			// printf("hello");
			// fflush(stdout);
			// dup2(cpy_stdout,1);
			// close(cpy_stdout);
		}
		else if (strcmp(args[i], "<") == 0)
		{
			cpy_stdin = dup(0);
			int infd = open(args[i + 1], O_RDONLY);
			dup2(infd, 0);
			break;
			// char * temp=malloc(10);
			// scanf("%s",temp);
			// fflush(stdin);
			// dup2(cpy_stdin,0);
			// close(cpy_stdin);
			// printf("%s",temp);
		}
		else if (args[i] == "&")
			back_f = 1;
		else
		{
			strcpy(buff[i_buff], args[i]);
		}
	}

	int pid = fork();
	if (pid == 0)
	{
		if (execvp(args[0], args) < 0)
		{
			fflush(stdout);
			write(cpy_stdout, &errno, sizeof(errno));
		}
	}
}
