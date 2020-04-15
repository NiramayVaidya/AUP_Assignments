#include <stdio.h>
#include <pthread.h>

#define MAX_SIZE 5

// pthread_mutex_t bufLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t bufLock;
int count;

void producer(char* buf) {
    for (int i = 0; i < 9; i++) {
		printf("Producer\n");
        while (count == MAX_SIZE);
        pthread_mutex_lock(&bufLock);
        // buf[count] = getChar();
		buf[count] = 'p';
        count++;
        pthread_mutex_unlock(&bufLock);
    }
	/* try commenting this out in case of thread executing consumer
	 */
	pthread_exit(NULL);
}

void consumer(char* buf) {
    for (int i = 0; i < 9; i++) {
		printf("Consumer\n");
        while (count == 0);
        pthread_mutex_lock(&bufLock);
        // useChar(buf[count - 1]);
		printf("%c\n", buf[count - 1]);
        count--;
        pthread_mutex_unlock(&bufLock);
    }
	/* try commenting this out in case of thread executing consumer
	 */
	pthread_exit(NULL);
}

int main() {
    char buffer[MAX_SIZE];
    pthread_t p;
    count = 0;
    pthread_mutex_init(&bufLock, NULL);
    pthread_create(&p, NULL, (void*)producer, buffer);
    // pthread_create(&p, NULL, (void*)consumer, buffer);
    consumer(buffer);
	// producer(buffer);
	pthread_join(p, NULL);
    return 0;
}
