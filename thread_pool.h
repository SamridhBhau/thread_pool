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
  struct queue work_queue;
  pthread_mutex_t queue_rw_mutex;  
  size_t thread_created;

  pthread_mutex_t queue_status;
  pthread_cond_t cond;

  size_t threads_working;
  pthread_mutex_t count_lock;

  pthread_cond_t threads_idle;

  bool stop;
};

int add_work(struct threadpool *p, void (*f)(void *), void *arg);
int threadpool_init(struct threadpool *p, size_t size);
void *consumer(void *pool);
void threadpool_wait(struct threadpool *p);

#endif
