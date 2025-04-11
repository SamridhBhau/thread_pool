#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stdbool.h>
#include "queue.h"
#include <pthread.h>

struct work{
  void (*work)(void *);
  void *args;
};

struct thread {
  pthread_t thread;
  pthread_attr_t thread_attr;
};

struct threadpool{
  struct queue worker_queue;
  struct queue work_queue;
  size_t size;
  pthread_cond_t cond;
};

int add_work(struct threadpool *p, void (*f)(void *), void *arg);
int threadpool_init(struct threadpool *p, size_t size);
void *consumer(void *pool);

#endif
