#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void main(int arparam, char *argv[]){
  if(arparam != 2)  {
    fprintf(2, "you need to have 1 argument for sleep\n");
    exit(1);
  }
  int sleepTime = atoi(argv[1]);
  printf("(nothing happens for a little while)\n");
  sleep(sleepTime);
  exit(0);
}
  
