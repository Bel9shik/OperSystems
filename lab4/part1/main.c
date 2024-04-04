#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int globalNotInit;
int globalInit = 4;
const int globalConstInit = 6;
const int globalConstNotInit;


int localFunc() {
    int a = 1;
    int static b = 2;
    const int c = 3;
    printf("local var in func: %p\n", &a);
    printf("static var in func: %p\n", &b);
    printf("const local var in func: %p\n", &c);
    sleep(1); //10
}

int *addressVar() {
    int h = 5;
    return &h;
}

void tmpFunc() {
    char *buf = (char *) malloc(100);
    strcpy(buf, "hello world");
    printf("buf in func before free : %s\n", buf);
    free(buf);
    printf("buf in func after free : %s\n", buf);

    char *buf1 = (char *) malloc(100);
    strcpy(buf1, "hello world1");
    printf("buf1 in func: %s\n", buf1);
    buf1 += 50;
//    sleep(100);
    free(buf1);
    printf("buf1 in func after move pointer: %s\n", buf1);
}

void workWithEnvVar() {
    char* env_variable_name = "MY_ENV_VARIABLE";

    if (setenv(env_variable_name, "VALUE", 1) == 0) {
        printf("Environment variable %s is set to VALUE\n", env_variable_name);
    } else {
        printf("Failed to set environment variable %s\n", env_variable_name);
    }

    char* env_variable_value = getenv(env_variable_name);

    if (env_variable_value != NULL) {
        printf("Value of environment variable %s: %s\n", env_variable_name, env_variable_value);
    } else {
        printf("Environment variable %s is not set\n", env_variable_name);
    }

    if (setenv(env_variable_name, "NEW_VALUE", 1) == 0) {
        printf("Environment variable %s is set to NEW_VALUE\n", env_variable_name);
    } else {
        printf("Failed to set environment variable %s\n", env_variable_name);
    }

    env_variable_value = getenv(env_variable_name);
    if (env_variable_value != NULL) {
        printf("Value of environment variable %s: %s\n", env_variable_name, env_variable_value);
    } else {
        printf("Environment variable %s is not set\n", env_variable_name);
    }

}

int main() {
    printf("my PID: %d\n", getpid());
    localFunc();
    printf("global init var: %p\n", &globalInit);
    printf("global not init var: %p\n", &globalNotInit);
    printf("global const init var: %p\n", &globalConstInit);
    printf("global const not init var: %p\n", &globalConstNotInit);

    printf("returned address from func: %p\n", addressVar());
    tmpFunc();
    workWithEnvVar();
    sleep(1000);
}