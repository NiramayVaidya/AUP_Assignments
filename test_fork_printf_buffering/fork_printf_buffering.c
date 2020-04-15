#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NO_NEWLINE 0
#define print(x) (NO_NEWLINE ? printf("%d", x) : printf("%d\n", x))

/* according to my understanding, this happens without newline-
 * output is- 42301
 * every process to begin with sees an empty buffer when storing in it from the
 * printf call and since printf is line buffered, every process flushes to
 * stdout only when \n hits. hence the first process that gets terminated is the
 * leaf of the process tree created here and hence it flushes the 4 to stdout
 * stored in the buffer it sees by the printf call. Then the internal node of
 * this process tree stores 2 and 3 (since wait immediately returns as this
 * process's child has already terminated) in the buffer it sees and when it
 * terminates 2 and 3 are flushed to stdout in that order. Finally, the same
 * happens with the root of this process tree.
 * this happens with newline-
 * output is- 0\n2\n4\n3\n1
 * every process flushes the buffer it sees immediately upon seeing the \n in
 * the printf call due to which the root prints 0 immediately then waits for its
 * child to terminate, which when created prints 2 immediately and waits in turn
 * for its child to terminate, which then prints 4 and terminates. As a result,
 * wait in its parent process returns and this process prints 3 and then
 * terminates, and in turn the wait in its parent returns and then this process
 * prints 1 and then finally terminates.
 */

/* see slides 39-40 of process control PPT 1, the example says the printf is
 * fully buffered when stdout is redirected to a file, and according to the
 * output shown there, the buffer of the parent has been copied to the child,
 * hence the entire buffer in both the parent and the child get flushed into the
 * file only upon termination of these processes
 * If this logic is applied here, then if the buffer gets copied to the child,
 * the lowest child will have buffer content of 0 from the first parent which
 * gets copied to its child i.e. parent of the current child which appends 2 to
 * this buffer copy and passes on the buffer with 02 as contents to this lowest
 * child. When this child terminates, 024 should get flushed out to stdout.
 * After its termination the middle process's buffer which contains 02 should
 * get appended with 3 and 023 should get printed and then this process should
 * terminate. Finally, the first parent has buffer contents 0 to which 1 gets
 * appended and 01 should get printed on stdout, then this process will
 * terminate too. Hence, the output should be 02402301.
 *
 * The first explanation is wrong and the one given above is partially correct
 * Here is an explanation that makes sense and justifies the output-
 * https://unix.stackexchange.com/questions/447898/why-does-a-program-with-fork-sometimes-print-its-output-multiple-times
 * upon fork, the child process inherits every part of the parent process,
 * including the unflushed buffer. This effectively copies the unflushed buffer
 * to each child process. Hence, in this case, the last created child inherits
 * an empty buffer from its parent which has already inherited an empty buffer
 * from its parent in turn, since none of these parents call print before this
 * last child calls print due to which it appends 4 to this empty buffer and
 * terminates and as a result the buffer is flushed to stdout. Then its parent
 * add 2 to an empty buffer, its wait immediately return since its child has
 * already terminated, and then it appends 3 to this buffer and terminates which
 * prints 23 to stdout. Finally, the first parent adds 0 to an its empty buffer,
 * wait immediately returns since its child has already terminated and then
 * appends 1 to this buffer and terminates. Hence, 01 gets printed.
 * TODO- justify why the order of execution among the three processes varies in
 * this particular manner when printf is called with and without newline. With
 * newline, the process tree executes top down and without it, it executes
 * bottom up
 */

int main() {
	if (fork()) {
		print(0);
		wait(NULL);
		print(1);
	}
	else if (fork()) {
		print(2);
		wait(NULL);
		print(3);
	}
	else {
		print(4);
	}
	return 0;
}
