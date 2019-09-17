#include "task.h"
#include "mem.h"
#include "i386.h"
#include "conio.h"
#include "helper.h"
#include "types.h"
#include "status.h"
#include "ktrace.h"
#include "object.h"
#include "resource.h"

#define STACK_SIZE	2

char *gdt;
task *KernelTask;
task *CurrentTask;
unsigned long CurrentTaskNum;
uint32 nextentry;
taskq TaskList[MaxProc];
uint32 *KernelPageTable;
uint32 _cr3;
task *irq_task_map[16];
uint32 MaxUsedProc;
uint32 SystemTimer;

extern void k_debugger();

STATUS
PsCreateNewTask(IN uint32 ip, IN struct _process *Process) {
	task *t;
	uint32 Stack;
	uint32 UserPageTable;
	uint32 holder;
	uint32 holder2;
	STATUS Status;
	struct OBJECT *obj;
	uint32 i;
	uint32 memcount = 0;

	// We have created a new thread for the address space so increment
	// the thread count
	// NOTE: THIS SHOULD ACTUALLY BE IN Process. FIX THIS
	Process->addr->refcount++;

	Status = ObCreateObject(OBJ_TYPE_THREAD, Process, (void *)&obj);
	if(Status != STATUS_SUCCESS) 
	{
		kprintf("Error creating task object: 0x%x\n", Status);
		return STATUS_GENERAL_ERROR;
	}
	t = (task *) obj;
		

	//
	// Put the stack by the 2gb boundary so  their is room for it to grow later
	// We don't want it to be smack in the middle of the heap
	//
	Stack = (uint32) MmAllocPagesLo(Process->addr, STACK_SIZE, 7, &holder);
	kunmap_Page(Process->addr, Stack, &holder2, STACK_SIZE);

	//
	// Find first free space for stack
	//
	for(i = 1023; i > 0; i--)
	{
		if(!Process->addr->ptab[i])
			memcount++;
		else
			memcount = 0;

		if(memcount == STACK_SIZE)
			break;
	}
	if(!i)
		return STATUS_GENERAL_ERROR;

	aspace_map(Process->addr, (holder / 4096), i, STACK_SIZE, 7);


	//
	// Create a kernel stack that has some meat to it
	//
#ifdef KERNELSTACK
	Stack = (uint32) MmAllocPages(Process->addr, STACK_SIZE, 3, NULL);
	//t->tss.esp0 = (uint32) ( ((char *) Stack) + ((STACK_SIZE - 1) * 4096) + 4092 );
	t->tss.esp0 = Stack + 4096;

kprintf("Kernel stack starts at: 0x%x is set to: 0x%x\n", Stack, t->tss.esp0);
#else
	t->tss.esp0 = (uint32) ( ((char *) t) + + 4096 + 4092 );
#endif
	t->tss.ss0 = SEL_KDATA;
	t->tss.eax = 0xDEADBEEF;
	t->tss.ebx = 0xDEADBEEF;
	t->tss.ecx = 0xDEADBEEF;
	t->tss.edx = 0xDEADBEEF;
	t->tss.esp1 = t->tss.esp2 = t->tss.ss1 = t->tss.ss2 = 0;
	t->tss.cr3 = (((uint32) Process->addr->pdir[0]) & 0xFFFFFF00) - 4096;
	t->tss.eip = (uint32) ip;
	t->tss.eflags = 0x7202;
	t->tss.esp = (i * 4096) + ((STACK_SIZE - 1) * 4096) + 4092;
	t->tss.cs = SEL_UCODE | 3;
	t->tss.ds = t->tss.es = t->tss.ss = 
		t->tss.fs = t->tss.gs = SEL_UDATA | 3;
	t->tss.ldt = t->tss.debugtrap = t->tss.iomapbase = 0;
	t->state = tREADY;
	t->tid = 0;
	t->Parent = Process;
//	t->shm = 0;
//	t->shmlen = 0;

	t->rid = rsrc_new_task(t);
	PsAddTaskToQueue(t, 0);

	return STATUS_SUCCESS;
}

void
task_call(task *t) {
	uint32 sel[2];
	sel[0] = 0;
	sel[1] = SEL_UTSS /* t->tid */;
	__asm__("lcall %0; clts"::"m" (*sel));
}


