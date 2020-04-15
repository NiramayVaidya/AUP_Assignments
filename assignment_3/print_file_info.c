#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <pwd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: ./print_file_info <directory_name>\n");
		exit(0);
	}
	DIR *d;
	struct dirent *dir;
	struct stat st;
	char *type, *input;
	struct passwd *pwd;
	int len;
	d = opendir(argv[1]);
	if (d == NULL) {
		perror("Opendir failed\n");
		return errno;
	}
	if (argv[1][strlen(argv[1]) - 1] != '/') {
		len = strlen(argv[1]) + 1;
		strcat(argv[1], "/");
	} else {
		len = strlen(argv[1]);
	}
	printf("%s : %s : %s\n", "file", "owner", "type");
	while ((dir = readdir(d)) != NULL) {
		input = (char *)malloc(len + strlen(dir->d_name) + 1);
		strcpy(input, argv[1]);
		strcat(input, dir->d_name);
		if (lstat(input, &st) == 0) {
			//TODO find out why getpwnam fails
			/* how does getpwuid know to return struct passwd corresponding to a
			 * particular file when a uid is passed, since many files can have 
			 * this same uid
			 */
			// printf("%d\n", st.st_uid);
			pwd = getpwuid(st.st_uid);
			if (pwd == NULL) {
				perror("Getpwuid failed\n");
				return 0;
			}
			printf("%s : %s : ", input, pwd->pw_name);
			if ((st.st_mode & S_IFMT) == S_IFREG) {
				type = "regular";
			}
			else if ((st.st_mode & S_IFMT) == S_IFDIR) {
				type = "directory";
			}
			else if ((st.st_mode & S_IFMT) == S_IFCHR) {
				type = "character special";
			}
			else if ((st.st_mode & S_IFMT) == S_IFBLK) {
				type = "block special";
			}
			else if ((st.st_mode & S_IFMT) == S_IFLNK) {
				type = "symbolic link";
			}
			else if ((st.st_mode & S_IFMT) == S_IFIFO) {
				type = "fifo";
			}
			else if ((st.st_mode & S_IFMT) == S_IFSOCK) {
				type = "socket";
			}
			else {
				type = "unknown";
			}
		}
		else {
			perror("Lstat failed\n");
			return errno;
		}
		printf("%s\n", type);
		free(input);
	}
	return 0;
}
