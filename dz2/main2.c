#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define size_ 128

typedef struct {
	int txd[2];
	int rxd[2];
} fdpipe_t;

char* shift (char* buf, size_t size ) {
	char* temp2 = (char*)malloc((size+1)*sizeof(char));
	int i;
	temp2[0] = buf[size-2];
	for (i =1; i < size-1; i++) {
		temp2[i] = buf[i-1];
	}
	temp2[size-1]='\0';
	return temp2;
}




int main (){
	char buf1[size_];
	char buf2[size_];
	int size1;
	int size2;
	fdpipe_t fd;
	int status;

	if(pipe(fd.txd) < 0 || pipe(fd.rxd) < 0) {
		perror("pipe creation is failed");
		exit(1);
	}

	const pid_t pid = fork();

	if(pid < 0) {
		perror("fork creation is failed");
		exit(1);
	}

	if (pid > 0) {
		close(fd.txd[0]);
		close(fd.rxd[1]);

		while(1) {
			int size = read(0, buf1, sizeof(buf1) - 1);
				buf1[size] = '\0';
				write(fd.txd[1], buf1, size+1);
				if(strcmp(buf1, "exit\n")==0) {
					waitpid(pid, &status, 0);
					printf("process exited with %d\n", WEXITSTATUS(status));
					exit(0);
				}
				printf("sent to child: %s\n", buf1);

			while(1) {
				size2 = read(fd.rxd[0], buf2, sizeof(buf2) - 1);
				printf("recieved from child: %s\n", buf2);
				break;
			}
		}
	} else {
		close(fd.rxd[0]);
		close(fd.txd[1]);

		while(1) {
			while((size1 = read(fd.txd[0], buf1, sizeof(buf1) - 1)) == 0);
			if(strcmp(buf1, "exit\n")==0)
			{	
				exit(0);
			}
			printf("recieved from parent: %s\n", buf1);
			char* temp = shift(buf1, size1-1);
			printf("sent to parent: %s\n", temp);
			write(fd.rxd[1], temp, size1);
			free(temp);
			
		}
	}
}