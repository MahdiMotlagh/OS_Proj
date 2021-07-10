#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"
#include "mmu.h"
#include "memlayout.h"
#include "param.h"
#include "proc.h"

int main(int argc, const char *argv[])
{

    const int pcount = atoi(argv[1]);
    int len;
    int i;
    struct proc_info *procs = NULL;

    for (i = 0; i < pcount; ++i)
    {
        int f = fork();
        if (f==0)
        {
            malloc(getpid() * 11228 + 78);
            while(1==1){}
            exit();
        }
    }

    sleep(100);

    procs = malloc(sizeof(struct proc_info) * NPROC);
    proc_dump(procs, &len);
    printf(1, "pid\tsize\n");

    for (i = 0; i < len; ++i)
        printf(1, "%d\t%d\n", procs[i].pid, procs[i].memsize);

    free(procs);

    exit();
    return 0;
}
