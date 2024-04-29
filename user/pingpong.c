#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char** argv)
{
  int pid;
  int p[2];
  char data;

  pipe(p);
  if (fork() == 0)
  {
    write(p[1], "a", 1);
    pid = getpid();
    fprintf(1, "%d: received ping\n", pid);
    exit(0);
  }
  else
  {
    wait(0);
    read(p[0], (void*)&data, 1);
    pid = getpid();
    fprintf(1, "%d: received pong\n", pid);
  }
  exit(0);
}