#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char *argv[]) {
	struct stat st;
	char *type;
	if (lstat(".", &st) == -1) {
		perror("Lstat failed on dot\n");
		return errno;
	}
	else {
		printf("Dot inode: %ld\n", st.st_ino);
		printf("Dot hard link count: %ld\n", st.st_nlink);
		type = (st.st_mode & S_IFMT) == S_IFDIR ? "directory" : ((st.st_mode & S_IFMT) == S_IFLNK ? "symlink" : "other");
		printf("Dot file type: %s\n", type);
	}
	if (lstat("../test_dot_dotdot_symlink", &st) == -1) {
		perror("Lstat failed on directory file -> test_dot_dotdot_symlink\n");
		return errno;
	}
	else {
		printf("Directory file test_dot_dotdot_symlink inode: %ld\n", st.st_ino);
		printf("Directory file test_dot_dotdot_symlink hard link count: %ld\n", st.st_nlink);
	}
	if (lstat("..", &st) == -1) {
		perror("Lstat failed on dot-dot\n");
		return errno;
	}
	else {
		printf("Dot-dot inode: %ld\n", st.st_ino);
		printf("Dot-dot hard link count: %ld\n", st.st_nlink);
		type = (st.st_mode & S_IFMT) == S_IFDIR ? "directory" : ((st.st_mode & S_IFMT) == S_IFLNK ? "symlink" : "other");
		printf("Dot-dot file type: %s\n", type);
	}
	if (lstat("../../AUP", &st) == -1) {
		perror("Lstat failed on directory file -> AUP\n");
		return errno;
	}
	else {
		printf("Directory file AUP inode: %ld\n", st.st_ino);
		printf("Directory file AUP hard link count: %ld\n", st.st_nlink);
	}
	return 0;
}
