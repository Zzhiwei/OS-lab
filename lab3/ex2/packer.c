#include "packer.h"
#include "semaphore.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

#define DUMMY_INTEGER 58327329
#define QUEUE_SIZE 200
#define INIT_QUEUE(qName) Queue qName = {.front = -1, .back = -1};

/* constants */
int RED = 1;
int GREEN = 2;
int BLUE = 3;
int NUM_PER_PACK;

/* declare/initialize variables */
int red_packed_count = 0;
int green_packed_count = 0;
int blue_packed_count = 0;

int red_count = 0;
int green_count = 0;
int blue_count = 0;

int waiting_red_id;
int waiting_green_id;
int waiting_blue_id;

sem_t red_mutex;
sem_t green_mutex;
sem_t blue_mutex;

// 1 - first barrier, 2 - 2nd barrier, 3 - group barrier
sem_t red_turnstile_1;
sem_t red_turnstile_2;
sem_t red_turnstile_3; 
sem_t green_turnstile_1;
sem_t green_turnstile_2;
sem_t green_turnstile_3;
sem_t blue_turnstile_1;
sem_t blue_turnstile_2;
sem_t blue_turnstile_3;

int numDeq;

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
INIT_QUEUE(green_inqueue);
INIT_QUEUE(blue_inqueue);

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

int at(Queue *q, int index) {
    int translatedIndex = q->front + index;
    if (translatedIndex < q->front || translatedIndex > q->back) {
        printf(">>>>>");
        printf("accessing %d, but queue = ", index);
        show(q);
        return DUMMY_INTEGER;
    }
    return q->arr[translatedIndex];
}

// ================================================
// End of Queue Definition
// ================================================

void packer_init(int balls_per_pack) {
    // Write initialization code here (called once at the start of the program).
    NUM_PER_PACK = balls_per_pack;

    sem_init(&red_turnstile_1, 0, 0);
    sem_init(&red_turnstile_2, 0, 1);
    sem_init(&red_turnstile_3, 0, NUM_PER_PACK);
    sem_init(&green_turnstile_1, 0, 0);
    sem_init(&green_turnstile_2, 0, 1);
    sem_init(&green_turnstile_3, 0, NUM_PER_PACK);
    sem_init(&blue_turnstile_1, 0, 0);
    sem_init(&blue_turnstile_2, 0, 1);
    sem_init(&blue_turnstile_3, 0, NUM_PER_PACK);

    sem_init(&red_mutex, 0, 1);
    sem_init(&green_mutex, 0, 1);
    sem_init(&blue_mutex, 0, 1);

}

void packer_destroy(void) {
    // Write deinitialization code here (called once at the end of the program).
    sem_destroy(&red_turnstile_1);
    sem_destroy(&red_turnstile_2);
    sem_destroy(&red_turnstile_3);
    sem_destroy(&green_turnstile_1);
    sem_destroy(&green_turnstile_2);
    sem_destroy(&green_turnstile_3);
    sem_destroy(&blue_turnstile_1);
    sem_destroy(&blue_turnstile_2);
    sem_destroy(&blue_turnstile_3);
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

sem_t *getGroupTurnstile(int colour) {
    if (colour == RED) {
        return &red_turnstile_3;
    } else if (colour == GREEN) {
        return &green_turnstile_3;
    } else if (colour == BLUE) {
        return &blue_turnstile_3;
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

int *getPackedCount(int colour) {
    if (colour == RED) {
        return &red_packed_count;
    } else if (colour == GREEN) {
        return &green_packed_count;
    } else if (colour == BLUE) {
        return &blue_packed_count;
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

void saveToOtherIds(int currId, int *other_ids, Queue *queue) {
    int j = 0;
    int i, idFromQueue;
    for (i=0;i<NUM_PER_PACK;i++) {
        idFromQueue = at(queue, i);
        if (currId == idFromQueue) {
            continue;
        }
        other_ids[j] = idFromQueue;
        j++;
    }
}

void enterCriticalSection(sem_t *mutex) {
    sem_wait(mutex);
}

void exitCriticalSection(sem_t *mutex) {
    sem_post(mutex);
}

void pack_ball(int colour, int id, int *other_ids) {
    sem_t *turnstile_1 = getFirstTurnstile(colour);
    sem_t *turnstile_2 = getSecondTurnstile(colour);
    sem_t *group_turnstile = getGroupTurnstile(colour);
    sem_t *mutex = getMutex(colour);

    int *count = getCount(colour);
    int *packed_count = getPackedCount(colour);
    Queue *inqueue = getInQueue(colour);
    
    // prevents balls to be packed in the next group to start executing before the current group has been packed
    sem_wait(group_turnstile);

    /* ======================== FIRST BARRIER ======================== */
    enterCriticalSection(mutex);
    *count = *count + 1;
    enQueue(id, inqueue);
    if (*count == NUM_PER_PACK) {
        sem_wait(turnstile_2); // lock second 1 -> 0
        sem_post(turnstile_1); // unlock first
    }
    exitCriticalSection(mutex);

    sem_wait(turnstile_1); // first barrier
    sem_post(turnstile_1);

    /* ======================== SECOND BARRIER ======================== */
    enterCriticalSection(mutex);
    *count = *count - 1;
    if (*count == 0) {
        sem_wait(turnstile_1); // lock first (for next group)
        sem_post(turnstile_2); // unlock second (for balls in same pack alr waiting to be released) 0 -> 1
    }
    exitCriticalSection(mutex);

    sem_wait(turnstile_2); // second barrier,  1 -> 0

    /* ======================== SAVE IDs ======================== */
    enterCriticalSection(mutex);
    saveToOtherIds(id, other_ids, inqueue);
    (*packed_count)++;
    if (*packed_count == NUM_PER_PACK) {
        *packed_count = 0;
        int i;
        for (i=0;i<NUM_PER_PACK;i++) {
            deQueue(inqueue);
            // IMPORTANT: can only release group turnstile when the last ball finishes and the packed_count has been reset! 
            sem_post(group_turnstile);
        }
    }
    exitCriticalSection(mutex);

    sem_post(turnstile_2); // 0 -> 1

}
