#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define BUFFER_LENGTH 256

void
readline_(const char* str, char* buf, int buf_len, const char* sub_str)
{
  char c = str[0];
  int i = 0;
  while (c != 0 && i < buf_len)
  {
    if (c != '\n')
    {
      buf[i] = c;
    }
    else
    {
      break;
    }
    i++;
    c = str[i];
  }
  sub_str = str + (i + 1);
}

int
readline(int fd, char* buf, int buf_len)
{
  char c = 0;
  int i = 0;
  while (read(fd, &c, sizeof(c)))
  {
    if (i < buf_len)
    {
      if (c != '\n')
      {
        buf[i] = c;
        i++;
      }
      else
      {
        break;
      }
    }
    else
    {
      if (c != '\n')
      {
        continue;
      }
      else
      {
        break;
      }
    }
  }
  return i;
}

int
main(int argc, char** argv)
{
  char buf[BUFFER_LENGTH] = { 0 };
  char* exec_argv[MAXARG] = { 0 };
  int argv_num = 0;
  if (argv[1] == 0)
  {
    fprintf(2, "xargs need at least 1 parameter\n");
  }
  while (argv[argv_num + 1] != 0)
  {
    exec_argv[argv_num] = argv[argv_num + 1];
    argv_num++;
  }

  while (readline(0, buf, BUFFER_LENGTH))
  {
    // fprintf(1, "%s\n", buf);
    exec_argv[argv_num] = buf;
    if (fork() == 0)
    {
      exec(exec_argv[0], exec_argv);
      fprintf(2, "exec failed!\n");
      exit(0);
    }
    wait(0);
    memset(buf, 0, BUFFER_LENGTH);
  }
  exit(0);
}