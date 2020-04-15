#include <stdio.h>
#include <unistd.h>
#include <errno.h>

/* sudo chown root unlink
 * sudo chgrp root unlink
 * sudo chmod u+s unlink
 */

int main(int argc, char *argv[]) {
	if (unlink(".") == -1) {
		perror("Unlink failed\n");
		return errno;
	}
	return 0;
}
