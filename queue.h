#ifndef QUEUE_H
#define QUEUE_H
#include <stdbool.h>
#include <stddef.h>

struct queue_node;
struct queue;

bool isEmpty(struct queue*);
bool isFull(struct queue*);
int dequeue(struct queue*, void *);
int enqueue(struct queue*, void *);
int queue_destroy(struct queue*);
int queue_peek(struct queue* q, void *data);
struct queue* queue_init(size_t max_length, size_t data_size);

#endif
