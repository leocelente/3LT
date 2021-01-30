#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PROCESS_GOTO(x) (execl((x), "", NULL))

#define FORK_FAIL(x) ((x) < 0)
#define FORK_IS_CHILD(x) ((x) == 0)
#define FORK_IS_PARENT(x) (((x) > 0))

#include "protocol-channel/comm.h"

int main(int argc, char const *argv[]) {

  // SETUP Two-Way Communication
  mkfifo(COPI, 0666);
  mkfifo(POCI, 0666);

//   printf("main %d\n", getpid());
//   const int split = fork();
//   if (FORK_FAIL(split))
//     return -1;
//   else if (FORK_IS_CHILD(split)) {
//     PROCESS_GOTO("./peripheral/slave.out");
//   } else /*if (FORK_IS_PARENT(split)) */ {
//     PROCESS_GOTO("./microcontroller/master.out");
//   }
//   wait(NULL);
  return 0;
}
