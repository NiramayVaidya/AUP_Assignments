#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
	int fd = open("read_from_file", O_RDONLY);
	if (fd == -1) {
		perror("Open failed\n");
		return errno;
	}
	char buf[5] = {'#', '#', '#', '#', '\0'};
	printf("%s\n", buf);
	for (int i = 1; i < 4; i++) {
		// if (read(fd, buf, 1) == -1) {
		if (read(fd, &buf[i], 1) == -1) {
			perror("Read failed\n");
			return errno;
		}
	}
	printf("%s\n", buf);
	return 0;
}
