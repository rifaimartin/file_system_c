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
    char data[512];
};

void create_fs(); //initiate new file system
void sync_fs(); // write file system

void mount_fs();   // load a file sysmte
void print_fs(); // printout information regarding the file

void run_virtual_file_system();


