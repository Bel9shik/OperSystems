//#include <stdio.h>
//#include <unistd.h>
//#include <stdlib.h>
//#include <sys/types.h>
//#include <sys/wait.h>
//#include <sys/mman.h>
//#include <string.h>
//
//void initArray() {
////    char *tmpBuf = (char *) malloc(4096 * 8);
//    sleep(0.4);
//    initArray();
//}
//
//void sigsegv_handler(int signum) {
//    printf("Получен сигнал SIGSEGV (Segmentation fault)\n");
////    exit(EXIT_FAILURE);
//}
//
//int main(int argc, char *args[]) {
//    printf("watch cat /proc/%d/maps\n", getpid());
//    printf("PID процесса: %d\n", getpid());
//    sleep(10);
//    printf("let's go\n");
//    //stack
////    initArray();
//
////heap
//    for (int i = 0; i < 10; ++i) {
//        size_t buffer_size = 100000000;
//        void *ptr = malloc(buffer_size);
//        if (ptr == NULL) {
//            fprintf(stderr, "Ошибка выделения памяти\n");
//            return 1;
//        }
//        printf("Выделено %zu байт памяти на куче\n", buffer_size);
//
////        sleep(1);
//
//        free(ptr);
//    }
////    sleep(3);
//
//    size_t buffer_size = 10 * getpagesize(); // 10 страниц
//
//    void *mapped_ptr = mmap(NULL, buffer_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
//    if (mapped_ptr == MAP_FAILED) {
//        fprintf(stderr, "Ошибка при присоединении памяти с помощью mmap\n");
//        return 1;
//    }
//    printf("Присоединено %zu байт памяти с помощью mmap\n", buffer_size);
//    memset(mapped_ptr, 'A', buffer_size);
//    signal(SIGSEGV, sigsegv_handler);
//    sleep(3);
//
//    if (mprotect(mapped_ptr, buffer_size, PROT_READ) == -1) {
//        fprintf(stderr, "Ошибка при изменении прав доступа с помощью mprotect\n");
//        return 1;
//    }
//    printf("Права доступа к региону изменены на чтение\n");
////    ((char *) mapped_ptr)[0] = 'B';
////    sleep(3);
//
//    if (mprotect(mapped_ptr, buffer_size, PROT_WRITE) == -1) {
//        fprintf(stderr, "Ошибка при изменении прав доступа с помощью mprotect\n");
//        return 1;
//    }
//    printf("Права доступа к региону изменены на запись\n");
//    printf("Symbol: %c\n" , ((char*)mapped_ptr)[0]);
//    sleep(10);
//    size_t offset = 3 * getpagesize();
//    size_t len = 3 * getpagesize();
//    if (munmap(mapped_ptr + offset, len) == -1) {
//        fprintf(stderr, "Ошибка при отсоединении страниц с помощью munmap\n");
//        return 1;
//    }
//    printf("Страницы с 4 по 6 отсоединены\n");
//
//    sleep(100);
//
//    return 0;
//}



// =======================================
// ADDITIONAL TASK
// =======================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>

#define PAGE_SIZE 4096 // Размер страницы памяти

uint64_t get_pagemap_entry(void* virtual_address) {
    uint64_t value;
    uint64_t page_offset = ((uint64_t)virtual_address / PAGE_SIZE) * sizeof(uint64_t);

    int fd = open("/proc/self/pagemap", O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (lseek(fd, page_offset, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (read(fd, &value, sizeof(value)) != sizeof(value)) {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    return value;
}

int main(int argc,char* args[]) {
    int var = 42; // Пример переменной
    void *var_address = &var;
    printf("Virtual address of variable: %p\n", var_address);

    uint64_t pagemap_entry = get_pagemap_entry(var_address);

    if (pagemap_entry & (1ULL << 63)) { // Проверка, присутствует ли страница
        uint64_t frame_number = pagemap_entry & (1ULL << 54); // Извлечение номера кадра
        uint64_t physical_address = (frame_number * PAGE_SIZE) + ((uint64_t)var_address % PAGE_SIZE);
        printf("Physical address of variable: 0x%" PRIx64 "\n", physical_address);
    } else {
        printf("Page not present in memory.\n");
    }

    sleep(1);
    if (execv(args[0], args) == -1) {
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    return 0;
}

