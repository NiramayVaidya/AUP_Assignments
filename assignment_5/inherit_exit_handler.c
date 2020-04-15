#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

void exit_handler(void) {
	printf("In the exit_handler\n");
}

int main(int arc, char *argv[], char *envp[]) {
	if (atexit(exit_handler) != 0) {
		perror("Atexit failed\n");
		return errno;
	}
	pid_t pid = 0;
	if ((pid = fork()) == -1) {
		perror("Fork failed\n");
		return errno;
	}
	else if (pid == 0) {
		printf("Child is done\n");
		exit(1);
	}
	else {
		printf("Parent is done\n");
	}
	// printf("Parent is done\n");
	return 0;
}
