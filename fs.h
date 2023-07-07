
#define BLOCKSIZE 512


struct superblock
{
    int num_inodes;
    int num_blocks;
    int size_blocks;
};

struct inode
{
    int size;
    char name[8];
    int first_block;
};

struct disk_block
{
    int next_block_num;
    char data[BLOCKSIZE];
};

void create_fs(); //initiate new file system
void sync_fs(); // write file system

void mount_fs();   // load a file sysmte
void print_fs(); // printout information regarding the file

void run_virtual_file_system();

// create a funtion allocate file, set size and write on its
int allocate_file(char name[8]);

void set_filesize(int filenum, int size);

