#include "stdio.h"

#define QUEUE_SIZE 100
#define INIT_QUEUE(qName) Queue qName = {.front = -1, .back = -1};

typedef struct PackerQueue {
    // cannot provide default value, must use macro seen above
    int front;
    int back; // next enqueue will insert item at (back + 1)
    int arr[QUEUE_SIZE];
} Queue;

INIT_QUEUE(red_queue);
INIT_QUEUE(green_queue);
INIT_QUEUE(blue_queue);

void enQueue(int x, Queue *q) {
    if (q->back == QUEUE_SIZE - 1) {
        printf("Queue Overflow\n");
        return;
    }
    
    if (q->front == -1) {
        q->front = 0;
    }
    q->back++;
    q->arr[q->back] = x;
}

void deQueue(Queue *q) {
    if (q->front == -1 || q->front > q->back) {
        printf("Queue Underflow\n");
    } else {
        q->front++;
    }
}

int peekQueue(Queue *q) {
    if (q->front == -1) {
        printf("Empty Queue\n");
        return -1;
    } else {
        return q->arr[q->front];
    }
}

void show(Queue *q)
{
    if (q->front == -1 || q->front > q->back)
        printf("Empty Queue\n");
    else {
        for (int i = q->front; i <= q->back; i++) {
            printf("%d ", q->arr[i]);
        }
        printf("\n");
    }
} 

int main() {
    enQueue(1, &red_queue);
    enQueue(2, &red_queue);
    enQueue(3, &red_queue);
    enQueue(4, &red_queue);
    show(&red_queue);
    deQueue(&red_queue);
    show(&red_queue);
    enQueue(7, &red_queue);
    show(&red_queue);
    deQueue(&red_queue);
    deQueue(&red_queue);
    deQueue(&red_queue);
    show(&red_queue);
    deQueue(&red_queue);
    show(&red_queue);
    deQueue(&red_queue);
    enQueue(9, &red_queue);
    enQueue(8, &red_queue);
    show(&red_queue);
}
