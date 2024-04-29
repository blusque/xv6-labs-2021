// Test that fork fails gracefully.
// Tiny executable so that the limit can be filling the proc table.
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
print(const char *s)
{
  write(1, s, strlen(s));
}

void
forktest2(void)
{
  int n = 2;
  int p[2] = { 0 };
  int pid = fork();

  print("fork test\n");

  if (pid != 0)
  {
    pid = wait((int *) 0);
    printf("parent: child=%d\n", pid);
    printf("child %d is done\n", pid);
    printf("n in parent: %d\n", n);
    printf("p in parent: %d, %d\n", p[0], p[1]);
  }
  else if (pid == 0)
  {
    printf("child: exiting\n");
    p[1] = 10;
    printf("n in child originally: %d\n", n);
    n = 3;
    printf("n in child: %d\n", n);
    printf("p in child: %d, %d\n", p[0], p[1]);
    exit(0);
  }
  else
  {
    printf("fork error\n");
    exit(1);
  }

  print("fork test OK\n");
}

int
main(void)
{
  forktest2();
  exit(0);
}