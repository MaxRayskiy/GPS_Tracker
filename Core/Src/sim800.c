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

void sim800_wait_for_resp(char* resp, int cnt) {
	  messages_rx_cnt = cnt;
	  LL_USART_EnableIT_RXNE(USART3);
	  LL_USART_EnableIT_ERROR(USART3);
	  while(SIM800.rx_complite != 1)
	  {LL_mDelay(100);}
	  memcpy(resp, SIM800.rx_buffer, strlen(SIM800.rx_buffer) - 1);
	  SIM800.rx_complite = 0;
}

int sim800_send_end_mark() {
	while(SIM800.tx_complite != 0);
	SIM800.tx_buffer[0] = (char)(26);
	SIM800.tx_buffer[1] = '\r';
	SIM800.tx_buffer[2] = '\n';
	SIM800.tx_cnt = 0;
	LL_USART_EnableIT_TXE(USART3);
	while(SIM800.tx_complite != 1);
	SIM800.tx_complite = 0;
}

int sim800_check_with_cmd(const char* cmd, const char *trg_resp) {
	int answers_cnt = 0;
	for (int i = 0; i < strlen(trg_resp); ++i) {
		if (trg_resp[i] == '\n') {
			++answers_cnt;
		}
	}


	char src_resp[128];


	sim800_send_cmd(cmd);
	//sim800_wait_for_resp(src_resp, 1);
	sim800_wait_for_resp(src_resp, answers_cnt + 1);
	LL_mDelay(100);
	return 1;


	/*
	sprintf(gps_buffer, "answers_cnt=%d\r\n", answers_cnt);
	LL_USART_EnableIT_TXE(USART1);
	LL_mDelay(100);

	int resp_pos = 0;
	while(answers_cnt != 0) {
		--answers_cnt;
		sim800_wait_for_resp(src_resp);

		sprintf(debug_buffer, "%s\r\n", src_resp);
		LL_USART_EnableIT_TXE(USART1);
		LL_mDelay(100);

		for (; resp_pos < strlen(trg_resp); ++resp_pos) {
			if (src_resp[resp_pos] != trg_resp[resp_pos]) {
				sprintf(debug_buffer, "  failed compare: %c!=%c, %d %d %d\r\n", src_resp[resp_pos], trg_resp[resp_pos], resp_pos, strlen(trg_resp), strlen(src_resp));
				LL_USART_EnableIT_TXE(USART1);
				LL_mDelay(100);
				sprintf(debug_buffer, "resp=%s\r\n", src_resp);
				LL_USART_EnableIT_TXE(USART1);
				LL_mDelay(100);
				break;
			}
			if (src_resp[resp_pos] == '\n' && src_resp[resp_pos - 1] == '\r') {
				break;
			}
		}
		LL_mDelay(100);
	}
	return 1;
	*/
}

void GPRS_RX_Callback() {

	char data = LL_USART_ReceiveData8(USART3);
	SIM800.rx_buffer[SIM800.rx_cnt++] = data;
	if (data == 0x0A) {
		--messages_rx_cnt;
		if (messages_rx_cnt == 0) {
			LL_USART_DisableIT_RXNE(USART3);
			LL_USART_DisableIT_ERROR(USART3);
			SIM800.rx_cnt = 0;
			messages_rx_cnt = 1;
			SIM800.rx_complite = 1;
		}
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
