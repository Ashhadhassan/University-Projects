#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include "producer.h"
#include "globals.h"

void* producer() {
    BufferSlot* buffer = getBuffer();
    int* in = getInPtr();
    pthread_mutex_t* mutex = getMutex();
    sem_t* full = getFullSem();
    sem_t* empty = getEmptySem();

    for (int i = 1; i <= 10; ++i) {
        sem_wait(empty);
        pthread_mutex_lock(mutex);

        char* chunk = malloc(32);
        sprintf(chunk, "Chunk %d", i);
        buffer[*in].data = chunk;
        buffer[*in].read_count = 0;

        printf("[Producer] Produced: %s at %d\n", chunk, *in);

        pthread_mutex_unlock(mutex);

        for (int j = 0; j < NUM_CONSUMERS; ++j) sem_post(full);

        sleep(rand() % 2);
        *in = (*in + 1) % BUFFER_SIZE;
    }

    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        sem_wait(empty);
        pthread_mutex_lock(mutex);
        buffer[*in].data = NULL;
        buffer[*in].read_count = NUM_CONSUMERS;
        pthread_mutex_unlock(mutex);
        for (int j = 0; j < NUM_CONSUMERS; ++j) sem_post(full);
        *in = (*in + 1) % BUFFER_SIZE;
    }

    return NULL;
}