#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void undo_setuid(uid_t ruid, uid_t euid) {
	int status;
#ifdef _POSIX_SAVED_IDS
	status = seteuid(ruid);
#else
	status = setreuid(euid, ruid);
#endif
	if (status < 0) {
		perror("Seteuid/setreuid failed in undo_setuid\n");
		exit(0);
	}
}

void do_setuid(uid_t ruid, uid_t euid) {
	int status;
#ifdef _POSIX_SAVED_IDS
	status = seteuid(euid);
#else
	status = setreuid(ruid, euid);
#endif
	if (status < 0) {
		perror("Seteuid/setreuid failed in do_setuid\n");
		exit(0);
	}
}

int main(int argc, char *argv[], char *envp[]) {
	uid_t ruid, euid, id;
	ruid = getuid();
	euid = geteuid();
	undo_setuid(ruid, euid);
	int fd_own = open("own", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	do_setuid(ruid, euid);
	int fd_scores = open("scores", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	close(fd_own);
	close(fd_scores);
	int num, len;
	char newline = '\n';
	char ch, *info;
	for (int i = 1; i <= 4; i++) {
		if (i % 2 == 1) {
			undo_setuid(ruid, euid);
		}
		else {
			do_setuid(ruid, euid);
		}
		id = geteuid();
		info = (id == euid) ? " euid:temp:1001" : " euid:niramay:1000";
		len = strlen(info);
		fd_own = open("own", O_WRONLY | O_APPEND);
		fd_scores = open("scores", O_WRONLY | O_APPEND);
		ch = (char)(i + 48);
		if ((num = write(fd_own, &ch, sizeof(char))) != -1) {
			if (num < sizeof(char)) {
				printf("Less than a byte written\n");
			}
		}
		if ((num = write(fd_own, info, len)) != -1) {
			if (num < len) {
				printf("Less than expected number of bytes written\n");
			}
		}
		if ((num = write(fd_own, &newline, sizeof(char))) != -1) {
			if (num < sizeof(char)) {
				printf("Less than a byte written\n");
			}
		}
		if ((num = write(fd_scores, &ch, sizeof(char))) != -1) {
			if (num < sizeof(char)) {
				printf("Less than a byte written\n");
			}
		}
		if ((num = write(fd_scores, info, len)) != -1) {
			if (num < len) {
				printf("Less than expected number of bytes written\n");
			}
		}
		if ((num = write(fd_scores, &newline, sizeof(char))) != -1) {
			if (num < sizeof(char)) {
				printf("Less than a byte written\n");
			}
		}
		close(fd_own);
		close(fd_scores);
	}
	return 0;
}
