#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include "consumer.h"
#include "globals.h"

void* consumer(void* arg) {
    int id = *(int*)arg;
    BufferSlot* buffer = getBuffer();
    int* out = getOutPtr();
    pthread_mutex_t* mutex = getMutex();
    sem_t* full = getFullSem();
    sem_t* empty = getEmptySem();
    char (*fifo_paths)[64] = getFifoPaths();

    int fd = open(fifo_paths[id - 1], O_WRONLY);
    if (fd < 0) {
        perror("open FIFO");
        pthread_exit(NULL);
    }

    while (1) {
        sem_wait(full);
        pthread_mutex_lock(mutex);

        BufferSlot* slot = &buffer[*out];
        if (slot->data == NULL) {
            pthread_mutex_unlock(mutex);
            break;
        }

        dprintf(fd, "[Consumer %d] Playing: %s\n", id, slot->data);
        slot->read_count++;

        if (slot->read_count >= NUM_CONSUMERS) {
            free(slot->data);
            slot->data = NULL;
            slot->read_count = 0;
            *out = (*out + 1) % BUFFER_SIZE;
            sem_post(empty);
        }

        pthread_mutex_unlock(mutex);
        sleep(rand() % 2);
    }

    dprintf(fd, "[Consumer %d] Exiting.\n", id);
    close(fd);
    return NULL;
}