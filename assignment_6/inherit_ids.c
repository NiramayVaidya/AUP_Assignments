#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>

int main(int argc, char *argv[], char *envp[]) {
	printf("Parent's real uid: %ld\n", (long)getuid());
	printf("Parent's effective uid: %ld\n", (long)geteuid());
	printf("Parent's real gid: %ld\n", (long)getgid());
	printf("Parent's effective gid: %ld\n", (long)getegid());
	printf("Parent's process id: %ld\n", (long)getpid());
	printf("Parent's parent process id: %ld\n", (long)getppid());
	pid_t pid = 0;
	if ((pid = fork()) == -1) {
		perror("Fork failed\n");
		return errno;
	}
	else if (pid == 0) {
		printf("Child's real uid: %ld\n", (long)getuid());
		printf("Child's effective uid: %ld\n", (long)geteuid());
		printf("Child's real gid: %ld\n", (long)getgid());
		printf("Child's effective gid: %ld\n", (long)getegid());
		printf("Child's process id: %ld\n", (long)getpid());
		printf("Child's parent process id: %ld\n", (long)getppid());
	}
	else {
		sleep(5);
	}
	return 0;
}
