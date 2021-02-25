#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../protocol-channel/comm.h"

#define BUFFER_SIZE 32

void prinf_reg_map(uint8_t *regmap, int len) {
  printf("Registers:\n");
  for (int i = 1; i <= len; i++) {
    printf("%02X ", regmap[i - 1]);
    if (i % 8 == 0) {
      printf("\n");
    }
  }
}

int main(int argc, char const *argv[]) {
  
  puts("Starting Peripheral.....");
  
  int master_pid = -1;
  printf("Enter the Master TOKEN: ");
  scanf("%d", &master_pid);
  if (master_pid <= 0)
    return -1;

  puts("Waiting for connection....");
  comm_t comm = {0};
  comm_init(&comm, ROLE_CHILD);
  puts("Communication Channel Established!");


  uint8_t recv_buff[BUFFER_SIZE] = {0};
  uint8_t tran_buff[BUFFER_SIZE] = {0};

  uint8_t register_map[BUFFER_SIZE] = {0};

  do {
    memset(&recv_buff, 0, sizeof(recv_buff));
    memset(&tran_buff, 0, sizeof(tran_buff));

    comm_read(&comm, recv_buff, sizeof(recv_buff));

    puts("Received a Command");

    switch (recv_buff[0]) {
    case 'e':
      memcpy(&tran_buff, &recv_buff[1], sizeof(recv_buff) - 1);
      break;
    case 't':
      memcpy(&tran_buff, "OK\n", 4);
      sleep((int)recv_buff[1]); //  this would probably be better as in thread
      kill(master_pid, SIGUSR1);
      break;
    case 'w':
      memcpy(&register_map[recv_buff[1]], &recv_buff[3], recv_buff[2]);
      prinf_reg_map(register_map, sizeof(register_map));
      break;
    case 'r':
      memcpy(&tran_buff, &register_map[recv_buff[1]], recv_buff[2]);
      prinf_reg_map(register_map, sizeof(register_map));
      break;
    default:
      memcpy(&tran_buff, "Unknown Command\n", 17);
      break;
    }
    comm_write(&comm, tran_buff, sizeof(tran_buff));
  } while (recv_buff[0] != 'q');

  memset(&recv_buff, 0, sizeof(recv_buff));
  memset(&tran_buff, 0, sizeof(tran_buff));

  comm_close(&comm);
  puts("End of Child");
  return 0;
}
