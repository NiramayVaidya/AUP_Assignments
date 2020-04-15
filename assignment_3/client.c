#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define FIFO_NAME "america"

int main(int argc, char *argv[1]) {
	char s[300];
	int num, fd;
	struct stat st;
	if (mkfifo(FIFO_NAME, S_IFIFO | 0666) == -1) {
		perror("Mkfifo failed\n");
		return errno;
	}
	printf("Waiting for readers\n");
	fd = open(FIFO_NAME, O_WRONLY);
	if (fd == -1) {
		perror("Open failed\n");
		return errno;
	}
	printf("Got a reader, provide text\n");
	while (gets(s), !feof(stdin)) {
		if (fstat(fd, &st) == -1) {
			perror("Stat failed\n");
			return errno;
		}
		printf("Size of FIFO before client writing: %d\n", (int)st.st_size);
		if ((num = write(fd, s, strlen(s))) == -1) {
			perror("Write failed\n");
			if (close(fd) == -1) {
				perror("Close failed\n");
				return errno;
			}
			return errno;
		}
		else {
			printf("Wrote %d bytes\n", num);
			if (fstat(fd, &st) == -1) {
				perror("Stat failed\n");
				return errno;
			}
			/* this will not exactly print the FIFO size as per given condition
			 * since write by default is blocking and hence will wait for the
			 * read to finish and only then will statements ahead of it in this
			 * code execute
			 */
			//TODO find out how to get FIFO size immediately after write and
			//before the server reads it
			printf("Size of FIFO after client writing but before server reading: %d\n", (int)st.st_size);
		}
	}
	if (close(fd) == -1) {
		perror("Close failed\n");
		return errno;
	}
	return 0;
}
