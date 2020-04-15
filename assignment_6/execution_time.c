#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[], char *envp[]) {
	pid_t pid = 0;
	float getpid_t = 0;
	float fork_t = 0;
	struct timeval tv;
	suseconds_t initial_t = 0;
	printf("Timings results are in microseconds\n");
	for (int i = 0; i < 10; i++) {
		if (gettimeofday(&tv, NULL) == -1) {
			perror("Gettimeofday failed\n");
			return errno;
		}
		initial_t = tv.tv_usec;
		pid = getpid();
		if (gettimeofday(&tv, NULL) == -1) {
			perror("Gettimeofday failed\n");
			return errno;
		}
		printf("getpid (%d) : %d\n", i + 1, (int)(tv.tv_usec - initial_t));
		getpid_t += (float)(tv.tv_usec - initial_t);
		if (gettimeofday(&tv, NULL) == -1) {
			perror("Gettimeofday failed\n");
			return errno;
		}
		initial_t = tv.tv_usec;
		if ((pid = fork()) == -1) {
			perror("Fork failed\n");
			return errno;
		}
		else if (pid == 0) {
			exit(1);
		}
		if (gettimeofday(&tv, NULL) == -1) {
			perror("Gettimeofday failed\n");
			return errno;
		}
		printf("fork (%d) : %d\n", i + 1, (int)(tv.tv_usec - initial_t));
		fork_t += (float)(tv.tv_usec - initial_t);
	}
	getpid_t /= 10.0;
	fork_t /= 10.0;
	printf("getpid avg : %f\n", getpid_t);
	printf("fork avg : %f\n", fork_t);
	return 0;
}
