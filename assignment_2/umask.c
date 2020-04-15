#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

mode_t getumask(void) {
	mode_t mask = umask(0);
	umask(mask);
	return mask;
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: ./umask <filename> <filename>\n");
	}
	int fd = open(argv[1], O_RDONLY | O_CREAT, 0666);
	if (fd == -1) {
		perror("Open failed\n");
		return errno;
	}
	printf("%d\n", getumask());
	int fd_new = open(argv[2], O_RDONLY | O_CREAT, 0666);
	if (fd_new == -1) {
		perror("Open failed\n");
		return errno;
	}
	if (close(fd) == -1) {
		perror("Close failed\n");
		return errno;
	}
	if (close(fd_new) == -1) {
		perror("Close failed\n");
		return errno;
	}
	return 0;
}
