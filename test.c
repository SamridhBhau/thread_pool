#include <stdio.h>
#include "queue.h"

int main() {
  struct queue *q = queue_init(5, sizeof(int));

  for (int i = 0; i < 10; i++){
    enqueue(q, &i);
  }

  for (int i = 0; i < 5; i++){
    int pop;
    if (dequeue(q, &pop))
      printf("Popped data: %d\n", pop);
  }

  int top;
  if (queue_peek(q, &top))
    printf("%d\n", top);

  return 0;
}
