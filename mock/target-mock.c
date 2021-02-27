#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "ipc.h"

#define WRITE_MSK (1 << 7)
#define BUFFER_SIZE 32

void prinf_reg_map(uint8_t *regmap, int len) {
	printf("Register Map:\n");
	for (int i = 1; i <= len; i++) {
		printf("%02X ", regmap[i - 1]);
		if (i % 8 == 0) { printf("\n"); }
	}
}

int main(int argc, char const *argv[]) {


	int master_pid = getppid();

	comm_t comm = { 0 };
	comm_init(&comm, ROLE_CHILD);

	uint8_t recv_buff[BUFFER_SIZE] = { 0 };
	uint8_t tran_buff[BUFFER_SIZE] = { 0 };

	uint8_t register_map[BUFFER_SIZE] = { 0 };

	do {
		memset(&recv_buff, 0, sizeof(recv_buff));
		memset(&tran_buff, 0, sizeof(tran_buff));

		comm_read(&comm, recv_buff, sizeof(recv_buff));

		const uint8_t is_write = recv_buff[0] & WRITE_MSK;
		const uint8_t address = recv_buff[0] & (~WRITE_MSK);
		const int len = recv_buff[1];
		if (address < sizeof(register_map)) {
			if (is_write) {
				printf("[WRITE] ADDR: %d\n", address);
				memcpy(&register_map[address], &recv_buff[2], len);
				prinf_reg_map(register_map, sizeof(register_map));
			} else {
				printf("[READ] ADDR: %d\n", address);
				memcpy(&tran_buff, &register_map[address], len);
			}
		}
		comm_write(&comm, tran_buff, sizeof(tran_buff));
	} while (recv_buff[0] != 'q');

	memset(&recv_buff, 0, sizeof(recv_buff));
	memset(&tran_buff, 0, sizeof(tran_buff));

	comm_close(&comm);
	puts("End of Child");
	return 0;
}
