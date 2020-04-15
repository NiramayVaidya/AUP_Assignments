#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {    
	int fd1, fd2;
	char string[128];
	fd1 = open("stdin", O_RDONLY);
	if(fd1 == -1) {
		printf("Error: %s\n", strerror(errno));
		return errno;
	}
	if (close(0) == -1) {
		perror("Close failed\n");
		return errno;
	}
	if (dup(fd1) == -1) {
		perror("Dup failed\n");
		return errno;
	}
	if (close(fd1) == -1) {
		perror("Close failed\n");
		return errno;
	}
	fd2 = open("stdout", O_WRONLY);
	if(fd2 == -1) {
		printf("Error: %s\n", strerror(errno));
		return errno;
	}
	scanf("%s", string);
	/* even if printf is called immediately after scanf, it prints to the stdout
	 * file instead of printing to the terminal, find out why this happens
	 */
	if (close(1) == -1) {
		perror("Close failed\n");
		return errno;
	}
	if (dup(fd2) == -1) {
		perror("Dup failed\n");
		return errno;
	}
	if (close(fd2) == -1) {
		perror("Close failed\n");
		return errno;
	}
	printf("%s", string);
	return 0;
}
