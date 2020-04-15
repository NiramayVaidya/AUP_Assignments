#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/* this code does not make any sense since the env var set by print_env_var
 * anyways has scope only inside the process the executable is running in and
 * does not add it to the bash's env vars and hence the existence of the env var
 * list of the child process ceases to exist when the code finishes executing
 * as seen in the first set of printed env vars, the set env var does not exist
 * since this env var list is again only within the scope of the
 * remove_env_var process
 */

int main(int argc, char *argv[], char *envp[]) {
	if (argc != 2) {
		printf("Usage ./remove_env_var <env_var_name>\n");
		exit(0);
	}
	int i;
	extern char **environ;
	for (i = 0; environ[i] != NULL; i++) {
		printf("%s\n", environ[i]);
	}
	if (unsetenv(argv[1]) == -1) {
		perror("Unsetenv failed\n");
		return errno;
	}
	for (i = 0; environ[i] != NULL; i++) {
		printf("%s\n", environ[i]);
	}
	return 0;
}
