#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#define FIFO_NAME "transfer"

int main(int argc, char *argv[], char *envp[]) {
	pid_t pid = 0;
	/* process 0 */
	if ((pid = fork()) == -1) {
		perror("Fork failed in process 0\n");
		return errno;
	}
	else if (pid == 0) {
		/* process 1 */
		if ((pid = fork()) == -1) {
			perror("Fork failed in process 1\n");
			return errno;
		}
		else if (pid == 0) {
			/* process 5 */
			printf("process 5 pid: %ld ppid: %ld\n", (long)getpid(), (long)getppid());
			int fd;
			/* this is required in the case when the transfer FIFO file does not
			 * exist in the current directory and process 5 executes before
			 * process 3, then open fails, hence process 5 should remain in a
			 * while loop and only exit it once process 3 has executed and
			 * created the transfer FIFO in which case open here will succeed
			 */
			while ((fd = open(FIFO_NAME, O_RDONLY)) == -1);
			// if (fd == -1) {
			// 	perror("Open failed\n");
			// 	return errno;
			// }
			int num;
			long pgid_3;
			if ((num = read(fd, &pgid_3, sizeof(long))) == -1) {
				perror("Read failed\n");
			}
			else if (num < sizeof(long)) {
				printf("Expected number of bytes not read\n");
				exit(0);
			}
			printf("pgid_3 in process 5: %ld (to check FIFO transfer of pgid_3)\n", pgid_3);
			printf("(before setpgid with pgid_3) process 5 pgid: %ld\n", (long)getpgid(0));
			if (setpgid(0, (pid_t)pgid_3) == -1) {
				perror("Setpgid failed in process 5 for process 5\n");
				return errno;
			}
			printf("(after setpgid with pgid_3) process 5 pgid: %ld\n", (long)getpgid(0));
			sleep(10);
			return 0;
		}
		else {
			printf("process 1 pid: %ld ppid: %ld\n", (long)getpid(), (long)getppid());
			sleep(10);
			return 0;
		}
	}
	else {
		if ((pid = fork()) == -1) {
			perror("Fork failed in process 0\n");
			return errno;
		}
		else if (pid == 0) {
			/* process 2 */
			if ((pid = fork()) == -1) {
				perror("Fork failed in process 2\n");
				return errno;
			}
			else if (pid == 0) {
				/* process 3 */
				if ((pid = fork()) == -1) {
					perror("Fork failed in process 3\n");
					return errno;
				}
				else if (pid == 0) {
					/* process 4 */
					printf("process 4 pid: %ld ppid: %ld\n", (long)getpid(), (long)getppid());
					/* if setpgid here gets called before the setpgid(0, 0) call
					 * in process 3, then pgid of process 4 will be set to the
					 * old pgid of process 3, not its new one
					 */
					// setpgid(0, getpgid(getppid()));
					/* even if just printf is called here, if it gets called
					 * before the setpgid(pid, getpid(0)) call in process 3,
					 * then the pgid of process 4 will be printed as the old
					 * pgid instead of the new one
					 */
					// printf("process 4 pgid: %ld\n", (long)getpgid(0));
					sleep(10);
					return 0;
				}
				else {
					printf("process 3 pid: %ld ppid: %ld\n", (long)getpid(), (long)getppid());
					if (setpgid(0, 0) == -1) {
						perror("Setpgid failed in process 3 for process 3\n");
						return errno;
					}
					printf("process 3 pgid: %ld\n", (long)getpgid(0));
					if (setpgid(pid, getpgid(0)) == -1) {
						perror("Setpgid failed in process 3 for process 4\n");
					}
					printf("process 4 pgid: %ld\n", (long)getpgid(pid));
					if (access(FIFO_NAME, F_OK) == -1) {
						printf("In access\n");
						if (mkfifo(FIFO_NAME, S_IFIFO | 0666) == -1) {
							perror("Mkfifo failed\n");
							return errno;
						}
					}
					int fd = creat(FIFO_NAME, 0666);
					if (fd == -1) {
						perror("Creat failed\n");
						return errno;
					}
					int num;
					long pgid_3 = (long)getpgid(0);
					printf("pgid_3 in process 3: %ld (to check FIFO transfer of pgid_3)\n", pgid_3);
					if ((num = write(fd, &pgid_3, sizeof(long))) == -1) {
						perror("Write failed\n");
						return errno;
					}
					else if (num < sizeof(long)) {
						printf("Expected number of bytes not written\n");
						exit(0);
					}
					if (close(fd) == -1) {
						perror("Close failed in process 3\n");
						return errno;
					}
					sleep(10);
					return 0;
				}
			}
			else {
				printf("process 2 pid: %ld ppid: %ld\n", (long)getpid(), (long)getppid());
				sleep(10);
				return 0;
			}
		}
		else {
			printf("process 0 pid: %ld\n", (long)getpid());
			sleep(10);
		}
	}
	return 0;
}
