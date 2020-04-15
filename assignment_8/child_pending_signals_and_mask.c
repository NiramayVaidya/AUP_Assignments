#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void sig_usr1(int signo) {
	printf("SIGUSR1 (%d) received, pid = %ld\n", signo, (long)getpid());
}

void sig_usr2(int signo) {
	printf("SIGUSR2 (%d) received, pid = %ld\n", signo, (long)getpid());
}

int main(int argc, char *argv[]) {
	pid_t pid;
	sigset_t sigset;
	printf("parent pid = %ld\n", (long)getpid());
	if (sigemptyset(&sigset) == -1) {
		perror("Sigemptyset failed\n");
		return errno;
	}
	if (sigaddset(&sigset, SIGUSR1) == -1) {
		perror("Sigaddset for SIGUSR1 failed\n");
		return errno;
	}
	if (sigaddset(&sigset, SIGUSR2) == -1) {
		perror("Sigaddset for SIGUSR2 failed\n");
		return errno;
	}
	if (sigprocmask(SIG_SETMASK, &sigset, NULL) == -1) {
		perror("Sigprocmask failed\n");
	}
	if (signal(SIGUSR1, sig_usr1) == SIG_ERR) {
		perror("Signal for SIGUSR1 failed\n");
		return errno;
	}
	if (signal(SIGUSR2, sig_usr2) == SIG_ERR) {
		perror("Signal for SIGUSR2 failed\n");
		return errno;
	}
	printf("Before SIGUSR1 and SIGUSR2 getting generated (before call to kill)\n");
	if (sigismember(&sigset, SIGUSR1)) {
		printf("SIGUSR1 has been blocked in parent\n");
	}
	if (sigismember(&sigset, SIGUSR2)) {
		printf("SIGUSR2 has been blocked in parent\n");
	}
	/* this will generate the SIGUSR1 and SIGUSR2 signals in parent but since
	 * they are blocked they will remain in the pending state
	 */
	if (kill(getpid(), SIGUSR1) == -1) {
		perror("Kill for SIGUSR1 failed\n");
		return errno;
	}
	if (kill(getpid(), SIGUSR2) == -1) {
		perror("Kill for SIGUSR2 failed\n");
		return errno;
	}
	sigset_t sigset_pending;
	if (sigpending(&sigset_pending) == -1) {
		perror("Sigpending failed\n");
		return errno;
	}
	printf("After SIGUSR1 and SIGUSR2 getting generated (after call to kill)\n");
	if (sigismember(&sigset_pending, SIGUSR1)) {
		printf("SIGUSR1 is pending in parent\n");
	}
	else {
		printf("SIGUSR1 is not pending in parent\n");
	}
	if (sigismember(&sigset_pending, SIGUSR2)) {
		printf("SIGUSR2 is pending in parent\n");
	}
	else {
		printf("SIGUSR2 is not pending in parent\n");
	}
	/* this will remove SIGUSR1 from being blocked and hence the signal will get
	 * out of its pending state to be delivered to the parent once sigprocmask
	 * is called to set this new mask without SIGUSR1 blocked
	 */
	if (sigdelset(&sigset, SIGUSR1) == -1) {
		perror("Sigdelset failed\n");
		return errno;
	}
	if (sigprocmask(SIG_SETMASK, &sigset, NULL) == -1) {
		perror("Sigprocmask failed\n");
	}
	if (sigpending(&sigset_pending) == -1) {
		perror("Sigpending failed\n");
		return errno;
	}
	printf("After SIGUSR1 is removed from being blocked but SIGUSR2 is not (after call to sigdelset)\n");
	if (sigismember(&sigset_pending, SIGUSR1)) {
		printf("SIGUSR1 is pending in parent\n");
	}
	else {
		printf("SIGUSR1 is not pending in parent\n");
	}
	if (sigismember(&sigset_pending, SIGUSR2)) {
		printf("SIGUSR2 is pending in parent\n");
	}
	else {
		printf("SIGUSR2 is not pending in parent\n");
	}
	if ((pid = fork()) == -1) {
		perror("Fork failed\n");
		return errno;
	}
	else if (pid == 0) {
		printf("child pid = %ld\n", (long )getpid());
		/* the child's signal mask will be inherited from the parent and will
		 * contain SIGUSR2 but not SIGUSR1, hence SIGUSR2 will be blocked but
		 * not SIGUSR1
		 */
		if (sigismember(&sigset, SIGUSR1)) {
			printf("SIGUSR1 has been blocked in child\n");
		}
		else {
			printf("SIGUSR1 has not been blocked in child\n");
		}
		if (sigismember(&sigset, SIGUSR2)) {
			printf("SIGUSR2 has been blocked in child\n");
		}
		else {
			printf("SIGUSR2 has not been blocked in child\n");
		}
		/* since pending signals from the parent are not inherited by the child,
		 * if SIGUSR2 is unblocked in the child, it will not be delivered to the
		 * child
		 */
		if (sigpending(&sigset_pending) == -1) {
			perror("Sigpending failed\n");
			return errno;
		}
		if (sigismember(&sigset_pending, SIGUSR1)) {
			printf("SIGUSR1 is pending in child\n");
		}
		else {
			printf("SIGUSR1 is not pending in child\n");
		}
		if (sigismember(&sigset_pending, SIGUSR2)) {
			printf("SIGUSR2 is pending in child\n");
		}
		else {
			printf("SIGUSR2 is not pending in child\n");
		}
		if (sigdelset(&sigset, SIGUSR2) == -1) {
			perror("Sigdelset failed\n");
			return errno;
		}
		if (sigprocmask(SIG_SETMASK, &sigset, NULL) == -1) {
			perror("Sigprocmask failed\n");
		}
		if (sigpending(&sigset_pending) == -1) {
			perror("Sigpending failed\n");
			return errno;
		}
		printf("After SIGUSR1 is removed from being blocked (after call to sigdelset)\n");
		if (sigismember(&sigset_pending, SIGUSR1)) {
			printf("SIGUSR1 is pending in child\n");
		}
		else {
			printf("SIGUSR1 is not pending in child\n");
		}
		if (sigismember(&sigset_pending, SIGUSR2)) {
			printf("SIGUSR2 is pending in child\n");
		}
		else {
			printf("SIGUSR2 is not pending in child\n");
		}
	}
	return 0;
}
