// Seung Ki Lee 2018 Spring

// For printf
#include <stdio.h>
// For atoi
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

// Declare required functions
unsigned int sleep (unsigned int seconds);
pid_t getpid (void);
void call_sleep (int seconds);

int main(int argc, char *argv[ ]) {

      int count;
      if (argc == 2)
            count = atoi(argv[1]);
      else
            count = 5;
      // call sleep
      call_sleep(count);
}

// make a function that uses sleep function that will stop when :
// 1. TICK !< seconds
// 2. signal caught & signal handler return
void call_sleep (int seconds) {
      // initialize TICK
      int tick = 1;
      // print initial pid at start
      // %d print the next argument as int
      printf ("%d-START\n", getpid());
      // loop through the seconds
      while (seconds > 0) {
            // sleep calling process
            sleep (1);
            // print time and pid
            printf ("%d-TICK\n", getpid(), tick);
            // increment counter variables for the loop
            seconds--;
            tick++;
      }
}
// Seung Ki Lee 2018 Spring

// SAMPLE OUTPUT : genuse34.lyle.smu.edu
// seungkil@genuse34.engr.smu.edu$ ./sleepy 5
// 5313-START
// 5313-TICK
// 5313-TICK
// 5313-TICK
// 5313-TICK
// 5313-TICK
// seungkil@genuse34.engr.smu.edu$ ./sleepy 5
// 5362-START
// 5362-TICK
// 5362-TICK
// 5362-TICK
// 5362-TICK
// 5362-TICK
// seungkil@genuse34.engr.smu.edu$ ./sleepy 5
// 5384-START
// 5384-TICK
// 5384-TICK
// 5384-TICK
// 5384-TICK
// 5384-TICK

// Question 1 : Does the program have same process id every time?
// Answer 1 : No. While each TICK has the same pid, every run of program has different process id everytime. 
// The process id increments. This makes sense because even if you run the same program each time new process is created.

// Question 2 : What state do you think the process transitions to after a call to sleep function?
// Answer 2 : Given that the process will stop and wait when sleep() function is called and will only respond to external inturrupts
// like signal handler returning or set timer(clock) reaching certain number, I would say the processor will transition into Blocked.

// Seung Ki Lee 2018 Spring