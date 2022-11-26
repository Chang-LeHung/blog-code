
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
#include <sys/signal.h>

pid_t pid;
void my_handler (int signo, siginfo_t *si, void*ucontext)
{
  int fd = open("./test1.txt", O_RDWR | O_CREAT);
  char s[1024];
  sprintf(s, "pid = %d si->si_pid = %d\n", pid, si->si_pid);
  write(fd, s, strlen(s));
  fsync(fd);
  _exit(0);
}

int main()
{
  pid = getppid();
  struct sigaction demo;
  demo.sa_handler = my_handler;
  demo.sa_flags |= SA_SIGINFO;
  sigaction(SIGHUP, &demo, NULL);
  int fd = open("./test2.txt", O_WRONLY | O_CREAT);
  char s[1024];
  while(1)
  {
    sprintf(s, "time = %lu\n", time(NULL));
    write(fd, s, strlen(s));
    int res = write(STDOUT_FILENO, s, strlen(s));
    if(res == -1)
    {
      sprintf(s, "ERROR = %d\n", errno);
      write(fd, s, strlen(s));
      if(errno == EIO)
      {
        sprintf(s, "ERROR = EIO\n");
        write(fd, s, strlen(s));
      }
    }
    sleep(1);
  }
  return 0;
}