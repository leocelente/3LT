#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "comm.h"

__attribute__((used)) // avoid linker removal
void isr() {
  // this is an interrupt service routine
  // not all functions are "safe" here e.g printf
  write(STDOUT_FILENO, "GOT SIGNAL\n", 12);
}

int main(int argc, char const *argv[]) {
  // signal(SIGPIPE, SIG_IGN);
  signal(SIGUSR1, isr);
  printf("Parent, %d\n", getpid());

  int i = 0;
  while (i < 16) {
    printf("i = %d\n", i++);
    usleep(250*1000);
  }

  //# 																			.
  struct comm_t comm;
  comm_init(&comm, ROLE_PARENT); //! blocks until child creates comm
  puts("created comm p");

  char data[] = "abcd";
  int len = sizeof(data) / sizeof(data[0]);
  puts("start write");
  write_child(&comm, data, len);
  puts("end write");
  wait(NULL);
  puts("End of Parent\n");

  return 0;
}
