
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

pid_t ppid;
void my_handler (int signo, siginfo_t *si, void*ucontext)
{
  int fd = open("./test1.txt", O_RDWR | O_CREAT, 0644);
  char s[1024];
  // ppid 和 si->si_pid 的进程号相等
  // si->si_pid 就是给这个进程发送信号的进程号
  sprintf(s, "ppid = %d si->si_pid = %d\n", ppid, si->si_pid);
  write(fd, s, strlen(s));
  fsync(fd);
  _exit(0);
}

int main()
{
  ppid = getppid();
  struct sigaction demo;
  demo.sa_handler = my_handler;
  demo.sa_flags |= SA_SIGINFO;
  sigaction(SIGHUP, &demo, NULL);
  int fd = open("./test2.txt", O_WRONLY | O_CREAT, 0644);
  char s[1024];
  while(1)
  {
    sprintf(s, "time = %lu\n", time(NULL));
    write(fd, s, strlen(s));
    int res = write(STDOUT_FILENO, s, strlen(s));
    sleep(1);
  }
  return 0;
}