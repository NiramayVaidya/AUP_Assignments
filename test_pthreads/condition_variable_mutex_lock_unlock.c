#include <stdio.h>
#include <pthread.h>

#define MAX_SIZE 5

pthread_mutex_t lock;
pthread_cond_t notFull, notEmpty;
int count;

void producer(char* buf) {
    for (int i = 0; i < 9; i++) {
		printf("Producer\n");
        pthread_mutex_lock(&lock);
        while (count == MAX_SIZE) {
	        pthread_cond_wait(&notFull, &lock);
		}
        // buf[count] = getChar();
        buf[count] = 'p';
        count++;
        pthread_cond_signal(&notEmpty);
        pthread_mutex_unlock(&lock);
    }
	pthread_exit(NULL);
}

void consumer(char* buf) {
    for (int i = 0; i < 9; i++) {
		printf("Consumer\n");
        pthread_mutex_lock(&lock);
        while (count == 0) {
	        pthread_cond_wait(&notEmpty, &lock);
		}
        // useChar(buf[count - 1]);
        printf("%c\n", buf[count - 1]);
        count--;
        pthread_cond_signal(&notFull);
        pthread_mutex_unlock(&lock);
    }
	pthread_exit(NULL);
}

int main() {
    char buffer[MAX_SIZE];
    pthread_t p;
    count = 0;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&notFull, NULL);
    pthread_cond_init(&notEmpty, NULL);
    pthread_create(&p, NULL, (void*)producer, buffer);
	// pthread_create(&p, NULL, (void*)consumer, buffer);
    consumer(buffer);
	// producer(buffer);
	pthread_join(p, NULL);
    return 0;
}
