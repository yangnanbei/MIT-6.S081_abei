#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void main(int arparam, char *argv[]){
  if(arparam != 2)  {	//如果输入的参数不是俩
    fprintf(2, "you need to have 1 argument for sleep\n");
    exit(1);	//异常推出
  }
  int sleepTime = atoi(argv[1]);	//argv[0]存的sleep argv[1]存的睡眠时间
  printf("(nothing happens for a little while)\n");
  sleep(sleepTime);
  exit(0);	//正常推出
}
  