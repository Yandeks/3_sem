
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

typedef struct{
	int ffd[2];
	int bfd[2];
} fdpipe_t;

#define length 13

void shiftbuf (int fd, char* buf, int size)
{
	char* shbuf = (char*)malloc((size+1)*sizeof(char));

	shbuf[size-1] = buf[1];
	for(int i = 0; i < size - 1; i++)
	{
		shbuf[i] = buf[i+1];
	}

	// to the execution 

	printf("RECIEVED_FROM_PARENT: %s\n", buf);

	write(fd, shbuf, size);

	free(shbuf);

	return;
}

int main()
{
	char buf1[length];
	char buf2[length];

	int size1;
	int size2;

	fdpipe_t fd;
	int status;

	if(pipe(fd.ffd) < 0)
	{
		perror("Pipe_Creation_Failed");
		return -1;
	}

	if(pipe(fd.bfd) < 0)
    {
    	perror("Pipe_Creation_Failed");
        return -1;
    }

    const pid_t pid = fork();
    if(pid < 0)
    {
    	perror("Fork_Creation_Failed");
		return -1;
    }

    if (pid)
    {
    	close(fd.ffd[1]);
    	close(fd.bfd[0]);

    	while(1)
    	{
    		while((size1 = read(0, buf1, sizeof(buf1) - 1 )) > 1)
    		{
    			if(buf1[size1 - 1] == '\n')
				size1--;
				buf1[size1] = '\0';

				write(fd.bfd[1], buf1, size1);

				if(!strcmp(buf1,"exit"))
                {
					waitpid(pid, &status, 0);
					printf("EXIT_CODE: %d\n", WEXITSTATUS(status));
					return 0;
                }

                printf("SEND_TO_CHILD: %s\n", buf1);

                if(size1 < length - 1)
                	break;

    		}

    		write(fd.bfd[1], "\n" , 1);

    		while(1)
    		{
    			size2 = read(fd.ffd[0], buf2, sizeof(buf2) - 1);
    			buf2[size2] = '\0';
    			if(buf2[size2 - 1] == '\n')
                {
                	if(size2 == 1)
                    	break;

                    buf2[size2 - 1] = '\0';
                    printf("RECIEVED_BACK_FROM_CHILD: %s\n", buf2);
                    break;
                }

                printf("RECIEVED_BACK_FROM_CHILD: %s\n", buf2);
    		}
    	}
    }
    else
    {
    	close(fd.bfd[1]);
    	close(fd.ffd[0]);

    	while(1)
    	{
    		size1 = read(fd.bfd[0], buf1, sizeof(buf1) - 1);
			buf1[size1] = '\0';
			if(!strcmp(buf1,"exit"))
				exit(1);

			if(buf1[size1 - 1] == '\n')
			{
				if(size1 == 1)
					break;
				buf1[size1 - 1] = '\0';
				shiftbuf(fd.ffd[1], buf1, size1 - 1);
				break;
			}

			shiftbuf(fd.ffd[1], buf1, size1);
    	}

			write(fd.ffd[1], "\n", 1);
    }

return 0;
}