STATUS
PsAddTaskToQueue(	IN task *t, 
					IN uint32 prior) {
	uint32 i = 0;

	while(i != MaxProc) {
		if(TaskList[i].t == 0) {
//kprintf("task_add: Added a task at #%d p:0x%x\n", i, t);
			TaskList[i].t = t;
			TaskList[i].prior = prior;
			t->tid = i;

			if(i > MaxUsedProc)
				MaxUsedProc = i;

			return STATUS_SUCCESS;
		}
		i++;
	}

	kprintf("TASK: ERROR the task list is full\n");

	return STATUS_GENERAL_ERROR;
}

STATUS
PsTerminateTask(IN task *t) {

	// Mark task as dead.  It will be cleaned up later
	t->state = tDEAD;
	PsSwitchTask();

	return STATUS_SUCCESS;
}

STATUS
PsRemoveTaskFromQueue(IN task *t) {
	unsigned long i = 0;

	while(i != MaxProc) {
		if(TaskList[i].t == t) {
			TaskList[i].t = 0;

			if(i == MaxUsedProc)
				MaxUsedProc--;

			return STATUS_SUCCESS;
		}
		i++;
	}
	return STATUS_GENERAL_ERROR;
}

void
PsPreemptTask( task *t)
{
	uint32 tcr3;
	uint32 x;

Again:
	if(CurrentTaskNum == (MaxProc - 1))
		CurrentTaskNum = 0;
	else
		CurrentTaskNum += 1;

	if(((uint32)TaskList[CurrentTaskNum].t) == 0) goto Again;

	// Is process the one passed in?
	if(TaskList[CurrentTaskNum].t != t) 
		goto Again;

	// If current task is running and not blocked set it to be ready
	// for execution
	if(CurrentTask->state == tRUNNING)
		CurrentTask->state = tREADY;

	CurrentTask = t;

	// Set new task to be running
	CurrentTask->state = tRUNNING;



	i386SetSegment(gdt + SEL_UTSS,
		(uint32) CurrentTask, 104,
		i386rPRESENT | ((CurrentTask->tid == 0) ? i386rDPL0 : i386rDPL3) | i386rTSS, 
		0);

	task_call(CurrentTask);

	asm("pushf ; pop %%eax ; andl $0xBFFF, %%eax ; push %%eax ;; popf":::"ax");
}

void
PsSwitchTask( void ) {
	uint32 tcr3;
	uint32 x;

	SystemTimer += 1;
	if(SystemTimer > 100)
	{
		SystemTimer = 0;
		for(x = 0; x < MaxUsedProc + 1; x++)
		{
			if(TaskList[x].t->state == tSLEEP_TIMER)
			{
				TaskList[x].t->state = tREADY;
				TaskList[x].t->timer = 0;
			}
		}
	}
		

Again:
	if((CurrentTaskNum == (MaxProc - 1)))// || (CurrentTaskNum == MaxUsedProc + 1))
		CurrentTaskNum = 0;
	else
		CurrentTaskNum += 1;

	// If current task is running and not blocked set it to be ready
	// for execution
	if(CurrentTask->state == tRUNNING)
		CurrentTask->state = tREADY;

	if(((uint32)TaskList[CurrentTaskNum].t) == 0) goto Again;


	// If we have found a dead task then we should clean it up
	if(TaskList[CurrentTaskNum].t->state == tDEAD) {
		PsCleanUpTask(TaskList[CurrentTaskNum].t);
		TaskList[CurrentTaskNum].t = 0;
		goto Again;
	}

#ifdef ndef
	switch(TaskList[CurrentTaskNum].t->state)
	{
		case tREADY:
			kprintf("<PsSwitch>: Pid: %d is READY\n", CurrentTaskNum);
			break;
		case tKERNEL:
			kprintf("<PsSwitch>: Pid: %d is KERNEL\n", CurrentTaskNum);
			break;
		case tRUNNING:
			kprintf("<PsSwitch>: Pid: %d is RUNNING\n", CurrentTaskNum);
			break;
		case tSLEEP_PORT:
			kprintf("<PsSwitch>: Pid: %d is SLEEPING on PORT\n", CurrentTaskNum);
			break;
		case tSLEEP_IRQ:
			kprintf("<PsSwitch>: Pid: %d is SLEEPING on IRQ\n", CurrentTaskNum);
			break;
		case tDEAD:
			kprintf("<PsSwitch>: Pid: %d is DEAD\n", CurrentTaskNum);
			break;
		default:
			kprintf("<PsSwitch>: Pid: %d has unknown state\n", CurrentTaskNum);
	}
#endif


	// Is process ready?
	if((TaskList[CurrentTaskNum].t->state != tREADY) && 
		(TaskList[CurrentTaskNum].t->state != tKERNEL)) {
		goto Again;
	}


	CurrentTask = TaskList[CurrentTaskNum].t;

	// Set new task to be running
	CurrentTask->state = tRUNNING;


	if(CurrentTask->tss.eip < 0x80)
	{
		kprintf("WARNING: eip < 0x80");
		k_debugger();
	}

#ifdef ndef
	if( (CurrentTask->tid == 3) || (CurrentTask->tid == 4))
	{
		kprintf("On task %d\n", CurrentTask->tid);
		k_debugger();
	}
#endif


	i386SetSegment(gdt + SEL_UTSS,
		(uint32) CurrentTask, 104,
		i386rPRESENT | ((CurrentTask->tid == 0) ? i386rDPL0 : i386rDPL3) | i386rTSS, 
		0);

	task_call(CurrentTask);

	asm("pushf ; pop %%eax ; andl $0xBFFF, %%eax ; push %%eax ;; popf":::"ax");
}

