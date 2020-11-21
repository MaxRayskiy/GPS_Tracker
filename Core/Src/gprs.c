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

	LL_mDelay(100);
	/*if (!GPRS_checkSIMStatus())
		return 0;
*/
	if (!sim800_check_with_cmd("AT+CNMI?\r\n", "+CNMI: 2,2,0,0,0\r\nOK\r\n")) {
		if (!sim800_check_with_cmd("AT+CNMI=2,2,0,0,0\r\n","OK\r\n")){
			LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_8);
		    return 0;
		}
	}
	LL_mDelay(100);
	if (!sim800_check_with_cmd("AT+CMGF?\r\n", "+CMGF: 1\r\nOK\r\n")) {
		if (!sim800_check_with_cmd("AT+CMGF=1\r\n","OK\r\n")) {
			LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_8);
		    return 0;
		}
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

__attribute__((naked)) static void delay(void)
{
    asm ("push {r7, lr}");    // Сохраняем регистр-ссылку LR и R7 в стек (чтобы вернуться обратно)
    asm ("ldr r6, [pc, #8]"); // Загружаем число 0x5b8d80 в регистр R6
    asm ("sub r6, #1");       // вычитаем 1
    asm ("cmp r6, #0");       // Проверяем на 0
    asm ("bne delay+0x4");    // Если не 0, то возвращаемся на строчку 3
    asm ("pop {r7, pc}");     // Выгружаем LR и R7 в регистры PC и R7,
                              // тем самым возвращаясь в место вызова функции
    asm (".word 0x5b8d80"); //6000000
}

int GPRS_power_on() {
  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4);
  LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
  LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_8);
  LL_mDelay(3000);
  LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_8);
  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
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

int GPRS_send_SMS_debug() {
	if(!sim900.check_with_cmd("AT+CMGF=1\r\n", "OK\r\n", CMD)) {
	    return false;
	  }

	  LL_mDelay(500);
	  sim900_send_cmd("AT+CMGS=\"");
	  sim900_send_cmd("79067281782");

	  if(!sim900_check_with_cmd("\"\r\n",">",CMD)) {
	      return false;
	  }

	  LL_mDelay(1000);
	  sim900_send_cmd("olleH");
	  LL_mDelay(500);
	  sim900_send_End_Mark();
	  return true;
}

int GPRS_connect(Protocol ptl, const char * host, int port, int timeout);
int GPRS_is_connected();
int GPRS_disconnect();

int GPRS_send_tcp(const char* data);
*/
