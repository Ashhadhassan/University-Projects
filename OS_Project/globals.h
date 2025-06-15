#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>
#include <semaphore.h>
#include "producer.h"

BufferSlot* getBuffer();
int* getInPtr();
int* getOutPtr();
pthread_mutex_t* getMutex();
sem_t* getFullSem();
sem_t* getEmptySem();
char (*getFifoPaths())[64];

#endif