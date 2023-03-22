#include "packer.h"
#include "semaphore.h"
#include "stdio.h"

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

int red_balls[2];
int green_balls[2];
int blue_balls[2];

void packer_init(void) {
    // Write initialization code here (called once at the start of the program).
    sem_init(&red_turnstile_1, 0, 0);
    sem_init(&red_turnstile_2, 0, 0);
    sem_init(&green_turnstile_1, 0, 0);
    sem_init(&green_turnstile_2, 0, 0);
    sem_init(&blue_turnstile_1, 0, 0);
    sem_init(&blue_turnstile_2, 0, 0);
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

int *getBalls(int colour) {
    if (colour == RED) {
        return red_balls;
    } else if (colour == GREEN) {
        return green_balls;
    } else if (colour == BLUE) {
        return blue_balls;
    }
}

int pack_ball(int colour, int id) {
    // printf("accepting: %d, %d\n", colour, id);
    sem_t *turnstile_1 = getFirstTurnstile(colour);
    // sem_t *turnstile_2 = getSecondTurnstile();
    sem_t* mutex = getMutex(colour);
    int *count = getCount(colour);
    int *balls = getBalls(colour);
    
    balls[*count] = id;

    sem_wait(mutex); // enter CS
    *count = *count + 1;
    sem_post(mutex);

    if (*count == 2) {
        sem_post(turnstile_1);
    }

    sem_wait(turnstile_1);
    sem_post(turnstile_1);
    
    return balls[0] == id ? balls[1] : balls[0];
}