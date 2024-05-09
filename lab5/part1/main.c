#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int a = 10;

int main() {

    int b = 5;

    printf("addr global var 'a' = %p\n", &a);
    printf("addr local var 'b' = %p\n", &b);

    printf("PID: %d\n", getpid());

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork error\n");
    }
    else if (pid == 0) {

        printf("in child process:\n");
        printf("child pid: %d, parent pid: %d\n", getpid(), getppid());
        printf("child: addr global var 'a' = %p\n", &a);
        printf("child: addr local var 'b' = %p\n", &b);

        a = 20;
        b = 10;
        printf("child: new a = %d, b = %d\n", a, b);
        sleep(30);
        return 5;
    } else {
//        sleep(10);
//        return 0;
        sleep(10);
        int status;
        waitpid(pid, &status, 0);
        printf("parent: new a = %d, b = %d\n", a, b);
        if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child process terminated by signal %d\n", WTERMSIG(status));
        }
        sleep(30);
        return 0;
    }
}