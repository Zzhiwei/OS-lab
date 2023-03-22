#include "packer.h"
#include <semaphore.h>

#define RED 1
#define GREEN 2
#define BLUE 3

// You can declare global variables here
sem_t red_mutex;
sem_t green_mutex;
sem_t blue_mutex;

sem_t red_sem;
sem_t green_sem;
sem_t blue_sem;

int red_count;
int green_count;
int blue_count;

int red_arr[2];
int green_arr[2];
int blue_arr[2];

void packer_init(void) {
  // Write initialization code here (called once at the start of the program).
  sem_init(&red_mutex, 0, 1);
  sem_init(&green_mutex, 0, 1);
  sem_init(&blue_mutex, 0, 1);

  sem_init(&red_sem, 0, 0);
  sem_init(&green_sem, 0, 0);
  sem_init(&blue_sem, 0, 0);

  red_count = 0;
  green_count = 0;
  blue_count = 0;
}

void packer_destroy(void) {
  // Write deinitialization code here (called once at the end of the program).
  sem_destroy(&red_mutex);
  sem_destroy(&green_mutex);
  sem_destroy(&blue_mutex);

  sem_destroy(&red_sem);
  sem_destroy(&green_sem);
  sem_destroy(&blue_sem);
}

void reset_sem(sem_t *sem) {
  sem_destroy(sem);
  sem_init(sem, 0, 0);
}

int pack_red(int id) {
  sem_wait(&red_mutex);
  red_arr[red_count] = id;
  red_count++;
  sem_post(&red_mutex);

  if (red_count == 2) {
    sem_post(&red_sem);
  }

  sem_wait(&red_sem);
  sem_post(&red_sem);

  sem_wait(&red_mutex);
  red_count--;
  sem_post(&red_mutex);

  if (red_count == 0) {
    reset_sem(&red_sem);
  }

  return red_arr[0] == id ? red_arr[1] : red_arr[0];
}

int pack_green(int id) {
  sem_wait(&green_mutex);
  green_arr[green_count] = id;
  green_count++;
  sem_post(&green_mutex);

  if (green_count == 2) {
    sem_post(&green_sem);    
  }

  sem_wait(&green_sem);
  sem_post(&green_sem);

  sem_wait(&green_mutex);
  green_count--;
  sem_post(&green_mutex);

  if (green_count == 0) {
    reset_sem(&green_sem);
  }
  return green_arr[0] == id ? green_arr[1] : green_arr[0];
}

int pack_blue(int id) {
  sem_wait(&blue_mutex);
  blue_arr[blue_count] = id;
  blue_count++;
  sem_post(&blue_mutex);

  if (blue_count == 2) {
    sem_post(&blue_sem);    
  }

  sem_wait(&blue_sem);
  sem_post(&blue_sem);

  sem_wait(&blue_mutex);
  blue_count--;
  sem_post(&blue_mutex);

  if (blue_count == 0) {
    reset_sem(&blue_sem);
  }

  return blue_arr[0] == id ? blue_arr[1] : blue_arr[0];
}

int pack_ball(int colour, int id) {
  // Write your code here.
  int other_id = -1;

  if (colour == RED) {
    other_id = pack_red(id);
  } else if (colour == GREEN) {
    other_id = pack_green(id);
  } else if (colour == BLUE) {
    other_id = pack_blue(id);
  }

  return other_id;
}