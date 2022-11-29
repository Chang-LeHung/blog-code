
#define _GNU_SOURCE
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

void my_handler (int signo, siginfo_t *si, void*ucontext)
{
  char* s = "Hello World\n";
  write(STDOUT_FILENO, s, strlen(s));
}

void sig (int signo)
{
  char* s = "Hello World\n";
  write(STDOUT_FILENO, s, strlen(s));
}

int main()
{
  printf("pid = %d\n", getpid());
  struct sigaction demo;
  demo.sa_sigaction = my_handler;
  demo.sa_handler = sig;
  demo.sa_flags |= SA_SIGINFO;
  demo.sa_flags |= SA_RESTART;
  demo.sa_flags &= ~SA_RESETHAND;
  sigaction(SIGHUP, &demo, NULL);
  sigaction(SIGCONT, &demo, NULL);
  sigaction(SIGSEGV, &demo, NULL);
  signal(SIGHUP, sig);
  signal(SIGSEGV, sig);
  signal(SIGCONT, sig);
  int * i = (void*)0;
  int j = *i;
  return 0;
}