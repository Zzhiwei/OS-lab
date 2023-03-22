#include "stdio.h"
#include "stdbool.h"

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

int deQueue(Queue *q) {
    if (q->front == -1 || q->front > q->back) {
        printf("Queue Underflow\n");
    } else {
        int res = q->arr[q->front];
        q->front++;
        return res;
    }
}

bool isQueueEmpty(Queue *q) {
    if (q->front == -1 || q->front > q->back) {
        return true;
    } else {
        return false;
    }
}

int len(Queue *q) {
    if (q->front == -1 || q->front > q->back) {
        return 0;
    }
    return q->back - q->front  + 1;
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
    // if (isQueueEmpty(&red_queue)) printf("queue is empty\n"); else printf("queue is not empty\n");
    enQueue(1, &red_queue);
    printf("qsize=%d\n", len(&red_queue));
    show(&red_queue);
    enQueue(2, &red_queue);
    show(&red_queue); // 1 2
    printf("qsize=%d\n", len(&red_queue)); // 2
    enQueue(3, &red_queue); 
    printf("qsize=%d\n", len(&red_queue)); // 3
    enQueue(4, &red_queue);
    show(&red_queue); // 1 2 3 4
    printf("qsize=%d\n", len(&red_queue)); // 4
    printf("dequeued %d\n", deQueue(&red_queue));
    show(&red_queue);
    printf("qsize=%d\n", len(&red_queue)); // 3
    enQueue(7, &red_queue);
    show(&red_queue);
    printf("qsize=%d\n", len(&red_queue)); // 4
    printf("dequeued %d\n", deQueue(&red_queue));
    printf("dequeued %d\n", deQueue(&red_queue));
    printf("dequeued %d\n", deQueue(&red_queue));

    show(&red_queue); // 7
    printf("qsize=%d\n", len(&red_queue)); // 1
    deQueue(&red_queue);
    printf("qsize=%d\n", len(&red_queue)); // 0
    show(&red_queue);
    deQueue(&red_queue);
    enQueue(9, &red_queue);
    enQueue(8, &red_queue);
    show(&red_queue);
}
