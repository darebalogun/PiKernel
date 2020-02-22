#include <stdint.h>
#include <kernel/list.h>

#ifndef PROCESS_H
#define PROCESS_H

typedef void (*kproc_function_f)(void);

typedef struct
{
    uint64_t tcr_el1;
    uint64_t sctlr_el1;
    uint64_t far_el1;
    uint64_t esr_el1;
    uint64_t spsr_el1;
    uint64_t elr_el1;
    uint64_t x29;
    uint64_t x28;
    uint64_t x27;
    uint64_t x26;
    uint64_t x25;
    uint64_t x24;
    uint64_t x23;
    uint64_t x22;
    uint64_t x21;
    uint64_t x20;
    uint64_t x19;
    uint64_t x18;
    uint64_t x17;
    uint64_t x16;
    uint64_t x15;
    uint64_t x14;
    uint64_t x13;
    uint64_t x12;
    uint64_t x11;
    uint64_t x10;
    uint64_t x9;
    uint64_t x8;
    uint64_t x7;
    uint64_t x6;
    uint64_t x5;
    uint64_t x4;
    uint64_t x3;
    uint64_t x2;
    uint64_t x1;
    uint64_t x0;
    uint64_t sp;
    uint64_t x30;

} proc_reg_t;

typedef struct pcb
{
    proc_reg_t *reg_state;
    void *stack_page;
    uint64_t pid;
    DEFINE_LINK(pcb);
    char name[40];
} proc_ctrl_blk_t;

DEFINE_LIST(pcb);

void process_init(void);
void create_process(kproc_function_f proc_func, char *name, int name_len);
void schedule(void);

#endif
