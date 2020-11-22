#include <usart.h>
#include <stdio.h>
#include <stdlib.h>

#include"gps.h"

GPS_status GPS;

void GPS_UART_callback() {
	uint8_t data = LL_USART_ReceiveData8(USART1);
	if (start_message == 1 || data == '$') {
		start_message = 1;
	} else {
		return;
	}
	if (output_buffer_num == 1) {
		output_buffer_1[gps_buffer_pointer++] = data;
	} else {
		output_buffer_2[gps_buffer_pointer++] = data;
	}
	if (data == 0x0A) {
		gps_buffer_pointer = 0;
		output_buffer_num = (output_buffer_num == 1) ? 2 : 1;

		gps_data_ready = 1;
		start_message = 0;
		LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_7);
  	}
}


void GPS_UART_debug_callback() {
	uint8_t data;
	if (GPS_DEBUG) {
		if(output_buffer_num == 1) {
			data = output_buffer_2[output_buffer_pointer++];
		} else {
			data = output_buffer_1[output_buffer_pointer++];
		}
	} else {
		data = debug_buffer[output_buffer_pointer++];
	}
	LL_USART_TransmitData8(USART1, data);
	if (data == 0x0A) {
		output_buffer_pointer = 0;
		LL_USART_DisableIT_TXE(USART1);
	}
	return;
}


void GPS_parse() {
	// struct is sending
	if (gps_dump == 1)
		return;
	GPS.status = 0;

	char* data_ptr;
	if (output_buffer_num == 1) {
		data_ptr = output_buffer_2;
	} else {
		data_ptr = output_buffer_1;
	}

	int i = 0;
	while (data_ptr[i++] != '$');

	data_ptr = &data_ptr[i];

	if (data_ptr[0] == 'G' &&
		//dataPtr[1]=='N' && // can be N or P
		data_ptr[2] == 'G' &&
		data_ptr[3] == 'G' &&
		data_ptr[4] == 'A') {

		GPS.status = 1;
		//"$GPGGA,%f,%f,%c,%f,%c,%d,%d,%f,%f,%c"
		char* beg = &data_ptr[6];
		char* end;

		if (beg[0] != ',')
			GPS.utc_time = (float)strtod(beg, &end);
		beg = end + 1;
		if (beg[0] != ',')
			GPS.latitude = (float)strtod(beg, &end) / 100;
		beg = end + 1;
		if (beg[0] != ',') {
			GPS.ns = beg[0];
			++beg;
		}
		++beg;
		if (beg[0] != ',')
			GPS.longitude = (float)strtod(beg, &end) / 100;
		beg = end + 1;
		if (beg[0] != ',') {
			GPS.ew = beg[0];
			++beg;
		}
		++beg;
		if (beg[0] != ',')
			GPS.lock = (int)strtod(beg, &end);
		beg = end + 1;
		if (beg[0] != ',')
			GPS.satelites = (int)strtod(beg, &end);
		beg = end + 1;
		if (beg[0] != ',')
			GPS.hdop = (float)strtod(beg, &end);
		beg = end + 1;
		if (beg[0] != ',')
			GPS.msl_altitude = (float)strtod(beg, &end);
		beg = end + 1;
		if (beg[0] != ',')
			GPS.msl_units = (int)strtod(beg, &end);
	}
}

void GPS_dump_buffer() {
	gps_dump = 1;
	if (GPS.status > 0) {
		if (GPS.lock == 1) { // GPS fix
			if (GPS.satelites < 7) {
				LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_9);
			} else {
				LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9);
			}
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_8);
		} else if (GPS.lock == 2) { // DGPS fix
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_8);
		} else {
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_8);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9);
		}
		sprintf(debug_buffer, "t=%.1f fix=%d lat=%f%c lon=%f%c sat=%d alt=%f\r\n", GPS.utc_time, GPS.lock, GPS.latitude, GPS.ns,  GPS.longitude, GPS.ew, GPS.satelites, GPS.msl_altitude);
		LL_USART_EnableIT_TXE(USART2);
	}
	gps_dump = 0;

	return;
}
