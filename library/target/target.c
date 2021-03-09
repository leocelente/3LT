#include "target.h"
#include "bsp.h"

target_err_t target_init(target_t *target, void *const spi) {
	target->mode = 0;
	return TARGET_OK;
}

target_err_t target_set_config(target_t *target, const int config) {
	char rx[1] = { 0 };
	char data[1] = { (char)config };
	bsp_transmit(MODE_REG | WRITE_MSK, data, 1, rx, 1);
	return TARGET_OK;
}

target_err_t target_send_packet(target_t *target, char data[], int len) {
	char rx[1] = { 0 };
	while (len--) { bsp_transmit(TX_FIFO | WRITE_MSK, &data[len], 1, rx, 1); }

	char send[] = { 0x01 };
	bsp_transmit(TX_SEND | WRITE_MSK, send, 1, rx, 1);
	return TARGET_OK;
}

target_err_t target_get_mode(target_t *target, char *rx_buffer, int size) {
	char data[] = { 0x00 };
	bsp_transmit(MODE_REG, data, 1, rx_buffer, 1);
	return TARGET_OK;
}
