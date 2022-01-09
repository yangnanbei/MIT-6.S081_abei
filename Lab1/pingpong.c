#include <kernel/types.h>
#include <user/user.h>

//Write a program that uses UNIX system calls to ''ping-pong'' a byte between two processes over a pair of pipes, one for each direction. The parent should send a byte to the child; the child should print "<pid>: received ping", where <pid> is its process ID, write the byte on the pipe to the parent, and exit; the parent should read the byte from the child, print "<pid>: received pong", and exit. Your solution should be in the file user/pingpong.c.
void main(){
  /*首先要对pingpong的原理图，心里有数
  */
  //pipe的两个端，规定[1]为读端，[0]为写端
  int pipe1[2],pipe2[2];
  //pingpong的字符，突然想起来C语言里没有String
  char buffer[]={'m'};
  //buffer的长度,后面做检验用
  long length = sizeof(buffer);
  //pipe1[1]与父进程读端相连，pipe1[0]与子进程写端相连
  //pipe2[0]与子进程写端相连，pipe2[1]与父进程读端相连
  pipe(pipe1);
  pipe(pipe2);
  //如果是子进程
  if ( fork() == 0 ) {
    //关闭不必要的读写端
    close(pipe1[1]);
    close(pipe2[0]);
    //用read()从pipe中读取数据，用write()向pipe里写数据
    //read(fd,buffer,length)的第一个参数是文件描述符，难道pipe[0]是文件描述符吗？
    //wiki says:In Unix and Unix-like computer operating systems, a file descriptor (FD, less frequently fildes) is a unique identifier (handle) for a file or other input/output resource, such as a pipe or network socket.
    if ( read(pipe1[0], buffer, length) != length ) {
      printf("father to child have read error!");
      exit(1);
    }
    printf("%d: received ping\n", getpid());
    if( write(pipe2[1], buffer, length) != length) {
      printf("child to father have write error!");
      exit(1);
    }
    exit(0);	//正常退出捏
  }
  
    //若为父进程  
    //关闭不必要的读写端
    close(pipe1[0]);
    close(pipe2[1]);
    if( write(pipe1[1], buffer, length) != length) {
        printf("father to child have write error!");
        exit(1);
    }
    if ( read(pipe2[0], buffer, length) != length ) {
        printf("child to father have read error!");
        exit(1);
    }
    printf("%d: received pong\n",getpid());
    exit(0);	//正常退出捏
//main()结束
}

