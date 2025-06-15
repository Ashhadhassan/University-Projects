#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "producer.h"
#include "consumer.h"
#include "globals.h"

void setup_terminals() {
    char (*fifo_paths)[64] = getFifoPaths();
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        sprintf(fifo_paths[i], "/tmp/consumer_fifo_%d", i);
        mkfifo(fifo_paths[i], 0666);

        char cmd[256];
        sprintf(cmd, "gnome-terminal -- bash -c 'cat %s; exec bash'", fifo_paths[i]);
        system(cmd);
    }
    sleep(1); // Allow terminals to launch
}

int main() {
    pthread_t prod;
    pthread_t consumers[NUM_CONSUMERS];
    int ids[NUM_CONSUMERS];

    pthread_mutex_init(getMutex(), NULL);
    sem_init(getEmptySem(), 0, BUFFER_SIZE);
    sem_init(getFullSem(), 0, 0);

    setup_terminals();

    pthread_create(&prod, NULL, producer, NULL);
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        ids[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer, &ids[i]);
    }

    pthread_join(prod, NULL);
    for (int i = 0; i < NUM_CONSUMERS; ++i) pthread_join(consumers[i], NULL);

    for (int i = 0; i < NUM_CONSUMERS; ++i) unlink(getFifoPaths()[i]);

    pthread_mutex_destroy(getMutex());
    sem_destroy(getEmptySem());
    sem_destroy(getFullSem());

    return 0;
}