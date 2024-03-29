//
// Created by bel9sh on 16.02.24.
//
#include <unistd.h>

int main() {
    write(1, "Hello world\n", 12);
    return 0;
}