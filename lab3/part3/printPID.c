#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

#define PAGE_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        return 1;
    }

    pid_t pid = atoi(argv[1]);
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/pagemap", pid);

    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    uint64_t pte;
    for (off_t offset = 0; read(fd, &pte, sizeof(pte)) == sizeof(pte); offset += sizeof(pte)) {
        if ((pte & 0x7fffffffffffff) == 0) continue;
        printf("Virtual Address: 0x%016lx ", offset / sizeof(pte) * PAGE_SIZE);
        printf("Physical Page Frame Number: 0x%016lx\n", pte & 0x7fffffffffffff);
    }

    close(fd);
    return 0;
}