process *
PsCreateProcess(	IN uint32 Phys, 
					IN uint32 LenInPages,
					IN unsigned char *name) {
	STATUS Status;
	task *Task = NULL;
	aspace *a;
	uint32 i;
	struct _process *Process;
	unsigned long Handle;
	struct OBJECT *Obj;

	//
	// Create a process structure
	//
	Status = ObCreateObject(OBJ_TYPE_PROCESS, &Handle, (void *)&Obj);
	Process = (process *) Obj;
#ifdef ndef
	Process = (process *) MmAllocPages((aspace *)KernelPageTable, 1, 7, NULL);
#endif


//	a = MmCreatePageTable();
	//
	// Create an address space object
	//
	Status = ObCreateObject(OBJ_TYPE_ASPACE, &Handle, (void *)&Obj);
	a = (aspace *)Obj;
	// Map in code - All programs start at virtual address 0x00... ?
	aspace_map(a, (Phys / 4096), 0x0, LenInPages, 7);
	// Map in video memory
	aspace_map(a, 0xB0, 0xB0, 0x10 + 2, 7);

	//
	// Initialize the process structure
	//
	for(i = 0; i < MAX_HANDLES; i++)
		Process->HandleList[i] = NULL;

	Process->addr = a;
	Process->pid = 1;
	for(i = 0; i < 30; i++)
		Process->name[i] = name[i];

//	Status = PsCreateNewTask(0x80, Process);

	return Process;
}

STATUS
PsCleanUpTask(IN task *Task) {
	uint32 i;
	STATUS Status;

// NOTE:  Should go through Handle List and clean up any resources that are
//        in use

	// Remove task from queue
	Status = PsRemoveTaskFromQueue(Task);	
	if(Status != STATUS_SUCCESS)
		kprintf("PsCleanUpTask: Error removing task from Queue\n");

	rsrc_del(Task->rid);

//return STATUS_SUCCESS;		// NOTE: CLEAN THIS UP

	// Decrement reference count to this address space
	Task->Parent->addr->refcount--;


	// If the address space is still in use then don't destroy it
	if(Task->Parent->addr->refcount)
		return STATUS_SUCCESS;

	// Free Any shared memory space
//	SmCloseSeg(Task->shmkey);

	// Free up video memory
	for(i = 0xB0; i < 0xC2; i++)
		Task->Parent->addr->ptab[i] = 0;

	// Free up all memory in address space
	for(i = 0; i < 1024; i++)
	{
		if(Task->Parent->addr->ptab[i] != 0)
		{
			// Mark physical memory as unused
			kunset_PageBit(Task->Parent->addr->ptab[i] / 4096);
			Task->Parent->addr->ptab[i] = (uint32) NULL;
		}
	}

	// Remove Address space from kernel memory
	Status = MmFreePages((aspace *)KernelPageTable, Task->Parent->addr, 2);
	if(Status != STATUS_SUCCESS)
		kprintf("PsCleanUpTask: Error freeing task address space from kernel memory\n");

	// Remove task from kernel memory
	Status = MmFreePages((aspace *)KernelPageTable, Task, 1);
	if(Status != STATUS_SUCCESS)
		kprintf("PsCleanUpTask: Error freeing task from kernel memory\n");

	return STATUS_SUCCESS;
}



