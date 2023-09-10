#include <syscall.h>
#include <bordel.h>

extern int main(int argc, char **argv);

int entry(int argc, char **argv) {
    // we need to call a other entry function
    int exit_code = main(argc, argv);

    // free memory
    int freed = c_mem_free_all(c_process_get_pid());
    serial_debug("%d alloc freed\n", freed);

    return exit_code;
}
