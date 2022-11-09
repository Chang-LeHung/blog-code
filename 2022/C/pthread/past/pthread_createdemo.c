
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void* func(void* args) {
  int i = 0;
  while(i < 10) {
    i++;
    sleep(1);
  }
  return NULL;
}

void* exit_(void* args) {
  
  sleep(1);
  _exit(0);
  return NULL;
}


int main() {

  pthread_t t1, t2;
  pthread_create(&t1, NULL, func, NULL);
  pthread_create(&t1, NULL, exit_, NULL);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  return 0;
}