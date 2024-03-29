#include <stdio.h>
#include <dlfcn.h>
//#include "library/hello.h"

int main() {
    printf("Hello, World!\n");
//    hello_from_static_lib();
//    hello_from_dynamic_lib();
    void* handle = dlopen("./library/libmy3.so", RTLD_LAZY);

    if (!handle) {
        fprintf(stderr, "Error loading dynamic library: %s\n", dlerror());
        return 1;
    }

    // Получение адреса функции из библиотеки
    void (*hello_func)() = dlsym(handle, "hello_from_dyn_runtime_lib");

    if (!hello_func) {
        fprintf(stderr, "Error finding function: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    // Вызов функции
    hello_func();

    // Закрытие библиотеки
    dlclose(handle);

    return 0;
}
