#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void workWithPID(char* args[]) {
    printf("PID процесса: %d\n", getpid());

    sleep(1);

    if (execv(args[0], args) == -1) {
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    // Выводим сообщение "Hello world" (этот код выполнится только если exec() не выполнится успешно)
    printf("Hello world\n");
}

int main(int argc, char* args[]) {
    int a = 10;
    printf("address: %p\n", &a);
    workWithPID(args);
    printf("Hello world\n");
    return 0;
}
