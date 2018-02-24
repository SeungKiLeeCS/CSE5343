// Seung Ki Lee
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int count; //   For calling twoSleepy n
    if (argc == 2)
        count = atoi(argv[1]);
    else
        count = 10;

    // duplicate using fork function
    int process = fork();
    // for 0 return on fork
    if (process == 0) {
        // jump to child code.
        for (int i = 0; i < count; i++) {
            printf("Child Process with PID: %d and PPID: %d; tick %d\n", getpid(), getppid(), i+1);
            sleep(1);
        }
        printf("Child process terminating\n");
        exit(0);
    }
    // Non Zero Fork jump to parents
    for (int j = 0; j < count; j++) {
        printf("Original Process with PID: %d and PPID: %d; tick %d\n", getpid(), getppid(), j+1);
        // comment out for PART II
        sleep(1);
    }
    printf("Parent process terminating\n");
    // use wait(NULL) to block the parent process until child process finishes. 
    // comment out for PART II
    wait(NULL);
}

// Seung Ki Lee

// PART I >>> Results

// seungkil@genuse34.engr.smu.edu$ gcc twoSleepy.c -o twoSleepy
// seungkil@genuse34.engr.smu.edu$ ./twoSleepy
// Original Process with PID: 27305 and PPID: 22833; tick 1
// Child Process with PID: 27306 and PPID: 27305; tick 1
// Original Process with PID: 27305 and PPID: 22833; tick 2
// Child Process with PID: 27306 and PPID: 27305; tick 2
// Original Process with PID: 27305 and PPID: 22833; tick 3
// Child Process with PID: 27306 and PPID: 27305; tick 3
// Original Process with PID: 27305 and PPID: 22833; tick 4
// Child Process with PID: 27306 and PPID: 27305; tick 4
// Child Process with PID: 27306 and PPID: 27305; tick 5
// Original Process with PID: 27305 and PPID: 22833; tick 5
// Child Process with PID: 27306 and PPID: 27305; tick 6
// Original Process with PID: 27305 and PPID: 22833; tick 6
// Child Process with PID: 27306 and PPID: 27305; tick 7
// Original Process with PID: 27305 and PPID: 22833; tick 7
// Child Process with PID: 27306 and PPID: 27305; tick 8
// Original Process with PID: 27305 and PPID: 22833; tick 8
// Child Process with PID: 27306 and PPID: 27305; tick 9
// Original Process with PID: 27305 and PPID: 22833; tick 9
// Child Process with PID: 27306 and PPID: 27305; tick 10
// Original Process with PID: 27305 and PPID: 22833; tick 10
// Child process terminating
// Parent process terminating

// PART I >>> Questions

// Each process will hits all five of the states. 
// The parent process will be created at NEW then move to READY queue, and eventually dispatched to RUNNNING. 
// When process hits sleep, the process moves to the BLOCKED state as we discovered in program 1.
// Then child process proceed to RUNNING, then eventually to BLOCKED. 
// After 1 second has ticked the parent moves back to READY queue and dispatched to RUNNING. 
// This will repeat until all instructions are completed, and all processes go to EXIT.

// Furthermore, this definitely explains why the parent and child outputs are evenly interleaved. 
// Child will execute only when Parent is sleeping. Once it goes to sleep, the parent wakes up and then this cycle continues.


// Part II >>> Results : Commented out sleep and wait function for parent process.

// seungkil@genuse34.engr.smu.edu$ ./twoSleepy_2
// Original Process with PID: 27684 and PPID: 22833; tick 1
// Original Process with PID: 27684 and PPID: 22833; tick 2
// Original Process with PID: 27684 and PPID: 22833; tick 3
// Original Process with PID: 27684 and PPID: 22833; tick 4
// Original Process with PID: 27684 and PPID: 22833; tick 5
// Original Process with PID: 27684 and PPID: 22833; tick 6
// Original Process with PID: 27684 and PPID: 22833; tick 7
// Original Process with PID: 27684 and PPID: 22833; tick 8
// Original Process with PID: 27684 and PPID: 22833; tick 9
// Original Process with PID: 27684 and PPID: 22833; tick 10
// Parent process terminating
// Child Process with PID: 27685 and PPID: 27684; tick 1
// seungkil@genuse34.engr.smu.edu$ Child Process with PID: 27685 and PPID: 1; tick 2
// Child Process with PID: 27685 and PPID: 1; tick 3
// Child Process with PID: 27685 and PPID: 1; tick 4
// Child Process with PID: 27685 and PPID: 1; tick 5
// Child Process with PID: 27685 and PPID: 1; tick 6
// Child Process with PID: 27685 and PPID: 1; tick 7
// Child Process with PID: 27685 and PPID: 1; tick 8
// Child Process with PID: 27685 and PPID: 1; tick 9
// Child Process with PID: 27685 and PPID: 1; tick 10
// Child process terminating

// PART II >>> Questions

// 1.  As we can see, the parent process will complete all ticks before child process starts. 
//     Interestingly, the terminal prompt actually displays before child process finishes. 
//     You can see from 
//     >>>    Child Process with PID: 27685 and PPID: 27684; tick 1
//     >>>    seungkil@genuse34.engr.smu.edu$ Child Process with PID: 27685 and PPID: 1; tick 2 
//     where after the child process starts prompt appears before it completes.

// 2.  The PPID in the child process is 1. 
//     With the help of Stack Overflow >>> https://stackoverflow.com/questions/13170757/why-does-this-ppid-1-am-i-missing-something-about-fork
//     We know that when a process is stopped before the child complete, the child processes get assigned PPID of 1.
//     This is consistent with our code because we commented out the sleep and wait function for parent process.

// Seung Ki Lee
