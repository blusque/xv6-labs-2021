#include "kernel/types.h"
#include "user/user.h"

void
drop(int p)
{

}

void
prime(int p)
{
  fprintf(1, "prime %d\n", p);
}

// int
// neighbor(int p, int pos, int* list, int size)
// {
//   if (pos == size)
//   {
//     prime(p);
//     list[pos] = p;
//     return pos + 1;
//   }
//   int n = list[pos];
//   // fprintf(1, "prime: %d\n", n);
//   if (p % n == 0)
//   {
//     drop(p);
//     return size;
//   }
//   else
//   {
//     return neighbor(p, pos + 1, list, size);
//   }
// }

// int
// main(int argc, char** argv)
// {
//   int size = 0, new_size = -1;
//   int list[35] = { 0 };
//   // int p[2] = {0};
//   for (int i = 2; i < 36; i++)
//   {
//     new_size = neighbor(i, 0, list, size);
//     if (new_size > size)
//     {
//       size = new_size;
//     }
//   }
//   exit(0);
// }

int
create(int p, int* pfd)
{
  int pid = 0;
  int n = 0;
  int new_pfd[2] = { 0 };
  int child = 0;
  int i = p;

  prime(p);

  if ((pid = fork()) == 0)
  {
    n = p;

    pipe(new_pfd);

    close(pfd[1]);
    // close(new_pfd[0]);

    while (read(pfd[0], &i, sizeof(i)))
    {
      if (i % n == 0)
      {
        drop(i);
      }
      else
      {
        write(new_pfd[1], &i, sizeof(i));
        if (!child)
        {
          child = create(i, new_pfd);
          close(new_pfd[0]);
        }
      }
    }
    close(new_pfd[1]);
    wait(0);
    exit(0);
  }
  return pid;
}

int
main(int argc, char** argv)
{
  int child = 0;
  int pfd[2] = { 0 };

  pipe(pfd);

  // printf("size of void*: %d\n", sizeof(void*));
  for (int i = 2; i < 36; i++)
  {
    write(pfd[1], &i, sizeof(i));
    if (!child)
    {
      child = create(i, pfd);
      close(pfd[0]);
    }
  }
  close(pfd[1]);
  wait(0);
  exit(0);
}