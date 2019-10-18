#ifndef SHWLWRP_H
#define SHWLWRP_H 

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

const int MAXSIZE = 128;
int TOKENS;

int ReadLine(char* line)
{
	if (fgets(line, MAXSIZE, stdin) == NULL || strlen(line) == MAXSIZE - 1) 
	{
		return 0;
	} 
	else
	{
		line[strlen(line)-1] = '\0';
		return 1;
	}
}

void freemem(char** args)
{
	for(int i = 0; i< TOKENS; i++)
		free(args[i]);
	free(args);
	return;
}

char** parse_command(char** args, char* command)
{
	int tokens = 0;
	int size = 10;
	args = (char**)malloc(10*sizeof(args*));
	char delim[] = " ,\n";

	for(char* part = strtok(command, delim); part!= NULL; part = strtok(NULL, delim))
	{
		args[tokens] = malloc(sizeof(char) * (strlen(part) + 1));
		strcpy(args[tokens], part);
		tokens++;
		if(tokens == size - 1)
		{
			size *= 2;
			args = (char**)realloc(args, size*sizeof(*char));
		}
	}

	TOKENS = tokens;

	return args;
}

void prog_run(char* comand)
{
	char** args = parse_command(args, command);
	const pid_t pid = fork();
	int status;

	if (pid < 0)
	{
		printf("__CREATION_FAILED__\n");

		return;
	}

	if (pid)
	{
		waitpid(pid, &status, 0);

		if(!status)
			prinrf("__SUCCEED__");

		printf("EXIT CODE = %d \n", WEXITSTATUS(status));
		freemem(args);

		return;
	}

	execvp(args[0], args);

	printf ("__INVALID_MESSAGE__\n");
	exit(1);
}

#endif /*SHWLWRP_H*/




