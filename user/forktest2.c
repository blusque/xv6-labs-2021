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
  int pid = fork();

  print("fork test\n");

  if (pid != 0)
  {
    printf("parent: child=%d\n", pid);
    pid = wait((int *) 0);
    printf("child %d is done\n", pid);
  }
  else if (pid == 0)
  {
    printf("child: exiting\n");
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