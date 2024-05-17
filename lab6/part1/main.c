#include <stdio.h>
#include <sys/mman.h>
#include <malloc.h>
#include <unistd.h>

#define PAGE_SIZE 4096

int main() {
    size_t len = PAGE_SIZE;
    unsigned int* mappedPtr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (mappedPtr == MAP_FAILED) {
        fprintf(stderr, "Ошибка при присоединении памяти с помощью mmap\n");
        return 1;
    }
    pid_t pid = fork();
    if (pid < 0) {
        perror("error fork");
        free(mappedPtr);
        return 0;
    } else if (pid == 0) {
        printf("first PID: %d, second PID: %d", getpid(), getppid());
        unsigned int count = 0;
        while (1) {
            for (int i = 0; i < PAGE_SIZE / sizeof (unsigned int); ++i) {
                (mappedPtr)[i] = count;
                ++count;
            }
        }
    } else {
        while (1) {
            usleep(500);
            for (int i = 1; i < PAGE_SIZE / sizeof (unsigned int); ++i) {
                int cur = mappedPtr[i];
                int prev = mappedPtr[i - 1];
                if ((cur != prev + 1)) {
                    printf("error sequence\n cur: %d, prev: %d, map(cur):%d, map(prev):%d\n\n", cur, prev, mappedPtr[i], mappedPtr[i - 1]);
                }
            }
        }
    }
}