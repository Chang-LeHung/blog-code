
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* func(void* arg)
{
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
  sleep(1);
  return NULL;
}

int main() {
  pthread_t t;
  pthread_create(&t, NULL, func, NULL);
  pthread_cancel(t);
  void* res;
  pthread_join(t, &res);
  if(res == PTHREAD_CANCELED)
  {
    printf("thread was canceled\n");
  }
  return 0;
}