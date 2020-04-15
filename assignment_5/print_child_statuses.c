#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>

int main(int argc, char *argv[], char *envp[]) {
	if (argc != 4) {
		printf("Usage: ./print_child_statuses <number of children> <sleep time for children (sec)> <sleep time for parent (sec)>\n");
		exit(0);
	}
	int num_children = atoi(argv[1]);
	int sleep_child = atoi(argv[2]);
	int sleep_parent = atoi(argv[3]);
	pid_t *pids = (pid_t *)malloc(num_children * sizeof(pid_t));
	if (pids == NULL) {
		perror("Malloc failed\n");
		return errno;
	}
	pid_t child_pid = 0;
	int i, pos = 0, sleep_time;
	for (i = 1; i <= num_children; i++) {
		if ((child_pid = fork()) < 0) {
			perror("Fork failed\n");
			return errno;
		}
		else if (child_pid == 0) {
			if (i == 1 || i == num_children - 2) {
				sleep_time = sleep_child * 10;
				sleep(sleep_time);
				// usleep(sleep_time);
				printf("Process with sleep %ds: %ld\n", sleep_time, (long)getpid());
			}
			else {
				sleep(sleep_child);
				// usleep(sleep_child);
			}
			// sleep(sleep_child);
			exit(i);
		}
		else {
			pids[pos++] = child_pid;
		}
	}
	sleep(sleep_parent);
	// usleep(sleep_parent);
	int status;
	int ret;
	for (i = 0; i < num_children; i++) {
		if ((ret = waitpid(pids[i], &status, WNOHANG)) == -1) {
			perror("Waitpid failed\n");
			return errno;
		}
		else if (ret == 0) {
			printf("Process with pid %ld not yet terminated\n", (long)pids[i]);
		}
		else {
			printf("Process with pid %ld terminated with status %d\n", (long)pids[i], WIFEXITED(status) ? WEXITSTATUS(status) : 0);
		}
	}
	free(pids);
	return 0;
}
