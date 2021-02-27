#ifndef TARGET_H_
#define TARGET_H_

/**
 * REGISTERS
 */
#define MODE_REG 0x00
#define TX_FIFO 0x02
#define TX_SEND 0x07
#define POWERDOWN 0xFF

/**
 * BITMASKS
 */
#define WRITE_MSK (1 << 7)

/**
 * OPTIONS
 */
#define MODE_PADRAO 0xF0

/**
 * TYPES
 */
typedef enum { TARGET_OK, TARGET_FAIL } target_err_t;

typedef struct {
	int mode;
} target_t;

/**
 * FUNCTIONS
 */

/**
 * @brief Inicializa struct do target
 *
 * @param target ponteiro para struct
 * @return int 0 se erro, 1 se OK
 */
target_err_t target_init(target_t *target, void *spi);

/**
 * @brief Set config
 *
 * @param target
 * @param config
 * @return target_err_t
 */
target_err_t target_set_config(target_t *target, const int config);


target_err_t target_send_packet(target_t * target, char data[], int len);


target_err_t target_get_mode(target_t *target, char* rx_buffer, int size);

#endif
