#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define FIFO_NAME "america"

int main(int argc, char *argv[]) {
	char s[100];
	int num, fd;
	struct stat st;
	printf("Waiting for writers\n");
	fd = open(FIFO_NAME, O_RDONLY);
	if (fd == -1) {
		perror("Open failed\n");
		return errno;
	}
	printf("Got a writer\n");
	do {
		if ((num = read(fd, s, 300)) == -1) {
			perror("Read failed\n");
			if (close(fd) == -1) {
				perror("Close failed\n");
				return errno;
			}
			return errno;
		}
		else {
			s[num] = '\0';
			printf("Read %d bytes: %s\n", num, s);
			if (fstat(fd, &st) == -1) {
				perror("Stat failed\n");
				return errno;
			}
			printf("Size of FIFO after server reading: %d\n", (int)st.st_size);
		}
	} while (num > 0);
	if (close(fd) == -1) {
		perror("Close failed\n");
		return errno;
	}
	return 0;
}
