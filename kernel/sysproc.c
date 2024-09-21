#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 start_va, start_pa;
  int len;
  uint64 kbitmask;
  uint64 ubitmask;
  pagetable_t pgtbl = myproc()->pagetable;
  int flag = 0;

  if (argaddr(0, &start_va) < 0)
    return -1;
  if (argint(1, &len) < 0)
    return -1;
  if (len > 8 * sizeof(uint64))
    return -1;
  if (argaddr(2, &ubitmask) < 0)
    return -1;

  memset((void*)&kbitmask, 0, sizeof(uint64));
  // printf("kbitmask1: %p\n", kbitmask);

  start_va = PGROUNDUP(start_va);
  // start_pa = walkaddr(pgtbl, start_va);

  for (int i = 0; i < len; i++){
    start_pa = walkaddr(pgtbl, start_va);
    for (char* p = (char*)start_pa; (uint64)p < start_pa + PGSIZE; p++) {
      if (*(char*)p != 0){
        flag++;
        kbitmask |= 1 << (i + 1);
      }
    }
    // printf("pa: %d\n", flag);
    flag = 0;
    start_va += PGSIZE;
  }
  // printf("kbitmask2: %p\n", kbitmask);
  
  copyout(pgtbl, ubitmask, (void*)&kbitmask, sizeof(uint64));

  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
