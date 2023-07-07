#include "fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // create_fs(); // first creating when u create new os
    // printf("File System is created and written to file. \n");
    // mount_fs(); // when u load u r os to vm
    // int file = allocate_file("New_File");

    // set_filesize(file,5000);
    run_virtual_file_system();

    // sync_fs();  //
    // print_fs();


    printf("Done. \n");


    return 0;
}