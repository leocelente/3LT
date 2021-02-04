#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "../protocol-channel/comm.h"

__attribute__((used)) // avoid linker removal
void isr() {
  // this is an interrupt service routine
  // not all functions are "safe" here e.g printf
  write(STDOUT_FILENO, "\nTimer is Done\n", 16);
}

int main(int argc, char const *argv[]) {
  signal(SIGUSR1, isr);
  puts("Starting Master...");
  printf("TOKEN: %d\n", getpid());

  struct comm_t comm;
  puts("Waiting for connection....");
  comm_init(&comm, ROLE_PARENT); //! blocks until child creates comm
  puts("Communication Channel Established!");

  uint8_t cmd_buff[32];
  uint8_t res_buff[32];

  int time = 0;
  do {
    memset(&cmd_buff, 0, sizeof(cmd_buff));
    memset(&res_buff, 0, sizeof(res_buff));
    printf("Enter command: ");
    scanf(" %c", &cmd_buff[0]);
    switch (cmd_buff[0]) {
    case 'e':
      printf("Enter string[30]: ");
      scanf("%s", &cmd_buff[1]);
      break;
    case 't':
      printf("Enter time: ");
      scanf("%d", &time);
      cmd_buff[1] = (char)time;
      break;
    case 'w':
      printf("Enter the address: ");
      scanf(" %02hhX", &cmd_buff[1]);
      printf("Enter the size: ");
      scanf(" %02hhX", &cmd_buff[2]);
      printf("Enter the data: ");
      scanf("%s", &cmd_buff[3]);
			break;
    case 'r':
      printf("Enter the address: ");
      scanf(" %02hhX", &cmd_buff[1]);
      printf("Enter the size: ");
      scanf(" %02hhX", &cmd_buff[2]);
    default:
      break;
    }
    write_child(&comm, cmd_buff, sizeof(cmd_buff));
    read_child(&comm, res_buff, sizeof(res_buff));
    printf("Response: %s\n", res_buff);
  } while (cmd_buff[0] != 'q');

  comm_close(&comm);
  // wait(NULL);

  return 0;
}
