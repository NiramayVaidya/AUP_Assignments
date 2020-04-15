#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void sig_chld(int signo) {
	printf("SIGCHLD (%d) received, pid = %d\n", signo, (int)getpid());
}

int main(int argc, char *argv[]) {
	pid_t pid;
	int status, err;
	sigset_t sigset, sigsetpending;
	if (signal(SIGCHLD, sig_chld) == SIG_ERR) {
		perror("Signal failed\n");
		return errno;
	}
	if (sigemptyset(&sigset) == -1) {
		perror("Sigemptyset failed\n");
		return errno;
	}
	if (sigaddset(&sigset, SIGCHLD) == -1) {
		perror("Sigaddset for SIGCHLD failed\n");
		return errno;
	}
	if (sigprocmask(SIG_SETMASK, &sigset, NULL) == -1) {
		perror("Sigprocmask failed\n");
	}
	if ((pid = fork()) == -1) {
		perror("Fork failed\n");
		return errno;
	}
	else if (pid == 0) {
		exit(0);
	}
	else {
		printf("Parent sleeping for 5 secs\n");
		sleep(5);
		if (sigpending(&sigsetpending) == -1) {
			perror("Sigpending failed\n");
			return errno;
		}
		if ((err = sigismember(&sigsetpending, SIGCHLD)) == -1) {
			perror("Sigismember failed\n");
			return errno;
		}
		else if (err == 1) {
			printf("SIGCHLD pending\n");
		}
		// if (waitpid(-1, &status, 0) == -1) {
		if (wait(&status) == -1) {
			perror("Wait failed\n");
			return errno;
		}
		else {
			printf("Wait returned\n");
		}
	}
	return 0;
}
