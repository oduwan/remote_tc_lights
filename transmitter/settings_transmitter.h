// settings_transmitter.h

#ifndef _SETTINGS_TRANSMITTER_h
#define _SETTINGS_TRANSMITTER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "SButton.h"

// ��������� �����������

#define RECEIVER_PIPE_ADDR 0xABCDABCDE1LL			//����� �������� ����� � �������������-���������
#define TRANSMITTER_PIPE_ADDR 0xABCDABCDE2LL		//����� �������� ����� � ������ ���������� - �����������
#define CHANNEL	10									//����� ������������� ��������������� ������
#define DATA_RATE RF24_1MBPS						// ��������� �������� �������� ������ (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1����/���
#define POWER_GAIN RF24_PA_MAX						// ��������� �������� ����������� (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)7

#define SET_UP_RF	radio.begin();\
radio.setChannel(CHANNEL);\
radio.setDataRate(DATA_RATE);\
radio.setPALevel(POWER_GAIN);\
radio.setAutoAck( false );\
radio.enableDynamicAck();\
radio.openWritingPipe (TRANSMITTER_PIPE_ADDR);\
radio.openReadingPipe (1, RECEIVER_PIPE_ADDR);\
radio.startListening  ();

#define RECEIVE_KEEPALIVES_TIMEOUT (30*(RECEIVER_QTY+1))
#define RESEND_TIMEOUT 5000									//������� ��������� �������������� �������� ������� ��������� � ������� ��������� ��������

//������������ ���-�� ����������
#define RECEIVER_QTY 7

//����������� �����
#define DEV_ADDR 0

//��������� ������� ������ ���������� - ���������� ���������
#define START_ADDR 1
#define START_RED_BUTTON A2
#define START_RED_BUTTON_LIGHT 2
#define START_GREEN_BUTTON A3
#define START_GREEN_BUTTON_LIGHT 3
#define START_DELAY 9500
#define START_RED_BUTTON_LIGHT_BLINK_PERIOD 125 //125 ms

//��������� ��������� ����� ������� ����������

#define CHANNEL_2_ADDR 2
#define CHANNEL_2_RED_BUTTON A4
#define CHANNEL_2_RED_BUTTON_LIGHT 4
#define CHANNEL_2_YELLOW_BUTTON A5
#define CHANNEL_2_YELLOW_BUTTON_LIGHT 5

#define CHANNEL_3_ADDR 3
#define CHANNEL_3_RED_BUTTON A6
#define CHANNEL_3_RED_BUTTON_LIGHT 6
#define CHANNEL_3_YELLOW_BUTTON A7
#define CHANNEL_3_YELLOW_BUTTON_LIGHT 7

#define CHANNEL_4_ADDR 4
#define CHANNEL_4_RED_BUTTON A8
#define CHANNEL_4_RED_BUTTON_LIGHT 8
#define CHANNEL_4_YELLOW_BUTTON A9
#define CHANNEL_4_YELLOW_BUTTON_LIGHT 9

#define CHANNEL_5_ADDR 5
#define CHANNEL_5_RED_BUTTON A10
#define CHANNEL_5_RED_BUTTON_LIGHT 10
#define CHANNEL_5_YELLOW_BUTTON A11
#define CHANNEL_5_YELLOW_BUTTON_LIGHT 11

#define CHANNEL_6_ADDR 6
#define CHANNEL_6_RED_BUTTON A12
#define CHANNEL_6_RED_BUTTON_LIGHT 12
#define CHANNEL_6_YELLOW_BUTTON A13
#define CHANNEL_6_YELLOW_BUTTON_LIGHT 13

#define CHANNEL_7_ADDR 7
#define CHANNEL_7_RED_BUTTON A14
#define CHANNEL_7_RED_BUTTON_LIGHT 14
#define CHANNEL_7_YELLOW_BUTTON A15
#define CHANNEL_7_YELLOW_BUTTON_LIGHT 15

//���������� - ��������� �������� �� ���� ���������� ����������
#define ALL_RED_BUTTON A0
#define ALL_RED_BUTTON_LIGHT 22
#define ALL_YELLOW_BUTTON A1
#define ALL_YELLOW_BUTTON_LIGHT 23



// ���� ������
#define RED 0
#define YELLOW 1
#define GREEN 2

//




#endif