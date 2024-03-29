#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *args[]) {
    if (argc == 2) {
        if (strcmp(args[0], "./mkdir") == 0) {
            mkdir(args[1], 0777);
        } else if (strcmp(args[0], "./ls") == 0) {
            DIR *dir = opendir(args[1]);
            if (!dir) {
                return -1;
            }
            struct dirent *entry;
            while ((entry = readdir(dir)) != NULL) {
                printf("%s\n", entry->d_name);
            }
            closedir(dir);
        } else if (strcmp(args[0], "./rmdir") == 0) {
            if (rmdir(args[1]) != 0) {
                return -1;
            }
        } else if (strcmp(args[0], "./touch") == 0) {
            FILE *file = fopen(args[1], "w");
            if (file == NULL) {
                printf("Ошибка при открытии файла.\n");
                return -1;
            }

            fclose(file);
        } else if (strcmp(args[0], "./rm") == 0) {
            if (remove(args[1]) != 0) {
                return -1;
            }
        } else if (strcmp(args[0], "./cat") == 0) {
            FILE *file = fopen(args[1], "r");
            int symbol;
            while ((symbol = fgetc(file)) != EOF) {
                printf("%c", symbol);
            }
        } else if (strcmp(args[0], "./sl") == 0) {
            if (symlink(args[1], "sLink") != 0) {
                return -1;
            }
        } else if (strcmp(args[0], "./ln") == 0) {
            if (link(args[1], "hLink") != 0) {
                return -1;
            }
        } else if (strcmp(args[0], "./lmod") == 0) {
            struct stat file_stat;
            if (stat(args[1], &file_stat) != 0) {
                return -1;
            }

            printf("Права доступа к файлу \"%s\": ", args[1]);
            printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
            printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
            printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
            printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
            printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
            printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
            printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
            printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
            printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
            printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
            printf("\n");

            printf("Количество жестких ссылок на файл \"%s\": %ld\n", args[1], (long) file_stat.st_nlink);

        }

        else if (strcmp(args[0], "./readSL") == 0) {
            char target[1024];
            ssize_t len = readlink(args[1], target, 1024 - 1);
            if (len != -1) {
                target[len] = '\0';
                printf("Symbolic link target: %s\n", target);
            } else {
                perror("Error");
        }
    }

    } else if (argc == 3) {
        if (strcmp(args[0], "./chmod") == 0) {
            mode_t new_mode = strtol(args[2], NULL, 8);

            // Изменяем права доступа к файлу
            if (chmod(args[1], new_mode) == -1) {
                perror("Ошибка при изменении прав доступа к файлу");
                return EXIT_FAILURE;
            }
        }
    }
    return 0;
}