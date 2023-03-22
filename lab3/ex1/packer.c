#include "packer.h"
#include "semaphore.h"
#include "stdio.h"

#define QUEUE_SIZE 100
#define INIT_QUEUE(qName) Queue qName = {.front = -1, .back = -1};


int RED = 1;
int GREEN = 2;
int BLUE = 3;

int red_count = 0;
int green_count = 0;
int blue_count = 0;

int waiting_red_id;
int waiting_green_id;
int waiting_blue_id;

sem_t red_mutex;
sem_t green_mutex;
sem_t blue_mutex;

sem_t red_turnstile_1;
sem_t red_turnstile_2;
sem_t green_turnstile_1;
sem_t green_turnstile_2;
sem_t blue_turnstile_1;
sem_t blue_turnstile_2;

// ================================================
// Start of Queue Definition
// ================================================

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

// ================================================
// End of Queue Definition
// ================================================

void packer_init(void) {
    // Write initialization code here (called once at the start of the program).
    sem_init(&red_turnstile_1, 0, 0);
    sem_init(&red_turnstile_2, 0, 1);
    sem_init(&green_turnstile_1, 0, 0);
    sem_init(&green_turnstile_2, 0, 1);
    sem_init(&blue_turnstile_1, 0, 0);
    sem_init(&blue_turnstile_2, 0, 1);

    sem_init(&red_mutex, 0, 1);
    sem_init(&green_mutex, 0, 1);
    sem_init(&blue_mutex, 0, 1);

}

void packer_destroy(void) {
    // Write deinitialization code here (called once at the end of the program).
    sem_destroy(&red_turnstile_1);
    sem_destroy(&red_turnstile_2);
    sem_destroy(&green_turnstile_1);
    sem_destroy(&green_turnstile_2);
    sem_destroy(&blue_turnstile_1);
    sem_destroy(&blue_turnstile_2);
}

sem_t *getFirstTurnstile(int colour) {
    if (colour == RED) {
        return &red_turnstile_1;
    } else if (colour == GREEN) {
        return &green_turnstile_1;
    } else if (colour == BLUE) {
        return &blue_turnstile_1;
    }
}

sem_t *getSecondTurnstile(int colour) {
    if (colour == RED) {
        return &red_turnstile_2;
    } else if (colour == GREEN) {
        return &green_turnstile_2;
    } else if (colour == BLUE) {
        return &blue_turnstile_2;
    }
}


sem_t *getMutex(int colour) {
    if (colour == RED) {
        return &red_mutex;
    } else if (colour == GREEN) {
        return &green_mutex;
    } else if (colour == BLUE) {
        return &blue_mutex;
    }
}

int *getCount(int colour) {
    if (colour == RED) {
        return &red_count;
    } else if (colour == GREEN) {
        return &green_count;
    } else if (colour == BLUE) {
        return &blue_count;
    }
}

int *getQueue(int colour) {
    if (colour == RED) {
        return &red_queue;
    } else if (colour == GREEN) {
        return &green_queue;
    } else if (colour == BLUE) {
        return &blue_queue;
    }
}

int pack_ball(int colour, int id) {
    sem_t *turnstile_1 = getFirstTurnstile(colour);
    sem_t *turnstile_2 = getSecondTurnstile(colour);
    sem_t *mutex = getMutex(colour);
    int *count = getCount(colour);
    int *balls = getBalls(colour);
    
    // balls[*count] = id;

    sem_wait(mutex); // enter CS
    *count = *count + 1;
    // enqueue here
    // 1st enqueue = 1st to exit based on question definition
    if (*count == 2) {
        sem_wait(turnstile_2); //lock second
        sem_post(turnstile_1); //unlock first
    }
    sem_post(mutex); // exit CS

    sem_wait(turnstile_1); //1st process blocks here,
    sem_post(turnstile_1);

    sem_wait(mutex); // enter CS
    *count = *count - 1;
    if (*count == 0) {
        sem_wait(turnstile_1); //lock first
        sem_post(turnstile_2); //unlock second
    }
    sem_post(mutex); // exit CS

    sem_wait(turnstile_2); //1st process blocks here again
    sem_post(turnstile_2);


    return balls[0] == id ? balls[1] : balls[0];
}