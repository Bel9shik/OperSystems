#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>

int main() {

    FILE* of = fopen("a.txt", "w");
    FILE* in = fopen("a.txt", "r");
    FILE* of1 = fopen("b.txt", "w");
    FILE* in1 = fopen("b.txt", "r");
    FILE* of2 = fopen("c.txt", "w");
    FILE* in2 = fopen("c.txt", "r");
    FILE* of3 = fopen("d.txt", "w");
    FILE* in3 = fopen("d.txt", "r");
    fprintf(of, "hello");
    fprintf(of1, "hello");
    fprintf(of2, "hello");
    fprintf(of3, "hello");
    char ar[10];
    fread(ar, 10, 10, in);
    fread(ar, 10, 10, in1);
    fread(ar, 10, 10, in2);
    fread(ar, 10, 10, in3);
    fclose(of);
    fclose(of1);
    syscall(SYS_write, 1, "Hello world\n", 12);
    return 0;
}