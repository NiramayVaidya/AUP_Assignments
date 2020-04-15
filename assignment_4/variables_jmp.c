#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

int globval;

int a(jmp_buf jmpbuffer) {
	int a_autoval = 9;
	static int a_statval = 10;
	int ret = setjmp(jmpbuffer);
	a_autoval = 19;
	a_statval = 20;
	printf("a_autoval = %d a_statval = %d\n", a_autoval, a_statval);
	return ret;
}

void b(jmp_buf jmpbuffer) {
	longjmp(jmpbuffer, 50);
}

int main(int argc, char *argv[], char *envp[]) {
	int autoval;
	static int statval;
	jmp_buf jmpbuffer;
	globval = 1;
	autoval = 2;
	statval = 3;
	if (a(jmpbuffer) != 0) {
		exit(0);
	}
	globval = 101;
	autoval = 102;
	statval = 103;
	b(jmpbuffer);
	printf("globval = %d autoval = %d statval = %d\n", globval, autoval, statval);
	return 0;
}
