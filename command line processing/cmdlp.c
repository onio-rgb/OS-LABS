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
	while (1)
	{
		printf("$ ");
		char *cmd = malloc(1024);
		scanf("%[^\n]%*c", cmd);
		fflush(stdin);
		if (strcmp(cmd, "exit") == 0)
			break;
		char *word = malloc(20);
		char args[20][50];
		int argc = 0;
		int n_read = 0;
		int cpy_stdout = 1;
		int cpy_stdin = 0;
		while (sscanf(cmd, "%s%n", word, &n_read) != EOF)
		{
			strcpy(args[argc], word);
			argc = argc + 1;
			cmd = cmd + n_read + 1;
		}
		int i_buff = 0;
		const char *buff[20];
		int back_f = 0;
		for (int i = 0; i < argc; i++)
		{

			if (strcmp(args[i], ">") == 0) // output redirection
			{
				cpy_stdout = dup(1);
				int outfd = open(args[i + 1], O_WRONLY | O_CREAT);
				dup2(outfd, 1);
				i = i + 1;
				// printf("hello");
				// fflush(stdout);
				// dup2(cpy_stdout,1);
				// close(cpy_stdout);
			}
			else if (strcmp(args[i], "<") == 0) // inout redirection
			{
				cpy_stdin = dup(0);
				int infd = open(args[i + 1], O_RDONLY);
				dup2(infd, 0);
				i = i + 1;
				// char * temp=malloc(10);
				// scanf("%s",temp);
				// fflush(stdin);
				// dup2(cpy_stdin,0);
				// close(cpy_stdin);
				// printf("%s",temp);
			}
			else if (strcmp(args[i], "&") == 0)
				back_f = 1;
			else
			{
				buff[i_buff] = args[i];
				i_buff = i_buff + 1;
			}
		}
		buff[i_buff] = NULL;
		int pid = fork();
		if (pid == 0)
		{
			if (execvp(buff[0], buff) < 0)
			{
				fflush(stdout);
				write(cpy_stdout, &errno, sizeof(errno));
			}
			return;
		}
		else if (back_f == 0)
		{
			int status;

			waitpid(pid, &status, 0);
		}

		if (cpy_stdin != 0)
		{
			dup2(cpy_stdin, 0);
			close(cpy_stdin);
		}
		if (cpy_stdout != 1)
		{
			dup2(cpy_stdout, 1);
			close(cpy_stdout);
		}
	}
}
