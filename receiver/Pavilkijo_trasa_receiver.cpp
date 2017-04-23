#include "arduino.h"
#include <SPI.h>
#include "nRF24L01.h"
#include <RF24.h>
#include "printf.h"
#include "settings.h"
#include "signals.h"
//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 8 & 9


void setup(void);
void loop(void);
RF24 radio(8,9);


// ���������� ����� ���������
uint8_t devAddr = 0;

COMMAND incomingCommand;




void setup(void)
{

	//�������������� �������������� �����
	ENABLE_DEV_ADDR
	delay(20);
	
	//��������� ����� ����������
	SET_DEV_ADDRESS
	
	//�������� ��������� ������ �� ���������� � ����������� �� ������
	
	switch (devAddr) {
		case 0:				// ��� ����� ������ ����������
		break;
		case 1:				// ��� ����� ���������� ���������
		ENABLE_TC_COLOR (RED)
		ENABLE_TC_COLOR (GREEN)
		break;
		default:
		ENABLE_TC_COLOR (RED)
		ENABLE_TC_COLOR (YELLOW)
		break;
	}
	
	//�������������� ����������
	SET_UP_RF

	//
	// Dump the configuration of the rf unit for debugging
	//

	radio.printDetails();
	

}



KEEPALIVE outgoingKeepalive;
unsigned long reciveTime = millis();
bool keepAliveSent = true;

void loop(void)
{
	uint8_t pipe_num = 255;
	// if there is data ready
	if ( radio.available(&pipe_num) && pipe_num == 1)
	{
		//�������� ������ � ����������� ���������
		for (int i = 0; i < RECEIVER_QTY; i++) incomingCommand.state[i] = 0;
		
		radio.read(&incomingCommand, sizeof(incomingCommand) );
		//printf("received pipe: %d, \"%s\".\n\r", pipe_num, &incomingCommand);
		
		switch (devAddr) {
			case 0:
			break;
			case 1:
			digitalWrite(RED, SET_COLOR(incomingCommand.state[devAddr],0));
			digitalWrite(GREEN, SET_COLOR(incomingCommand.state[devAddr],2));
			outgoingKeepalive = {devAddr, incomingCommand.state[devAddr], incomingCommand.transactNum};
			reciveTime = millis();
			keepAliveSent = false;
			break;
			default:
			digitalWrite(RED, SET_COLOR(incomingCommand.state[devAddr],0));
			digitalWrite(YELLOW, SET_COLOR(incomingCommand.state[devAddr],1));
			outgoingKeepalive = {devAddr, incomingCommand.state[devAddr], incomingCommand.transactNum};
			reciveTime = millis();
			keepAliveSent = false;
			break;
		}
	}
	// �������� �������������
	if (!keepAliveSent && devAddr) {
		if ((millis()-reciveTime) > (KEEPALIVE_TIMEOUT*devAddr)) {
				radio.stopListening();
				radio.write(&outgoingKeepalive, sizeof(outgoingKeepalive), true);
				radio.startListening();
				reciveTime = millis();
				keepAliveSent = true;
		}
	}

}

