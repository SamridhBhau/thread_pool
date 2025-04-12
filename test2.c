#include <stdio.h>
#include <stdint.h>
#include "thread_pool.h"

void work(void *args){
  int *arg = args;
  printf("Inside #%u with arg: %d\n", (int)pthread_self(), *arg);

}

int main() {
  struct threadpool pool;
  threadpool_init(&pool, 3);

  int i;
  for (i = 0; i < 50; i++){
    add_work(&pool, work, (void *)(uintptr_t)i);
  }

  threadpool_wait(&pool);

}
