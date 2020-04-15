#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
	struct stat st;
	if (stat("files/regular", &st) == 0) {
		printf("regular\n");
	}
	else {
		perror("failed\n");
		printf("%d\n", errno);
		return errno;
	}
	return 0;
}
