#include "thread_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add_work( struct threadpool *pool, void (*f)(void *args), void *arg){

  struct work w;
  w.work = f;
  w.args = arg;

  pthread_mutex_lock(&(pool->work_queue_lock));

  struct queue *work_queue = &(pool->work_queue);
  enqueue(work_queue, &w);

  pthread_cond_signal(&(pool->cond));
  pthread_mutex_unlock(&(pool->work_queue_lock));

  return 1;
}


int threadpool_init(struct threadpool *pool, size_t p_size){
  // queue of work to do 
  if(queue_init(&(pool->work_queue), sizeof(struct work)) == 0)
    return 0;

  pthread_mutex_init(&(pool->queue_status), NULL);
  pthread_cond_init(&(pool->cond), NULL);

  pthread_mutex_init(&(pool->work_queue_lock), NULL);

  pthread_mutex_init(&pool->count_lock, NULL);
  pthread_cond_init(&(pool->threads_idle), NULL);


  pool->stop = false;
  pool->thread_count = 0;
  pool->thread_created = 0;

  for (size_t i = 0; i < p_size; i++){
    pthread_t thread;
    pthread_create(&thread, NULL, consumer, pool);
    pthread_detach(thread);
  }

  while(pool->thread_created != p_size){
  }

  return 1;
}

void *consumer(void *p){

  struct threadpool *pool = p;

  pthread_mutex_lock(&pool->count_lock);
  pool->thread_created += 1;
  pthread_mutex_unlock(&pool->count_lock);

  while(1){

    pthread_mutex_lock(&(pool->queue_status));
    struct queue *work_queue = &(pool->work_queue);

    while(isEmpty(work_queue)){
      pthread_cond_wait(&(pool->cond) , &(pool->queue_status));
    }


    pthread_mutex_lock(&pool->count_lock);
    pool->thread_count++;
    pthread_mutex_unlock(&pool->count_lock);

    struct work c_work;

    pthread_mutex_lock(&pool->work_queue_lock);
    dequeue(work_queue, &c_work);
    pthread_mutex_unlock(&pool->work_queue_lock);

    void (*func)(void *) = c_work.work;
    void *arg = c_work.args;
    func(arg);

    pthread_mutex_unlock(&(pool->queue_status));

    
    pthread_mutex_lock(&pool->count_lock);
    pool->thread_count--;
    if (pool->thread_count == 0){
      pthread_cond_signal(&pool->threads_idle);
    }
    pthread_mutex_unlock(&pool->count_lock);
  }

  return 0;
}


void threadpool_wait(struct threadpool *pool){
  pthread_mutex_lock(&pool->count_lock);
  while((!isEmpty(&pool->work_queue)) || pool->thread_count){
    pthread_cond_wait(&pool->threads_idle, &pool->count_lock);
  }
  printf("released\n");
  pthread_mutex_unlock(&pool->count_lock);

  return;
}
