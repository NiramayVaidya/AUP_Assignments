#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
// #include <time.h>

static void sig_hup(int signo) {
	struct timeval tv;
	if (gettimeofday(&tv, NULL) < 0) {
		perror("Gettimeofday failed in sig_hup\n");
	}
	printf("SIGHUP (%d) received, pid = %d, time of execution = %llu\n", signo, (int)getpid(), (unsigned long long)tv.tv_usec);
	// printf("SIGHUP (%d) received, pid = %d, time = %d\n", signo, (int)getpid(), (int)time(NULL));
}

static void sig_cont(int signo) {
	struct timeval tv;
	if (gettimeofday(&tv, NULL) < 0) {
		perror("Gettimeofday failed in sig_hup\n");
	}
	printf("SIGCONT (%d) received, pid = %d, time of execution = %llu\n", signo, (int)getpid(), (unsigned long long)tv.tv_usec);
	// printf("SIGHUP (%d) received, pid = %d, time = %d\n", signo, (int)getpid(), (int)time(NULL));
}

static void pr_ids(char *name) {
	printf("%s: pid = %d, ppid = %d, pgrp = %d\n", name, (int)getpid(), (int)getppid(), (int)getpgrp());
	fflush(stdout);
}

int main(int argc, char *argv[]) {
	char c;
	pid_t pid;
	pr_ids("parent");
	if ((pid = fork()) < 0) {
		perror("Fork failed\n");
		return errno;
	}
	else if (pid > 0) {
		sleep(5);
	}
	else {
		pr_ids("child");
		if (signal(SIGHUP, sig_hup) == SIG_ERR) {
			perror("Signal for SIGHUP failed\n");
			return errno;
		}
		if (signal(SIGCONT, sig_cont) == SIG_ERR) {
			perror("Signal for SIGCONT failed\n");
			return errno;
		}
		if (kill(getpid(), SIGTSTP) < 0) {
			perror("Kill failed\n");
			return errno;
		}
		pr_ids("child");
	}
	return 0;
}
