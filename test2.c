#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "thread_pool.h"

struct task {
  int id;
};

void work(void *arg){
  struct task *t = (struct task *)arg;
	printf("Thread %u with arg %d\n", (int)pthread_self(), t->id);
  free(t);
}

int main() {
  struct threadpool pool;
  threadpool_init(&pool, 3);

  int i;

  for (i = 0; i < 20; i++){
    struct task *t1 = malloc(sizeof(struct task));
    t1->id = i;

    add_work(&pool, work, t1);
  }

  threadpool_wait(&pool);
  threadpool_destroy(&pool);
}
