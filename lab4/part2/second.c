#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

void initArray() {
//    char *tmpBuf = (char *) malloc(4096 * 8);
    sleep(0.4);
    initArray();
}

void sigsegv_handler(int signum) {
    printf("Получен сигнал SIGSEGV (Segmentation fault)\n");
//    exit(EXIT_FAILURE);
}

int main(int argc, char *args[]) {
    printf("watch cat /proc/%d/maps\n", getpid());
    printf("PID процесса: %d\n", getpid());
    sleep(10);
    printf("let's go\n");
    //stack
//    initArray();

//heap
//    for (int i = 0; i < 10; ++i) {
//        size_t buffer_size = 100000000;
//        void *ptr = malloc(buffer_size);
//        if (ptr == NULL) {
//            fprintf(stderr, "Ошибка выделения памяти\n");
//            return 1;
//        }
//        printf("Выделено %zu байт памяти на куче\n", buffer_size);
//
//        sleep(1);
//
//        free(ptr);
//    }
//    sleep(3);

    size_t buffer_size = 10 * getpagesize(); // 10 страниц

    void *mapped_ptr = mmap(NULL, buffer_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mapped_ptr == MAP_FAILED) {
        fprintf(stderr, "Ошибка при присоединении памяти с помощью mmap\n");
        return 1;
    }
    printf("Присоединено %zu байт памяти с помощью mmap\n", buffer_size);
    signal(SIGSEGV, sigsegv_handler);
    sleep(3);

    if (mprotect(mapped_ptr, buffer_size, PROT_READ) == -1) {
        fprintf(stderr, "Ошибка при изменении прав доступа с помощью mprotect\n");
        return 1;
    }
    printf("Права доступа к региону изменены на чтение\n");
//    *(int *)mapped_ptr = 43;
    sleep(3);

    if (mprotect(mapped_ptr, buffer_size, PROT_WRITE) == -1) {
        fprintf(stderr, "Ошибка при изменении прав доступа с помощью mprotect\n");
        return 1;
    }
    printf("Права доступа к региону изменены на запись\n");
//    *(int *)mapped_ptr = 43;
//    printf("%d", *(int *)mapped_ptr);
    sleep(10);
    size_t offset = 3 * getpagesize();
    size_t len = 3 * getpagesize();
    if (munmap(mapped_ptr + offset, len) == -1) {
        fprintf(stderr, "Ошибка при отсоединении страниц с помощью munmap\n");
        return 1;
    }
    printf("Страницы с 4 по 6 отсоединены\n");

    sleep(100);

    return 0;
}
