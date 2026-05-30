#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: dlsym_tester <path/to/library.so> <symbol name>\n");
        exit(1);
    }

    void * hand = dlopen(argv[1], RTLD_NOW | RTLD_GLOBAL);
    if (hand == NULL) {
        fprintf(stderr, "dlopen() failed: %s\n", dlerror());
        exit(1);
    }
    void * func = dlsym(hand, argv[2]);
    if (func == NULL) {
        fprintf(stderr, "dlsym() failed: %s\n", dlerror());
    }
    printf("(%s): found %s @ %p\n", argv[1], argv[2], func);
}