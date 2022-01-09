#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *direct, char *file)
{
	char buf[512];
	char *p;
	int fd;
	//learn from ls.c
	struct dirent de;
	/*
    struct dirent {
      ushort inum;	无符号short型
      char name[DIRSIZ];	字符数组
    };	*/
	struct stat st;
	/*
    struct stat {
      int dev;     // 文件系统的硬盘设备
      uint ino;    // Inode number
      short type;  // 文件类型	
      short nlink; // 指向此文件的链接数
      uint64 size; // 文件的比特数（大小）
    };	*/
    if((fd = open(direct,0)) < 0){
    	fprintf(1,"find: cannot open %s\n", direct);
    	return;
    }
    if (fstat(fd, &st) < 0) {
        fprintf(1, "find: cannot stat %s\n", direct);
        close(fd);
        return;
    }
	if (st.type != T_DIR){	//若不是一个目录
        fprintf(1, "find: %s is not a directory\n", direct);
        close(fd);
        return;
    }
	if(strlen(direct) + 1 + DIRSIZ + 1 > sizeof buf) {	//如果太长了
        fprintf(1, "find: directory too long\n");
        close(fd);
        return;
    }
	strcpy(buf, direct);	//将derect放到buf中
    p = buf+strlen(buf);
    *p++ = '/';
    //learn from ls
    while (read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
            continue;
        //strcmp将两者进行对比，注意.与..的情况
        if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        //异常判断
        if(stat(buf, &st) < 0) {
            fprintf(1, "find: cannot stat %s\n", buf);
            continue;
        }
		if (st.type == T_DIR) {
            find(buf, file);
        }
        else if (st.type == T_FILE && !strcmp(de.name, file)) {
            printf("%s\n", buf);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(1, "usage: find dirName fileName\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}
