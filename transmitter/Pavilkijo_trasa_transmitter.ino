#include "Timer.h"
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

RF24 radio(48,49);
STATE state = READ_BUTTONS;

COMMAND outgoingCommand;

void setup(void)
{

	//»нициализируем лампочки
	INIT_BUTTON_LIGHTS
	delay(20);
	
	//»нициализируем кнопки
	INIT_BUTTONS
	delay(20);
	
	//»нициализируем радиомодем
	SET_UP_RF
	delay(20);

	//
	// Dump the configuration of the rf unit for debugging
	//

	radio.printDetails();
	
	outgoingCommand.transactNum = 0;

}

//—оздаем экземпл¤ры кнопочек
CREATE_BUTTONS

KEEPALIVE receivedKeepalive;
Timer sendTime(RECEIVE_KEEPALIVES_TIMEOUT);
bool awaitingKeepAlive = false;
uint8_t receivedKeepalives = 0;
bool buttonAwaitingHandling = false;
bool awaitingStartSignal = false;	// индикатор нажати¤ стартовой кнопки
Timer startTimer(START_DELAY);  //таймер, отсчитывающий задержку старта. »де¤ в том, что после нажати¤ старовой кнопки ситема отсчитывает 10 секунд и автоматически переключает светофор.
Timer startButtonBlink(START_RED_BUTTON_LIGHT_BLINK_PERIOD);
Timer pushCommandTimer (RESEND_TIMEOUT);
Timer missedKAButtonBlink (START_RED_BUTTON_LIGHT_BLINK_PERIOD);


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
						startTimer.start();
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
			
				if (!startTimer.elapsed()) {
					// —обытие срабатывающее каждые 125 мс
					if( startButtonBlink.elapsed() ){
						startButtonBlink.start();
						// »нвертируем светодиод
						digitalWrite(START_RED_BUTTON_LIGHT,!(digitalRead(START_RED_BUTTON_LIGHT)));
					}
				}
				else
				{
					TURNOFF_BUTTON(START_ADDR,RED)
					CHANGE_BUTTON_STATE(START_ADDR,GREEN)
					awaitingStartSignal = false;
					startTimer.start();
				}
			}
		}
		case CHANGE_STATE:
		{
			
			if (buttonAwaitingHandling || (pushCommandTimer.elapsed())) 
			{
				state = SEND_COMMAND;
				break;
			}
			
			if (awaitingKeepAlive) {
				state = RECIEVE_KEEPALIVE;
				break;
			}
			
			if (receivedKeepalives != 254 && !awaitingKeepAlive && outgoingCommand.transactNum > 0) {
				state = HANDLE_MISSED_KEEPALIVES;
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
			sendTime.start();
			awaitingKeepAlive = true;
			receivedKeepalives = 0;
			state = RECIEVE_KEEPALIVE;
			break;
		}
		
		case RECIEVE_KEEPALIVE:
		{
			if (sendTime.elapsed()) {
				state = HANDLE_ALL_BUTTONS;
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
					if (receivedKeepalive.state == outgoingCommand.state[receivedKeepalive.address]) {
						 state = HANDLE_ALL_BUTTONS;
						 SET_BIT(receivedKeepalives,receivedKeepalive.address);
					}
				}
			}
			break;	
		}
		case HANDLE_ALL_BUTTONS:
			UPDATE_BUTTON_LIGHTS
			if (sendTime.elapsed()) {
				state = HANDLE_MISSED_KEEPALIVES;
				break;
			}
			state = READ_BUTTONS;
			break;
			
		case HANDLE_MISSED_KEEPALIVES:
		if( missedKAButtonBlink.elapsed() ){ // —обытие срабатывающее каждые 125 мс
			missedKAButtonBlink.start();
			for (int i = 1; i<RECEIVER_QTY; i++)
			{
				if(!IS_BIT_SET(receivedKeepalives,i)) {
					// »нвертируем светодиод
					digitalWrite(i*2,!(digitalRead(i*2)));
					digitalWrite(i*2+1,!(digitalRead(i*2+1)));
				}
			}
		}
		state = READ_BUTTONS;
		break;
			
		default: break;
	}

}