#include "packer.h"
#include "semaphore.h"
#include "stdio.h"
#include "stdbool.h"

#define DUMMY_INTEGER 58327329
#define QUEUE_SIZE 100
#define INIT_QUEUE(qName) Queue qName = {.front = -1, .back = -1};

int RED = 1;
int GREEN = 2;
int BLUE = 3;

int isFirst = true;
int firstId;

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

INIT_QUEUE(red_inqueue);
INIT_QUEUE(red_outqueue);
INIT_QUEUE(green_inqueue);
INIT_QUEUE(green_outqueue);
INIT_QUEUE(blue_inqueue);
INIT_QUEUE(blue_outqueue);

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
        return DUMMY_INTEGER;
    }
    int res = q->arr[q->front];
    q->front++;
    return res;
}

int peekQueue(Queue *q) {
    if (q->front == -1) {
        printf("Empty Queue\n");
        return -1;
    } else {
        return q->arr[q->front];
    }
}

int len(Queue *q) {
    if (q->front == -1 || q->front > q->back) {
        return 0;
    }
    return q->back - q->front  + 1;
}

bool isQueueEmpty(Queue *q) {
    if (q->front == -1 || q->front > q->back) {
        return true;
    } else {
        return false;
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
    return NULL;
}

sem_t *getSecondTurnstile(int colour) {
    if (colour == RED) {
        return &red_turnstile_2;
    } else if (colour == GREEN) {
        return &green_turnstile_2;
    } else if (colour == BLUE) {
        return &blue_turnstile_2;
    }
    return NULL;
}


sem_t *getMutex(int colour) {
    if (colour == RED) {
        return &red_mutex;
    } else if (colour == GREEN) {
        return &green_mutex;
    } else if (colour == BLUE) {
        return &blue_mutex;
    }
    return NULL;
}

int *getCount(int colour) {
    if (colour == RED) {
        return &red_count;
    } else if (colour == GREEN) {
        return &green_count;
    } else if (colour == BLUE) {
        return &blue_count;
    }
    return NULL;
}

Queue *getInQueue(int colour) {
    if (colour == RED) {
        return &red_inqueue;
    } else if (colour == GREEN) {
        return &green_inqueue;
    } else if (colour == BLUE) {
        return &blue_inqueue;
    }
    return NULL;
}

Queue *getOutQueue(int colour) {
    if (colour == RED) {
        return &red_outqueue;
    } else if (colour == GREEN) {
        return &green_outqueue;
    } else if (colour == BLUE) {
        return &blue_outqueue;
    }
    return NULL;
}

int pack_ball(int colour, int id) {
    sem_t *turnstile_1 = getFirstTurnstile(colour);
    sem_t *turnstile_2 = getSecondTurnstile(colour);
    sem_t *mutex = getMutex(colour);
    int *count = getCount(colour);
    Queue *inqueue = getInQueue(colour);
    
    int otherId;

    sem_wait(mutex); // *** enter CS ***
    *count = *count + 1;
    enQueue(id, inqueue);
    if (*count == 2) {
        sem_wait(turnstile_2); //lock second
        sem_post(turnstile_1); //unlock first
    }
    sem_post(mutex); // *** exit CS ***

    sem_wait(turnstile_1); // processes blocks here
    sem_post(turnstile_1); // allows paired process to proceed

    sem_wait(mutex); // *** enter CS ***
    *count = *count - 1;
    if (*count == 0) {
        sem_wait(turnstile_1); //lock first (for next pair)
        sem_post(turnstile_2); //unlock second (for its partner to proceed in wait tt2 below)
    }
    sem_post(mutex); // *** exit CS ***

    sem_wait(turnstile_2); // 1st processes blocks here because 2nd process locks second in 1st if block    
    sem_post(turnstile_2); // allows paired process that comes after it to proceed

    // the following cannot be placed btw wait and post turnstile_2 above, dk why
    sem_wait(mutex);
    if (isFirst) {
        firstId = deQueue(inqueue);
        otherId = peekQueue(inqueue);
        isFirst = false;
    } else {
        otherId = firstId;
        deQueue(inqueue);
        //reset
        isFirst = true;
        firstId = DUMMY_INTEGER;
    }
    sem_post(mutex);
    
    return otherId;
}