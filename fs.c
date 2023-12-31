#include "fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct superblock sb;
struct inode *inodes;
struct disk_block *dbs;


int find_empty_inode() {
    for (int i = 0; i < sb.num_inodes; i++)
    {
        if(inodes[i].first_block == -1) {
            return i;
        }
    }
    
    return -1;
}

int find_empty_block() {
    for (int i = 0; i < sb.num_blocks; i++)
    {
        if(dbs[i].next_block_num == -1) {
            return i;
        }
    }
    
    return -1;
}

void shorten_file(int bn) {
    int tmp = dbs[bn].next_block_num;
    if(tmp > 0) {
        shorten_file(tmp);
    }
    dbs[bn].next_block_num = -1;

}

void create_fs() {
    sb.num_inodes = 10;
    sb.num_blocks = 100;
    sb.size_blocks = sizeof (struct disk_block);

    int i;
    inodes = malloc(sizeof(struct inode) * sb.num_inodes);

    if (inodes == NULL) {
        printf("Memory allocation failed for inodes");
        return;
    }
    for (i = 0; i < sb.num_inodes; i++)
    {
        inodes[i].size = -1;
        inodes[i].first_block = -1;
        strcpy(inodes[i].name, "emptyfi");
    }

    dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);
    for (i = 0; i < sb.num_blocks; i++)
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

    for (i = 0; i < sb.num_blocks; i++)
    {
        fwrite(&(dbs[i]), sizeof(struct disk_block), 1 , file);
    }

    fclose(file);
    
}; // write file system

void mount_fs()
{
    FILE *file;
    file = fopen("fs_data", "r");

    // superblock
    fread(&sb, sizeof(struct superblock), 1, file);

    // write inodes
    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    fread(inodes, sizeof(struct inode), sb.num_inodes, file);


    dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);
    fread(dbs, sizeof(struct disk_block), sb.num_blocks, file);

    fclose(file);
}; // load a file sysmte

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
        fprintf(file, "\tblock num : %d, next_block: %d\n", i, dbs[i].next_block_num);
    }

    fclose(file);

    printf("File System information has been written to file_system_info.txt.\n");
}


void run_virtual_file_system() {
    int mode, inodeIndex;
    printf("Enter the mode (1 (create new), 2 (mount & sync), 3 (mount & synce & occupied byte): ");
    scanf("%d", &mode);
    if (mode == 3){
        scanf("%d", &inodeIndex);
        printf("masuk gak");
    }
    

    switch (mode) {
        case 1:
            printf("Running virtual file system in mode 1.\n");
            create_fs();
            sync_fs();
            print_fs();
            break;
        case 2:
            printf("Running virtual file system in mode 2.\n");
            mount_fs();
            int newFileNum = allocate_file();
            set_filesize(newFileNum, 1500);
            printf("newFileNum value: %d", newFileNum);
            // // char data = 'A';
            //  write_byte(newFileNum, 0, &data);
            sync_fs();
            print_fs();
            break;
        case 3:
            printf("Running virtual file system in mode 3.\n");
             mount_fs();
            // int newFileNum1 = allocate_file();
            // set_filesize(newFileNum1, 1500);
            // printf("newFileNum value 1: %d", newFileNum1);
            char data = 'A';
             write_byte(inodeIndex, 0, &data);
            sync_fs();
            print_fs();
            break;
        default:
            printf("Invalid mode!\n");
            break;
    }
}
// inderection
// direction


int allocate_file() {
    char name[100];
    time_t secs = time(0);

    /* convert to localtime */
    struct tm *local = localtime(&secs);

/* and set the string */
    sprintf(name, "-%02d:%02d:%02d", local->tm_hour, local->tm_min, local->tm_sec);
    int in = find_empty_inode();
    
    // find a disk block
    // ohh SHIT!!!, always retun 0 when i want find my empty block? why
    int block = find_empty_block();
    // printf("nilai bloc : %d",block);
    // claim/occupaid

    inodes[in].first_block = block;
    dbs[block].next_block_num = -2;

    strcpy(inodes[in].name, name);

    // return the fule descriptor
    return in;
}

void set_filesize(int filenum, int size)
{
    // how many blocks u r have?
    inodes[filenum].size = size;
    int tmp = size + BLOCKSIZE - 1;
    int num = tmp / BLOCKSIZE;

    int bn = inodes[filenum].first_block;
    num--;
    // grow file if necessary
    while (num > 0)
    {
            // check next block number
            int next_num = dbs[bn].next_block_num;
            if (next_num == -2)
            {
                int empty = find_empty_block();
                dbs[bn].next_block_num = empty;
                dbs[empty].next_block_num = -2;
            }
            bn = dbs[bn].next_block_num;
            num--;
    }

    // shorten file if necessary
    shorten_file(bn);
    dbs[bn].next_block_num = -2;
};

void write_byte(int fileNum, int pos, char *data) {
    // check if fileNum is not out of range
    if (fileNum >= sb.num_inodes || fileNum < 0) 
    {
        printf("File not found!\n");
        return;
    }
    // check file already closed
    if (inodes[fileNum].first_block == -1) 
    {
        printf("File already closed!\n");
        return;
    }

    // find block yg sesuai
    int blockIdx = inodes[fileNum].first_block;
    int bytesLeft = inodes[fileNum].size - pos;
    while (bytesLeft > 0) {
        if (pos < BLOCKSIZE) {
            // we are still in the current block
            break;
        } else {
            // move pointer to the next block
            blockIdx = dbs[blockIdx].next_block_num;
            pos -= BLOCKSIZE;
            bytesLeft -= BLOCKSIZE;
        }
    }
    if (bytesLeft <= 0) {
        printf("Error: Cannot write data beyond the end of the file.\n");
        return;
    }

    // write data to the corresponding block
    // copying and writing data ke posisi yg di tuju
    memcpy(dbs[blockIdx].data + pos, data, sizeof(char));
    printf("Data written at block index: %d\n", blockIdx);
}