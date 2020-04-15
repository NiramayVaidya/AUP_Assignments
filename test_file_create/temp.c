#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* TODO try these combinations and explain the permissions set seen in the
 * output of ll
 * delete all previously existing files of these same names
 * foo 0666
 * foo 666
 * foo none
 * temp 0666
 * temp 666
 * temp none
 */
int main(int argc, char *argv[]) {
	int fd = open("foo", O_RDONLY | O_CREAT, 0666);
	close(fd);
	return 0;
}
