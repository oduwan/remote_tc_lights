#include "arduino.h"
#include "macro.h"
#include "SButton.h"
#include "states.h"
#include <SPI.h>
#include "nRF24L01.h"
#include <RF24.h>
#include "printf.h"
#include "settings_transmitter.h"
#include "signals.h"
//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 8 & 9


void setup(void);
void loop(void);
RF24 radio(48,49);
STATE state = READ_BUTTONS;

void setup(void)
{

	//�������������� ��������
	INIT_BUTTON_LIGHTS
	delay(20);
	
	//�������������� ������
	INIT_BUTTONS
	delay(20);
	
	//�������������� ����������
	SET_UP_RF
	delay(20);

	//
	// Dump the configuration of the rf unit for debugging
	//

	radio.printDetails();

}

//������� ���������� ��������
CREATE_BUTTONS

COMMAND outgoingCommand;
KEEPALIVE receivedKeepalive;
unsigned long sendTime = millis();
bool awaitingKeepAlive = false;
uint8_t receivedKeepalives = 0;
bool buttonAwaitingHandling = false;
bool awaitingStartSignal = false;	// ��������� ������� ��������� ������
unsigned long startTimer = millis();  //������, ������������� �������� ������. ���� � ���, ��� ����� ������� �������� ������ ������ ����������� 10 ������ � ������������� ����������� ��������.
unsigned long startButtonBlink = millis();


void loop(void)
{
	switch (state) {
		case READ_BUTTONS:
		{
			switch( start_red_button.Loop() ){
				case SB_CLICK:
					TURNOFF_BUTTON(START_ADDR,GREEN)
					CHANGE_BUTTON_STATE(START_ADDR,RED)
				break;
				case SB_LONG_CLICK:
					CHANGE_BUTTON_STATE(START_ADDR,RED)
				break;
				default:
				break;
			}
			switch( start_green_button.Loop() ){
				case SB_CLICK:
					if (IS_BIT_SET(outgoingCommand.state[START_ADDR], RED) && awaitingStartSignal == false) {
						awaitingStartSignal = true;
						startTimer = millis();
					}
				break;
				case SB_LONG_CLICK:
					CHANGE_BUTTON_STATE(START_ADDR,GREEN)
				break;
				default:
				break;
			}
			CHECK_CHANNEL_BUTTON(2)
			CHECK_CHANNEL_BUTTON(3)
			CHECK_CHANNEL_BUTTON(4)
			CHECK_CHANNEL_BUTTON(5)
			CHECK_CHANNEL_BUTTON(6)
			CHECK_CHANNEL_BUTTON(7)
		}
		
		case HANDLE_START_BUTTON:
		{
			if (awaitingStartSignal) {
			
				if ((millis() - startTimer) < START_DELAY ) {
					// ������� ������������� ������ 125 ��
					if( ( millis() - startButtonBlink ) > START_RED_BUTTON_LIGHT_BLINK_PERIOD || millis() < startButtonBlink ){
						startButtonBlink = millis();
						// ����������� ���������
						digitalWrite(START_RED_BUTTON_LIGHT,!(digitalRead(START_RED_BUTTON_LIGHT)));
					}
				}
				else
				{
					TURNOFF_BUTTON(START_ADDR,RED)
					CHANGE_BUTTON_STATE(START_ADDR,GREEN)
					awaitingStartSignal = false;
					startTimer = millis();
				}
			}
		}
		case CHANGE_STATE:
		{
			
			if (buttonAwaitingHandling || ((millis() - sendTime) > RESEND_TIMEOUT)) 
			{
				state = SEND_COMMAND;
				break;
			}
			
			if (awaitingKeepAlive) {
				state = RECIEVE_KEEPALIVE;
				break;
			}
			
			state = READ_BUTTONS;
			break;
			
		}
		
		case SEND_COMMAND:
		{
			radio.stopListening();
			outgoingCommand.transactNum++;
			radio.write(&outgoingCommand, sizeof(outgoingCommand), true);
			radio.startListening();
			sendTime = millis();
			awaitingKeepAlive = true;
			receivedKeepalives = 0;
			state = RECIEVE_KEEPALIVE;
			break;
		}
		
		case RECIEVE_KEEPALIVE:
		{
			if ((millis()-sendTime) > RECEIVE_KEEPALIVES_TIMEOUT) {
				state = CHANGE_STATE;
				awaitingKeepAlive = false;
				break;
			}
			
			uint8_t pipe_num = 255;
			// if there is data ready
			if ( radio.available(&pipe_num) && pipe_num == 1)
			{	
				radio.read(&receivedKeepalive, sizeof(receivedKeepalive) );
				//printf("received pipe: %d, \"%s\".\n\r", pipe_num, &incomingCommand);
				if ((receivedKeepalive.transactNum = outgoingCommand.transactNum) && (!(IS_BIT_SET(receivedKeepalives,receivedKeepalive.address)))) {
					if (receivedKeepalive.state == outgoingCommand.state[receivedKeepalive.address]) state = HANDLE_ALL_BUTTONS;
				}
			}
			break;	
		}
		case HANDLE_ALL_BUTTONS:
			UPDATE_BUTTON_LIGHTS
			state = READ_BUTTONS;
			break;
		default: break;
	}

}

