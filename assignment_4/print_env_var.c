#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[], char *envp[]) {
	if (argc != 4) {
		printf("Usage ./print_env_var <env_var_name> <env_var_value> <rewrite (Y/N)>\n");
		exit(0);
	}
	/* ./print_env_var name value
	 * ./print_env_var temp name
	 * the code below does not add the above key value pair to the system env
	 * vars since executing env | grep "name"/"temp"/"value" outputs nothing
	 */
	/*
	int i;
	for (i = 0; envp[i] != NULL; i++) {
		printf("%s\n", envp[i]);
	}
	//TODO find out why using only argv[1] and argv[2] below does not work
	// with strdup, string is name=value
	// without strdup, string is name=
	envp[i] = strcat(strcat(strdup(argv[1]), "="), strdup(argv[2]));
	envp[i + 1] = NULL;
	for (i = 0; envp[i] != NULL; i++) {
		printf("%s\n", envp[i]);
	}
	*/
	/* man 7 environ
	 * _GNU_SOURCE not defined
	 * cpp -dM /dev/null | grep "_GNU_SOURCE" outputs nothing
	 */
	extern char **environ;
	int i, rewrite;
	if (!strcmp(argv[3], "Y")) {
		rewrite = 1;
	}
	else if (!strcmp(argv[3], "N")) {
		rewrite = 0;
	}
	else {
		printf("Usage ./print_env_var <env_var_name> <env_var_value> <rewrite (Y/N)>\n");
		exit(0);
	}
	for (i = 0; environ[i] != NULL; i++) {
		printf("%s\n", environ[i]);
	}
	/* putenv can also be used
	 */
	if (setenv(argv[1], argv[2], rewrite) == -1) {
		perror("Setenv failed\n");
		return errno;
	}
	/* oddly enough, in this case too env | grep "name" outputs nothing
	 * if this is the case, the code commented above also works the same way
	 * this one does
	 */
	/* this executable will execute in a child process whose parent process is
	 * the running shell (bash) and a child inherits the parent's env vars but
	 * can only set its own env vars and cannot modify those of the parent
	 * this follows the concept of copy-on-write between a parent and its child,
	 * the process memory is shared by the parent and the child till the any one
	 * of them tries to make modifications to the content shared by them, at
	 * this point a separate copies are made for the parent and child and
	 * whoever wants to makes modifications can then do so accordingly and these
	 * will not be reflected in the other process (parent or child)
	 * env | grep "name" outputs nothing because it shows the env vars of the
	 * bash i.e. the parent process of our executable's process in this case and
	 * changes in the child's env vars will not be reflected in the parent's env
	 * vars
	 */
	for (i = 0; environ[i] != NULL; i++) {
		printf("%s\n", environ[i]);
	}
	return 0;
}
