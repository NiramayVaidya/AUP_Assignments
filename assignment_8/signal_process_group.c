#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>

#define FIFO_NAME "transfer"

long *shared_pgid_3;

void sig_usr1(int signo) {
	printf("SIGUSR1 (%d) received, pid = %ld\n", signo, (long)getpid());
}

void sig_alrm(int signo) {
	printf("SIGALRM (%d) received, pid = %ld\n", signo, (long)getpid());
	/* this is being done to detect that the signal handler for SIGALRM has been
	 * hit and hence the while loop in process 0 has run for too long since the
	 * value of shared_pgid_3 which was set to -1 at the start in process 0 has
	 * not yet been changed by process 5, hence make its value -2 to get out of
	 * the while loop and print corresponding message by checking the value to
	 * be -2
	 */
	*shared_pgid_3 = -2;
}

int main(int argc, char *argv[], char *envp[]) {
	pid_t pid = 0;
	/* process 0 */
	/* calling mmap to create a shared memory region here ensures that
	 * process 5 has already been moved to the process group created by
	 * 3
	 * the shared memory will contain the pgid of process group created
	 * by 3 which will then be read by process 0 to give a signal to it
	 *
	 * calling mmap here to create a shared memory region so that children of 
	 * process 0 and their children will have read+write access to this shared 
	 * memory since the call returns a pointer to such an allocated memory whose
	 * location remains constantly pointed to be the global shared_pgid_3
	 * pointer, this global pointer gets copied down the process tree to process
	 * 5 which can then store the obtained pgid_3 in it at the location pointed
	 * to by shared_pgid_3
	 */
	if ((shared_pgid_3 = (long *)mmap(NULL, (size_t)sizeof(long), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) {
		perror("Mmap failed\n");
		return errno;
	}
	/* this is being done to keep running a while loop for some finite amount of
	 * time defined by the alarm calls in process 0 by checking the value
	 * pointed to by shared_pgid_3 against -1, the while loop will exit either
	 * when the alaram expires or process 5 changes the value pointed to by
	 * shared_pgid_3 to anything other than -2 and -1, since these are not valid
	 * process group ids
	 */
	*shared_pgid_3 = -1;
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
			/* checking the value of shared_pgid_3 to validate functioning of
			 * mmap, ideally the value should be -1 as set by process 0
			 * TODO check if another possible value could be -2 in the scenario
			 * when process 0 continues till the alarm expires and the sig_alrm
			 * handler changes the value to -2 and process 5 executes 
			 * just before the if condition check following the break of while 
			 * loop in process 0
			 */
			printf("Value pointed to by shared_pgid_3 (-1) in process 5 before it changes the value to pgid_3: %ld (to check working of mmap)\n", *shared_pgid_3);
			/* storing the value of pgid_3 in the shared memory region created
			 * by process 0 pointed to by shared_pgid_3
			 * this is being done here instead of doing it in process 3 to
			 * ensure that process 5 has been shifted to the proces group
			 * created by 3 beforehand so that it too receives SIGUSR1 signal
			 * from process 0
			 */
			*shared_pgid_3 = pgid_3;
			/* setting signal handler for SIGUSR1 in process 5
			 */
			if (signal(SIGUSR1, sig_usr1) == SIG_ERR) {
				perror("Signal failed for SIGUSR1 in process 5\n");
				return errno;
			}
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
					/* setting signal handler for SIGUSR1 in process 4
					 */
					if (signal(SIGUSR1, sig_usr1) == SIG_ERR) {
						perror("Signal failed for SIGUSR1 in process 4\n");
						return errno;
					}
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
					/* setting signal handler for SIGUSR1 in process 3
					 */
					if (signal(SIGUSR1, sig_usr1) == SIG_ERR) {
						perror("Signal failed for SIGUSR1 in process 3\n");
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
			/* setting a signal handler for alarm and then setting an alarm to
			 * break out of the while loop by changing the value of
			 * shared_pgid_3 to -2 in the signal handler for alarm
			 */
			if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
				perror("Signal failed for SIGALRM in process 0\n");
				return errno;
			}
			alarm(5);
			while (*shared_pgid_3 == -1);
			alarm(0);
			if (*shared_pgid_3 == -2) {
				printf("Stuck in while for too long, shared memory transfer failed, signal to process group created by 3 not generated\n");
				return 0;
			}
			printf("Value pointed to by shared_pgid_3 (pgid_3) in process 0: %ld (to check working of mmap)\n", *shared_pgid_3);
			printf("Sending SIGUSR1 signal to the process group created by 3...\n");
			/* sending the SIGUSR1 signal to process group created by 3
			 * kill succeeds here i.e. in process 0 since its real uid equals
			 * those of all processes in the process group created by 3 i.e. 3, 
			 * 4 and 5
			 */
			if (kill((pid_t)(-(*shared_pgid_3)), SIGUSR1) == -1) {
				perror("Kill failed\n");
				return errno;
			}
			else {
				printf("SIGUSR1 signal sent to the process group created by 3 successfully\n");
			}
			/* to undo the shared memory setting using shared_pgid_3 made by mmap
			 * though this region will automatically get unmapped after process
			 * 0 terminates
			 */
			if (munmap((void *)shared_pgid_3, (size_t)sizeof(long)) == -1) {
				perror("Munmap failed\n");
				return errno;
			}
			if (shared_pgid_3 == NULL) {
				printf("Shared memory region reference pointer shared_pgid_3 in process 0: NULL (to check working of munmap)\n");
			}
			/* if uncommented, this causes a segmentation fault to occur due to
			 * invalid memory reference as a result of calling munmap
			 */
			printf("Value pointed to by shared_pgid_3 in process 0: %ld (to check working of munmap)\n", *shared_pgid_3);
			sleep(10);
		}
	}
	return 0;
}
