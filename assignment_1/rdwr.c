#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: ./rdwr <filename>\n");
		return EINVAL;
	}
	// file already exists, opening in read write flag
	int fd = open(argv[1], O_RDWR);
	if (fd == -1) {
		perror("Open failed\n");
		return errno;
	}
	int bytes;
	char chars[10];
	// assumption that 10 characters have to be read hence there will be at
	// least 10 characters in the file
	if ((bytes = read(fd, chars, 10)) == -1) {
		perror("Read failed\n");
		return errno;
	}
	if (bytes < 10) {
		printf("Less bytes read than requested\n");
		exit(0);
	}
	int ch;
	while (read(fd, &ch, 1) == 1 && ch != EOF);
	if ((bytes = write(fd, "hello", 5)) == -1) {
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
	return 0;
}
