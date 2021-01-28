#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "comm.h"

int main(int argc, char const *argv[]) {
  printf("Child A %d, parent %d\n", getpid(), getppid());
  
  usleep(250*1000);
  kill(getppid(), SIGUSR1);
  
  usleep(250*1000);
  kill(getppid(), SIGUSR1);
  

  
  int x = 1;
  while (1) {
    scanf("%d", &x);
    if (x == 0) {
      kill(getppid(), SIGUSR1);
    } else if (x == -1)
      break;
  }

  //# 																			.
  // this should go in the start the file
  struct comm_t comm;
  comm_init(&comm, ROLE_CHILD);
  puts("created comm c");


  char buffer[10];
  puts("start read");
  int bytes_read = read_parent(&comm, buffer, 10);
  puts("end read");

  for (int i = 0; i < bytes_read; i++) {
    printf("%c\t", buffer[i]);
  }
  puts("");

  puts("End of Child");
  return 0;
}
