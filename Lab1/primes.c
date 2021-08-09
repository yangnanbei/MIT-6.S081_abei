#include "kernel/types.h"
#include "user/user.h"

/*每个子进程只对一个管道负责*/
void childProcess(int p1[]){
  close(p1[1]);
  int primeNumber;	//素数
  if ( read(p1[0], &primeNumber, 4) > 0) {	//如果读出来有数据
    fprintf(1, "prime %d\n", primeNumber);
    int p2[2];
    pipe(p2);	//创建p2的管道
    int pid = fork();
    if ( pid > 0 ) {	//若为父进程
      close(p2[0]);	//因为首个传过来的都是素数，所以关闭p2的读端
      int num;
      while(read(p1[0], &num ,4) > 0) {
        if ( num % primeNumber != 0) {
          write(p2[1], &num ,4);	//是素数，将其写入
        }
      }
      close(p2[1]);
      wait(0);
    }
    else {
      close(p1[0]);
      childProcess(p2);
    }
  }
}


void main(){
  //规定[0]是读端，[1]是写端
  int p1[2];
  pipe(p1);	//创建p1的管道
  //pid捏
  int pid = fork();
  int num = 2;
  //总体上分为parent和child端，有算法原理可知child端需repeat。
  //若为parent
  if ( pid > 0 ) {
    close(p1[0]);
    fprintf(1, "prime %d\n", num);
    for ( num++ ; num <= 35; num += 1 ) {
      if ( num % 2 != 0 ) {	//若num不能被2所整除
        write( p1[1], &num, 4);	//向写端写入数据,每次写4个字节,到这里2-35的数据都给注满了
      }
    }
    close(p1[1]);
    wait(0);	//等待子进程的结束
  }
  else {
    childProcess(p1);
  }
  exit(0);
}