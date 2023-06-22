#include <stdlib.h>    // for exit() and atexit()
#include <stdio.h>     // for printf()
#include <unistd.h>    // for getpid() and fork()
#include <signal.h>    // for signal()
#include <sys/wait.h>  // for waitpid()

// A macro representing the number of children to fork and reap
#define NUM_CHILDREN 5

// Global variables here
volatile int fork_count = 0;
volatile int reap_count = 0;

// Function declarations here
void exit_handler(void);
void sigchld_handler(int signum);
void disable_buffering(void);

// Main function
int main(void) {
    // Disable buffering
    disable_buffering();

    // Install exit handler and signal handler for SIGCHLD
    atexit(exit_handler);
    signal(SIGCHLD, sigchld_handler);

    // Print parent's PID
    printf("Parent says: my PID is %d\n", getpid());

    // Loop until X children have been forked
    while (fork_count < NUM_CHILDREN) {
        pid_t pid;

        // Fork a child process
        fork_count++;
        pid = fork();

        if (pid == 0) {
            // Child process
            printf("Child says: child %d with PID %d exiting\n", fork_count, getpid());
            exit(fork_count);
        } else if (pid > 0) {
            // Parent process
            // Do nothing, wait for SIGCHLD
        } else {
            // Fork failed
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    // Wait until X children have been reaped
    while (reap_count < NUM_CHILDREN) {
        pause();  // Suspend the parent process until a signal is caught
    }

    printf("Parent says: exiting\n");
    exit(0);
}

// Function definitions
void exit_handler(void) {
    printf("Process %d is exiting\n", getpid());
}

void sigchld_handler(int signum) {
    int wstatus;
    (void)signum; // To avoid unused parameter warning

    while (waitpid(-1, &wstatus, WNOHANG) > 0) {
        // Reap a zombie child
        reap_count++;
        if (WIFEXITED(wstatus)) {
            int exit_status = WEXITSTATUS(wstatus);
            printf("Parent says: child %d reaped with exit status %d\n", reap_count, exit_status);
        }
    }
}

void disable_buffering(void) {
    setbuf(stdout, NULL);
}
