#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

/* atime for a file changes when read is called or when an application like cat,
 * vi, etc. opens the file
 */

int main(int argc, char *argv[]) {
	int fd;
	struct stat statbuf;
	char buf[8];
	printf("Times for file before open:\n");
	if (stat("before_after", &statbuf) == -1) {
		perror("Stat failed\n");
		return errno;
	}
	printf("atime: %ld mtime: %ld ctime: %ld\n", statbuf.st_atime, statbuf.st_mtime, statbuf.st_ctime);
	printf("Times for parent directory before open:\n");
	if (stat(".", &statbuf) == -1) {
		perror("Stat failed\n");
		return errno;
	}
	printf("atime: %ld mtime: %ld ctime: %ld\n", statbuf.st_atime, statbuf.st_mtime, statbuf.st_ctime);
	fd = open("before_after", O_RDWR);
	if(fd == -1) {
		printf("Error: %s\n", strerror(errno));
		return errno;
	}
	printf("Times for file after open:\n");
	if (stat("before_after", &statbuf) == -1) {
		perror("Stat failed\n");
		return errno;
	}
	printf("atime: %ld mtime: %ld ctime: %ld\n", statbuf.st_atime, statbuf.st_mtime, statbuf.st_ctime);
	printf("Time for parent directory after open:\n");
	if (stat(".", &statbuf) == -1) {
		perror("Stat failed\n");
		return errno;
	}
	printf("atime: %ld mtime: %ld ctime: %ld\n", statbuf.st_atime, statbuf.st_mtime, statbuf.st_ctime);
	sleep(1);
	printf("Times for file before write:\n");
	if (stat("before_after", &statbuf) == -1) {
		perror("Stat failed\n");
		return errno;
	}
	printf("atime: %ld mtime: %ld ctime: %ld\n", statbuf.st_atime, statbuf.st_mtime, statbuf.st_ctime);
	printf("Times for parent directory before write:\n");
	if (stat(".", &statbuf) == -1) {
		perror("Stat failed\n");
		return errno;
	}
	printf("atime: %ld mtime: %ld ctime: %ld\n", statbuf.st_atime, statbuf.st_mtime, statbuf.st_ctime);
	if (write(fd, "coep", sizeof("coep")) == -1) {
		printf("Error: %s\n", strerror(errno));
		return errno;
	}
	printf("Times for file after write:\n");
	if (stat("before_after", &statbuf) == -1) {
		perror("Stat failed\n");
		return errno;
	}
	printf("atime: %ld mtime: %ld ctime: %ld\n", statbuf.st_atime, statbuf.st_mtime, statbuf.st_ctime);
	printf("Time for parent directory after write:\n");
	if (stat(".", &statbuf) == -1) {
		perror("Stat failed\n");
		return errno;
	}
	printf("atime: %ld mtime: %ld ctime: %ld\n", statbuf.st_atime, statbuf.st_mtime, statbuf.st_ctime);
	sleep(1);
	printf("Times for file before access:\n");
	if (stat("before_after", &statbuf) == -1) {
		perror("Stat failed\n");
		return errno;
	}
	printf("atime: %ld mtime: %ld ctime: %ld\n", statbuf.st_atime, statbuf.st_mtime, statbuf.st_ctime);
	printf("Times for parent directory before access:\n");
	if (stat(".", &statbuf) == -1) {
		perror("Stat failed\n");
		return errno;
	}
	printf("atime: %ld mtime: %ld ctime: %ld\n", statbuf.st_atime, statbuf.st_mtime, statbuf.st_ctime);
	if(access("before_after", F_OK) == -1) {
		printf("Error: %s\n", strerror(errno));
		return errno;
	}
	printf("Times for file after access:\n");
	if (stat("before_after", &statbuf) == -1) {
		perror("Stat failed\n");
		return errno;
	}
	printf("atime: %ld mtime: %ld ctime: %ld\n", statbuf.st_atime, statbuf.st_mtime, statbuf.st_ctime);
	printf("Time for parent directory after access:\n");
	if (stat(".", &statbuf) == -1) {
		perror("Stat failed\n");
		return errno;
	}
	printf("atime: %ld mtime: %ld ctime: %ld\n", statbuf.st_atime, statbuf.st_mtime, statbuf.st_ctime);
	sleep(1);
	printf("Times for file before chmod:\n");
	if (stat("before_after", &statbuf) == -1) {
		perror("Stat failed\n");
		return errno;
	}
	printf("atime: %ld mtime: %ld ctime: %ld\n", statbuf.st_atime, statbuf.st_mtime, statbuf.st_ctime);
	printf("Times for parent directory before chmod:\n");
	if (stat(".", &statbuf) == -1) {
		perror("Stat failed\n");
		return errno;
	}
	printf("atime: %ld mtime: %ld ctime: %ld\n", statbuf.st_atime, statbuf.st_mtime, statbuf.st_ctime);
	if (chmod("before_after", 0666) == -1) {
		printf("Error: %s\n", strerror(errno));
	}
	printf("Times for file after chmod:\n");
	if (stat("before_after", &statbuf) == -1) {
		perror("Stat failed\n");
		return errno;
	}
	printf("atime: %ld mtime: %ld ctime: %ld\n", statbuf.st_atime, statbuf.st_mtime, statbuf.st_ctime);
	printf("Time for parent directory after chmod:\n");
	if (stat(".", &statbuf) == -1) {
		perror("Stat failed\n");
		return errno;
	}
	printf("atime: %ld mtime: %ld ctime: %ld\n", statbuf.st_atime, statbuf.st_mtime, statbuf.st_ctime);
	close(fd);
	return 0;
}
