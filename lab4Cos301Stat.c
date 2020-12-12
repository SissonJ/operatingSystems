#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	struct stat sb;
	
	stat(argv[1], &sb);

	printf("File: %s\n", argv[1]);
	printf("Size: %lld\tBlocks: %lld\tIO Block: %ld\n",
		       	(long long)sb.st_size,
		       	(long long)sb.st_blocks,
		       	(long)sb.st_blksize);
	printf("Device: %s\tInode: %ld\tLinks: %ld\n",
			sb.st_dev,
		       	(long)sb.st_ino,
		       	(long)sb.st_nlink);
	printf("Access: %lo\tUid: %ld\tGid: %ld\n", 
			(unsigned long)sb.st_mode,
		       	(long)sb.st_uid,
		       	(long)sb.st_gid);
	printf("Access: %s\n", ctime(&sb.st_atime));
	printf("Modify: %s\n", ctime(&sb.st_mtime));
	printf("Change: %s\n", ctime(&sb.st_ctime));



	return 0;
}
