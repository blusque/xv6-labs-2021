#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char** argv)
{
  int time;
  if (argv[1] == 0)
  {
    fprintf(1, "sleep time should be specified\n");
    exit(1);
  }
  time = atoi(argv[1]);
  sleep(time);
  exit(0);
}