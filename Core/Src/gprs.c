#include "gprs.h"


int GPRS_init(void) {
	if (!sim800_check_with_cmd("AT\r\n","OK\r\n")) {
		LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_6);
	    return 0;
	}

	LL_mDelay(100);
	if (!sim800_check_with_cmd("AT+CFUN=1\r\n","OK\r\n")){
		LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_6);
	    return 0;
	}

	LL_mDelay(200);
	/*if (!GPRS_checkSIMStatus())
		return 0;
	*/
	if (!sim800_check_with_cmd("AT+CNMI=2,2,0,0,0\r\n","OK\r\n")){
		LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_8);
		return 0;
	}//
	LL_mDelay(100);
	if (!sim800_check_with_cmd("AT+CMGF=1\r\n","OK\r\n")) {
		LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_8);
		return 0;
	}
	LL_mDelay(100);
	if (!sim800_check_with_cmd("AT+CLIP=1\r\n","OK\r\n")) {
		LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_8);
	    return 0;
	}
	LL_mDelay(100);
	return 1;
}

int GPRS_check_power_up() {
	return sim800_check_with_cmd("AT\r\n","OK\r\n");
}


/*
int GPRS_power_off() {
	   pinMode(_pkPin, OUTPUT);
	    if (digitalRead(_stPin)) {
	        digitalWrite(_pkPin, HIGH);
	        delay(3000);
	    }
	    digitalWrite(_pkPin, LOW);
	    delay(3000);
	}

int GPRS_check_SIM_status(void)
	{
	  char gprsBuffer[32];
	  int count = 0;
	  sim800_clean_buffer(gprsBuffer,32);
	  while(count < 3) {
	    sim800_send_cmd("AT+CPIN?\r\n");
	    sim800_read_buffer(gprsBuffer,32,DEFAULT_TIMEOUT);
	    if((NULL != strstr(gprsBuffer,"+CPIN: READY"))) {
	      break;
	    }
	    count++;
	    delay(300);
	  }
	  if(count == 3) {
	    return false;
	  }
	  return true;
}
/*
int GPRS_send_SMS(const char* phone_nm, const char* data){
		  // Set message mode to ASCII
		  if(!sim900_check_with_cmd("AT+CMGF=1\r\n", "OK\r\n", CMD)) {
		    return false;
		  }

		  delay(500);
		  sim900_send_cmd("AT+CMGS=\"");
		  sim900_send_cmd(number);

		  if(!sim900_check_with_cmd("\"\r\n",">",CMD)) {
		      return false;
		  }

		  delay(1000);
		  sim900_send_cmd(data);
		  delay(500);
		  sim900_send_End_Mark();
		  return true;
}
*/
int GPRS_send_SMS_debug() {
	if(!sim800_check_with_cmd("AT+CMGF=1\r\n", "OK\r\n")) {
		sprintf(debug_buffer, "SMS FAIL_1\r\n");
		LL_USART_EnableIT_TXE(USART1);
	    return 0;
	}

	LL_mDelay(500);
	if (!sim800_check_with_cmd("AT+CMGS=\"+79xxxxxxxxx\"\r\n",">")) {
		sprintf(debug_buffer, "SMS FAIL_2\r\n");
		LL_USART_EnableIT_TXE(USART1);
	    return 0;
	}
	LL_mDelay(800);
	sim800_send_cmd("Time for math stats\r\n");
	LL_mDelay(500);
	sim800_send_end_mark();
	return 1;
}

int GPRS_connect_TCP() {
	/*
	if (!sim800_check_with_cmd("AT+CPIN?\r\n", "+CPIN: READY\r\n\r\nOK\r\n\r\n")) {
		//sprintf(debug_buffer, "1\r\n");
		//LL_USART_EnableIT_TXE(USART1);
	    return 0;
	}*/
	if (!sim800_check_with_cmd("AT+CIPMODE=0\r\n", "OK\r\n")) {

	    return 0;
	}
	if (!sim800_check_with_cmd("AT+CIPMUX=0", "OK\r\n")) {

	    return 0;
	}
	/*
	if (!sim800_check_with_cmd("AT+CIPSTATUS\r\n", "OK\r\n\r\nSTATE: IP INITIAL\r\n")) {
		sprintf(debug_buffer, "4\r\n");
		LL_USART_EnableIT_TXE(USART1);
	    return 0;
	}*/
	if (!sim800_check_with_cmd("AT+CSTT=\"internet\"\r\n", "OK\r\n")) {

	    return 0;
	}
	LL_mDelay(100);
	//LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_7);
	sim800_check_with_cmd("AT+CIICR\r\n", "111\r\n");
	//LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_8);
	sim800_check_with_cmd("AT+CIFSR\r\n", "111\r\n");

	//LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_7);
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_8);
	sim800_check_with_cmd("AT+CIPSTART=\"TCP\",\"130.193.34.111\",1234\r\n", "OK\r\n\r\nCONNECT OK\r\n");

	sim800_check_with_cmd("AT+CIPQSEND=1\r\n", "OK\r\n"); // fast mode
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_8);
	return 1;


	//sprintf(debug_buffer, "%s\r\n", tmp);

	LL_USART_EnableIT_TXE(USART1);
	LL_mDelay(2000);
	if (!sim800_check_with_cmd("AT+CIPSTATUS\r\n", "OK\r\n\r\nSTATE: IP STATUS\r\n")) {

	    return 0;
	}
	return 1;
	if (!sim800_check_with_cmd("\r\n", "OK\r\n")) {

	    return 0;
	}
	if (!sim800_check_with_cmd("\r\n", "OK\r\n")) {

	    return 0;
	}
	if (!sim800_check_with_cmd("\r\n", "OK\r\n")) {
	    return 0;
	}

}

int GPRS_is_connected() {

}

int GPRS_disconnect() {

}

int GPRS_send_tcp() {
	gprs_dump = 1;
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_7);


	sim800_send_cmd("AT+CIPSEND\r\n", ">");
	LL_mDelay(800);
	sim800_send_cmd(gprs_report);
	sim800_send_end_mark();
	sprintf(gprs_report, "sent\r\n");
	gprs_dump = 0;



	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_7);
}

