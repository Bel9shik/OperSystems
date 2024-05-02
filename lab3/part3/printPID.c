#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

#define PAGE_SIZE 4096
#define ENTRY_SIZE 8
#define BUFFER_SIZE (1024 * ENTRY_SIZE)  // Обрабатываем по 1024 записи за раз

int main() {
    char pagemap_path[100];
    sprintf(pagemap_path, "/proc/%d/pagemap", getpid());

    int fd = open(pagemap_path, O_RDONLY);
    if (fd < 0) {
        perror("Error opening pagemap file");
        return 1;
    }

    uint64_t buffer[BUFFER_SIZE / sizeof(uint64_t)];
    ssize_t bytes_read;
    off_t offset = 0;

    while ((bytes_read = pread(fd, buffer, sizeof(buffer), offset)) > 0) {
        ssize_t num_entries = bytes_read / sizeof(uint64_t);
        for (ssize_t i = 0; i < num_entries; i++) {
            uint64_t entry = buffer[i];
            uint64_t frame_number = entry & 0x7FFFFFFFFFFFFF;
            int present_bit = (entry >> 63) & 1;
            int swapped_bit = (entry >> 62) & 1;
            int soft_dirty_bit = (entry >> 55) & 1;
            int exclusively_mapped_bit = (entry >> 56) & 1;
            int file_shared_bit = (entry >> 61) & 1;

            if (present_bit == 1) {
                printf("Frame Number: %lx, Present Bit: %d, Swapped Bit: %d, Soft Dirty Bit: %d, Exclusively Mapped Bit: %d, File Shared Bit: %d\n",
                       frame_number, present_bit, swapped_bit, soft_dirty_bit, exclusively_mapped_bit, file_shared_bit);
            }
        }
        offset += bytes_read;
    }

    if (bytes_read < 0) {
        perror("Error reading pagemap file");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
