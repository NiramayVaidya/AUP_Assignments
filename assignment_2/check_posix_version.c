#include <stdio.h>
#include <unistd.h>

/* 200809 is returned by the macro for POSIX.1-2017
 * S_ISLNK on st_mode in stat struct is not available in POSIX.1-1996
 * see man 2 stat
 * execute cpp -dM /dev/null to view list of system specific predefined macros
 * for example, __unix__, __linux__, __gnu_linux__
 */

int main() {
	printf("%ld\n", _POSIX_VERSION);
	return 0;
}
