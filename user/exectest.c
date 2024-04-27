#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(void)
{
  char* argv[3];

  argv[0] = "echo";
  argv[1] = "hello world";
  argv[2] = 0;

  exec("./echo", argv);
  printf("exec failed!");
  return 0;
};
