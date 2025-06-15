#include <pthread.h>
#include <semaphore.h>
#include "producer.h"

BufferSlot buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
pthread_mutex_t mutex;
sem_t sem_empty;
sem_t sem_full;

char fifo_paths[NUM_CONSUMERS][64];

BufferSlot* getBuffer() { return buffer; }
int* getInPtr() { return &in; }
int* getOutPtr() { return &out; }
pthread_mutex_t* getMutex() { return &mutex; }
sem_t* getFullSem() { return &sem_full; }
sem_t* getEmptySem() { return &sem_empty; }
char (*getFifoPaths())[64] { return fifo_paths; }