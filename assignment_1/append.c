#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: ./append <filename>\n");
		return EINVAL;
	}
	int fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (fd == -1) {
		perror("Open failed\n");
		return errno;
	}
	int bytes;
	if ((bytes = write(fd, "abcde", 5)) == -1) {
		perror("Write failed\n");
		return errno;
	}
	if (bytes < 5) {
		printf("Less bytes written than requested\n");
		exit(0);
	}
	if (close(fd) == -1) {
		perror("Close failed\n");
		return errno;
	}
	int fd_new = open(argv[1], O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (fd_new == -1) {
		perror("Open failed\n");
		return errno;
	}
	if (lseek(fd, 0, SEEK_SET) == -1) {
		perror("Lseek failed\n");
		return errno;
	}
	if ((bytes = write(fd_new, "12345", 5)) == -1) {
		perror("Write failed\n");
		return errno;
	}
	if (bytes < 5) {
		printf("Less bytes written than requested\n");
		exit(0);
	}
	if (close(fd_new) == -1) {
		perror("Close failed\n");
		return errno;
	}
	return 0;
}
