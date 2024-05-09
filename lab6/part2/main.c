#include <stdio.h>
#include <unistd.h>

int main() {
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        return -1;
    }

    pid_t cpid = fork();
    if (cpid == -1){
        perror("pid");
        return -1;
    } else if (cpid == 0) { // child write to pipe
        close(pipefd[0]); // close unused read end

        unsigned int count = 0;

        while (1) {
            write(pipefd[1], &count, 1); //repeat ?
            count++;
        }
    } else { //parent reads from pipe
        close(pipefd[1]); //close unused write end
        unsigned int prevValue = 0;
        unsigned int curValue = 0;
        while (1) {
            read (pipefd[0], &prevValue, 1);
            read (pipefd[0], &curValue, 1);
            if (curValue != prevValue + 1) {
                printf("error sequence\n");
                printf("cur: %d, prev: %d\n\n", curValue, prevValue);
            }
        }
    }
}