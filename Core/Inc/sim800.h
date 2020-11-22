#include "usart.h"

typedef struct {
	char tx_buffer[128];
	int tx_cnt;
	int tx_complite;

	char rx_buffer[128];
	int rx_cnt;
	int rx_complite;
} SIM800C ;

SIM800C SIM800;

void sim800_init();

void send_cmd(const char* cmd);
void send_AT(void);
//int sim800_send_end_mark();

int wait_for_resp(const char* resp, unsigned int timeout);
int check_with_cmd(const char* cmd, const char *resp, unsigned int timeout);


void GPRS_RX_Callback();
void GPRS_TX_Callback();
