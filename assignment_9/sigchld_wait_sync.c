#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void sig_chld(int signo) {
	int status;
	printf("SIGCHLD (%d) received, pid = %d\n", signo, (int)getpid());
	printf("Before wait in SIGCHLD handler\n");
	// if (waitpid(-1, &status, 0) == -1) {
	if (wait(&status) == -1) {
		printf("Wait failed in sig_chld\n");
	}
	else {
		printf("Wait succeeded in sig_chld\n");
	}
}

int main(int argc, char *argv[]) {
	pid_t pid;
	int status;
	if (signal(SIGCHLD, sig_chld) == SIG_ERR) {
		perror("Signal failed\n");
		return errno;
	}
	if ((pid = fork()) == -1) {
		perror("Fork failed\n");
		return errno;
	}
	else if (pid == 0) {
		int tsec = 5;
		printf("Child sleeping for %d secs\n", tsec);
		sleep(tsec);
	}
	else {
		printf("Before wait in parent\n");
		// if (waitpid(-1, &status, 0) == -1) {
		if (wait(&status) == -1) {
			printf("Wait failed in parent\n");
		}
		else {
			printf("Wait succeeded in parent\n");
		}
	}
	return 0;
}
