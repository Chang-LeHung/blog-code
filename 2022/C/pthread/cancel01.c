

#include <stdio.h>
#include <pthread.h>
#include <assert.h>

void* task(void* arg) {


  while(1) {
    printf("hello\n");
  }
  return NULL;
}

int main() {

  void* res;
  pthread_t t;
  pthread_create(&t, NULL, task, NULL);
  int s = pthread_cancel(t);
  if(s != 0)
    fprintf(stderr, "cancel failed\n");
  pthread_join(t, &res);
  assert(res == PTHREAD_CANCELED);
  return 0;
}