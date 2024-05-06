
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h> 
#define BUFFER_SIZE 10

char buffer[BUFFER_SIZE][256];
int in = 0;
int out = 0;
int count = 0;

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

void* producer(void* arg) {
    FILE *f = fopen("data.txt", "r");
    if (f == NULL) {
        printf("Failed to open file\n");
        return NULL;
    }
    char line[256];
    srand(time(NULL) + pthread_self()); 
    while (fgets(line, sizeof(line), f)) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        strcpy(buffer[in], line);
        printf("Producer: %sRemaining space: %d, Items in buffer: %d\n", buffer[in], BUFFER_SIZE - count - 1, count + 1);
        in = (in + 1) % BUFFER_SIZE;
        count++;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);

        sleep(rand() % 3 + 1); 
    }
    fclose(f);
    return NULL;
}

void* consumer(void* arg) {
    srand(time(NULL) + pthread_self() * 2);
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        if (count == 0) {
            pthread_mutex_unlock(&mutex);
            sem_post(&empty);
            break;
        }

        printf("Consumer: %sRemaining space: %d, Items in buffer: %d\n", buffer[out], BUFFER_SIZE - count + 1, count - 1);
        out = (out + 1) % BUFFER_SIZE;
        count--;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        sleep(rand() % 5 + 3); 
    }
    return NULL;
}

int main() {
    pthread_t producers[3], consumers[4];
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < 3; i++) {
        pthread_create(&producers[i], NULL, producer, NULL);
    }
    for (int i = 0; i < 4; i++) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < 4; i++) {
        pthread_join(consumers[i], NULL);
    }

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
