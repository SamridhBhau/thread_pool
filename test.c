#include <stdio.h>
#include "queue.h"
#include <stdlib.h>

int main() {
  struct queue q; 
  if (queue_init(&q, 5, sizeof(int)) == 0){
    perror("Initialization Failed\n");
    return 1;
  }

  for (int i = 0; i < 10; i++){
    enqueue(&q, &i);
  }

  for (int i = 0; i < 5; i++){
    int pop;
    if (dequeue(&q, &pop))
      printf("Popped data: %d\n", pop);
  }

  int top;
  if (queue_peek(&q, &top))
    printf("%d\n", top);

  queue_clear(&q);
  struct person {
    char *name;
    int age;
  };

  if (queue_init(&q, 3, sizeof(struct person)) == 0){
    perror("Initialization Failed\n");
    return 1;
  }
  struct person p1 = {"john", 23};
  enqueue(&q, &p1);
  struct person p2 = {"george", 21};
  enqueue(&q, &p2);

  struct person p;
  queue_peek(&q, &p);
  printf("name: %s, age: %d\n", p.name, p.age);

  queue_clear(&q);

  if (queue_init(&q, 6, sizeof(char *)) == 0){
    perror("Initialization Failed\n");
    return 1;
  }
  enqueue(&q, "A");
  enqueue(&q, "B");
  enqueue(&q, "C");
  enqueue(&q, "D");

  for (int i = 0; i < 6; i++){
    char *data;
    if (dequeue(&q, data) == 0)
      printf("No more elements to pop\n");
    else 
      printf("Popped position %d: %s\n", i, data);
  }

  queue_clear(&q);

  return 0;
}
