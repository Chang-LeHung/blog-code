
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define KiB * 1 << 10
#define MiB * 1 << 20
#define STACK_SIZE 12 MiB

int times = 0;
void* stack_overflow(void* args) {
  printf("times = %d\n", ++times);
  char s[1 << 20]; // 1 MiB
  stack_overflow(NULL);
  return NULL;
}

int main() {
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  void* stack = malloc(STACK_SIZE);
  pthread_t t;
  pthread_attr_setstack(&attr, stack, STACK_SIZE);
  pthread_create(&t, &attr, stack_overflow, NULL);
  pthread_join(t, NULL);
  return 0;
}