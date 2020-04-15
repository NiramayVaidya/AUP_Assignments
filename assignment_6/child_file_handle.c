#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {
	int fd = open("parent_file", O_RDWR | O_CREAT | O_APPEND, 0666);
	pid_t pid = 0;
	if ((pid = fork()) == -1) {
		perror("Fork failed\n");
		return errno;
	}
	else if (pid == 0) {
		int size = 0;
		if ((size = write(fd, "coep", sizeof(char) * 4)) == -1) {
			perror("Write failed\n");
			return errno;
		}
		else if (size < (sizeof(char) * 4)) {
			printf("Less number of bytes written than provided\n");
			exit(0);
		}
	}
	else {
		sleep(5);
	}
	return 0;
}
