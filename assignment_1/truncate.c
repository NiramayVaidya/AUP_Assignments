#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int mytruncate(int fd, int offset) {
	int size = lseek(fd, 0, SEEK_END);
	if (size == -1) {
		// perror("Lseek failed in truncate\n");
		// return errno;
		return 1;
	}
	if (size == offset) {
		return 0;
	}
	else if (size < offset) {
		if (lseek(fd, 1, SEEK_END) == -1) {
			return 1;
		}
		char ch = '\0';
		for (int i = offset - size; i > 0; i--) {
			if (write(fd, &ch, 1) != 1) {
				return 1;
			}
		}
		return 0;
	}
	else { // size > offset
		int pos;
		if ((pos = lseek(fd, offset, SEEK_SET)) == -1) {
			return 1;
		}
		char ch = '\0';
		for (int i = pos; i <= size; i++) {
			if (write(fd, &ch, 1) != 1) {
				return 1;
			}
		}
		return 0;
	}
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: ./truncate <filename> <offset>\n");
		return EINVAL;
	}
	// file already exists, opening in read write flag
	int fd = open(argv[1], O_RDWR);
	if (fd == -1) {
		perror("Open failed\n");
		return errno;
	}
	if (mytruncate(fd, atoi(argv[2])) == 1) {
		printf("Truncate failed\n");
		exit(0);
	}
	if (close(fd) == -1) {
		perror("Close failed\n");
		return errno;
	}
	return 0;
}
