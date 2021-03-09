#include "ipc.h"
#include "target/target.h"
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

comm_t gcomm;

target_t target;

__attribute__((used)) void isr() {
    on_interrupt(&target);
}

int main(int argc, char **argv) {
	int child_pid = *(int *)argv[0];
	comm_init(&gcomm, ROLE_PARENT);

	signal(SIGUSR1, isr);
	/* USER CODE BEGIN */


	target_init(&target, &gcomm);

	target_set_config(&target, MODE_PADRAO);

	char tx[] = { 0xAD, 0xDE };
	target_send_packet(&target, tx, sizeof(tx));
	puts("MCU Continues....");

	char rx[1];
	target_get_mode(&target, rx, 1);
	printf("RX: %02X \n", (uint8_t)rx[0]);


	/* USER CODE END */

	char _data[COMM_BUFF_SZ] = { 'q' };
	uint8_t _rx[COMM_BUFF_SZ] = { 0 };
	comm_write(&gcomm, (uint8_t *)_data, COMM_BUFF_SZ);
	comm_read(&gcomm, _rx, COMM_BUFF_SZ);
	puts("Waiting for Child...");
	wait(NULL);
	comm_close(&gcomm);
	puts("End of Parent");
}
