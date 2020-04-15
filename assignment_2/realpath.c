#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

/* assuming that current directory will have a max total of 256 files and
 * subdirectories both inclusive
 */
#define MAX_FILES_IN_DIR 256

int pos = 0;
char *strs[MAX_FILES_IN_DIR];

char *myrealpath(char *path) {
	long max = pathconf("/", _PC_PATH_MAX);
	if (max == -1) {
		return NULL;
	}
	char *currdir = (char *)malloc(max);
	getcwd(currdir, max);
	strcat(currdir, "/");
	if (path == NULL) { // search in current working directory
		DIR *d;
		struct dirent *dir;
		d = opendir(".");
		if (d == NULL) {
			return NULL;
		}
		char *buf1 = (char *)malloc(PATH_MAX);
		char *buf2 = (char *)malloc(PATH_MAX);
		int size, flag = 0;
		while ((dir = readdir(d)) != NULL) {
			//TODO figure out why the masks are not working to check is a
			//file is a symbolic link or not
			/*
			struct stat buf;
			if (stat(dir->d_name, &buf) == 0) {
				// if (S_ISLNK(buf.st_mode)) {
				if ((buf.st_mode & S_IFMT) == S_IFLNK) {
					strs[pos] = dir->d_name;
					printf("%s\n", strs[pos]);
					pos++;
				}
			}
			else {
				return NULL;
			}
			*/
			strcpy(buf2, dir->d_name);
			// puts(buf2);
			while ((size = readlink(buf2, buf1, PATH_MAX)) > 0) {
				flag = 1;
				buf1[size] = '\0';
				if (strstr(buf1, "..") != NULL) {
					if (strstr(dirname(strdup(buf2)), ".") == NULL) {
						buf1 = strcat(strcat(dirname(strdup(buf2)), "/"), buf1);
					}
				}
				else {
					if (strstr(dirname(strdup(dir->d_name)), ".") == NULL) {
						buf1 = strcat(strcat(dirname(strdup(dir->d_name)), "/"), buf1);
					}
				}
				// puts(buf1);
				strcpy(buf2, buf1);
			}
			if (flag) {
				//TODO return absolute path of the file
				// strs[pos] = strcat(strdup(currdir), basename(strdup(buf1)));
				strs[pos] = basename(strdup(buf1));
				pos++;
			}
			flag = 0;
		}
		free(buf1);
		free(buf2);
		closedir(d);
		/* since the current directory may have multiple symbolic links, return
		 * char * pointing to the first resolved link and iterate using pos
		 * global variable in the user program by incrementing the returned 
		 * char * to get all resolved symbolic links
		 */
		return strs[0];
	}
	else {
		char *buf1 = (char *)malloc(PATH_MAX);
		char *buf2 = (char *)malloc(PATH_MAX);
		strcpy(buf2, path);
		int size;
		// puts(buf2);
		while ((size = readlink(buf2, buf1, PATH_MAX)) > 0) {
			buf1[size] = '\0';
			if (strstr(buf1, "..") != NULL) {
				if (strstr(dirname(strdup(buf2)), ".") == NULL) {
					/* pass duplicate of a string to dirname and basename since the
					 * input string may be changed by these functions
					 */
					buf1 = strcat(strcat(dirname(strdup(buf2)), "/"), buf1);
				}
			}
			else {
				if (strstr(dirname(strdup(path)), ".") == NULL) {
					buf1 = strcat(strcat(dirname(strdup(path)), "/"), buf1);
				}
			}
			// puts(buf1);
			strcpy(buf2, buf1);
		}
		free(buf2);
		// strs[0] = strcat(strdup(currdir), buf1);
		strs[0] = basename(strdup(buf1));
		// free(buf1);
		/* will effectively return only a single char * to be iterated over
		 * using pos in the user program since the file has been specified by
		 * the user
		 */
		pos = 1;
		//TODO return absolute path of the file
		return strs[0];
	}
}

int main(int argc, char *argv[]) {
	char *str;
	if (argc == 2) {
		str = myrealpath(argv[1]);
	}
	else if (argc == 1) {
		str = myrealpath(NULL);
	}
	else {
		printf("Usage: ./realpath <filename> [OPTIONAL]\n");
	}
	if (str == NULL) {
		printf("Realpath failed\n");
		exit(0);
	}
	else {
		for (int i = 0; i < pos; i++) {
			printf("%s\n", strs[i]);
			strs[i]++;
		}
	}
	/* the actual realpath function returns a malloced char * which needs to be
	 * freed in the user program
	 * myrealpath returns basename returned char * to a const null-terminated 
	 * string i.e. not malloced, on which free must not be called or it results
	 * in a core dump
	 */
	// free(str);
	return 0;
}
