#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int a = 10;

void sigkillHandler(int signum) {
    printf("SIGKILL caught\n");
    exit(signum);
}

int main() {

//    signal(SIGTERM, sigkillHandler);
    int b = 5;

    printf("addr global var 'a' = %p\n", &a);
    printf("addr local var 'b' = %p\n", &b);

    printf("PID: %d\n", getpid());
    pid_t grandFatherPID = getpid();
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork error\n");
    } else if (pid == 0) {

        printf("in child process:\n");
        printf("child: addr global var 'a' = %p\n", &a);
        printf("child: addr local var 'b' = %p\n", &b);

        a = 20;
        b = 10;

        printf("new a = %d, b = %d\n", a, b);
        sleep(5);
        pid_t pid2 = fork();
        if (pid2 < 0) {
            perror("fork error\n");
        } else if (pid2 == 0) {
            signal(SIGTERM, sigkillHandler);
            printf("child pid: %d, parent pid: %d, grandfather pid: %d\n", getpid(), getppid(), grandFatherPID);
            sleep(300);
            exit(5);
        } else {
            signal(SIGTERM, sigkillHandler);
            sleep(10);
            printf("parent became zombie\n");
            exit(5);
        }
    } else {
        signal(SIGTERM, sigkillHandler);
        sleep(10000);
        printf("new a = %d, b = %d\n", a, b);
        return 0;
    }
}