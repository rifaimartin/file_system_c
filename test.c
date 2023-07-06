#include "fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    create_fs();
    sync_fs();
    print_fs();

    printf("File System is created and written to file. \n");

    run_virtual_file_system();
    return 0;
}