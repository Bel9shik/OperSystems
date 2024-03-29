#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc, char* args[]) {

    system("sudo cat /proc/%d/pagemap");

    int a = 5;

    while (1) {
        printf("my PID: %d\n", getpid());
        sleep(1);
    }

    return 0;
}