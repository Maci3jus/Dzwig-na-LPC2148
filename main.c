
#include "pre_emptive_os/api/osapi.h"
#include "pre_emptive_os/api/general.h"
#include <printf_P.h>
#include <ea_init.h>
#include <lpc2xxx.h>
#include <consol.h>

#define PROC1_STACK_SIZE 2048
#define PROC2_STACK_SIZE 2048

#define INIT_STACK_SIZE  400

static tU8 proc1Stack[PROC1_STACK_SIZE];
static tU8 proc2Stack[PROC2_STACK_SIZE];

static tU8 initStack[INIT_STACK_SIZE];
static tU8 pid1;
static tU8 pid2;


static void proc1(void* arg);
static void proc2(void* arg);

static void initProc(void* arg);

void startTimer1(void);


int
main(void)
{
  tU8 error;
  tU8 pid;

  //immediately turn off buzzer (if connected)
  IODIR0 |= 0x00000080;
  IOSET0  = 0x00000080;
  
  osInit();
  osCreateProcess(initProc, initStack, INIT_STACK_SIZE, &pid, 1, NULL, &error);
  osStartProcess(pid, &error);
  
  osStart();
  return 0;
}

static void
proc1(void* arg)
{
	tU8  centerReleased = TRUE;
	tU8  error;
    osCreateProcess(proc2, proc2Stack, PROC2_STACK_SIZE, &pid2, 3, NULL, &error);
    osStartProcess(pid2, &error);

    osSleep(100);

    IOPIN &= ~0x001f0000;
    for(;;){

      if ((IOPIN & 0x00010000) == 0)
      {
      	if (centerReleased == TRUE)
      	{
      		centerReleased = FALSE;
      	}
      }
      else
      {
      	if (centerReleased == FALSE)
      	centerReleased = TRUE;
      }
      osSleep(2);
    }
}
static void
proc2(void* arg)
{
	startTimer1();
	startMenu();
}
static void
initProc(void* arg)
{
  tU8 error;

  eaInit();
  osCreateProcess(proc1, proc1Stack, PROC1_STACK_SIZE, &pid1, 3, NULL, &error);
  osStartProcess(pid1, &error);

  osDeleteProcess();
}

