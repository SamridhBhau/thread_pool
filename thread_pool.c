#include "thread_pool.h"

int add_work( struct threadpool *pool, void (*f)(void *args), void *args ){

  struct work w;
  w.work = f;
  w.args = args;
  struct queue *work_queue = &(pool->work_queue);
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_lock(&lock);

  enqueue(work_queue, &w);

  pthread_cond_signal(&(pool->cond));
  pthread_mutex_unlock(&lock);


  return 1;
}


int threadpool_init(struct threadpool *pool, size_t p_size){
  // queue of threads
  if (queue_init(&(pool->worker_queue), sizeof(pthread_t)) == 0)
    return 0;

  // queue of work to do 
  if(queue_init(&(pool->work_queue), sizeof(struct work)) == 0)
    return 0;

  for (size_t i = 0; i < p_size; i++){
    pthread_t thread;
    pthread_create(&thread, NULL, consumer, (void *)&pool->work_queue);
    pthread_detach(thread);
    enqueue(&(pool->worker_queue), &thread);
  }

  pool->size = p_size;
  pthread_cond_init(&(pool->cond), NULL);
  return 1;
}

void *consumer(void *p){
  struct threadpool *pool = p;
  struct queue *work_queue = &(pool->work_queue);

  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_lock(&lock);
  while(isEmpty(work_queue))
    pthread_cond_wait(&(pool->cond) ,&lock);

  struct work c_work;
  dequeue(work_queue, &c_work);
  c_work.work(c_work.args);

  pthread_mutex_unlock(&lock);

  return 0;
}
