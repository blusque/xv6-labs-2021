#include "kernel/types.h"
#include "user/user.h"

void
duptest(void)
{
  int fd = dup(1);
  write(1, "hello ", 6);
  write(fd, "world\n", 6);
}

int
main(void)
{
  duptest();
  return 0;
}