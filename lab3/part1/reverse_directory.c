#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_LEN 1024

void copyFileReverse(const char* src_path, const char* dest_path) {
    FILE* src_file = fopen(src_path, "r");
    FILE* dest_file = fopen(dest_path, "w");

    if (src_file == NULL || dest_file == NULL) {
        exit(EXIT_FAILURE);
    }

    fseek(src_file, 0, SEEK_END);
    long size = ftell(src_file);
    fseek(src_file, -1, SEEK_CUR);

    char* buffer = (char*)malloc(size);
    if (buffer == NULL) {
        exit(EXIT_FAILURE);
    }

    for (long i = size - 1; i >= 0; i--) {
        fseek(src_file, i, SEEK_SET);
        fread(&buffer[size - 1 - i], sizeof(char), 1, src_file);
    }

    fwrite(buffer, sizeof(char), size, dest_file);

    free(buffer);
    fclose(src_file);
    fclose(dest_file);
}

char* reverseString(const char* str) {
    int len = strlen(str);
    char* result = malloc(len + 1);
    if (result == NULL) {
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < len; i++) {
        result[i] = str[len - i - 1];
    }
    result[len] = '\0';
    return result;
}

void reverseDir(const char* source_dir) {
    DIR* dir = opendir(source_dir);
    if (!dir) {
        return;
    }

    char* dest_dir_name = reverseString(source_dir);
    mkdir(dest_dir_name, 0777);

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char src_file_path[MAX_LEN], dest_file_path[MAX_LEN];

            sprintf(src_file_path, "%s/%s", source_dir, entry->d_name);
            char* reversed_file_name = reverseString(entry->d_name);
            sprintf(dest_file_path, "%s/%s", dest_dir_name, reversed_file_name);
            free(reversed_file_name);

            copyFileReverse(src_file_path, dest_file_path);
        }
    }
    closedir(dir);

    free(dest_dir_name);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source_directory>\n", argv[0]);
        return EXIT_FAILURE;
    }

    reverseDir(argv[1]);

    return EXIT_SUCCESS;
}