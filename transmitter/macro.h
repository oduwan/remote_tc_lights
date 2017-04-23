// macro.h

#ifndef _MACRO_h
#define _MACRO_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "settings_transmitter.h"

//Инициализируем все кнопки

#define INIT_SPEC_BUTTONS	pinMode(START_RED_BUTTON, INPUT_PULLUP);\
pinMode(START_GREEN_BUTTON, INPUT_PULLUP);\
pinMode(ALL_RED_BUTTON, INPUT_PULLUP);\
pinMode(ALL_YELLOW_BUTTON, INPUT_PULLUP);

#define INIT_CHANNEL_BUTTON(i)	pinMode(CHANNEL_##i##_RED_BUTTON, INPUT_PULLUP);\
pinMode(CHANNEL_##i##_YELLOW_BUTTON, INPUT_PULLUP);

#define INIT_BUTTONS	INIT_SPEC_BUTTONS\
						INIT_CHANNEL_BUTTON(2)\
						INIT_CHANNEL_BUTTON(3)\
						INIT_CHANNEL_BUTTON(4)\
						INIT_CHANNEL_BUTTON(5)\
						INIT_CHANNEL_BUTTON(6)\
						INIT_CHANNEL_BUTTON(7)


//Создаем кнопки
#define CREATE_SPEC_BUTTONS	SButton start_red_button (START_RED_BUTTON,50,1000,0,500);\
SButton start_green_button (START_GREEN_BUTTON,50,1000,0,500);\
SButton all_red_button (ALL_RED_BUTTON,50,1000,0,500);\
SButton all_yellow_button (ALL_YELLOW_BUTTON,50,1000,0,500);

#define CREATE_CHANNEL_BUTTON(i)	SButton channel_##i##_red_button (CHANNEL_##i##_RED_BUTTON,50,1000,0,500);\
SButton channel_##i##_yellow_button (CHANNEL_##i##_YELLOW_BUTTON,50,1000,0,500);

#define CREATE_BUTTONS	CREATE_SPEC_BUTTONS\
						CREATE_CHANNEL_BUTTON(2)\
						CREATE_CHANNEL_BUTTON(3)\
						CREATE_CHANNEL_BUTTON(4)\
						CREATE_CHANNEL_BUTTON(5)\
						CREATE_CHANNEL_BUTTON(6)\
						CREATE_CHANNEL_BUTTON(7)

//Инициализируем лампочки
#define  INIT_SPEC_BUTTONS_LIGHTS	pinMode(START_RED_BUTTON_LIGHT, OUTPUT);\
digitalWrite(START_RED_BUTTON_LIGHT, LOW);\
pinMode(START_GREEN_BUTTON_LIGHT, OUTPUT);\
digitalWrite(START_GREEN_BUTTON_LIGHT, LOW);\
pinMode(ALL_RED_BUTTON_LIGHT,OUTPUT);\
pinMode(ALL_YELLOW_BUTTON_LIGHT,OUTPUT);\
digitalWrite(ALL_RED_BUTTON_LIGHT,LOW);\
digitalWrite(ALL_YELLOW_BUTTON_LIGHT,LOW);

#define INIT_CHANNEL_BUTTON_LIGHTS(i)	pinMode(CHANNEL_##i##_RED_BUTTON_LIGHT, OUTPUT);\
pinMode(CHANNEL_##i##_YELLOW_BUTTON_LIGHT, OUTPUT);\
digitalWrite(CHANNEL_##i##_RED_BUTTON_LIGHT, LOW);\
digitalWrite(CHANNEL_##i##_YELLOW_BUTTON_LIGHT, LOW);


#define INIT_BUTTON_LIGHTS	INIT_SPEC_BUTTONS_LIGHTS\
							INIT_CHANNEL_BUTTON_LIGHTS(2)\
							INIT_CHANNEL_BUTTON_LIGHTS(3)\
							INIT_CHANNEL_BUTTON_LIGHTS(4)\
							INIT_CHANNEL_BUTTON_LIGHTS(5)\
							INIT_CHANNEL_BUTTON_LIGHTS(6)\
							INIT_CHANNEL_BUTTON_LIGHTS(7)


#define IS_BIT_SET(number,bit) (!!(number & (1 << bit)))
#define SET_BIT(number,bit) (number | (1 << bit))
#define CHANGE_BUTTON_STATE(addr, color)	if (IS_BIT_SET(outgoingCommand.state[addr],color)) {outgoingCommand.state[addr] = (outgoingCommand.state[addr] & (0 << color));} else { outgoingCommand.state[addr] = (outgoingCommand.state[addr] | (1 << color)); }\
buttonAwaitingHandling = true;

#define TURNOFF_BUTTON(addr,color)	if (IS_BIT_SET(outgoingCommand.state[addr],color)) outgoingCommand.state[addr] = (outgoingCommand.state[addr] | (0 << color));

#define CHECK_CHANNEL_BUTTON(i) switch( channel_##i##_red_button.Loop() ){\
	case SB_CLICK:\
	CHANGE_BUTTON_STATE(CHANNEL_##i##_ADDR,RED)\
	break;\
	default:\
	break;\
}\
switch( channel_##i##_yellow_button.Loop() ){\
	case SB_CLICK:\
	CHANGE_BUTTON_STATE(CHANNEL_##i##_ADDR,YELLOW)\
	break;\
	default:\
	break;\
}

#define CHANGE_BUTTON_LIGHT_STATE(button_light) digitalWrite(button_light,!(digitalRead(button_light)));

#define UPDATE_SPEC_BUTTONS_LIGHTS	digitalWrite(START_RED_BUTTON_LIGHT, IS_BIT_SET(outgoingCommand.state[START_ADDR],RED));\
digitalWrite(START_GREEN_BUTTON_LIGHT, IS_BIT_SET(outgoingCommand.state[START_ADDR],GREEN));

#define UPDATE_CHANNEL_BUTTON_LIGHTS(i)	digitalWrite(CHANNEL_##i##_RED_BUTTON_LIGHT, IS_BIT_SET(outgoingCommand.state[CHANNEL_##i##_ADDR],RED));\
digitalWrite(CHANNEL_##i##_YELLOW_BUTTON_LIGHT, IS_BIT_SET(outgoingCommand.state[CHANNEL_##i##_ADDR],YELLOW));


#define UPDATE_BUTTON_LIGHTS	UPDATE_SPEC_BUTTONS_LIGHTS\
								UPDATE_CHANNEL_BUTTON_LIGHTS(2)\
								UPDATE_CHANNEL_BUTTON_LIGHTS(3)\
								UPDATE_CHANNEL_BUTTON_LIGHTS(4)\
								UPDATE_CHANNEL_BUTTON_LIGHTS(5)\
								UPDATE_CHANNEL_BUTTON_LIGHTS(6)\
								UPDATE_CHANNEL_BUTTON_LIGHTS(7)





#endif

