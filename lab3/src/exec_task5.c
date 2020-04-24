#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv) {

    pid_t pid = fork();

    if (pid == 0) {
        execl("sequential_min_max", "sequential_min_max", "1", "5", NULL);
    }
    else {
        wait(0);
        printf("Parent\n");
    }
fflush(stdout);
    return 0;
}