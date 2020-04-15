#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: ./hole_in_file <filename with hole> <filename to copy to without hole>\n");
	}
	int fd = open(argv[1],  O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (fd == -1) {
		perror("Open failed\n");
		return errno;
	}
	if (lseek(fd, 10, SEEK_SET) == -1) {
		perror("Lseek failed\n");
		return errno;
	}
	int bytes;
	if ((bytes = write(fd, "##########", 10)) == -1) {
		perror("Write failed\n");
		return errno;
	}
	if (bytes < 10) {
		printf("Less bytes written than requested\n");
		exit(0);
	}
	if (lseek(fd, 30, SEEK_SET) == -1) {
		perror("Lseek failed\n");
		return errno;
	}
	if ((bytes = write(fd, "##########", 10)) == 1) {
		perror("Write failed\n");
		return errno;
	}
	if (bytes < 10) {
		printf("Less bytes written than requested\n");
		exit(0);
	}
	if (lseek(fd, 0, SEEK_SET) == -1) {
		perror("Lseek failed\n");
	}
	system("od -c temp_file_with_hole.txt");
	int fd_new = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (fd_new == -1) {
		perror("Open failed\n");
		return errno;
	}
	char ch;
	while (read(fd, &ch, 1) == 1 && ch != EOF) {
		if (ch != '\0') {
			if (bytes = write(fd_new, &ch, 1) == -1) {
				perror("Write failed\n");
				return errno;
			}
			/*
			if (bytes < 1) {
				printf("Less bytes written than requested\n");
				exit(0);
			}
			*/
		}
	}
	system("od -c temp_file_without_hole.txt");
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
