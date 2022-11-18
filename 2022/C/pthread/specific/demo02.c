


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_key_t key;

void key_destructor1(void* arg) 
{
  printf("arg = %d\n", *(int*)arg);
  free(arg);
}


void thread_local() 
{
  int* q = pthread_getspecific(key);
  printf("q == %d\n", *q);
}

void* func1(void* arg)
{
  printf("In func1\n");
  int* s = malloc(sizeof(int));
  *s = 100;
  pthread_setspecific(key, s);
  thread_local();
  printf("Out func1\n");
  sleep(2);
  return NULL;
}

void* func2(void* arg)
{
  printf("In func2\n");
  int* s = malloc(sizeof(int));
  *s = -100;
  pthread_setspecific(key, s);
  thread_local();
  printf("Out func2\n");
  sleep(2);
  return NULL;
}


int main() {
  pthread_key_create(&key, key_destructor1);
  pthread_t t1, t2;
  pthread_create(&t1, NULL, func1, NULL);
  pthread_create(&t2, NULL, func2, NULL);
  sleep(1);
  pthread_cancel(t1);
  pthread_cancel(t2);
  void* res1, *res2;
  pthread_join(t1, &res1);
  pthread_join(t2, &res2);
  if(res1 == PTHREAD_CANCELED) 
  {
    printf("thread1 was canceled\n");
  }

  if(res2 == PTHREAD_CANCELED) 
  {
    printf("thread2 was canceled\n");
  }
  pthread_key_delete(key);
  return 0;
}