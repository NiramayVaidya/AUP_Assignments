#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf jmpbuffer;
static int globval;

static void f2(void) {
	longjmp(jmpbuffer, 1);
}

static void f1(int av, int rv, int vv, int sv) {
	printf("in f1():\n");
	printf("globval = %d, autoval = %d, regival = %d, volaval = %d, statval = %d\n", globval, av, rv, vv, sv);
	f2();
}

int main(int argc, char *argv[], char *envp[]) {
	int autoval;
	register int regival;
	volatile int volaval;
	static int statval;
	globval = 1;
	autoval = 2;
	regival = 3;
	volaval = 4;
	statval = 5;
	if (setjmp(jmpbuffer) != 0) {
		printf("after longjmp:\n");
		printf("globval = %d, autoval = %d, regival = %d, volaval = %d, statval = %d\n", globval, autoval, regival, volaval, statval);
		exit(0);
	}
	globval = 95;
	autoval = 96;
	regival = 97;
	volaval = 98;
	statval = 99;
	f1(autoval, regival, volaval, statval);
	return 0;
}
