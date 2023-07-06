#include "fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct superblock sb;
struct inode *inodes;
struct disk_block *dbs;

void create_fs() {
    sb.num_inodes = 10;
    sb.num_blocks = 100;
    sb.size_blocks = sizeof (struct disk_block);

    int i;
    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    for (i = 0; i < sb.num_inodes; i++)
    {
        inodes[i].size = -1;
        inodes[i].first_block = -1;
        strcpy(inodes[i].name, "emptyfi");
    }

    dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);
    for (i = 0; i < sb.num_inodes; i++)
    {
        dbs[i].next_block_num = -1;
    }
    

}; //initiate new file system

void sync_fs() {
    FILE *file;
    file = fopen("fs_data", "w+");

    // superblock
    fwrite(&sb, sizeof(struct superblock), 1, file);

    // write inodes
    int i;
    for (i = 0; i < sb.num_inodes; i++)
    {
        fwrite(&(inodes[i]), sizeof(struct inode), 1 , file);
    }

    for (i = 0; i < sb.num_inodes; i++)
    {
        fwrite(&(inodes[i]), sizeof(struct disk_block), 1 , file);
    }

    fclose(file);
    
}; // write file system

void mount_fs(){
     FILE *file;
    file = fopen("fs_data", "r");

    // superblock
    fread(&sb, sizeof(struct superblock), 1, file);

    // write inodes
    int i;
    for (i = 0; i < sb.num_inodes; i++)
    {
        fread(&(inodes[i]), sizeof(struct inode), 1 , file);
    }

    for (i = 0; i < sb.num_inodes; i++)
    {
        fread(&(inodes[i]), sizeof(struct disk_block), 1 , file);
    }

    fclose(file);
};   // load a file sysmte

//// printout information regarding the file
void print_fs() {
    FILE *file = fopen("file_system_info.txt", "w"); // Membuka file untuk penulisan

    if (file == NULL) {
        printf("Gagal membuka file sistem!\n");
        return;
    }

    fprintf(file, "Superblock info:\n");
    fprintf(file, "\t num inodes: %d\n", sb.num_inodes);
    fprintf(file, "\t num blocks: %d\n", sb.num_blocks);
    fprintf(file, "\t size blocks: %d\n\n", sb.size_blocks);

    fprintf(file, "Inodes:\n");
    for (int i = 0; i < sb.num_inodes; i++) {
        fprintf(file, "\tsize: %d, first_block: %d, name: %s\n", inodes[i].size, inodes[i].first_block, inodes[i].name);
    }
    fprintf(file, "\n");

    fprintf(file, "Disk Blocks:\n");
    for (int i = 0; i < sb.num_blocks; i++) {
        fprintf(file, "\tblock: %d, next_block: %d\n", i, dbs[i].next_block_num);
    }

    fclose(file);

    printf("File System information has been written to file_system_info.txt.\n");
}


void run_virtual_file_system() {
    int mode;
    printf("Enter the mode (1, 2, 3): ");
    scanf("%d", &mode);

    switch (mode) {
        case 1:
            printf("Running virtual file system in mode 1.\n");
            printf("Mode 1 feature 1\n");
            break;
        case 2:
            printf("Running virtual file system in mode 2.\n");
            printf("Mode 1 feature 2\n");
            break;
        case 3:
            printf("Running virtual file system in mode 3.\n");
            printf("Mode 1 feature 3\n");
            break;
        default:
            printf("Invalid mode!\n");
            break;
    }
}
// inderection
// direction


