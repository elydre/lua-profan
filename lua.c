#include <syscall.h>

#define LOAD_ADDRESS 0x4000000

int entry(int argc, char **argv) {
    c_fs_read_file("/bin/tools/lua.bin", (void *) LOAD_ADDRESS);
    int (*func)(int, char **) = (int (*)(int, char **)) LOAD_ADDRESS;
    return func(argc, argv);
}
