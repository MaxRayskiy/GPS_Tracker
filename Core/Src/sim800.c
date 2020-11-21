#include "sim800.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sim800_init() {
	SIM800.tx_cnt = 0;
	SIM800.tx_complite = 0;
	SIM800.rx_cnt = 0;
	SIM800.rx_complite = 0;
}

void sim800_send_cmd(const char* cmd) {
	while(SIM800.tx_complite != 0);
	memcpy(SIM800.tx_buffer, cmd, strlen(cmd));
	SIM800.tx_cnt = 0;
	LL_USART_EnableIT_TXE(USART3);
	while(SIM800.tx_complite != 1);
	SIM800.tx_complite = 0;
}

void sim800_send_AT(void) {
	while(SIM800.tx_complite != 0);
	memcpy(SIM800.tx_buffer, "AT\r\n", strlen("AT\r\n"));
	LL_USART_EnableIT_TXE(USART3);
	while(SIM800.tx_complite != 1);
	SIM800.tx_complite = 0;
}

int sim800_wait_for_resp(char* resp) {
	  LL_USART_EnableIT_RXNE(USART3);
	  LL_USART_EnableIT_ERROR(USART3);
	  while(SIM800.rx_complite != 1);
	  memcpy(resp, SIM800.rx_buffer, strlen(SIM800.rx_buffer) - 1);
	  SIM800.rx_complite = 0;
	  return 1;
}

int sim800_check_with_cmd(const char* cmd, const char *trg_resp) {
	//sprintf(gps_buffer, "%s 11\r\n", cmd);
	//LL_USART_EnableIT_TXE(USART1);
	sim800_send_cmd(cmd);
	char src_resp[128];
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_7);
	sim800_wait_for_resp(src_resp);
	sim800_wait_for_resp(src_resp);
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_7);
	sprintf(gps_buffer, "%s", cmd);
	LL_USART_EnableIT_TXE(USART1);
	LL_mDelay(100);
	sprintf(gps_buffer, "%s", src_resp);
	LL_USART_EnableIT_TXE(USART1);
	LL_mDelay(100);

	//sprintf(gps_buffer, "%d\r\n", strlen(trg_resp));
	//LL_USART_EnableIT_TXE(USART1);
	//return 0;
	for (int i = 0; i < strlen(trg_resp); ++i) {
		if (src_resp[i] != trg_resp[i]) {
			sprintf(gps_buffer, "%d %d %d\r\n", i, strlen(trg_resp), strlen(src_resp));
			LL_USART_EnableIT_TXE(USART1);
			break;
			LL_mDelay(100);
			return 0;
		}
	}
	LL_mDelay(100);
	return 1;
}

void GPRS_RX_Callback() {
	//LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9);
	char data = LL_USART_ReceiveData8(USART3);
	SIM800.rx_buffer[SIM800.rx_cnt++] = data;
	if (data == 0x0A) {
		LL_USART_DisableIT_RXNE(USART3);
		LL_USART_DisableIT_ERROR(USART3);
		SIM800.rx_cnt = 0;
		SIM800.rx_complite = 1;
	}
}

void GPRS_TX_Callback() {
	char data = SIM800.tx_buffer[SIM800.tx_cnt++];
	LL_USART_TransmitData8(USART3, data);
	if (data == 0x0A) {
		LL_USART_DisableIT_TXE(USART3);
		SIM800.tx_cnt = 0;
		SIM800.tx_complite = 1;
	}
}
