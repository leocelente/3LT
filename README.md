<h1 align="center" style="color:white; background-color:black">Fake Interrupts com IPC</h1>
<h4 align="center">Experimentos simulando comportamento de software de baixo nivel usando unix IPC </h4>

<p align="center">
	<a href="http://zenith.eesc.usp.br/">
    <img src="https://img.shields.io/badge/Zenith-Embarcados-black?style=for-the-badge"/>
    </a>
    <a href="https://eesc.usp.br/">
    <img src="https://img.shields.io/badge/Linked%20to-EESC--USP-black?style=for-the-badge"/>
    </a>
     <a href="https://github.com/leocelente/simulando-interrupts-linux-signals/blob/main/LICENSE">
    <!-- <a href="https://github.com//blob/main/LICENSE"> -->
    <img src="https://img.shields.io/github/license/leocelente/simulando-interrupts-linux-signals?style=for-the-badge"/>
    <!-- <img src="https://img.shields.io/github/license/?style=for-the-badge"/> -->
    </a>
    </a>
    <a href="https://github.com/leocelente/simulando-interrupts-linux-signals/issues">
    <!-- <a href="https://github.com//issues"> -->
    <img src="https://img.shields.io/github/issues/leocelente/simulando-interrupts-linux-signals?style=for-the-badge"/>
    <!-- <img src="https://img.shields.io/github/issues/?style=for-the-badge"/> -->
    </a>
    </a>
    <a href="https://github.com/leocelente/simulando-interrupts-linux-signals/commits/main">
    <!-- <a href="https://github.com//commits/main"> -->
    <img src="https://img.shields.io/github/commit-activity/m/leocelente/simulando-interrupts-linux-signals?style=for-the-badge">
    <!-- <img src="https://img.shields.io/github/commit-activity/m/?style=for-the-badge"> -->
    </a>
    </a>
    <a href="https://github.com/leocelente/simulando-interrupts-linux-signals/graphs/contributors">
    <!-- <a href="https://github.com//graphs/contributors"> -->
    <img src="https://img.shields.io/github/contributors/leocelente/simulando-interrupts-linux-signals?style=for-the-badge"/>
    <!-- <img src="https://img.shields.io/github/contributors/?style=for-the-badge"/> -->
    </a>
    </a>
    <a href="https://github.com/leocelente/simulando-interrupts-linux-signals/commits/main">
    <!-- <a href="https://github.com//commits/main"> -->
    <img src="https://img.shields.io/github/last-commit/leocelente/simulando-interrupts-linux-signals?style=for-the-badge"/>
    <!-- <img src="https://img.shields.io/github/last-commit/?style=for-the-badge"/> -->
    </a>
    </a>
    <a href="https://github.com/leocelente/simulando-interrupts-linux-signals/issues">
    <!-- <a href="https://github.com//issues"> -->
    <img src="https://img.shields.io/github/issues-raw/leocelente/simulando-interrupts-linux-signals?style=for-the-badge" />
    <!-- <img src="https://img.shields.io/github/issues-raw/?style=for-the-badge" /> -->
    </a>
    </a>
    <a href="https://github.com/leocelente/simulando-interrupts-linux-signals/pulls">
    <!-- <a href="https://github.com//pulls"> -->
    <img src = "https://img.shields.io/github/issues-pr-raw/leocelente/simulando-interrupts-linux-signals?style=for-the-badge">
    <!-- <img src = "https://img.shields.io/github/issues-pr-raw/?style=for-the-badge"> -->
    </a>
</p>

## O que é:

Uma ferramenta para debuggar bibliotecas de baixo-nivel.

## Como Funciona:

Suponha que estamos programando uma biblioteca para um sensor que descreve seu mapa de registradores internos da seguinte forma:

<table align="center">
<tr>
<th>Endereço</th> <th>Função</th>
</tr>
<tr>
<td> 00</td><td> [R/W] Modo de Operação  </td>
</tr>
<tr>
<td> 02</td><td> [R/W] FIFO de Entrada </td>
</tr>
</table>
<p align="center">
    O <b>7º</b> bit do endereço determina Escrita (1) ou Leitura (0)
</p>

Podemos então construir uma biblioteca, supondo que as funções `bsp_*` são uma forma de isolar funções de HAL de um microcontrolador espeçifico. Então nossa biblioteca é generica para diferentes plataformas de hardware. Assim:

```c
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
	return TARGET_OK;
}

target_err_t target_get_mode(target_t *target, char *rx_buffer, int size) {
	char data[] = { 0x00 };
	bsp_transmit(MODE_REG, data, 1, rx_buffer, 1);
	return TARGET_OK;
}
```
 Então testariamos nossa biblioteca dessa forma:
```c
target_t target;
target_init(&target, &gcomm);

target_set_config(&target, MODE_PADRAO);

char tx[] = { 0xAD, 0xDE };
target_send_packet(&target, tx, sizeof(tx));

char rx[1];
target_get_mode(&target, rx, 1);
printf("RX: %02X \n", (uint8_t)rx[0]);

```
Usando essa ferramenta de debugação é possivel rodar esse código no PC. Dado que possamos programar um mock do sensor, como: 
```c
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
```
As funções `comm_*` fazem a comunicação entre o processo da biblioteca ("microcontrolador") e o sensor. Mais detalhes no arquivo `src/mock/target-mock.c`.

Executar o código então nos daria a saida:

```shell
[WRITE] ADDR: 0
Register Map:
F0 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
[WRITE] ADDR: 2
Register Map:
F0 00 DE 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
[WRITE] ADDR: 2
Register Map:
F0 00 AD 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
[READ] ADDR: 0
RX: F0 
```

Teriamos um log da comunicação entre a biblioteca e o sensor. Então seria trivial encontrar erros de programação e diferenças do esperado pelo datasheet.



<p align="center">
    <a href="http://zenith.eesc.usp.br">
    <img src="https://img.shields.io/badge/Check%20out-Zenith's Oficial Website-black?style=for-the-badge" />
    </a> 
    <a href="https://www.facebook.com/zenitheesc">
    <img src="https://img.shields.io/badge/Like%20us%20on-facebook-blue?style=for-the-badge"/>
    </a> 
    <a href="https://www.instagram.com/zenith_eesc/">
    <img src="https://img.shields.io/badge/Follow%20us%20on-Instagram-red?style=for-the-badge"/>
    </a>

</p>
<p align = "center">
<a href="zenith.eesc@gmail.com">zenith.eesc@gmail.com</a>
</p>
