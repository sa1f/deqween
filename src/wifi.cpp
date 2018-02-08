#include "../include/wifi.h"

Wifi::Wifi(uint32_t baseAddr){
	// Set registers
	mWifi_Status		= (uint8_t*)baseAddr;
	mWifi_Control 		= (uint8_t*)baseAddr;
	mWifi_TxData		= (uint8_t*)baseAddr + 0x2;
	mWifi_RxData		= (uint8_t*)baseAddr + 0x2;
	mWifi_Baud			= (uint8_t*)baseAddr + 0x4;

	// set up 6850 control register to utilize a divide by 16 clock,
	// set rts low, use 8 bits of data, no parity, 1 stop bit,
	// transmitter interrupt disabled
	*(mWifi_Control) = 0x95;

	// program baud rate generator to use 115k baud
	*(mWifi_Baud) = 0x1;
}

int Wifi::putchar_wifi(char c) {

	// poll Tx bit in 6850 status register. Wait for it to become '1'
	while( *(mWifi_Status) & 0x2 != 0x2){
	}
	// write 'c' to the 6850 TxData register to output the character
	*(mWifi_TxData) = c;
 	return c ; // return c
}

void Wifi::send_message_nowait(std::string message)
{
	int i;
	printf("Printing: %s", message.c_str());
	for(i = 0; i <  message.length(); i++) {
		putchar_wifi(message[i]);
		usleep(100000);
		printf("%c\n", message[i]);
	}
}

void Wifi::test_wifi(){

	usleep(1000000);
	send_message_nowait("dofile(\"send_text_message_FULL.lua\")\r\n");
	usleep(2000000);
	send_message_nowait("check_wifi()\r\n");
	usleep(2000000);
		send_message_nowait("check_wifi()\r\n");
		usleep(2000000);
			send_message_nowait("check_wifi()\r\n");
			usleep(2000000);
				send_message_nowait("check_wifi()\r\n");
				usleep(2000000);
					send_message_nowait("check_wifi()\r\n");
}
