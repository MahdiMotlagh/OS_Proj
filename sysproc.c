#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
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

int
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

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_proc_dump(void)
{
  int array_size;
  struct proc_info *array_proc;

  if(argint(1, &array_size) < 0)
    return -1;
  if(argptr(0, (void*)&array_proc, sizeof(*array_proc)) < 0)
    return -1;

  sort_all_running_proccesses(array_proc, array_size);
  return 1;
}

void sort_all_running_proccesses(struct proc_info *array_proc, int array_size)
{
  int i = 0;
  for(; i < array_size; i++){
    array_proc[i].pid = -1;
    array_proc[i].memsize = -1;
  }

  struct ptable *ptable = proc_table();
  struct proc *p;
  p = ptable->proc;
  
  int i = 0;
  for (i = 0; i < NPROC; ++i, ++p)
  {
    if (p->state == RUNNING || p->state == RUNNABLE){
      array_proc[i].pid = p->pid;
	    array_proc[i].memsize = p->sz;
    }
  }

  int j = 0;
  for (i = 0; i < NPROC; i++){
    for (j = i; j < NPROC; j++){
      if(array_proc[i].memsize > array_proc[j].memsize){
        struct proc_info tmp = array_proc[i];
        array_proc[i] = array_proc[j];
        array_proc[j] = tmp;
      }
    }
  }

}
