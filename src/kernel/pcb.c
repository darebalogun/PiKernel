#include <kernel/pcb.h>
#include <kernel/mem.h>
#include <kernel/timer.h>
#include <common/stdlib.h>
#include <kernel/kerio.h>

static uint32_t next_proc = 1;
#define NEXT_PID next_proc++;

extern uint8_t __end;
extern void switch_process(proc_ctrl_blk_t *old, proc_ctrl_blk_t *new);

IMPLEMENT_LIST(pcb);

pcb_list_t queue;
pcb_list_t all_proc;

proc_ctrl_blk_t *curr_proc;

void schedule(void)
{
    disable_interrupts();
    proc_ctrl_blk_t *new_proc;
    proc_ctrl_blk_t *old_proc;

    // If there are no processes in the queue return
    if (size_pcb_list(&queue) == 0)
    {
        return;
    }

    // Get the next thread to run
    new_proc = pop_pcb_list(&queue);
    old_proc = curr_proc;
    curr_proc = new_proc;

    // Put current process back in run queue
    append_pcb_list(&queue, old_proc);

    //Context Switch
    printf("Before switch\n");
    switch_process(old_proc, new_proc);
    printf("After switch\n");
    enable_interrupts();
}

void process_init(void)
{
    proc_ctrl_blk_t *main_pcb;
    INITIALIZE_LIST(queue);
    INITIALIZE_LIST(all_proc);

    // Allocate memory
    main_pcb = malloc(sizeof(proc_ctrl_blk_t));
    main_pcb->stack_page = (void *)&__end;
    main_pcb->pid = NEXT_PID;
    memcpy(main_pcb->name, "Init Proc", 10);

    // Add self to process list
    append_pcb_list(&all_proc, main_pcb);

    curr_proc = main_pcb;
}

static void delete_process(void)
{
    disable_interrupts();
    proc_ctrl_blk_t *new_proc;
    proc_ctrl_blk_t *old_proc;

    //If nothing on run queue we have nothing to do;
    if (size_pcb_list(&queue) == 0)
    {
        return;
    }

    //Run next thread. Round Robin
    new_proc = pop_pcb_list(&queue);
    old_proc = curr_proc;
    curr_proc = new_proc;

    // Free resources used by current (now old) process
    free_page(old_proc->stack_page);
    free(old_proc);

    //Switch to new thread
    switch_process(old_proc, new_proc);
}

void create_process(kproc_function_f proc_func, char *name, int name_len)
{
    proc_ctrl_blk_t *pcb;
    proc_reg_t *new_reg_state;

    pcb = malloc(sizeof(proc_ctrl_blk_t));
    pcb->stack_page = alloc_page();
    pcb->pid = NEXT_PID;
    memcpy(pcb->name, name, MIN(name_len, 39));
    pcb->name[MIN(name_len, 39) + 1] = 0;

    // Get location of stack pointer
    new_reg_state = pcb->stack_page + PAGE_SIZE - sizeof(proc_reg_t);
    pcb->reg_state = new_reg_state;

    // Set up stack that will be restored in context switch
    bzero(new_reg_state, sizeof(proc_reg_t));
    new_reg_state->x30 = (uint64_t)proc_func;
    new_reg_state->sp = (uint64_t)delete_process;

    // Add thread to list
    append_pcb_list(&all_proc, pcb);
    append_pcb_list(&queue, pcb);
}