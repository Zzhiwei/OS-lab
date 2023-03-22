#include "packer.h"
#include "semaphore.h"

int RED = 1;
int GREEN = 2;
int BLUE = 3;

int numRed = 0;
int numGreen = 0;
int numBlue = 0;

sem_t red_turnstile_1;
sem_t red_turnstile_2;
sem_t green_turnstile_1;
sem_t green_turnstile_2;
sem_t blue_turnstile_1;
sem_t blue_turnstile_2;

void packer_init(void) {
    // Write initialization code here (called once at the start of the program).
    sem_init(&red_turnstile_1, 0, 0);
    sem_init(&red_turnstile_2, 0, 0);
    sem_init(&green_turnstile_1, 0, 0);
    sem_init(&green_turnstile_2, 0, 0);
    sem_init(&blue_turnstile_1, 0, 0);
    sem_init(&blue_turnstile_2, 0, 0);

}

void packer_destroy(void) {
    // Write deinitialization code here (called once at the end of the program).
}

int pack_ball(int colour, int id) {
    // Write your code here.
    sem_destroy(&sem_name);
    return 0;
}